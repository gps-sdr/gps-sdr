/*! \file SV_Select.cpp
	Implements member functions of SV_Select class.
*/
/************************************************************************************************
Copyright 2008 Gregory W Heckler

This file is part of the GPS Software Defined Radio (GPS-SDR)

The GPS-SDR is free software; you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GPS-SDR is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with GPS-SDR; if not,
write to the: 

Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
************************************************************************************************/

#include "sv_select.h"

/*----------------------------------------------------------------------------------------------*/
void *SV_Select_Thread(void *_arg)
{

	SV_Select *aSV_Select = pSV_Select;

	while(grun)
	{
		aSV_Select->Inport();
		aSV_Select->Acquire();		
		aSV_Select->Export();
	}
	
	pthread_exit(0);
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
SV_Select::SV_Select()
{
	sv = 0;	
	mode = COLD_START;
	memset(&type[0], 0x0, NUM_CODES*sizeof(int32));
	mask_angle = 0.0;
	
	pnav = &input_s.master_nav;	
	pclock = &input_s.master_clock;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
SV_Select::~SV_Select()
{

	printf("Destructing SV_Select\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::Inport()
{
	int32 bread;
	
	/* Pend on PVT sltn */
	read(PVT_2_SV_Select_P[READ], &input_s, sizeof(PVT_2_SV_Select_S));
	
	/* If the PVT has converged and is < 600 seconds old, use it! */
	if(pnav->stale_ticks < (600*TICS_PER_SECOND) && pnav->initial_convergence)
	{
		mode = HOT_START;
		MaskAngle(); //Update the mask angle 
	}
	else
	{
		mode = COLD_START;
	}
		
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::Acquire()
{
	int32 lcv, chan, already;
	
	chan = 666;
	already = 0;

	/* See if any correlators are available */
	pthread_mutex_lock(&mInterrupt);

	/* If an empty channel exists, ask for an acquisition */	
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		if(gInterrupt[lcv] == 0)
		{
			chan = lcv;
			break;
		}	
		
	sv_prediction[sv].tracked = false;		

	/* If the SV is already being tracked skip the acquisition */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		if(pChannels[lcv]->getActive())
			if(pChannels[lcv]->getSV() == sv)
			{
				already = 666;
				sv_prediction[sv].tracked = true;
			}

	pthread_mutex_unlock(&mInterrupt);
	
	if(already == 666)
	{
		GetAlmanac(sv);		
		SV_Position(sv);
		SV_LatLong(sv);
		SV_Predict(sv);
		UpdateState();
		return;
	}
	
	/* Run the SV prediction routine based on Almanac data */
	GetAlmanac(sv);
	SV_Position(sv);
	SV_LatLong(sv);
	SV_Predict(sv);
		
	/* Do something with acquisition */
	if(chan != 666)
	{
		/* Tell the acquisition what to try */
		if(SetupRequest())
		{

			/* Send to the acquisition thread */
			write(Trak_2_Acq_P[WRITE], &request, sizeof(Acq_Request_S));
		
			/* Wait for acq to return, do stuff depending on the state */
			read(Acq_2_Trak_P[READ], &result, sizeof(Acq_Result_S));
	
			/* If the acq was succesful! */		
			if(result.success)
			{
				/* Else start a channel */
				result.chan = chan;
			
				/* Map receiver channels to channels on correlator */
				write(Trak_2_Corr_P[result.chan][WRITE], &result, sizeof(Acq_Result_S));
			}
		}
		
		UpdateState();		
		
	}
				
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::Export()
{

	output_s.mask_angle = mask_angle;
	output_s.mode = mode;
	output_s.type = type[sv];
	
	memcpy(&output_s.sv_predicted[0], &sv_prediction[0], NUM_CODES*sizeof(Acq_Predicted_S));
	write(SV_Select_2_Telem_P[WRITE], &output_s, sizeof(SV_Select_2_Telem_S));
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
bool SV_Select::SetupRequest()
{

	Acq_Predicted_S *ppred;
	int32 doppler;
	
	/* Initialize parameters */
	request.state = 1;
	request.type = type[sv];
	request.sv = sv;
	request.mindopp = -MAX_DOPPLER;
	request.maxdopp = MAX_DOPPLER;	
	
	if(mode == COLD_START)
	{	
		return(true);
	}
	else if(mode == WARM_START)
	{
		ppred = &sv_prediction[sv];
				
		if(ppred->visible)
			return(true);
		else
			return(false);
	}
	else
	{

		ppred = &sv_prediction[sv];
				
		if(almanacs[sv].decoded)
		{				
			if(ppred->visible)
			{		
	
				/* Round to a kHz value */
				doppler = (int32)ppred->doppler;
				doppler = doppler - (doppler % 1000);
				
				/* Give it a 3 kHz error range */
				request.mindopp = (doppler - 4000);
				request.maxdopp = (doppler + 4000);
							
				return(true);
				
			}
			else
			{
				return(false);
			}
		}
		else
		{
			return(true);
		}
	}
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::UpdateState()
{
	
	sv++;
	sv %= NUM_CODES;

	type[sv]++;

	if(type[sv] > ACQ_MEDIUM)
		type[sv] = ACQ_STRONG;
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::GetAlmanac(int32 _sv)
{

	pEphemeris->Lock();
	almanacs[_sv] = pEphemeris->getAlmanac(_sv);
	pEphemeris->Unlock();

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

	Almanac_S *alm;
	SV_Position_S *psv;
	
	if(almanacs[_sv].decoded)
	{

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
		for (iter = 0; iter < 20; iter++)
		{
			sE = sin(E);
			cE = cos(E);
			dEdM = 1.0 / (1.0 - ecc * cE);
			if (fabs (dtemp = (M - E + ecc * sE) * dEdM) < 1.0E-14)
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

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::SV_Predict(int32 _sv)
{
	
	float elev;
	float azim;
	float dx, dy, dz;
	float ct, cp;
	float st, sp;
	float sct, scp;
	float sst, ssp;
	float theta;
	float phi;
	float rho;
	float e, n, u;
	float radius;
	float dt;
	float relvel;
	
	SV_Position_S *psv;
	Acq_Predicted_S *ppred;	
	
	if(almanacs[_sv].decoded)
	{

		psv = &sv_positions[_sv];
		ppred = &sv_prediction[_sv];

		/* Use the SV position and PVT sltn to make a prediction of: Elev, Azim, Delay, Doppler */
		theta = pnav->longitude; phi = pnav->latitude;
		ct = cos(theta); st = sin(theta);
		cp = cos(phi);   sp = sin(phi);
	


		dx = (psv->x - pnav->x);
		dy = (psv->y - pnav->y);
		dz = (psv->z - pnav->z);
		
		radius = sqrt(dx*dx + dy*dy + dz*dz);
		
		dt = radius / SPEED_OF_LIGHT;
		
		dx /= radius;
		dy /= radius;
		dz /= radius;
		
		/* Velocity along line of sight */
		relvel	 = 	dx * (pnav->vx - psv->vx) + 
			      	dy * (pnav->vy - psv->vy) +
			      	dz * (pnav->vz - psv->vz);  		
		
		/* Elevation of SV relative to Vehicle (cone of exclusion) */
		e = -st*dx    +  ct*dy;
		n = -sp*ct*dx + -sp*st*dy + cp*dz;
		u =  cp*ct*dx +  cp*st*dy + sp*dz;
		
		rho = sqrt(n*n + e*e);

		ppred->sv = _sv;
		ppred->elev = atan2(u, rho);
		ppred->azim = atan2(e, n);

		/* Use the SV position and PVT sltn to make a prediction of: Elev, Azim, Delay, Doppler */
		theta = psv->longitude; phi = psv->latitude;
		ct = cos(theta); st = sin(theta);
		cp = cos(phi);   sp = sin(phi);

		dx = -dx; dy = -dy; dz = -dz;

		/* Elevation of Vehicle relative to SV (cone of inclusion) */
		e = -st*dx    +  ct*dy;
		n = -sp*ct*dx + -sp*st*dy + cp*dz;
		u =  cp*ct*dx +  cp*st*dy + sp*dz;
		
		rho = sqrt(n*n + e*e);

		ppred->v_elev = atan2(u, rho);
		ppred->v_azim = atan2(e, n);
		
		/* Predict time of flight in terms of chips */
		ppred->delay = dt + pnav->clock_bias/SPEED_OF_LIGHT - psv->clock_bias; 	/* Seconds */
		
		/* Predict observed doppler in terms of Hz */
		ppred->doppler = relvel - pnav->clock_rate - psv->frequency_bias*SPEED_OF_LIGHT;/* meters/second */
		ppred->doppler = ppred->doppler*L1/SPEED_OF_LIGHT;								/* Hz */
		
		/* This will be replaced by a more detailed cone of inclusion and exclusion model later */
		if(ppred->elev > mask_angle)
			ppred->visible = true;
		else
			ppred->visible = false;
			
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::SV_LatLong(int32 _sv)
{

	float a = 6378137;
	float b = 6356752.314;
	float eprime2 = 0.00673949681994; 
	float e2 = 0.00669438006676;
	float p;
	float theta;
	float N;
	float latitude;
	float longitude;
	float altitude;
	
	SV_Position_S *psv;

	if(almanacs[_sv].decoded)
	{

		psv			= &sv_positions[_sv];

		p			= sqrt(psv->x*psv->x + psv->y*psv->y);
		
		theta		= atan( (psv->z*a)/ (p*b) );
		
		latitude	= atan( (psv->z+eprime2*b*sin(theta)*sin(theta)*sin(theta)) /
						(p-e2*a*cos(theta)*cos(theta)*cos(theta))  );
						
		longitude	= atan2( psv->y/p, psv->x/p);
	
		N			= a*a / sqrt(a*a*cos(latitude)*cos(latitude) + b*b*sin(latitude)*sin(latitude));
	
		altitude	= p/cos(latitude) - N;N;
	
		psv->latitude	= latitude;
		psv->longitude	= longitude;
		psv->altitude	= altitude;
	}

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
	
	if(pnav->altitude < 1000)
	{
		mask_angle = 0.0;
	}
	else
	{
	
		radius = pnav->altitude + b; 
		mask_angle = -acos(b/radius);
	}	
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::Start()
{
	pthread_attr_t tattr;
	pthread_t tid;
	int32 ret;
	sched_param param;
	
	/* Unitialized with default attributes */
	ret = pthread_attr_init (&tattr);
	
	/*Ssafe to get existing scheduling param */
	ret = pthread_attr_getschedparam (&tattr, &param);
	
	/* Set the priority; others are unchanged */
	param.sched_priority = TRAK_PRIORITY;
	
	/* Setting the new scheduling param */
	ret = pthread_attr_setschedparam(&tattr, &param);
	ret = pthread_attr_setschedpolicy(&tattr, SCHED_RR);
	
	/* With new priority specified */
	pthread_create(&thread, NULL, SV_Select_Thread, NULL);
	
	if(gopt.verbose)
		printf("SV_Select thread started\n");	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void SV_Select::Stop()
{
	pthread_join(thread, NULL);
	
	if(gopt.verbose)
		printf("SV_Select thread stopped\n");
}
/*----------------------------------------------------------------------------------------------*/

