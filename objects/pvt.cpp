/*! \file pvt.cpp
//
// FILENAME: pvt.cpp
//
// DESCRIPTION: Implement the PVT class.
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

#include "pvt.h"

/*----------------------------------------------------------------------------------------------*/
void *PVT_Thread(void *_arg)
{

	while(1)
	{
		pPVT->Import();
		pPVT->Navigate();
		pPVT->Export();
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Start()
{

	Start_Thread(PVT_Thread, NULL);

	if(gopt.verbose)
		printf("PVT thread started\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
PVT::~PVT()
{
	if(gopt.verbose)
		printf("Destructing PVT\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
PVT::PVT():Threaded_Object("PVTTASK")
{

	object_mem = this;
	size = sizeof(PVT);

	Reset();

	master_nav.stale_ticks = STALE_SPS_VALUE;

	if(gopt.verbose)
		printf("Creating PVT\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Import()
{
	int32 lcv;
	int32 sv, chan;
	uint32 bread;
	Measurement_M temp;

	/* Get number of channels coming off the pipe */
	read(ISRP_2_PVT_P[READ], &preamble, sizeof(Preamble_2_PVT_S));

	Lock();

	/* Latch PPS accumulation value */
	master_clock.pps_accum = preamble.pps_accum;

	/* Set good_channels to false */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		good_channels[lcv] = false;
	}

	/* Always clear out this sheit */
	memset(&measurements[0], 0x0, MAX_CHANNELS*sizeof(Measurement_M));
	memset(&pseudoranges[0], 0x0, MAX_CHANNELS*sizeof(Pseudorange_M));

	/* Initial set of nav_channels, gets refined in Error_Check() */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		read(ISRM_2_PVT_P[READ], &temp, sizeof(Measurement_M));

		if(temp.navigate == true)
		{
			//temp.subframe_sec *= 6; //!< Turn into seconds
			sv = temp.sv;
			chan = temp.chan;

			if(chan < 0 || chan >= MAX_CHANNELS)
				continue;

			if(sv < 0 || sv >= MAX_SV)
				continue;

			if(master_sv[chan] == NON_EXISTENT_SV)	/* This SV has not been assigned to a channel */
			{
				master_sv[chan] = sv;
				measurements[chan] = temp;
			}
			else if(master_sv[chan] != sv)			/* The SV on this channel has changed */
			{
				ResetChannel(chan);
				master_sv[chan] = sv;
				measurements[chan] = temp;
			}
			else 									/* Everything is good just update the measurement */
			{
				measurements[chan] = temp;
			}
		}
	}

	/* Set navigation channels as true! */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(measurements[lcv].navigate == true)
		{
			sv_codes[lcv] = PVT_ERROR_NOMINAL;
			good_channels[lcv] = true;
		}
		else
		{
			ResetChannel(lcv); /* Reset channels that arent navigating */
		}
	}

	/* Make sure the pipes are EMPTY */
	PipeCheck();

	IncStartTic();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::PipeCheck()
{

//	Measurement_M tempm;
//	Preamble_2_PVT_S tempp;
//	uint32  bread;
//	uint32 	lcv;
//	char	name[8];
//	void	*start_address;
//	uint32	pipe_size;
//	uint32	available;
//	uint32	messages;
//	OPTION	message_type;
//	uint32	message_size;
//	OPTION	suspend_type;
//	uint32	tasks_waiting;
//	NU_TASK *first_task;
//
//	NU_Pipe_Information(&ISRP_2_PVT_P, name, &start_address, &pipe_size, &available, &messages,
//			&message_type, &message_size, &suspend_type, &tasks_waiting, &first_task);
//	if(messages != 0)
//	{
//		for(lcv = 0; lcv < messages; lcv++)
//		{
//			NU_Receive_From_Pipe(&ISRP_2_PVT_P, &tempp, sizeof(Preamble_2_PVT_S), &bread, NU_SUSPEND);
//		}
//	}
//
//	NU_Pipe_Information(&ISRM_2_PVT_P, name, &start_address, &pipe_size, &available, &messages,
//			&message_type, &message_size, &suspend_type, &tasks_waiting, &first_task);
//	if(messages != 0)
//	{
//		for(lcv = 0; lcv < messages; lcv++)
//		{
//			NU_Receive_From_Pipe(&ISRM_2_PVT_P, &tempm, sizeof(Measurement_M), &bread, NU_SUSPEND);
//		}
//	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Export()
{
	int32 lcv;
	int32 integer_second;

	master_nav.nsvs = 0;
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			master_nav.nsvs |= (0x1 << lcv);
		}

		master_nav.chanmap[lcv] = master_sv[lcv];

		sv_positions[lcv].chan = lcv;
		pseudoranges[lcv].chan = lcv;
		measurements[lcv].chan = lcv;
		sv_positions[lcv].tic = master_nav.tic;
		pseudoranges[lcv].tic = master_nav.tic;
		measurements[lcv].tic = master_nav.tic;
		sv_positions[lcv].sv = master_sv[lcv];
		pseudoranges[lcv].sv = master_sv[lcv];
		measurements[lcv].sv = master_sv[lcv];

	}

	master_clock.tic = master_nav.tic;
	tot.tic = master_nav.tic;

	/* Note, this is some shady shit done to save SRAM !*/
	memcpy(&tlm_s.sps, 			&master_nav, 		sizeof(SPS_M));
	memcpy(&tlm_s.clock, 		&master_clock, 		sizeof(Clock_M));
	memcpy(&tlm_s.sv_positions[0],&sv_positions[0],	sizeof(SV_Position_M)*MAX_CHANNELS);
	memcpy(&tlm_s.pseudoranges[0],&pseudoranges[0], sizeof(Pseudorange_M)*MAX_CHANNELS);
	memcpy(&tlm_s.measurements[0],&measurements[0], sizeof(Measurement_M)*MAX_CHANNELS);
	memcpy(&tlm_s.tot, 			&tot,		 		sizeof(TOT_M));

//	write(PVT_2_PPS_P[WRITE], &tlm_s, sizeof(PVT_2_PPS_S));
	write(PVT_2_SVS_P[WRITE], &tlm_s, sizeof(PVT_2_SVS_S));
	write(PVT_2_TLM_P[WRITE], &tlm_s, sizeof(PVT_2_TLM_S));

	/* Send info to GEONS aligned with the GPS second mod GEONS_MOD */
//	integer_second = (int32)floor(master_clock.time + .5);
//	if((integer_second % EKF_UPDATE_PERIOD) == 0)
//		write(&PVT_2_EKF_P, &tlm_s, sizeof(PVT_2_EKF_S), NU_NO_SUSPEND);

	Unlock();

	IncStopTic();

	IncExecTic();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Navigate()
{

	double dt;
	int32 lcv;

	/* Always tag nav sltn with current tic */
	master_nav.tic = preamble.tic_measurement;

	/* Update receiver time */
	Update_Time();

	/* Get Ephemerides */
	Get_Ephemerides();

	/* Either completely initialize the clock or just the bias */
	if(master_clock.state == PVT_CLOCK_UNINITIALIZED)
		ClockInit();

	/* Do SV Positions */
	SV_Positions();
	SV_TransitTime();
	SV_Correct();

	/* Pseudoranges */
	PseudoRange();

	if(PreErrorCheck())  //If everything looks good then navigate
	{
		/* Copy over master_nav to temp_nav */
		memcpy(&temp_nav, &master_nav, sizeof(SPS_M));

		/* This can be edited out to have PVT iterate from center of Earth */
//		temp_nav.x = 0; temp_nav.y = 0; temp_nav.z = 0;
//		temp_nav.vx = 0; temp_nav.vy = 0; temp_nav.vz = 0;

		/* Ummm Yeah, you need to do the point solution */
		for(lcv = 0; lcv < PVT_ITERATIONS; lcv++)
		{
			FormModel();
			dt = PVT_Estimation();

			/* Break out if we are iterating from a previous position */
			if((fabs(dt) < 1e-4) && temp_nav.converged)
				break;
		}

		master_nav.iterations = lcv;

		if(PostErrorCheck())
		{
			master_nav.converged = true;
			master_nav.converged_ticks++;
			master_nav.stale_ticks = 0;

			//SV_Elevations();
			ClockUpdate();
			LatLong();
			DOP();
		}
		else
		{
			ZeroNav();
			master_nav.converged = false;
			master_nav.converged_ticks = 0;
			master_nav.stale_ticks++;
		}

	}
	else
	{
		ZeroNav();
		master_nav.converged = false;
		master_nav.converged_ticks = 0;
		master_nav.stale_ticks++;
	}

	/* Last step is to form time of tone packet */
	UTCTime();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Update_Time()
{

	master_clock.receiver_time	+= SECONDS_PER_TICK;
	master_clock.time_raw 		= master_clock.time0 + master_clock.receiver_time;
	master_clock.time 			= master_clock.time_raw - master_clock.bias;

	/* Week rollover ! */
	if(master_clock.time > SECONDS_IN_WEEK)
	{

		master_clock.week += 1;
		master_clock.time0 -= SECONDS_IN_WEEK;

		//recompute these
		master_clock.time_raw 	= master_clock.time0 + master_clock.receiver_time;
		master_clock.time 		= master_clock.time_raw - master_clock.bias;
	}

	master_nav.time = master_clock.time;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::ClockUpdate()
{

	/* Update GPStime for current iteration (for use in PPS timer block) */
	master_clock.time 	-= master_nav.clock_bias * INVERSE_SPEED_OF_LIGHT;

	/* Update components for next iteration */
	master_clock.bias 	+= master_nav.clock_bias * INVERSE_SPEED_OF_LIGHT;
	master_clock.rate 	 = master_nav.clock_rate;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::UTCTime()
{

	double dtutc;

	if(utc.valid)
	{
		dtutc = utc.dtls + utc.a0 + utc.a1*(master_clock.time - utc.tot + SECONDS_IN_WEEK*(master_clock.week - utc.wnt));
		tot.second = fmod(master_clock.time - dtutc, SECONDS_IN_DAY);
		tot.day = (uint32)floor((master_clock.time - dtutc)/SECONDS_IN_DAY);
		tot.week = master_clock.week;
		tot.valid = true;
	}
	else
	{
		tot.second = 0;
		tot.day  = 0;
		tot.week = 0;
		tot.valid = false;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Get_Ephemerides()
{

	int32 lcv;
	int32 sv;

	/* Protect the Ephemeris object with a mutex */
	pEphemeris->Lock();

	/* ALWAYS GRAB THE EPHEMERIS */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			sv = master_sv[lcv];
			if((sv >= 0) && (sv < MAX_SV))
			{
				ephemerides[lcv] = pEphemeris->getEphemeris(sv);
			}
			else
			{
				good_channels[lcv] = false;
			}
		} /* If good_channel */
	}/* For channels */

	/* Grab the UTC info at the same time */
	utc = pEphemeris->getUTC();

	pEphemeris->Unlock();

	/* Recalculate good channels */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv] && ephemerides[lcv].valid)
		{
			good_channels[lcv] = true;
		}
		else
		{
			sv_codes[lcv] = PVT_ERROR_EPHEM;
			good_channels[lcv] = false;
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::ClockInit()
{

	int32 lcv;
	double code_phase, code_time;

	/* Initialize the clock to the time-of-transmission of the first GPS signal that we find, this is accurate to within the transit time */
	if(master_clock.state == PVT_CLOCK_UNINITIALIZED)
		for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
			if(good_channels[lcv])
			{

				/* Calculate the code phase */
				code_phase = (double)(measurements[lcv].code_phase) * 1.0 +
							 (double)(measurements[lcv].frac_code_phase) * TWO_N31;

				/* Calculate the total code time */
				code_time = code_phase * INVERSE_CODE_RATE +
							(double)(measurements[lcv]._20ms_epoch * .02) +
							(double)(measurements[lcv]._1ms_epoch * .001);

				master_clock.time0 			= (double)measurements[lcv].subframe_sec + code_time + .200; /* Best guess of GPS time */
				master_clock.receiver_time 	= 0;
				master_clock.rate 			= 0;
				master_clock.bias 			= 0;
				master_clock.time 			= master_clock.time0;
				master_clock.time_raw		= master_clock.time0;
				master_clock.week 			= ephemerides[lcv].week_number;
				master_clock.state 			= PVT_CLOCK_NOMINAL;
				break;
			}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::SV_Positions()
{

	int32 lcv;
	int32 iter;
	double dtemp, M, E, cE, sE, dEdM, P, U, R, I, cU, sU, Xp, Yp, L, sI, cI, sL, cL, ecc, s2P, c2P, toc;
	double Edot, Pdot, Udot, Rdot, sUdot, cUdot, Xpdot, Ypdot, Idot, Ldot;
	double Mdot, sqrt1mee;
	double tk_p_toe, toe;
	double dtk;
	double tk;
	double whole_sec, partial_sec;
	double code_phase, code_time;
	Ephemeris_M *e;
	SV_Position_M *s;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			e = &ephemerides[lcv];
			s = &sv_positions[lcv];

			/* Compute the difference, in seconds, between the epoch and wk & sec. */
			toe = (double) e->toe;

			/* Calculate the code phase */
			code_phase = (double)(measurements[lcv].code_phase) * 1.0 +
						 (double)(measurements[lcv].frac_code_phase) * TWO_N31;

			/* Calculate the total code time */
			code_time = code_phase * INVERSE_CODE_RATE +
						(double)(measurements[lcv]._20ms_epoch * .02) +
						(double)(measurements[lcv]._1ms_epoch * .001);

			/* Time of transmission is calculated directly tracking channel */
			partial_sec = code_time - s->clock_bias;
			whole_sec = (double)measurements[lcv].subframe_sec;

			dtk = partial_sec + whole_sec;

			tk = (double)(dtk - (double)toe);
			tk_p_toe = (double)(dtk);

			if (tk > HALF_OF_SECONDS_IN_WEEK)
				tk -= SECONDS_IN_WEEK;
			else if (tk < (-HALF_OF_SECONDS_IN_WEEK))
				tk += SECONDS_IN_WEEK;

			//Mean anomaly, M (rads).
			Mdot = e->n0 + e->deltan;
			M = e->m0 + Mdot * tk;

			// Obtain eccentric anomaly E by solving Kepler's equation.
			ecc = e->ecc;

			sqrt1mee = sqrt (1.0 - ecc * ecc);
			E = M;
			for (iter = 0; iter < 20; iter++)
			{
				sE = sin(E); cE = cos(E);
				dEdM = 1.0 / (1.0 - ecc * cE);
				if (fabs (dtemp = (M - E + ecc * sE) * dEdM) < 1.0E-14)
					break;
				E += dtemp;
			}

			/* Compute the relativistic correction term (seconds). */
			e->relativistic = (double)(-4.442807633E-10) * ecc * e->sqrta * sE;

			Edot = dEdM * Mdot;

			/* Compute the argument of latitude, P. */
			P = atan2 (sqrt1mee * sE, cE - ecc) + e->argp;
			Pdot = sqrt1mee * dEdM * Edot;

			/* Generate harmonic correction terms for P and R. */
			s2P = sin (2.0 * P);
			c2P = cos (2.0 * P);

			/* Compute the corrected argument of latitude, U. */
			U = P + (e->cus * s2P + e->cuc * c2P);
			sU = sin (U);
			cU = cos (U);
			Udot = Pdot * (1.0 + 2.0 * (e->cus * c2P - e->cuc * s2P));
			sUdot = cU * Udot;
			cUdot = -sU * Udot;

			/* Compute the corrected radius, R. */
			R = e->a * (1.0 - ecc * cE) + (e->crs * s2P +
				e->crc * c2P);
			Rdot = e->a * ecc * sE * Edot + 2.0 * Pdot
				* (e->crs * c2P - e->crc * s2P);

			/* Compute the corrected orbital inclination, I. */
			I = e->in0 + e->idot * tk
			+ (e->cis * s2P + e->cic * c2P);
			sI = sin (I);
			cI = cos (I);
			Idot = e->idot + 2.0 * Pdot * (e->cis * c2P - e->cic * s2P);

			/* Compute the satellite's position in its orbital plane, (Xp,Yp). */
			Xp = R * cU;
			Yp = R * sU;
			Xpdot = Rdot * cU + R * cUdot;
			Ypdot = Rdot * sU + R * sUdot;

			/* Compute the longitude of the ascending node, L. */
			L = e->om0 + tk * (e->omd - (double)WGS84OE) - (double)WGS84OE * e->toe;
			Ldot = e->omd - (double)WGS84OE;
			sL = sin (L);
			cL = cos (L);

			/* Compute the satellite's position in space, (x,y,z). */
			s->x = Xp * cL - Yp * cI * sL;
			s->y = Xp * sL + Yp * cI * cL;
	        s->z = Yp * sI;


	        /* Compute SV clock correction */
			s->time = tk;
			toc = e->toc;


			s->clock_bias = e->af0 +
													(e->af1 *(tk_p_toe - toc)) +
													(e->af2 *(tk_p_toe - toc)*(tk_p_toe - toc)) +
													e->relativistic;

			s->clock_bias -= e->tgd;

			s->frequency_bias = e->af1 + (e->af2 *(tk_p_toe - toc)*2.0);


			/* Satellite's velocity, (vx,vy,vz). */
			s->vx = -Ldot * (s->y)
			+ Xpdot * cL
			- Ypdot * cI * sL
			+ Yp * sI * Idot * sL;

			s->vy = Ldot * (s->x)
			+ Xpdot * sL
			+ Ypdot * cI * cL
			- Yp * sI * Idot * cL;

			s->vz = +Yp * cI * Idot
			+ Ypdot * sI;

		} //end if good channel
	}	//end lcv

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::SV_TransitTime()
{

	int32 lcv;


	/* Calculate transit time to SV */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			sv_positions[lcv].transit_time = sqrt( (master_nav.x - sv_positions[lcv].x)*(master_nav.x - sv_positions[lcv].x) +
												   (master_nav.y - sv_positions[lcv].y)*(master_nav.y - sv_positions[lcv].y) +
												   (master_nav.z - sv_positions[lcv].z)*(master_nav.z - sv_positions[lcv].z) );
			sv_positions[lcv].transit_time *= INVERSE_SPEED_OF_LIGHT;
		}

	}


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::SV_Correct()
{

	double x_n, y_n, cw, sw;
	int32 lcv;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{

		if(good_channels[lcv])
		{
			cw = cos(WGS84OE * sv_positions[lcv].transit_time);
			sw = sin(WGS84OE * sv_positions[lcv].transit_time);
			x_n = cw * sv_positions[lcv].x + sw * sv_positions[lcv].y;
			y_n = -sw * sv_positions[lcv].x + cw * sv_positions[lcv].y;

			sv_positions[lcv].x = x_n;
			sv_positions[lcv].y = y_n;
		}

	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::SV_Elevations()
{

	int32 lcv;
	float dx, dy, dz;
	float ct, cp;
	float st, sp;
	float theta;
	float phi;
	float rho;
	float e, n, u;
	float radius;

	theta = master_nav.longitude;
	phi = master_nav.latitude;

	ct = cos(theta); st = sin(theta);
	cp = cos(phi);   sp = sin(phi);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			radius = 1.0 / pseudoranges[lcv].meters;

			dx = (sv_positions[lcv].x - master_nav.x) * radius;
			dy = (sv_positions[lcv].y - master_nav.y) * radius;
			dz = (sv_positions[lcv].z - master_nav.z) * radius;

			e = -st*dx    +  ct*dy;
			n = -sp*ct*dx + -sp*st*dy + cp*dz;
			u =  cp*ct*dx +  cp*st*dy + sp*dz;

			rho = sqrt(n*n + e*e);
			sv_positions[lcv].elev = atan2(u, rho);
			sv_positions[lcv].azim = atan2(e, n);
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::PseudoRange()
{
	int32 lcv;
	double cp;
	double cp_prev;
	double cp_adjust;
	double code_time;
	double code_phase;
	double time;
	double time_rate;
	double ctime_rate;

	cp_adjust = (double)MEASUREMENTS_PER_SECOND/(double)(2*ICP_TICS);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{

			/* Calculate the code phase */
			code_phase =  (double)(measurements[lcv].code_phase)*1.0;
			code_phase += (double)(measurements[lcv].frac_code_phase)*TWO_N31;

			/* Calculate the total code time */
			code_time = code_phase * INVERSE_CODE_RATE;
			code_time += (double)(measurements[lcv]._20ms_epoch * .02);
			code_time += (double)(measurements[lcv]._1ms_epoch * .001);

			/* Clear out the residual */
			pseudoranges[lcv].residual = 0;

			/* Time of transmission is calculated directly tracking channel */
			time = master_clock.time - code_time - (double)measurements[lcv].subframe_sec;
			pseudoranges[lcv].meters = time*(double)SPEED_OF_LIGHT;

			/* GEONS wants raw measurements, not ones that have the GPS clock bias added in */
			time = master_clock.time_raw - code_time - (double)measurements[lcv].subframe_sec;
			pseudoranges[lcv].uncorrected = time*(double)SPEED_OF_LIGHT;

			/* Pseudorange RATE from integrated carrier phase */
			cp_prev = (double)measurements[lcv].carrier_phase_prev + (double)measurements[lcv].frac_carrier_phase_prev*(double)TWO_N32;
			cp      = (double)measurements[lcv].carrier_phase      + (double)measurements[lcv].frac_carrier_phase*(double)TWO_N32;
			time_rate = (cp_prev - cp)*cp_adjust + (double)ZERO_DOPPLER_RATE; //!< Note the sign flip
			pseudoranges[lcv].meters_rate = time_rate * (double)C_OVER_L1;
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 PVT::PreErrorCheck()
{
	int32 lcv;
	int32 num_edited_channels = 0;
	int32 nav_channels;
	double radius;

	/* First filter out cross correlations */
	//ErrorCheckCrossCorr();

	/* Channel by channel resets */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			/* First check SV positions */
			radius = sqrt(sv_positions[lcv].x*sv_positions[lcv].x + sv_positions[lcv].y*sv_positions[lcv].y + sv_positions[lcv].z*sv_positions[lcv].z);

			if((radius < 2.4e7) || (radius > 2.9e7))
			{
				sv_codes[lcv] = PVT_ERROR_POSITION;
				good_channels[lcv] = false;
				num_edited_channels++;
				continue;
			}

			/* Check for isnan velocities */
			if(isnan(sv_positions[lcv].vx) || isnan(sv_positions[lcv].vy) || isnan(sv_positions[lcv].vz))
			{
				sv_codes[lcv] = PVT_ERROR_VELOCITY;
				good_channels[lcv] = false;
				num_edited_channels++;
				continue;
			}
		}
	}

	/* Recompute number of good channels */
	nav_channels = 0;
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		if(good_channels[lcv])
			nav_channels++;

	if(nav_channels > 3)
		return(true);
	else
		return(false);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::ErrorCheckCrossCorr()
{

	int32 lcv, lcv2, cross, icn0;
	float fcn0[MAX_CHANNELS];
	double a, in0, ecc;

	/* Get CN0s real quickly */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			icn0 = pChannels[lcv]->getCN0();
			fcn0[lcv] = icn0;
			//fcn0[lcv] = icn0_2_fcn0(icn0);
		}
	}

	/* Channel by channel resets */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv] && (fcn0[lcv] > 40.0))
		{

			a = ephemerides[lcv].a;
			in0 = ephemerides[lcv].in0;
			ecc = ephemerides[lcv].ecc;

			for(lcv2 = 0; lcv2 < MAX_CHANNELS; lcv2++)
			{
				if(lcv2 == lcv)
					continue;

				if(good_channels[lcv2] && (fcn0[lcv2] < 40.0))
				{
					cross = false;

					if(ephemerides[lcv2].a == a)
						cross = true;

					if(ephemerides[lcv2].in0 == in0)
						cross = true;

					if(ephemerides[lcv2].ecc == ecc)
						cross = true;

					/* Detected cross-correlation, dump SV with lower CN0, kill channel, mark as bad measurement */
					if(cross)
					{
						good_channels[lcv2] = false;
						pEphemeris->Lock();
						pEphemeris->ClearEphemeris(master_sv[lcv2]);
						pEphemeris->Unlock();
						//Kill_channel[lcv2] = 1;
					}
				}
			}
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::FormModel()
{

	int32 lcv;
	double relvel, range;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{

		if(good_channels[lcv])
		{
			range =	sqrt( 	(temp_nav.x - sv_positions[lcv].x)*(temp_nav.x - sv_positions[lcv].x) +
							(temp_nav.y - sv_positions[lcv].y)*(temp_nav.y - sv_positions[lcv].y) +
					   		(temp_nav.z - sv_positions[lcv].z)*(temp_nav.z - sv_positions[lcv].z) );

			pseudorangeres[lcv] = pseudoranges[lcv].meters - (range + temp_nav.clock_bias - sv_positions[lcv].clock_bias*SPEED_OF_LIGHT);

			range = 1.0 / range;

			dircos[lcv][0] = (temp_nav.x - sv_positions[lcv].x) * range;
			dircos[lcv][1] = (temp_nav.y - sv_positions[lcv].y) * range;
			dircos[lcv][2] = (temp_nav.z - sv_positions[lcv].z) * range;
			dircos[lcv][3] = 1.0;

			relvel	 = 			dircos[lcv][0] * (temp_nav.vx - sv_positions[lcv].vx) +
				      			dircos[lcv][1] * (temp_nav.vy - sv_positions[lcv].vy) +
				      			dircos[lcv][2] * (temp_nav.vz - sv_positions[lcv].vz);

		    pseudorangerateres[lcv] = pseudoranges[lcv].meters_rate - (relvel +  temp_nav.clock_rate  - sv_positions[lcv].frequency_bias*SPEED_OF_LIGHT);

		} //end if good_channels

	}//end for

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
double PVT::PVT_Estimation()
{

	int32 lcv, lcv2, lcv3;
	int32 inv, nav_channels;
	double D[MAX_CHANNELS];
	double L[MAX_CHANNELS];
	double sum, dt;

	nav_channels = 0;
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			L[nav_channels] = pseudorangeres[lcv];
			D[nav_channels] = pseudorangerateres[lcv];
			alpha[nav_channels] = dircos[lcv];
			nav_channels++;
		}
	}

	/* Form alpha_2 = A'A */
	for(lcv = 0; lcv < 4; lcv++)
	{
		for(lcv2 = 0; lcv2 < 4; lcv2++)
		{
			sum = 0;
			for(lcv3 = 0; lcv3 < nav_channels; lcv3++)
				sum += alpha[lcv3][lcv]*alpha[lcv3][lcv2];
			alpha_2[lcv][lcv2] = sum;
		}
	}

	/* Invert alpha_2 */
	inv = Invert4x4(alpha_2, alpha_inv);

	/* alpha_pinv = inv(A'A)*A' */
	for(lcv = 0; lcv < 4; lcv++)
	{
		for(lcv2 = 0; lcv2 < nav_channels; lcv2++)
		{
			sum = 0;
			for(lcv3 = 0; lcv3 < 4; lcv3++)
				sum += alpha_inv[lcv][lcv3]*alpha[lcv2][lcv3];
			alpha_pinv[lcv][lcv2] = sum;
		}
	}

	/* Estimate Postion and Clock Bias Updates */
	//dR = A_pinv*L;
	for(lcv = 0; lcv < 4; lcv++)
	{
		sum = 0;
		for(lcv2 = 0; lcv2 < nav_channels; lcv2++)
			sum += alpha_pinv[lcv][lcv2]*L[lcv2];
		dr[lcv] = sum;
	}

	/* Update Postion and Clock Bias */
	temp_nav.x += dr[0];
	temp_nav.y += dr[1];
	temp_nav.z += dr[2];
	temp_nav.clock_bias += dr[3];

	dt = dr[3];

	/* Estimate Velocity and Clock Rate Updates	 */
	//dR = A_pinv*D
	for(lcv = 0; lcv < 4; lcv++)
	{
		sum = 0;
		for(lcv2 = 0; lcv2 < nav_channels; lcv2++)
			sum += alpha_pinv[lcv][lcv2]*D[lcv2];
		dr[lcv] = sum;
	}

	/* Update Velocity and Clock Rate */
	temp_nav.vx += dr[0];
	temp_nav.vy += dr[1];
	temp_nav.vz += dr[2];
	temp_nav.clock_rate += dr[3];

	return(dt);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 PVT::PostErrorCheck()
{

	/* Catch any serious errors */
	if(isnan(temp_nav.x) || isnan(temp_nav.y) || isnan(temp_nav.z))
		return(false);

	if(isnan(temp_nav.vx) || isnan(temp_nav.vy) || isnan(temp_nav.vz))
		return(false);

	if(isinf(temp_nav.x) || isinf(temp_nav.y) || isinf(temp_nav.z))
		return(false);

	if(isinf(temp_nav.vx) || isinf(temp_nav.vy) || isinf(temp_nav.vz))
		return(false);

	/* Outside of 100 earth radii */
	if(fabs(temp_nav.x) > WGS84_MAJOR_AXIS*100.0)
		return(false);

	if(fabs(temp_nav.y) > WGS84_MAJOR_AXIS*100.0)
		return(false);

	if(fabs(temp_nav.z) > WGS84_MAJOR_AXIS*100.0)
		return(false);

	/* Faster than 50 km/s */
	if(fabs(temp_nav.vx) > 5e4)
		return(false);

	if(fabs(temp_nav.vy) > 5e4)
		return(false);

	if(fabs(temp_nav.vz) > 5e4)
		return(false);

	/* Compute residuals */
	Residuals();

	/* Check for convergence */
	if(Converged())
	{
		/* Else copy over to master nav */
		master_nav.x = temp_nav.x;
		master_nav.y = temp_nav.y;
		master_nav.z = temp_nav.z;
		master_nav.clock_bias = temp_nav.clock_bias;

		master_nav.vx = temp_nav.vx;
		master_nav.vy = temp_nav.vy;
		master_nav.vz = temp_nav.vz;
		master_nav.clock_rate = temp_nav.clock_rate;

		return(true);
	}
	else
		return(false);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Residuals()
{

	int32 lcv;
	double range, dx, dy, dz, relvel;

	/* Pseudorange Residuals */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			range      =  	sqrt(	(temp_nav.x - sv_positions[lcv].x)*(temp_nav.x - sv_positions[lcv].x) +
						   			(temp_nav.y - sv_positions[lcv].y)*(temp_nav.y - sv_positions[lcv].y) +
						   			(temp_nav.z - sv_positions[lcv].z)*(temp_nav.z - sv_positions[lcv].z)	);


			pseudoranges[lcv].residual = pseudoranges[lcv].meters - (range + temp_nav.clock_bias - sv_positions[lcv].clock_bias*SPEED_OF_LIGHT);

			range = 1.0 / range;

			dx = (temp_nav.x - sv_positions[lcv].x) * range;
			dy = (temp_nav.y - sv_positions[lcv].y) * range;
			dz = (temp_nav.z - sv_positions[lcv].z) * range;

			relvel	 = 	dx * (temp_nav.vx - sv_positions[lcv].vx) +
				      			dy * (temp_nav.vy - sv_positions[lcv].vy) +
				      			dz * (temp_nav.vz - sv_positions[lcv].vz);

			pseudoranges[lcv].residual_rate = pseudoranges[lcv].meters_rate - (relvel + temp_nav.clock_rate - sv_positions[lcv].frequency_bias*SPEED_OF_LIGHT);

		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 PVT::Converged()
{
	int32 lcv;
	double k = 0.0;
	double residual_avg = 0.0;

	/* Check residuals? */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			residual_avg += fabs(pseudoranges[lcv].residual);
			k += 1.0;
		}
	}

	if(k > 0.0)
	{
		residual_avg /= k;

		if(residual_avg < 2000.0)
		{
			temp_nav.converged = true;
		}
		else
		{
			temp_nav.converged = false;
		}
	}
	else
	{
		temp_nav.converged = false;
	}

	return(temp_nav.converged);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::LatLong()
{

    //xyzzy - these constants be declared const
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

    p           = sqrt(master_nav.x*master_nav.x + master_nav.y*master_nav.y);

    theta       = atan( (master_nav.z*a)/ (p*b) );

    sin_theta   = sin( theta ); // cache repeated call
    cos_theta   = cos( theta ); // cache repeated call

    latitude    = atan( (master_nav.z + eprime2*b*sin_theta*sin_theta*sin_theta) /
                        (p - e2*a*cos_theta*cos_theta*cos_theta)  );

    sin_latitude = sin( latitude ); // cache repeated call
    cos_latitude = cos( latitude ); // cache repeated call

    longitude   = atan2( master_nav.y/p, master_nav.x/p);

    N           = a_sqrd / sqrt(a_sqrd*cos_latitude*cos_latitude + b_sqrd*sin_latitude*sin_latitude);

    altitude    = p / cos_latitude - N;

    // save results
    master_nav.latitude  = latitude;
    master_nav.longitude = longitude;
    master_nav.altitude  = altitude;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::DOP()
{
	int32 lcv, lcv2, nav_channels;

	double gdop, pdop, tdop;

	nav_channels = 0;
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			nav_channels++;
		}
	}

	/* Position DOP */
	pdop = 0.0;
	for (lcv = 0; lcv < 3; lcv++)
		for (lcv2 = 0; lcv2 < nav_channels; lcv2++)
			pdop += alpha_pinv[lcv][lcv2]*alpha_pinv[lcv][lcv2];

	/* Time DOP */
	tdop = 0.0;
	for (lcv2 = 0; lcv2 < nav_channels; lcv2++)
		tdop += alpha_pinv[3][lcv2]*alpha_pinv[3][lcv2];

	/* GDOP */
	gdop = pdop + tdop;

	master_nav.gdop = sqrt(gdop);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Reset()
{

	int32 lcv;

	/* Reset to center of the earth	 */
	memset(&master_clock,0x0,sizeof(Clock_M));
	memset(&master_nav,0x0,sizeof(SPS_M));
	memset(&temp_nav,0x0,sizeof(SPS_M));

	/* Reset Each Channel */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		ResetChannel(lcv);

	master_nav.stale_ticks = STALE_SPS_VALUE;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::ZeroNav()
{

	master_nav.x = 0;
	master_nav.y = 0;
	master_nav.z = 0;
	master_nav.vx = 0;
	master_nav.vy = 0;
	master_nav.vz = 0;
	master_nav.clock_bias = 0;
	master_nav.clock_rate = 0;
	master_nav.time = 0;
	master_nav.latitude = 0;
	master_nav.longitude = 0;
	master_nav.altitude = 0;
	master_nav.gdop = 0;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::ResetChannel(int32 _chan)
{

	/* Reset master SV */
	master_sv[_chan] = NON_EXISTENT_SV;

	/* Reset Error Codes */
	sv_codes[_chan] = PVT_ERROR_INACTIVE;

	/* Zero out data associated with SV */
	memset(&measurements[_chan], 0x0, sizeof(Measurement_M));
	memset(&sv_positions[_chan], 0x0, sizeof(SV_Position_M));
	memset(&pseudoranges[_chan], 0x0, sizeof(Pseudorange_M));
	memset(&ephemerides[_chan],  0x0, sizeof(Ephemeris_M));

	good_channels[_chan] = false;

}
/*----------------------------------------------------------------------------------------------*/

