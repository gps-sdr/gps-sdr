/*----------------------------------------------------------------------------------------------*/
/*! \file sv_select.cpp
//
// FILENAME: sv_select.cpp
//
// DESCRIPTION: Implement the SV_Select class.
//
// DEVELOPERS: Gregory W. Heckler (2003-2009)
//
// LICENSE TERMS: Copyright (c) Gregory W. Heckler 2009
//
// This file is part of the GPS Software Defined Radio (GPS-SDR)
//
// The GPS-SDR is free software; you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version. The GPS-SDR is distributed in the hope that
// it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// Note:  Comments within this file follow a syntax that is compatible with
//        DOXYGEN and are utilized for automated document extraction
//
// Reference:
 */
/*----------------------------------------------------------------------------------------------*/

#include "sv_select.h"

/*----------------------------------------------------------------------------------------------*/
void *SV_Select_Thread(void *_arg)
{

	while(1)
	{
		pSV_Select->Import();
	}

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::Start()
{
	/* With new priority specified */
	Start_Thread(SV_Select_Thread, NULL);

	if(gopt.verbose)
		printf("SV_Select thread started\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
SV_Select::SV_Select():Threaded_Object("SVSTASK")
{

	object_mem = this;
	size = sizeof(SV_Select);

	strong_sv = 0;
	weak_sv = 0;
	type = ACQ_TYPE_STRONG;
	mode = ACQ_MODE_COLD;
	mask_angle = PI/2;
	command.evenodd = 0;

	pnav = &pvt_s.sps;
	pclock = &pvt_s.clock;

	pnav->stale_ticks 		= STALE_SPS_VALUE;

	config.weak_modulo		= ACQ_MODULO_WEAK;
	config.warm_doppler 	= MAX_DOPPLER_WARM;
	config.weak_doppler 	= MAX_DOPPLER_WEAK;
	config.strong_doppler 	= MAX_DOPPLER_STRONG;
	config.weak_operation	= ACQ_OPERATION_WEAK;
	config.strong_operation	= ACQ_OPERATION_STRONG;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
SV_Select::~SV_Select()
{
	if(gopt.verbose)
		printf("Destructing SV Select\n");
}
/*----------------------------------------------------------------------------------------------*/


#define EKF_STATE_INITIALIZED (1)
/*----------------------------------------------------------------------------------------------*/
void SV_Select::Import()
{

	uint32 status, acqs_per_pvt;
	uint32 bread, k, nsvs;

	/* Pend on PVT sltn */
	read(PVT_2_SVS_P[READ], &pvt_s, sizeof(PVT_2_SVS_S));

	/* Receive from EKF */
	read(EKF_2_SVS_P[READ], &ekf_s, sizeof(EKF_2_SVS_S));

	 /* Use PVT if it is up to date */
	if(pnav->stale_ticks == 0)
	{
		mode = ACQ_MODE_HOT;
	}
	else if(ekf_s.state.status & (0x1 << EKF_STATE_INITIALIZED)) /* IF GEONS is around, use it!  */
	{
		mode = ACQ_MODE_WARM;
		Geons_2_Nav();
	}
	else /* Cold start, hardest way of doing things (sniffles) */
	{
		mode = ACQ_MODE_COLD;
	}

	/* Compute Mask Angle */
	MaskAngle();

	nsvs = 0;
	for(k = 0; k < MAX_CHANNELS; k++)
		if((pnav->nsvs >> k) & 0x1)
			nsvs++;

	/* Slow down acquisition if PVT is doing fine */
	acqs_per_pvt = MAX_CHANNELS - nsvs;

	if(acqs_per_pvt < 1)
		acqs_per_pvt = 1;

	/* Multiple acqs in some cases */
	for(k = 0; k < acqs_per_pvt; k++)
	{
		Lock();
		Acquire();
		Unlock();
	}

	IncExecTic();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::Export(int32 _sv)
{

	status.mask_angle = mask_angle;
	status.mode = mode;

	sv_prediction[_sv].sv = _sv;

	if(sv_prediction[_sv].predicted != true)
	{
		status.state[_sv] = SV_NOT_PREDICTED;
	}
	else
	{
		if(sv_prediction[_sv].visible == true)
			status.state[_sv] = SV_VISIBLE;
		else
			status.state[_sv] = SV_NOT_VISIBLE;
	}

	/* Dump prediction to SV Select */
	write(SVS_2_TLM_P[WRITE], &sv_prediction[_sv], sizeof(SVS_2_TLM_S));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::Acquire()
{

	uint32 bread, status;
	int32 lcv;
	int32 chan;
	int32 already;
	int32 current_sv;
	int32 doacq;

	chan = 666;
	already = 666;

	switch(type)
	{
		case ACQ_TYPE_STRONG:
			current_sv = strong_sv; break;
		default:
			current_sv = weak_sv; break;
	}

	sv_prediction[current_sv].tracked = false;

	/* Find an empty channel and if the given SV is currently being tracked */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(pChannels[lcv]->getState() == CHANNEL_EMPTY)
		{
			chan = lcv;
		}
		else if(pChannels[lcv]->getSV() == current_sv)
		{
			already = lcv;
			sv_prediction[current_sv].tracked = true;
		}
	}

	/* Up to date PVT */
	read(PVT_2_SVS_P[READ], &pvt_s, sizeof(PVT_2_SVS_S));

	/* Receive from GEONS? */
	bread = read(EKF_2_SVS_P[READ], &ekf_s, sizeof(EKF_2_SVS_S));
	if((bread == sizeof(EKF_2_SVS_S)) && (mode == ACQ_MODE_WARM))
	{
		Geons_2_Nav();
	}

	/* Update SV's predicted state */
	doacq = SetupRequest(current_sv);

	/* Do something with acquisition */
	if(doacq && (already == 666) && (chan != 666))
	{
		command.chan = chan;

		/* Send to the acquisition thread */
		write(SVS_2_ACQ_P[WRITE], &command, sizeof(Acq_Command_S));

		/* Wait for acq to return, do stuff depending on the state */
		read(ACQ_2_SVS_P[READ], &command, sizeof(Acq_Command_S));

	}

	/* Dump state info */
	Export(current_sv);

	/* Increment SV */
	UpdateState();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 SV_Select::SetupRequest(int32 _sv)
{

	SV_Prediction_M *ppred;
	uint32 return_val;
	int32 mdoppler;
	double dt;

	IncStartTic();

	return_val = true;

	switch(type)
	{
		case ACQ_TYPE_STRONG:
			mdoppler = config.strong_doppler;
			break;
		case ACQ_TYPE_WEAK:
			mdoppler = config.weak_doppler;
			break;
		default:
			mdoppler = config.strong_doppler;
			break;
	}

	/* Initialize acquisition command parameters */
	command.antenna 	= 0;
	command.type 		= type;
	command.sv 			= _sv;
	command.mode		= ACQ_MODE_COLD;
	command.magnitude 	= 0;
	command.doppler 	= 0;
	command.cendopp 	= 0;
	command.mindopp 	= -mdoppler;
	command.maxdopp 	= mdoppler;
	command.accel		= 0;

	/* Set maximum Doppler bounds and return based on acquisition type */
    if(type == ACQ_TYPE_STRONG)
	{
		/* Turned off */
    	if(config.strong_operation == ACQ_OPERATION_DISABLED)
    		return_val = false;

    	/* Turned off for cold start */
    	if((config.strong_operation == ACQ_OPERATION_WARM) && (mode == ACQ_MODE_COLD))
    		return_val = false;
	}
	else
	{
		/* Turned off */
    	if(config.strong_operation == ACQ_OPERATION_DISABLED)
    		return_val = false;

    	/* Turned off for cold start */
    	if((config.strong_operation == ACQ_OPERATION_WARM) && (mode == ACQ_MODE_COLD))
    		return_val = false;
	}

	/* Only update prediction if it is longer than 30 seconds old */
//    dt = pclock->time - sv_prediction[_sv].time;
//	if(fabs(dt) > 30.0)
//	{
//		 /* Log updated time */
//		dt = 0;
//		/* Get the almanac */
//
//	}

	GetAlmanac(_sv);
	sv_prediction[_sv].time = (double)pclock->time;

	/* Use almanac info if it is available */
	if((almanacs[_sv].valid == true) && (mode != ACQ_MODE_COLD))
	{
		/* Update predicted state */
		SV_Position(_sv);
		SV_Predict(_sv);

		/* Get the predicted state */
		ppred = &sv_prediction[_sv];

		/* Value to stuff into w accumulator */
		command.accel = (int32)(ppred->doppler_rate * 4096.0);

		/* Search over the defined range */
		//command.cendopp = (int32)ppred->doppler + dt*ppred->doppler_rate;
		command.cendopp = (int32)ppred->doppler;

		/* Bound the Doppler (dont trust the accel either in this case) */
		if(command.cendopp < -MAX_DOPPLER_ABSOLUTE)
		{
			command.accel = 0;
			command.cendopp = -MAX_DOPPLER_ABSOLUTE;
		}

		if(command.cendopp > MAX_DOPPLER_ABSOLUTE)
		{
			command.accel = 0;
			command.cendopp = MAX_DOPPLER_ABSOLUTE;
		}

		/* Search over the given range */
		command.mindopp = command.cendopp - config.warm_doppler;
		command.maxdopp = command.cendopp + config.warm_doppler;

		/* Tag the mode */
		command.mode = mode;

		/* Dont search for the given SV if it has a bad health */
		if(almanacs[_sv].health != 0)
			return_val = false;

		/* If not visible don't bother to search */
		if(ppred->visible == false)
			return_val = false;
	}

	IncStopTic();

	return(return_val);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::UpdateState()
{

	if(type == ACQ_TYPE_STRONG)
	{
		strong_sv = (strong_sv + 1) % MAX_SV;

		if(strong_sv == 0)
			type = ACQ_TYPE_WEAK;
	}
	else
	{
		if(command.evenodd == 0)
		{
			command.evenodd = 1;
		}
		else
		{
			command.evenodd = 0;
			weak_sv = (weak_sv + 1) % MAX_SV;
		}

		if((weak_sv % config.weak_modulo) == 0)
		{
			type = ACQ_TYPE_STRONG;
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::GetAlmanac(int32 _sv)
{

	if((_sv >= 0) && (_sv < MAX_SV))
	{
		pEphemeris->Lock();
		almanacs[_sv] = pEphemeris->getAlmanac(_sv);
		pEphemeris->Unlock();
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::SV_Position(int32 _sv)
{

	int32 iter;
	double a, n0, M, E, P, R, I, L;
	double sE, cE, sI, cI, sL, cL, sP, cP;
	double dEdM, ecc, sqrt1mee, dtemp, Xp, Yp;
	double Edot, Pdot, Rdot, Idot, Xpdot, Ypdot, Ldot, sPdot, cPdot;
	double tk;

	Almanac_M *alm;
	SV_Position_M *psv;

	alm = &almanacs[_sv];
	psv = &sv_positions[_sv];

	/* Time to calculate position */
	tk = pclock->time - alm->toa;

	if (tk > HALF_OF_SECONDS_IN_WEEK)
		tk -= SECONDS_IN_WEEK;
	else if (tk < (-HALF_OF_SECONDS_IN_WEEK))
		tk += SECONDS_IN_WEEK;

	/* Mean motion */
	a = alm->sqrta * alm->sqrta;
	n0 = sqrt(GRAVITY_CONSTANT/(a*a*a));

	/* Mean anomaly, M (rads). */
	M = alm->m0 + n0 * tk;
	M = fmod(M, TWO_PI);

	/* Obtain eccentric anomaly E by solving Kepler's equation. */
	ecc = alm->ecc;

	sqrt1mee = sqrt (1.0 - ecc * ecc);

	E = M;

	for(iter = 0; iter < 20; iter++)
	{
		sE = sin(E);
		cE = cos(E);
		dEdM = 1.0 / (1.0 - ecc * cE);
		if (fabs(dtemp = (M - E + ecc * sE) * dEdM) < 1.0E-14)
			break;
		E += dtemp;
	}

	Edot = dEdM * n0;

	/* Compute the argument of latitude, P. */
	P = atan2 (sqrt1mee * sE, cE - ecc) + alm->argp;
	Pdot = sqrt1mee * dEdM * Edot;
	sP = sin(P);
	cP = cos(P);
	sPdot = cP*Pdot;
	cPdot = -sP*Pdot;

	/* Compute the corrected radius, R. */
	R = a * (1.0 - ecc * cE);
	Rdot = a * ecc * sE * Edot;

	/* Compute the corrected orbital inclination, I. */
	I = alm->in0; Idot = 0.0;
	sI = sin (I); cI = cos (I);

	/* Compute the satellite's position in its orbital plane, (Xp,Yp) */
	Xp = R * cP;
	Yp = R * sP;

	/* Compute the longitude of the ascending node, L. */
	L = alm->om0 + tk * (alm->omd - (double)WGS84OE) - (double)WGS84OE * alm->toa;
	Ldot = alm->omd - (double)WGS84OE;
	sL = sin (L); cL = cos (L);

	/* Compute the satellite's position in space, (x,y,z). */
	psv->x = Xp * cL - Yp * cI * sL;
	psv->y = Xp * sL + Yp * cI * cL;
	psv->z = Yp * sI;

	/* Compute the satellite's velocity in its orbital plane, (Xpdot,Ypdot) */
	Xpdot = Rdot * cP + R * cPdot;
	Ypdot = Rdot * sP + R * sPdot;

	/* Satellite's velocity, (vx,vy,vz). */
	psv->vx = -Ldot * (psv->y)
	+ Xpdot * cL
	- Ypdot * cI * sL;

	psv->vy = Ldot * (psv->x)
	+ Xpdot * sL
	+ Ypdot * cI * cL;

	psv->vz = Ypdot * sI;

	 /* Compute SV clock correction */
	psv->time = tk;
	psv->clock_bias = alm->af0 + tk * alm->af1;
	psv->frequency_bias = alm->af1;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::SV_Predict(int32 _sv)
{
	/* Ugh */
	double rho, theta;
	double fdoppler;

	/* Accelerations */
	double nax, nay, naz;
	double sax, say, saz;
	double ra;						/* Raw acceleration */
	double a, b, c;					/* Vector lengths */
	double dx, dy, dz, radius;		/* Relative position */
	double dvx, dvy, dvz, relvel;	/* Relative velocity */
	double dax, day, daz, relaccel;	/* Relative acceleration */
	double ux, uy, uz;				/* Unit vector */
	double dux, duy, duz;			/* Derivative of unit vector */

	/* Only needed for SV azimuth/elevation */
//	double elev, azim;
	double phi;
	double e, n, u;
	double ct, cp;
	double st, sp;
//	double sct, scp;
//	double sst, ssp;

	SV_Position_M *psv;
	SV_Prediction_M *ppred;

	psv = &sv_positions[_sv];
	ppred = &sv_prediction[_sv];
	ppred->sv 	= _sv;
	ppred->predicted = true;

	/* Relative position vector */
	dx = (psv->x - pnav->x);
	dy = (psv->y - pnav->y);
	dz = (psv->z - pnav->z);

	/* Triangle Leg Lengths */
	a = pnav->x*pnav->x + pnav->y*pnav->y + pnav->z*pnav->z;	/* Vehicle */
	b = psv->x*psv->x + psv->y*psv->y + psv->z*psv->z;			/* SV */
	c = dx*dx + dy*dy + dz*dz;									/* Vehicle->SV */

	/* Law of cosines beotch! OR Dot product relationship */
	rho   = a + c - b;
	theta = b + c - a;

	a = sqrt(a); b = sqrt(b); c = sqrt(c);

	rho	= rho / (2*a*c);		rho	= acos(rho);	 /* Angle of SV relative to vehicle */
	theta = theta / (2*b*c);	theta = acos(theta); /* Angle of vehicle relative to SV */

	/* Predict visibility */
	if(rho > mask_angle)
		ppred->visible = true;
	else
		ppred->visible = false;

	/* Last section, pseudorange acceleration, AKA Doppler rate */

	/* Newton's Law */
	ra = GRAVITY_CONSTANT / (a*a*a);

	/* Acceleration is INTO the Earth */
	nax = -pnav->x * ra;
	nay = -pnav->y * ra;
	naz = -pnav->z * ra;

	/* ECEF Correction */
	nax +=  2.0*WGS84OE*pnav->vy + WGS84OE*WGS84OE*pnav->x;
	nay += -2.0*WGS84OE*pnav->vx + WGS84OE*WGS84OE*pnav->y;

	/* Newton's Law */
	ra = GRAVITY_CONSTANT / (b*b*b);

	/* Acceleration is INTO the Earth */
	sax = -psv->x * ra;
	say = -psv->y * ra;
	saz = -psv->z * ra;

	/* ECEF Correction */
	sax +=  2.0*WGS84OE*psv->vy + WGS84OE*WGS84OE*psv->x;
	say += -2.0*WGS84OE*psv->vx + WGS84OE*WGS84OE*psv->y;

	/* Relative velocity vector */
	dvx = (psv->vx - pnav->vx);
	dvy = (psv->vy - pnav->vy);
	dvz = (psv->vz - pnav->vz);

	/* Relative acceleration vector */
	dax = (sax - nax);
	day = (say - nay);
	daz = (saz - naz);

	/* Reduce some divides */
	radius = c*c;

	/* Relative unit vector */
	ux = dx / c;
	uy = dy / c;
	uz = dz / c;

	/* Line of sight velocity */
	relvel = dvx*ux + dvy*uy + dvz*uz;

	/* Derivative of unit vector */
	dux = (dvx * c - dx * relvel) / radius;
	duy = (dvy * c - dy * relvel) / radius;
	duz = (dvz * c - dz * relvel) / radius;

	/* Relative acceleration along line of sight */
	relaccel = 	ux * dax + dux * dvx +
				uy * day + duy * dvy +
				uz * daz + duz * dvz;

	/* Predict time of flight in terms of chips */
	//ppred->delay = (c + pnav->clock_bias)*INVERSE_SPEED_OF_LIGHT - psv->clock_bias;	/* Seconds */
	ppred->delay = c*INVERSE_SPEED_OF_LIGHT - psv->clock_bias;

	/* Predict observed doppler in terms of Hz */
	//fdoppler = -relvel - pnav->clock_rate - psv->frequency_bias*SPEED_OF_LIGHT; /* Meters/second */
	fdoppler = -relvel - psv->frequency_bias*SPEED_OF_LIGHT;		/* Meters/second */
	fdoppler *= L1_OVER_C; /* Hz */
	ppred->doppler = fdoppler;

	/* Predicted observed doppler rate in terms of Hz */
	/* THIS NEEDS TO INCLUDE THE CLOCK ACCELERATION EVENTUALLY */
	relaccel *= L1_OVER_C;
	ppred->doppler_rate = -relaccel;

	/* Use the SV position and PVT sltn to make a prediction of: Elev, Azim, Delay, Doppler */
	theta = pnav->longitude; phi = pnav->latitude;
	ct = cos(theta); st = sin(theta);
	cp = cos(phi);   sp = sin(phi);

	/* Normalize the direction vector */
	dx = (psv->x - pnav->x) / c;
	dy = (psv->y - pnav->y) / c;
	dz = (psv->z - pnav->z) / c;

	/* Elevation/Azimuth of SV relative to Vehicle */
	e = -st*dx    +  ct*dy;
	n = -sp*ct*dx + -sp*st*dy + cp*dz;
	u =  cp*ct*dx +  cp*st*dy + sp*dz;

	rho = sqrt(n*n + e*e);

	ppred->elev = atan2(u, rho);
	ppred->azim = atan2(e, n);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::MaskAngle()
{

	float radius;
	float a = 6378137;
	float b = 6356752.314;
	float c;

	c = .5*(a + b);

	if(mode != ACQ_MODE_COLD)
	{
		if(pnav->altitude < 10000)
		{
			mask_angle = PI/2;
		}
		else
		{
			radius = pnav->altitude + c;
			mask_angle = atan(c/radius);
		}
	}
	else
	{
		mask_angle = PI/2;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::Geons_2_Nav()
{

    const double a = WGS84_MAJOR_AXIS;
    const double a_sqrd = a*a;
    const double b = WGS84_MINOR_AXIS;
    const double b_sqrd = b*b;
    const double eprime2 = 0.00673949681994;
    const double e2 = 0.00669438006676;
    double p;
    double theta;
    double sin_theta;
    double cos_theta;
    double N;
    double latitude;
    double sin_latitude;
    double cos_latitude;
    double longitude;
    double altitude;

	pnav->x 		= ekf_s.state.x;
	pnav->y 		= ekf_s.state.y;
	pnav->z 		= ekf_s.state.z;
	pnav->vx 		= ekf_s.state.vx;
	pnav->vy 		= ekf_s.state.vy;
	pnav->vz 		= ekf_s.state.vz;
	pnav->clock_bias = ekf_s.state.clock_bias;
	pnav->clock_rate = ekf_s.state.clock_rate;
	pclock->time = ekf_s.state.time;

    p           = sqrt(pnav->x*pnav->x + pnav->y*pnav->y);

    theta       = atan( (pnav->z*a)/ (p*b) );

    sin_theta   = sin( theta );
    cos_theta   = cos( theta );

    latitude    = atan( (pnav->z + eprime2*b*sin_theta*sin_theta*sin_theta) /
                        (p - e2*a*cos_theta*cos_theta*cos_theta)  );

    sin_latitude = sin( latitude );
    cos_latitude = cos( latitude );

    longitude   = atan2( pnav->y/p, pnav->x/p);

    N           = a_sqrd / sqrt(a_sqrd*cos_latitude*cos_latitude + b_sqrd*sin_latitude*sin_latitude);

    altitude    = p/cos_latitude - N;

	pnav->latitude	= latitude;
	pnav->longitude = longitude;
	pnav->altitude  = altitude;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::setConfig(SV_Select_Config_S *_config)
{
	memcpy(&config, _config, sizeof(SV_Select_Config_S));
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
SV_Select_Config_S SV_Select::getConfig()
{
	return(config);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
SV_Select_Status_M SV_Select::getStatus()
{
	return(status);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
SV_Prediction_M SV_Select::getPrediction(uint32 _sv)
{
	return(sv_prediction[_sv]);
}
/*----------------------------------------------------------------------------------------------*/



