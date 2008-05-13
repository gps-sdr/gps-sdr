/*! \file PVT.h
	Implements member functions of PVT class to  navigation
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

#include "pvt.h"

/*----------------------------------------------------------------------------------------------*/
void *PVT_Thread(void *_arg)
{
	
	PVT *aPVT = pPVT;
	int32 key;
	
	while(grun)
	{
		aPVT->Inport();
		aPVT->Navigate();
		aPVT->Export();
	}
	
	pthread_exit(0);
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Start()
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
	param.sched_priority = PVT_PRIORITY;
	
	/* Setting the new scheduling param */
	ret = pthread_attr_setschedparam(&tattr, &param);
	ret = pthread_attr_setschedpolicy(&tattr, SCHED_FIFO);
	
	/* With new priority specified */
	pthread_create(&thread, NULL, PVT_Thread, NULL);
	
	if(gopt.verbose)
		printf("PVT thread started\n");	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Stop()
{
	pthread_join(thread, NULL);
	
	if(gopt.verbose)
		printf("PVT thread stopped\n");		
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
PVT::PVT()
{
	
	Reset();
	
	//master_clock.time0 = GPSTime();
		
	if(gopt.verbose)	
		printf("Creating PVT\n");
	
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
void PVT::Inport()
{
	int32 success, lcv, lcv2;
	int32 num_chans;	
	int32 bytes_read;
	int32 messagesize;
	int32 sv, chan, bread;
	Measurement_S temp;

	/* Get number of channels coming off the pipe */	
	read(FIFO_2_PVT_P[READ], &telem, sizeof(FIFO_2_Telem_S));
	
	nav_channels = 0;
	
	/* Set good_channels to false */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		good_channels[lcv] = false;
		measurements[lcv].navigate = false; //important!
	}

	/* Initial set of Nav Channels, gets refined in Error_Check() */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		read(Corr_2_PVT_P[lcv][READ], &temp, sizeof(Measurement_S));
		
		if(temp.navigate == true)
		{
			sv_codes[lcv] = NOMINAL;
			good_channels[lcv] = true;
			nav_channels++;
					
			if(master_sv[lcv] == 666)			/* This SV has not been assigned to a channel */
			{
				master_sv[lcv] = temp.sv;
				measurements[lcv] = temp;
			}
			else if(master_sv[lcv] != temp.sv)	/* The SV on this channel has changed */
			{			
				Reset(lcv);
				master_sv[lcv] = temp.sv;
				measurements[lcv] = temp;			
			} 
			else 								/* Everything is good just update the measurement */
			{
				measurements[lcv] = temp;
			}
		}
		else
			Reset(lcv);
	}
	
	/* Do these quick */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{	
		if(good_channels[lcv])
		{	
			/* Calculate the total code time */
			measurements[lcv].code_time = (measurements[lcv].code_phase_mod/CODE_RATE) +	
										(double)(measurements[lcv]._20ms_epoch * .02) +
										(double)(measurements[lcv]._1ms_epoch * .001);
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Export()
{
	int32 lcv;
	
	master_nav.tic = telem.tic;
	
	master_nav.nsvs = 0;
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv] && ephemerides[lcv].valid)
		{
			master_nav.nsvs += (0x1 << lcv);
		}
		master_nav.chanmap[lcv] = ephemerides[lcv].valid;
	}
	
	memcpy(&output.master_nav,   &master_nav,   sizeof(Nav_Solution_S));
	memcpy(&output.master_clock, &master_clock, sizeof(Clock_S));
	memcpy(&output.sv_positions, &sv_positions, MAX_CHANNELS*sizeof(SV_Position_S));	
	memcpy(&output.pseudoranges, &pseudoranges, MAX_CHANNELS*sizeof(Pseudorange_S));
	memcpy(&output.measurements, &measurements, MAX_CHANNELS*sizeof(Measurement_S));
	
	write(PVT_2_Telem_P[WRITE], &output, sizeof(PVT_2_Telem_S));
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Navigate()
{

	int32 iter;

	Update_Time();
	Get_Ephemerides();	
	
	/* Either completely initialize the clock or just RCObias */	
	if(master_clock.state == CLOCK_UNINITIALIZED)
		ClockInit();
				
	SV_Positions();
	SV_TransitTime();		
	SV_Correct();
	SV_Elevations();
	PseudoRange();
	FormModel();

	/* Make sure everything is in order, if not nav_channels < 3 and solution wont be updated */
	ErrorCheck();
	ErrorCheckCrossCorr();		

	if(nav_channels >= 4)
	{
		
		/* Ummm Yeah, you need to do the point solution */
		PVT_Estimation();
		FormModel();
		PVT_Estimation();
		FormModel();
		PVT_Estimation();
		FormModel();
		PVT_Estimation();
		
		/* If 5 SVs are available do a RAIM PVT solution */				
//		if(nav_channels >= 5)
//		{
//			Raim();
//		}
//		else
//		{				
//			FormModel();
//			PVT_Estimation();
//		}
				
		/* If we make it through the iterations increment nav_ticks */
		nav_ticks++;
		
		LatLong();
		DOP();
		Converged();
		Residuals();
		ClockUpdate();
		
		
	}
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Update_Time()
{
		
	master_clock.receiver_time	+= MEASUREMENT_INT*.001;			      
	master_clock.time_raw 		= master_clock.time0 + master_clock.receiver_time;
	master_clock.time 			= master_clock.time_raw - master_clock.bias;      
					      
	/* Week Rollover ! */
	if(master_clock.time > SECONDS_IN_WEEK)	
	{	
	
		master_clock.week += 1.0;			      
		master_clock.time0 -= SECONDS_IN_WEEK;
		
		//recompute these
		master_clock.time_raw 	= master_clock.time0 + master_clock.receiver_time;
		master_clock.time 		= master_clock.time_raw - master_clock.bias;      
	}
			      
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::ClockUpdate()
{

	/* Update GPStime for current iteration (for use in PPS timer block) */
	master_clock.time 	-= master_nav.clock_bias/SPEED_OF_LIGHT; 
											    	
	/* Update components for next iteration */
	master_clock.bias 	+= master_nav.clock_bias/SPEED_OF_LIGHT;
	master_clock.rate 	 = master_nav.clock_rate;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Get_Ephemerides()
{

	int32 lcv;
	int32 temp_iode, sv;

	/* Protect the Ephemeris object with a mutex */
	pEphemeris->Lock();
	
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])		
		{
			sv = master_sv[lcv];
			temp_iode = pEphemeris->getIODE(sv);
		 	if(temp_iode != master_iode[lcv]) /* Catch a ephemeris change */
			{
				ephemerides[lcv] = pEphemeris->getEphemeris(sv);
				if(ephemerides[lcv].valid)
				{
					master_iode[lcv] = temp_iode;
				}
			}
			
		} /* if good_channel */
	}/* for channels */

	pEphemeris->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::ClockInit()
{

	int32 lcv;
	double time_of_transmission;
	
	/* Initialize the clock to the time-of-transmission of the first GPS signal that we find, this is accurate to within the transit time */
	if(master_clock.state == CLOCK_UNINITIALIZED)
		for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
			if(good_channels[lcv] && ephemerides[lcv].valid)
			{
					time_of_transmission		= (double)measurements[lcv]._z_count + measurements[lcv].code_time;
					master_clock.time0 			= time_of_transmission; /* Best guess of GPS time */
					master_clock.receiver_time 	= 0;
					master_clock.rate 			= 0;
					master_clock.bias 			= 0;
					master_clock.time 			= time_of_transmission;
					master_clock.time_raw		= time_of_transmission;
					master_clock.week 			= ephemerides[lcv].week_number;
					master_clock.state 			= CLOCK_NOMINAL;
					break;
			}
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::SV_Positions()
{

	int32 lcv;
	int32 iter;
	double dtemp, M, E, cE, sE, dEdM, P, U, R, I, cU, sU, Xp, Yp, L, sI, cI, sL, cL, ecc, s2P, c2P, svirel, toc;
	double Edot, Pdot, Udot, Rdot, sUdot, cUdot, Xpdot, Ypdot, Idot, Ldot;
	double Mdot, sqrt1mee;
	double clock_corr;
	double tk_p_toe, toe;
	double dtk;
	double tk;

	double whole_sec;
	double partial_sec;
	
	Ephemeris_S* ephem;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv] && ephemerides[lcv].valid)
		{
			ephem = &ephemerides[lcv];
			
			/* Compute the difference, in seconds, between the epoch and wk & sec. */ 
			toe = (double) ephem->toe;
					 	
			/* Time of transmission is calculated directly tracking channel */
			partial_sec = measurements[lcv].code_time - sv_positions[lcv].clock_bias;
			whole_sec = (double)measurements[lcv]._z_count;

			dtk = partial_sec + whole_sec;

			tk = (double)(dtk - (double)toe);
			tk_p_toe = (double)(dtk);

			if (tk > HALF_OF_SECONDS_IN_WEEK)
				tk -= SECONDS_IN_WEEK;
			else if (tk < (-HALF_OF_SECONDS_IN_WEEK))
				tk += SECONDS_IN_WEEK;

			//Mean anomaly, M (rads). 
			Mdot = ephem->n0 + ephem->deltan;
			M = ephem->m0 + Mdot * tk;

			// Obtain eccentric anomaly E by solving Kepler's equation. 
			ecc = ephem->ecc;
						
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

			// Compute the relativistic correction term (seconds). 
			ephem->relativistic = (double)(-4.442807633E-10) * ecc * ephem->sqrta * sE;

			Edot = dEdM * Mdot;

			// Compute the argument of latitude, P. 
			P = atan2 (sqrt1mee * sE, cE - ecc) + ephem->argp;
			Pdot = sqrt1mee * dEdM * Edot;

			// Generate harmonic correction terms for P and R. 
			s2P = sin (2.0 * P);
			c2P = cos (2.0 * P);

			// Compute the corrected argument of latitude, U. 
			U = P + (ephem->cus * s2P + ephem->cuc * c2P);
			sU = sin (U);
			cU = cos (U);
			Udot = Pdot * (1.0 + 2.0 * (ephem->cus * c2P - ephem->cuc * s2P));
			sUdot = cU * Udot;
			cUdot = -sU * Udot;

			// Compute the corrected radius, R. 
			R = ephem->a * (1.0 - ecc * cE) + (ephem->crs * s2P + 
				ephem->crc * c2P);
			Rdot = ephem->a * ecc * sE * Edot + 2.0 * Pdot
				* (ephem->crs * c2P - ephem->crc * s2P);

			// Compute the corrected orbital inclination, I. 
			I = ephem->in0 + ephem->idot * tk
			+ (ephem->cis * s2P + ephem->cic * c2P);
			sI = sin (I);
			cI = cos (I);
			Idot = ephem->idot + 2.0 * Pdot * (ephem->cis * c2P - ephem->cic * s2P);

			// Compute the satellite's position in its orbital plane, (Xp,Yp). 
			Xp = R * cU;
			Yp = R * sU;
			Xpdot = Rdot * cU + R * cUdot;
			Ypdot = Rdot * sU + R * sUdot;

			// Compute the longitude of the ascending node, L. 
			L = ephem->om0 + tk * (ephem->omd - (double)WGS84OE) - (double)WGS84OE * ephem->toe;
			Ldot = ephem->omd - (double)WGS84OE;
			sL = sin (L);
			cL = cos (L);

			// Compute the satellite's position in space, (x,y,z). 
			sv_positions[lcv].x = Xp * cL - Yp * cI * sL;
			sv_positions[lcv].y = Xp * sL + Yp * cI * cL;
	        sv_positions[lcv].z = Yp * sI;
	        
	        
	        // Compute SV clock correction
			sv_positions[lcv].time = tk;
			toc = ephem->toc;
			
			
			sv_positions[lcv].clock_bias = ephem->af0 + 
													(ephem->af1 *(tk_p_toe - toc)) + 
													(ephem->af2 *(tk_p_toe - toc)*(tk_p_toe - toc)) + 
													ephem->relativistic; 

			sv_positions[lcv].clock_bias -= ephem->tgd;
			
			sv_positions[lcv].frequency_bias = ephem->af1 + (ephem->af2 *(tk_p_toe - toc)*2.0);


			// Satellite's velocity, (vx,vy,vz). 
			sv_positions[lcv].vx = -Ldot * (sv_positions[lcv].y)
			+ Xpdot * cL
			- Ypdot * cI * sL
			+ Yp * sI * Idot * sL;

			sv_positions[lcv].vy = Ldot * (sv_positions[lcv].x)
			+ Xpdot * sL
			+ Ypdot * cI * cL
			- Yp * sI * Idot * cL;

			sv_positions[lcv].vz = +Yp * cI * Idot
			+ Ypdot * sI; 
				
		} //end if good channel
		else
		{
			sv_positions[lcv].vx = 0;
			sv_positions[lcv].vy = 0;
        	sv_positions[lcv].vz = 0;			
			sv_positions[lcv].x = 0;
			sv_positions[lcv].y = 0;
        	sv_positions[lcv].z = 0;
			sv_positions[lcv].elev = 0;
			sv_positions[lcv].azim = 0;
			sv_positions[lcv].clock_bias = 0;
			sv_positions[lcv].frequency_bias = 0;
			sv_positions[lcv].transit_time = 0;
			sv_positions[lcv].time = 0;
		}
		
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
		if(good_channels[lcv] && ephemerides[lcv].valid)
		{
			sv_positions[lcv].transit_time = sqrt( (master_nav.x - sv_positions[lcv].x)*(master_nav.x - sv_positions[lcv].x) + 
												   (master_nav.y - sv_positions[lcv].y)*(master_nav.y - sv_positions[lcv].y) + 
												   (master_nav.z - sv_positions[lcv].z)*(master_nav.z - sv_positions[lcv].z) );
			sv_positions[lcv].transit_time /= SPEED_OF_LIGHT;
		}
		
	}


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::SV_Correct()
{

	double x_n, y_n, cw, sw;
	double timelag;
	int32 lcv;


	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{

		if(good_channels[lcv] && ephemerides[lcv].valid)
		{
			cw = cos(WGS84OE * sv_positions[lcv].transit_time );
			sw = sin(WGS84OE * sv_positions[lcv].transit_time );
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
	float elev;
	float azim;
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
	
		if(good_channels[lcv] && ephemerides[lcv].valid)
		{
			radius = pseudoranges[lcv].time*SPEED_OF_LIGHT;	
			dx = (sv_positions[lcv].x - master_nav.x)/radius;
			dy = (sv_positions[lcv].y - master_nav.y)/radius;
			dz = (sv_positions[lcv].z - master_nav.z)/radius;

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

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv] && ephemerides[lcv].valid)
		{
			/* Clear out the residual */
			pseudoranges[lcv].residual = 0; 
				
			/* Time of transmission is calculated directly tracking channel */
			pseudoranges[lcv].time = master_clock.time - measurements[lcv].code_time - (double)measurements[lcv]._z_count;
				
			/* Convert to meters */
			pseudoranges[lcv].meters = pseudoranges[lcv].time*(double)SPEED_OF_LIGHT;	
				
			/* Raw measurements, not ones that have the GPS clock bias added in */
			pseudoranges[lcv].time_uncorrected = measurements[lcv].code_time + (double)measurements[lcv]._z_count;
						
			/* Pseudorange RATE from instantaneous Doppler (NOISY) */
//			pseudoranges[lcv].time_rate = -((double)(measurements[lcv].carrier_nco) - IF_FREQUENCY);
			
			/* Pseudorange RATE from integrated carrier phase (LESS NOISY) */
			pseudoranges[lcv].time_rate = measurements[lcv].carrier_phase_prev - measurements[lcv].carrier_phase; //Must switch signs
			pseudoranges[lcv].time_rate *=  (double)(1000/MEASUREMENT_INT); //Convert cycles/meas period to cycles/sec
			pseudoranges[lcv].time_rate += (double) IF_FREQUENCY;

			/* Convert rate to meters */
			pseudoranges[lcv].meters_rate = SPEED_OF_LIGHT * pseudoranges[lcv].time_rate / L1;
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::ErrorCheckCrossCorr()
{
	int32 lcv, lcv2, cross;
	double a, in0, ecc;
	
	/* Channel by channel resets */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv] && (pChannels[lcv]->getCN0() > 45.0))
		{
			
			a = ephemerides[lcv].a;
			in0 = ephemerides[lcv].in0;
			ecc = ephemerides[lcv].ecc;
			
			for(lcv2 = 0; lcv2 < MAX_CHANNELS; lcv2++)
			{
				if(lcv2 == lcv)
					continue;
					
				if(good_channels[lcv2] && (pChannels[lcv2]->getCN0() < 40.0))
				{
					cross = false;
					
					if(ephemerides[lcv2].a == a)
						cross = true;		
					
					if(ephemerides[lcv2].in0 == in0) 					
						cross = true;
				
					if(ephemerides[lcv2].ecc == ecc)
						cross = true;
						
					/* Detected cross-correlation, dump SV with lower CN0, kill channel */
					if(cross)
					{
						pEphemeris->Lock();
						pEphemeris->ClearSV(master_sv[lcv2]);
						pEphemeris->Unlock();
						pChannels[lcv2]->Stop();
					}
				}
			}
		}
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::ErrorCheck()
{
	double radius;
	int32 lcv, good_chan;
	int32 num_edited_channels = 0;

	/* Took out the Reset(lcv) if an error is caught, we want to be able to inspect the bad data! */

	/* Channel by channel resets */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
		
			/* Make sure the ephemeris has been decoded	- need several checks here to be reasonably sure */
			if(ephemerides[lcv].valid != 1)
			{
				sv_codes[lcv] = EPHEM_ERR;
				good_channels[lcv] = false;
				num_edited_channels++;
				continue;
			}

			/* First check SV positions */
			radius = sqrt(sv_positions[lcv].x*sv_positions[lcv].x +
						  sv_positions[lcv].y*sv_positions[lcv].y +
						  sv_positions[lcv].z*sv_positions[lcv].z);

			if((radius < 2.4e7) || (radius > 2.9e7))
			{
				sv_codes[lcv] = POS_ERR;
				good_channels[lcv] = false;
				num_edited_channels++;
				continue;
			}
			
			
			if(isnan(sv_positions[lcv].vx) || isnan(sv_positions[lcv].vy) || isnan(sv_positions[lcv].vz))
			{
				sv_codes[lcv] = POS_ERR;
				good_channels[lcv] = false;
				num_edited_channels++;
				continue;
			}
					
		}
	}
	
	/* Recompute number of good channels */
	nav_channels = 0;
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
			nav_channels++;
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
			range =	sqrt( 	(master_nav.x - sv_positions[lcv].x)*(master_nav.x - sv_positions[lcv].x) +  		
							(master_nav.y - sv_positions[lcv].y)*(master_nav.y - sv_positions[lcv].y) + 
					   		(master_nav.z - sv_positions[lcv].z)*(master_nav.z - sv_positions[lcv].z) );
				
			pseudorangeres[lcv] = pseudoranges[lcv].meters - (range + master_nav.clock_bias - sv_positions[lcv].clock_bias*SPEED_OF_LIGHT);

			dircos[lcv][0] = (master_nav.x - sv_positions[lcv].x)/range;
			dircos[lcv][1] = (master_nav.y - sv_positions[lcv].y)/range;
			dircos[lcv][2] = (master_nav.z - sv_positions[lcv].z)/range;
			dircos[lcv][3] = 1.0;	
			
			relvel	 = 			dircos[lcv][0] * (master_nav.vx - sv_positions[lcv].vx) + 
				      			dircos[lcv][1] * (master_nav.vy - sv_positions[lcv].vy) +
				      			dircos[lcv][2] * (master_nav.vz - sv_positions[lcv].vz);  

		    pseudorangerateres[lcv] = pseudoranges[lcv].meters_rate - (relvel +  master_nav.clock_rate); //sv clock rate is negligible
		
		} //end if good_channels
	
	}//end for 
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::PVT_Estimation()
{

	int32 lcv, k;
	int32 i, j, Inv;
	double D[MAX_CHANNELS];
	double L[MAX_CHANNELS];
	double sum;
	 
	k = 0;
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{	
			L[k] = pseudorangeres[lcv];
			D[k] = pseudorangerateres[lcv];
			alpha[k] = dircos[lcv];
			k++;
		}
	}

	/* Form alpha_2 = A'A */
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			sum = 0;
			for(k = 0; k < nav_channels; k++)
				sum += alpha[k][i]*alpha[k][j];
			alpha_2[i][j] = sum;
		}
	}

	/* Invert alpha_2 */
	Inv = Invert4x4(alpha_2, alpha_inv);
		
	/* alpha_pinv = inv(A'A)*A' */
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < nav_channels; j++)
		{
			sum = 0;
			for(k = 0; k < 4; k++)
				sum += alpha_inv[i][k]*alpha[j][k];
			alpha_pinv[i][j] = sum;
		}
	}
	
	/* Estimate Postion and Clock Bias Updates */
	//dR = A_pinv*L;
	for(i = 0; i < 4; i++)
	{
		sum = 0;
		for(k = 0; k < nav_channels; k++)
			sum += alpha_pinv[i][k]*L[k];
		dr[i] = sum;
	}
	
	/* Update Postion and Clock Bias */
	master_nav.x += dr[0];
	master_nav.y += dr[1];
	master_nav.z += dr[2];
	master_nav.clock_bias += dr[3];
		
	/* Estimate Velocity and Clock Rate Updates	 */
	//dR = A_pinv*D
	for(i = 0; i < 4; i++)
	{
		sum = 0;
		for(k = 0; k < nav_channels; k++)
			sum += alpha_pinv[i][k]*D[k];
		dr[i] = sum;
	}
	
	/* Update Velocity and Clock Rate */
	master_nav.vx += dr[0];
	master_nav.vy += dr[1];
	master_nav.vz += dr[2];
	master_nav.clock_rate += dr[3];

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Residuals()
{

	int32 lcv;
	double range;
	
	/* Pseudorange Residuals */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			range      =  	sqrt(	(master_nav.x - sv_positions[lcv].x)*(master_nav.x - sv_positions[lcv].x) +  		
						   			(master_nav.y - sv_positions[lcv].y)*(master_nav.y - sv_positions[lcv].y) + 
						   			(master_nav.z - sv_positions[lcv].z)*(master_nav.z - sv_positions[lcv].z)	);

				pseudoranges[lcv].residual = pseudoranges[lcv].meters - (range + master_nav.clock_bias - sv_positions[lcv].clock_bias*SPEED_OF_LIGHT);
		}
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Raim()
{

	int32 lcv, lcv2, max_index;
	double residual_sum;
	double max_sum;

	max_sum = 0;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(good_channels[lcv])
		{
			/* Set to false */
			good_channels[lcv] = false;
			
			/* Redo PVT calculation */
			FormModel();
			PVT_Estimation();
			Residuals();			
			
			residual_sum = 0;
			
			/* Check residuals? */
			for(lcv2 = 0; lcv2 < MAX_CHANNELS; lcv2++)
				if(good_channels[lcv2])
					residual_sum += fabs(pseudoranges[lcv2].residual);

			/* Minimize the residuals */					
			if(residual_sum > max_sum)
			{
				max_index = lcv;
				residual_sum = max_sum;
			}
			
			good_channels[lcv] = true;					
		}
	}	

	/* Set to false */
	good_channels[max_index] = false;
	sv_codes[max_index] = RAIM_ERR;
				
	/* Redo PVT calculation */
	FormModel();
	PVT_Estimation();
	Residuals();			
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::LatLong()
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

	p			= sqrt(master_nav.x*master_nav.x + master_nav.y*master_nav.y);
	theta		= atan( (master_nav.z*a)/ (p*b) );
	latitude	= atan( (master_nav.z+eprime2*b*sin(theta)*sin(theta)*sin(theta)) /
						(p-e2*a*cos(theta)*cos(theta)*cos(theta))  );
	longitude	= atan2( master_nav.y/p, master_nav.x/p);
	N			= a*a / sqrt(a*a*cos(latitude)*cos(latitude) + b*b*sin(latitude)*sin(latitude));
	altitude	= p/cos(latitude) - N;

	master_nav.latitude	 = latitude;
	master_nav.longitude = longitude;
	master_nav.altitude  = altitude;

}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void PVT::DOP()
{

	int32 lcv, lcv2;

	double gdop, pdop, tdop, hdop, vdop, temp;
	double verx, very, verz;

	/* Using nav matrix, calculate DOPS */

	/* position DOP */
	pdop = 0.0;
	for (lcv = 0; lcv < 3; lcv++)
		for (lcv2 = 0; lcv2 < nav_channels; lcv2++)
			pdop += alpha_pinv[lcv][lcv2]*alpha_pinv[lcv][lcv2];


	/* time DOP */
	tdop = 0.0;
	for (lcv2 = 0; lcv2 < nav_channels; lcv2++)
		tdop += alpha_pinv[3][lcv2]*alpha_pinv[3][lcv2];


	/* make some unit vectors */
	temp = sqrt(master_nav.x * master_nav.x +
				master_nav.y * master_nav.y +
				master_nav.z * master_nav.z);

	verx = master_nav.x/temp;
	very = master_nav.y/temp;
	verz = master_nav.z/temp;
	
	/* vertical DOP */
	vdop = 0.0;
	for (lcv2 = 0; lcv2 < nav_channels; lcv2++)
	{
		vdop += (alpha_pinv[0][lcv2] * verx + alpha_pinv[1][lcv2] * very + alpha_pinv[2][lcv2] * verz) * 
				(alpha_pinv[0][lcv2] * verx + alpha_pinv[1][lcv2] * very + alpha_pinv[2][lcv2] * verz);
	}

	/* other DOPS */
	gdop = pdop + tdop;
	hdop = pdop - vdop;
	
	master_nav.gdop = sqrt(gdop);
	master_nav.pdop = sqrt(pdop);
	master_nav.tdop = sqrt(tdop);
	master_nav.hdop = sqrt(hdop);
	master_nav.vdop = sqrt(vdop);
  
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Converged()
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
		if(residual_avg < 200.0)
		{
			master_nav.converged = true;
			converged_ticks++;
		}
		else
		{
			master_nav.converged = false;
			converged_ticks = 0;
		}
	}
	else
	{
		master_nav.converged = false;
		converged_ticks = 0;
	}


	/* Catch the NaN bug */
	if(isnan(master_nav.x) || isnan(master_nav.y) || isnan(master_nav.z))
		Reset();

	if(isnan(master_nav.vx) || isnan(master_nav.vy) || isnan(master_nav.vz))
		Reset();

	if(isinf(master_nav.x) || isinf(master_nav.y) || isinf(master_nav.z))
		Reset();

	if(isinf(master_nav.vx) || isinf(master_nav.vy) || isinf(master_nav.vz))
		Reset();

}
/*----------------------------------------------------------------------------------------------*/

	
/*----------------------------------------------------------------------------------------------*/
void PVT::Reset()
{

	int32 lcv;
	
	/* No succesful nav ticks */
	nav_ticks = 0;
	converged_ticks = 0;
	nav_channels = 0;

	/* Reset to center of the earth	 */
	memset(&master_clock,0x0,sizeof(Clock_S));
	memset(&master_nav,0x0,sizeof(Nav_Solution_S));

	/* Reset Each Channel */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		Reset(lcv);
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PVT::Reset(int32 _chan)
{

	/* Reset IODE */
	master_iode[_chan] = 9999;

	/* Reset master SV */
	master_sv[_chan] = 666;

	/* Reset Error Codes */
	sv_codes[_chan] = INACTIVE;
	
	/* Zero out data associated with SV */
	memset(&measurements[_chan], 0x0, sizeof(Measurement_S));	
	memset(&sv_positions[_chan], 0x0, sizeof(SV_Position_S));
	memset(&pseudoranges[_chan], 0x0, sizeof(Pseudorange_S));
	memset(&ephemerides[_chan],  0x0, sizeof(Ephemeris_S));
	
	good_channels[_chan] = false;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
double PVT::GPSTime()
{
	
	double epochdiff = 315964819;	//Difference in zero epochs
	double offest = 10;				//Unix to TAI;
	double leap_seconds = 23;		//TAI to UTC/GPS
	double gps_second;
	timeval tv;
		
	gettimeofday(&tv, NULL);
	
	gps_second = (double)tv.tv_sec + (double)tv.tv_usec * 1e-6;
	gps_second = fmod(gps_second,SECONDS_IN_WEEK);
	
	return(gps_second);
}
/*----------------------------------------------------------------------------------------------*/
