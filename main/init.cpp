/*! \file Init.cpp
	Initializes receiver
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

#include "includes.h"

/*! Print out command arguments to std_out */
/*----------------------------------------------------------------------------------------------*/
void usage(int32 argc, char* argv[])
{
	int32 lcv;
	
	fprintf(stderr, "\n");
	for(lcv = 0; lcv < argc; lcv++)
		fprintf(stderr, "%s ",argv[lcv]);
	fprintf(stderr, "\nusage: [-p] [-o] [-l] [-v]\n");
	fprintf(stderr, "[-p] <filename> use prerecorded data\n");
	fprintf(stderr, "[-o] <filename1> <filename2> do ocean reflection\n");
	fprintf(stderr, "[-c] log high rate channel data\n");
	fprintf(stderr, "[-l] log navigation data\n");
	fprintf(stderr, "[-v] be verbose \n");
	fprintf(stderr, "[-n] ncurses OFF \n");
	fprintf(stderr, "[-w] start receiver in warm start, using almanac and last good position\n");
	fprintf(stderr, "\n");
	
	exit(1);
}
/*----------------------------------------------------------------------------------------------*/


/*! Print out command arguments to std_out */
/*----------------------------------------------------------------------------------------------*/
void echo_options()
{
	FILE *fp;
	
	/* Do some error checking */
	if(gopt.post_process)
	{
		fp = fopen(gopt.filename_direct,"r");
		if(fp == NULL)
		{
			printf("\nCould not open %s for reading\n\n",gopt.filename_direct);
			exit(1);
		}
	
	}
	
	if(gopt.ocean)
	{
		fp = fopen(gopt.filename_direct,"r");
		if(fp == NULL)
		{
			printf("\nCould not open %s for reading\n\n",gopt.filename_direct);
			exit(1);
		}
		
		fp = fopen(gopt.filename_reflected,"r");
		if(fp == NULL)
		{
			printf("\nCould not open %s for reading\n\n",gopt.filename_reflected);
			exit(1);
		}		
	}
	

	fprintf(stderr, "\n");
	fprintf(stderr, "verbose:\t\t %d\n",gopt.verbose);
	fprintf(stderr, "realtime:\t\t %d\n",gopt.realtime);
	fprintf(stderr, "post_process:\t\t %d\n",gopt.post_process);
	fprintf(stderr, "ocean:\t\t\t %d\n",gopt.ocean);
	fprintf(stderr, "log_channel:\t\t %d\n",gopt.log_channel);
	fprintf(stderr, "ncurses:\t\t %d\n",gopt.ncurses);
	fprintf(stderr, "filename_direct:\t %s\n",gopt.filename_direct);
	fprintf(stderr, "filename_reflected:\t %s\n",gopt.filename_reflected);
	fprintf(stderr, "\n");	
	
}
/*----------------------------------------------------------------------------------------------*/


/*! Parse command line arguments to setup functionality */
/*----------------------------------------------------------------------------------------------*/
void Parse_Arguments(int32 argc, char* argv[])
{

	int32 lcv;
	
	/* Set default options */
	gopt.verbose 		= 0;
	gopt.realtime 		= 1;
	gopt.post_process 	= 0;
	gopt.ocean 			= 0;
	gopt.log_channel 	= 0;
	gopt.log_nav		= 0;
	gopt.ncurses 		= 1;
	gopt.doppler_min 	= -MAX_DOPPLER;
	gopt.doppler_max 	= MAX_DOPPLER;
	gopt.corr_sleep 	= 500;
	gopt.startup		= COLD_START;
	strcpy(gopt.filename_direct, "data.bda");
	strcpy(gopt.filename_reflected, "rdata.bda");
	
	for(lcv = 1; lcv < argc; lcv++)
	{
		if(strcmp(argv[lcv],"-p") == 0)
		{
			gopt.post_process = 1;
			gopt.realtime = 0;
			gopt.ocean = 0;
			gopt.corr_sleep = 100;
						
			if(argc < lcv+2)
				usage(argc, argv);
				
			strcpy(gopt.filename_direct, argv[lcv+1]);
			lcv += 1;			
		}
		else if(strcmp(argv[lcv],"-o") == 0)
		{
			gopt.post_process = 0;
			gopt.realtime = 0;
			gopt.ocean = 1;
			
			if(argc < lcv+3)
				usage(argc, argv);
				
			strcpy(gopt.filename_direct, argv[lcv+1]);
			strcpy(gopt.filename_reflected, argv[lcv+2]);
			lcv += 2;			
		}
		else if(strcmp(argv[lcv],"-v") == 0)
		{
			gopt.verbose = 1;
		}
		else if(strcmp(argv[lcv],"-c") == 0)
		{
			gopt.log_channel = 1;
		}
		else if(strcmp(argv[lcv],"-l") == 0)
		{
			gopt.log_nav = 1;
		}
		else if(strcmp(argv[lcv],"-n") == 0)
		{
			gopt.ncurses = 0;
		}
		else if(strcmp(argv[lcv],"-w") == 0)
		{
			gopt.startup = WARM_START;
		}

		else
			usage(argc, argv);
	}
	
	echo_options();

}
/*----------------------------------------------------------------------------------------------*/


