/*! \file Telemetry.cpp
	Implements member functions of Telemetry class.
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

#include "telemetry.h"

/*----------------------------------------------------------------------------------------------*/
void *Telemetry_Thread(void *_arg)
{
	
	Telemetry *aTelemetry = pTelemetry;

	aTelemetry->InitScreen();

	while(grun)
	{
		aTelemetry->Inport();
		aTelemetry->Export();
	}
	
	pthread_exit(0);
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::Start()
{
	pthread_attr_t tattr;
	sched_param param;	
	int32 ret;
	
	/* Unitialized with default attributes */
	ret = pthread_attr_init(&tattr);
	
	/*Ssafe to get existing scheduling param */
	ret = pthread_attr_getschedparam(&tattr, &param);
	
	/* Set the priority; others are unchanged */
	param.sched_priority = TELEM_PRIORITY;
	
	/* Setting the new scheduling param */
	ret = pthread_attr_setschedparam(&tattr, &param);
	ret = pthread_attr_setschedpolicy(&tattr, SCHED_FIFO);
	
	/* With new priority specified */
	pthread_create(&thread, NULL, Telemetry_Thread, NULL);
	
	if(gopt.verbose)
		printf("Telemetry thread started\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::Stop()
{
	pthread_join(thread, NULL);
	
	if(gopt.verbose)
		printf("Telemetry thread stopped\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Telemetry::Telemetry(int32 _ncurses_on)
{
	
	display = 0;
	count = 0;
	ncurses_on = _ncurses_on;
	
	if(gopt.log_nav)	
	{
		fp_nav = fopen("navigation.tlm","wt");
		fp_pseudo = fopen("pseudorange.tlm","wt");
		fp_meas = fopen("measurement.tlm","wt");
		fp_chan = fopen("tracking.tlm","wt");
		fp_sv = fopen("satellites.tlm","wt");
	}
	
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_unlock(&mutex);
		
	if(gopt.verbose)	
		printf("Creating Telemetry\n");
		
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Telemetry::~Telemetry()
{
	
	if(gopt.log_nav)	
	{
		fclose(fp_nav);
		fclose(fp_pseudo);
		fclose(fp_meas);
		fclose(fp_chan);	
		fclose(fp_sv);
	}
	
	if(ncurses_on)
	{
		EndScreen();
	}
	
	pthread_mutex_destroy(&mutex);

	if(gopt.verbose)	
		printf("Destroying Telemetry\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::Lock()
{
	pthread_mutex_lock(&mutex);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::Unlock()
{
	pthread_mutex_unlock(&mutex);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::Inport()
{
	Chan_Packet_S temp;
	int32 bread, lcv, num_chans;
	
	read(FIFO_2_Telem_P[READ], &tFIFO, sizeof(FIFO_2_Telem_S));

	/* Lock correlator status */
	pthread_mutex_lock(&mInterrupt);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{	
		if(gInterrupt[lcv])
		{
			tChan[lcv] = pChannels[lcv]->getPacket();
			active[lcv] = 1;
		}
		else
			active[lcv] = 0;
	}
	
	/* Unlock correlator status */
	pthread_mutex_unlock(&mInterrupt);	
	
	read(PVT_2_Telem_P[READ], &tNav, sizeof(PVT_2_Telem_S));
	
	bread = sizeof(Acq_Result_S);
	while(bread == sizeof(Acq_Result_S))
		bread = read(Acq_2_Telem_P[READ],&tAcq,sizeof(Acq_Result_S));
	
	bread = sizeof(Ephem_2_Telem_S);
	while(bread == sizeof(Ephem_2_Telem_S))
		bread = read(Ephem_2_Telem_P[READ], &tEphem, sizeof(Ephem_2_Telem_S));
	
	bread = sizeof(SV_Select_2_Telem_S);
	while(bread == sizeof(SV_Select_2_Telem_S))
		bread = read(SV_Select_2_Telem_P[READ], &tSelect, sizeof(SV_Select_2_Telem_S));
		
	if(ncurses_on)
		UpdateScreen();

}	
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::Export()
{
	
	/* Cut down the logging rate to 1 time/second even though GUI updates more often */
	if(gopt.log_nav && (count++%(1000/MEASUREMENT_INT) == 0))
	{
		LogNav();
		LogPseudo();
		LogTracking();
		LogSV();
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::InitScreen()
{
	
	if(ncurses_on)
	{
		mainwnd = initscr();
		noecho();
		cbreak();
		nodelay(mainwnd, TRUE);
		refresh();
		wrefresh(mainwnd);
		screen = newwin(400, 400, 1, 1);
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::UpdateScreen()
{
	curs_set(0);
	
	line = 0;
	
	wclear(screen);
	
	mvwprintw(screen,line,1,"                                                                               ");
	mvwprintw(screen,line++,1,"FIFO:\t%d\t%d\t%d\t%d",(FIFO_DEPTH-(tFIFO.head-tFIFO.tail)) % FIFO_DEPTH,tFIFO.count,tFIFO.agc_scale,tFIFO.overflw);
	
	Lock();
	
	switch(display)
	{
		case 0:
			PrintChan();
			PrintSV();
			PrintNav();
			PrintEphem();
			break;
		case 1:
			PrintAlmanac();
			break;
		default:
			PrintChan();
			PrintSV();
			PrintNav();
			PrintEphem();
			break;
	}	
	
	Unlock();
	
	wrefresh(screen);
	   
	refresh();
   
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::EndScreen(void)
{
   endwin();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::PrintChan()
{
	Chan_Packet_S *p;
	Nav_Solution_S		*pNav		= &tNav.master_nav;				/* Navigation Solution */
	Clock_S				*pClock		= &tNav.master_clock;			/* Clock solution */
	
	int32 lcv;
	char buff[1024];
	float cn0;
	
	line++;
	mvwprintw(screen,line++,1,"Ch#  SV   CL       Faccel          Doppler     CN0   BE       Locks        Power   Active\n");  	
	mvwprintw(screen,line++,1,"-----------------------------------------------------------------------------------------\n");
	
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		p = &tChan[lcv];
		if(active[lcv])
		{
			
			strcpy(buff, "---------");
		
			/*Flag buffer*/
			((int32)p->fll_lock_ticks > 200)   ? buff[0] = 'p'  : buff[0] = 'f';
			((int32)p->bit_lock)   ? buff[1] = 'B'  : buff[1] = '-';
			((int32)p->frame_lock) ? buff[2] = 'F'  : buff[2] = '-';
			(pNav->nsvs >> lcv) & 0x1 ? buff[3] = 'N'  : buff[3] = '-';
			
			if(((int32)p->subframe > 0) &&  ((int32)p->subframe < 6))
				buff[(int32)p->subframe+3] = (int32)p->subframe + 48;
		
			buff[9] = '\0';
			
			cn0 = p->CN0 > p->CN0_old ? p->CN0 : p->CN0_old;
			
			mvwprintw(screen,line++,1,"%2d   %2d   %2d   %10.3f   %14.3f   %5.2f   %2d   %9s   %10.0f   %6d",	
				lcv,  
				(int32)p->sv+1,
				(int32)p->len,
				p->w,
				p->carrier_nco - IF_FREQUENCY,
				cn0,
				(int32)p->best_epoch,
				buff,
				p->P_avg,
				(int32)p->count/1000);
				
		}
		else
		{
			mvwprintw(screen,line++,1,"%2d   --   --   ----------   --------------   -----   --   ---------   ----------   ------",lcv);
		}
	}	

}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void Telemetry::PrintSV()
{
	SV_Position_S *pPos;
	Pseudorange_S *pPseudo;
	Chan_Packet_S *pChan;
	Nav_Solution_S		*pNav		= &tNav.master_nav;				/* Navigation Solution */
	Clock_S				*pClock		= &tNav.master_clock;			/* Clock solution */
	
	int32 lcv;
	
	line++;
	
	/* Residuals */
	mvwprintw(screen,line++,1,"Ch#  SV         SV Time        VX        VY        VZ    Transit Time        Residual\n");		
	mvwprintw(screen,line++,1,"-------------------------------------------------------------------------------------\n");

	for(lcv	= 0; lcv < MAX_CHANNELS; lcv++)
	{
		pPos    = (SV_Position_S *)	&tNav.sv_positions[lcv];
		pChan   = (Chan_Packet_S *)	&tChan[lcv];
		pPseudo = (Pseudorange_S *)	&tNav.pseudoranges[lcv];
		
		
		if((pNav->nsvs >> lcv) & 0x1)
		{
			mvwprintw(screen,line++,1,"%2d   %2d  %14.7f  %8.2f  %8.2f  %8.2f  %14.8f  %14.8f\n", 
					lcv,  
					(int32)pChan->sv+1,
					pPos->time,
					pPos->vx,
					pPos->vy,
					pPos->vz,
					pPseudo->time,
					pPseudo->residual);
		}
		else
		{
			
			mvwprintw(screen,line++,1,"%2d   --  --------------  --------  --------  --------  --------------  --------------\n",lcv);
		}
	}	
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::PrintNav()
{
	
	Nav_Solution_S		*pNav		= &tNav.master_nav;				/* Navigation Solution */
	Clock_S				*pClock		= &tNav.master_clock;			/* Clock solution */
	char buff[1024];
	int32 nsvs, lcv;	
	int32 k = 32;
	
	line++;
	
	switch(tAcq.type)
	{
		case 0: strcpy(buff,"STRONG"); break;
		case 1:	strcpy(buff,"MEDIUM"); break;
		case 2:	strcpy(buff,"  WEAK"); break;
	}
	
	mvwprintw(screen,line++,1,"Last Acq: %s, %02d, %7.2f, %7.0f, %10.0f\n",buff,tAcq.sv+1, tAcq.delay, tAcq.doppler, tAcq.magnitude);
	
	line++;
	
	/* Nav Solution */
	nsvs = 0;	
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if((pNav->nsvs >> lcv) & 0x1)
			nsvs++;
	}
	
	mvwprintw(screen,line++,1,"Nav SVs:\t%-2d\n",nsvs);
	mvwprintw(screen,line++,1,"Receiver Tic:\t%-6d\n",pNav->tic);
	mvwprintw(screen,line++,1,"\t\t\t      X\t\t      Y\t\t      Z\n");
	mvwprintw(screen,line++,1,"Position (m):\t%15.2f\t%15.2f\t%15.2f\n",pNav->x,pNav->y,pNav->z);
	mvwprintw(screen,line++,1,"Vel (cm/s):\t%15.2f\t%15.2f\t%15.2f\n",100.0*pNav->vx,100.0*pNav->vy,100.0*pNav->vz);
	mvwprintw(screen,line++,1,"");
	mvwprintw(screen,line++,1,"\t\t\t    Lat\t\t   Long\t\t    Alt\n");
	mvwprintw(screen,line++,1,"\t\t%15.9f\t%15.9f\t%15.4f\n",pNav->latitude*RAD_2_DEG,pNav->longitude*RAD_2_DEG,pNav->altitude);
	mvwprintw(screen,line++,1,"");
	mvwprintw(screen,line++,1,"\t\t     Clock Bias\t     Clock Rate\t       GPS Time\n");
	mvwprintw(screen,line++,1,"\t\t%15.6f\t%15.7f\t%15.6f\n",pClock->bias,pClock->rate,pClock->time);
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::PrintEphem()
{
	
	int32 k, lcv;
	
	k = 1;
	line++;
	
	mvwprintw(screen,line,k++,"EPH: ");
	k += 4;
	
	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		if(tEphem.valid[lcv])
		{
			mvwprintw(screen,line,k++,"%2d",lcv+1);
			k+=2;
		}
	}
	
	k = 1;
	line++;
	
	mvwprintw(screen,line,k++,"ALM: ");
	k += 4;
	
	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		if(tEphem.avalid[lcv])
		{
			mvwprintw(screen,line,k++,"%2d",lcv+1);
			k+=2;
		}
	}	

	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::PrintAlmanac()
{
	
	int32 lcv, nvis, ntrack;
	float elev, azim;
	Acq_Predicted_S *psv;
	line++;
	
	nvis = 0;
	ntrack = 0;
	
	switch(tSelect.mode)
	{
		case 0:	mvwprintw(screen,line++,1,"Acq Mode:\t  COLD\n");	break;
		case 1: mvwprintw(screen,line++,1,"Acq Mode:\t  WARM\n");	break;
		case 2:	mvwprintw(screen,line++,1,"Acq Mode:\t   HOT\n");	break;
		default:mvwprintw(screen,line++,1,"Acq Mode:\t  COLD\n");	break;
	}


	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		
		psv =  &tSelect.sv_predicted[lcv];
		if(psv->visible) nvis++;
		if(psv->tracked) ntrack++;
						
	}

	mvwprintw(screen,line++,1,"Mask Angle:\t%6.2f\n",tSelect.mask_angle*(180/PI));
	mvwprintw(screen,line++,1,"Visible:\t%6d\n",nvis);
	mvwprintw(screen,line++,1,"Tracked:\t%6d\n",ntrack);
	
	line++;
	mvwprintw(screen,line++,1,"SV        Elev        Azim     Doppler           Delay   Visible    Tracked\n");
	mvwprintw(screen,line++,1,"---------------------------------------------------------------------------\n");
	
	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		psv =  &tSelect.sv_predicted[lcv];
		elev = psv->elev*180/PI;
		azim = psv->azim*180/PI;
		
		if(elev != 0.0)
		{
			if(psv->visible && psv->tracked)
				mvwprintw(screen,line++,1,"%02d  %10.2f  %10.2f  %10.2f  %14.8f       YES        YES\n",lcv+1,elev,azim,psv->doppler,psv->delay);
				
			if(psv->visible && !psv->tracked)
				mvwprintw(screen,line++,1,"%02d  %10.2f  %10.2f  %10.2f  %14.8f       YES         NO\n",lcv+1,elev,azim,psv->doppler,psv->delay);
				
			if(!psv->visible && psv->tracked)
				mvwprintw(screen,line++,1,"%02d  %10.2f  %10.2f  %10.2f  %14.8f        NO        YES\n",lcv+1,elev,azim,psv->doppler,psv->delay);
				
			if(!psv->visible && !psv->tracked)
				mvwprintw(screen,line++,1,"%02d  %10.2f  %10.2f  %10.2f  %14.8f        NO         NO\n",lcv+1,elev,azim,psv->doppler,psv->delay);
		}
		else
				mvwprintw(screen,line++,1,"--  ----------  ----------  ----------  --------------       ---        ---\n");
	}
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::LogNav()
{
	
	int32 lcv;
	int32 nsvs;
	
	Nav_Solution_S		*pNav		= &tNav.master_nav;				/* Navigation Solution */
	Clock_S				*pClock		= &tNav.master_clock;			/* Clock solution */
	
	nsvs = 0;	
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if((pNav->nsvs >> lcv) & 0x1)
			nsvs++;
	}	
		/* Nav solution */
	fprintf(fp_nav,"%01d,%02d,%08d,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e\n",
		pNav->converged,
		nsvs,
		pNav->tic,
		pNav->x,
		pNav->y,
		pNav->z,
		pNav->vx,
		pNav->vy,
		pNav->vz,
		pClock->bias,
		pClock->rate,
		pClock->time,
		pNav->gdop,
		pNav->hdop,
		pNav->tdop,
		pNav->vdop,
		pNav->pdop);
		
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::LogPseudo()
{
	int32 lcv;
	Pseudorange_S *pPseudo;
	Measurement_S *pMeas;	
	
	/* Pseudo ranges */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		pPseudo = (Pseudorange_S *)	&tNav.pseudoranges[lcv];
		pMeas = (Measurement_S *)	&tNav.measurements[lcv];
		
		fprintf(fp_pseudo,"%02d,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e\n",
			lcv,		
			pPseudo->time,
			pPseudo->time_rate,
			pPseudo->meters,
			pPseudo->meters_rate,
			pPseudo->residual,
			pPseudo->time_uncorrected);
			
		fprintf(fp_meas,"%02d,%02d,%01d,%8d,%8d,%8d,%.16e,%.16e,%.16e,%.16e\n",
			lcv,
			pMeas->sv,
			pMeas->navigate,
			pMeas->_1ms_epoch,
			pMeas->_20ms_epoch,
			pMeas->_z_count,
			pMeas->code_phase_mod,
			pMeas->code_phase,
			pMeas->carrier_phase_mod,
			pMeas->carrier_phase);
	}
		
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::LogTracking()
{
	
	int32 lcv;
	Chan_Packet_S *pChan;
	
	/* Pseudo ranges */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		pChan = (Chan_Packet_S *) &tChan[lcv];
		
		fprintf(fp_chan,"%02d,%02d,%08d,%01d,%01d,%01d,%02d,%.16e,%.16e,%.16e,%.16e,%.16e\n",
		lcv,
		(int32)pChan->sv,
		(int32)pChan->count,
		(int32)pChan->bit_lock,		
		(int32)pChan->frame_lock,		
		(int32)pChan->subframe,		
		(int32)pChan->len,
		pChan->P_avg,		
		pChan->CN0,		
		pChan->fll_lock,		
		pChan->pll_lock,		
		pChan->fll_lock_ticks);						
	}	
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::LogSV()
{
	
	int32 lcv;
	SV_Position_S *pSV;
	Chan_Packet_S *pChan;
	
	/* Pseudo ranges */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		pSV = (SV_Position_S *) &tNav.sv_positions[lcv];
		pChan = (Chan_Packet_S *) &tChan[lcv];
		
		fprintf(fp_sv,"%02d,%02d,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e\n",
		lcv,
		(int32)pChan->sv,
		pSV->time,
		pSV->x,
		pSV->y,
		pSV->z,
		pSV->vx,
		pSV->vy,
		pSV->vz,
		pSV->elev,
		pSV->azim);
	}	
	
}
/*----------------------------------------------------------------------------------------------*/