/*! Initialize any hardware (for realtime mode) */
/*----------------------------------------------------------------------------------------------*/
int32 Hardware_Init(void)
{
	
	if(CPU_MMX())
	{
		if(gopt.verbose)
			printf("Detected MMX\n");
	}
	else
		return(false);
		
	if(CPU_SSE())
	{
		if(gopt.verbose)
			printf("Detected SSE\n");
	}
	else
		return(false);

	if(CPU_SSE2())
	{
		if(gopt.verbose)
			printf("Detected SSE2\n");
	}
	else
		return(false);

	if(CPU_SSE3())
	{
		if(gopt.verbose)
			printf("Detected SSE3\n");
	}

	if(CPU_SSSE3())
	{
		if(gopt.verbose)
			printf("Detected SSSE3\n");
	}

	if(CPU_SSE41())
	{
		if(gopt.verbose)
			printf("Detected SSE4.1\n");
	}

	if(CPU_SSE42())
	{
		if(gopt.verbose)
			printf("Detected SSE4.2\n");
	}

	return(1);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! Initialize all threaded objects and global variables */
int32 Object_Init(void)
{
	int32 lcv;
	int32 failed;
	
	/* Create Keyboard objec to handle user input */
	pKeyboard = new Keyboard;
	
	/* Now do the hard work? */
	pAcquisition = new Acquisition(IF_SAMPLE_FREQUENCY, IF_FREQUENCY);
	
	pEphemeris = new Ephemeris;
	
	/* Get data from either the USRP or disk */
	pFIFO = new FIFO;
	
	pSV_Select = new SV_Select;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		pChannels[lcv] = new Channel(lcv);
		
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		pCorrelators[lcv] =  new Correlator(lcv);
		
	pTelemetry = new Telemetry(gopt.ncurses);		
	
	pPVT = new PVT(gopt.startup);
		
	if(gopt.post_process)
		pPost_Process = new Post_Process(gopt.filename_direct);
		
	pthread_mutex_init(&mAcq, NULL);
	pthread_mutex_unlock(&mAcq);
	int32 gAcq_high = false;
	
	pthread_mutex_init(&mInterrupt, NULL);
	pthread_mutex_unlock(&mInterrupt);
	memset(gInterrupt, 0x0, MAX_CHANNELS*sizeof(uint32));
	
	//if(gopt.verbose)
	{
		printf("Cleared Object Init\n");
		fflush(stdout);
	}
		
	return(1);
	
}
/*----------------------------------------------------------------------------------------------*/


/*! Initialize all pipes */
/*----------------------------------------------------------------------------------------------*/
int32 Pipes_Init(void)
{
	int32 lcv;
		
	/* Acq and track play together */
	pipe((int *)Trak_2_Acq_P);
	pipe((int *)Acq_2_Trak_P);
	pipe((int *)FIFO_2_Telem_P);
	pipe((int *)FIFO_2_PVT_P);
	pipe((int *)PVT_2_Telem_P);
	pipe((int *)Chan_2_Ephem_P);
	pipe((int *)Ephem_2_Telem_P);
	fcntl(Ephem_2_Telem_P[READ], F_SETFL, O_NONBLOCK);
	pipe((int *)Acq_2_Telem_P);
	fcntl(Acq_2_Telem_P[READ], F_SETFL, O_NONBLOCK);
	pipe((int *)SV_Select_2_Telem_P);
	fcntl(SV_Select_2_Telem_P[READ], F_SETFL, O_NONBLOCK);	
	pipe((int *)PVT_2_SV_Select_P);
	fcntl(PVT_2_SV_Select_P[WRITE], F_SETFL, O_NONBLOCK);
	fcntl(PVT_2_SV_Select_P[READ], F_SETFL, O_NONBLOCK);	
		
	/* Channel and correlator */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		pipe((int *)Trak_2_Corr_P[lcv]);
		pipe((int *)Corr_2_PVT_P[lcv]);
		pipe((int *)PVT_2_Corr_P[lcv]);
		fcntl(Trak_2_Corr_P[lcv][READ], F_SETFL, O_NONBLOCK); 
	}	
			
	//if(gopt.verbose)
	{
		printf("Cleared Pipes Init\n");
		fflush(stdout);
	}
		
	return(1);
			
}
/*----------------------------------------------------------------------------------------------*/


/*! Finally start up the threads */
/*----------------------------------------------------------------------------------------------*/
int32 Thread_Init(void)
{
	int32 lcv;
	
	/* Set the global run flag to true */
	grun = 0x1;

	if(gopt.post_process)
		pPost_Process->Start();	

	/* Start the keyboard thread to handle user input from stdio */
	pKeyboard->Start();
	 
	/* Startup the PVT sltn */
	pPVT->Start();

	/* Start up the FIFO */
	pFIFO->Start();

	/* Start up the correlators */		 
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		pCorrelators[lcv]->Start();

	/* Start up the acquistion */
	pAcquisition->Start();
	
	/* Start up the ephemeris */
	pEphemeris->Start();
	
	/* Start the SV select thread */
	if(gopt.realtime)
		pSV_Select->Start();
	
	//if(gopt.verbose)
	{	
		printf("Cleared Thread Init\n");
		fflush(stdout);
	}

	/* Last thing to do */
	pTelemetry->Start();
	
	return(1);

}
/*----------------------------------------------------------------------------------------------*/
