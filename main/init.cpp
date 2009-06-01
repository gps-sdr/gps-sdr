/*----------------------------------------------------------------------------------------------*/
/*! \file init.cpp
//
// FILENAME: init.cpp
//
// DESCRIPTION: Initializes the receiver software
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


/*----------------------------------------------------------------------------------------------*/
#include "includes.h"
#include "fifo.h"				//!< Circular buffer for Importing IF data
#include "keyboard.h"			//!< Handle user input via keyboard
#include "channel.h"			//!< Tracking channels
#include "correlator.h"			//!< Correlator
#include "acquisition.h"		//!< Acquisition
#include "pvt.h"				//!< PVT solution
#include "ephemeris.h"			//!< Ephemeris decode
#include "telemetry.h"			//!< Serial/GUI telemetry
#include "commando.h"			//!< Command interface
#include "sv_select.h"			//!< Drives acquisition/reacquisition process
#include "gps_source.h"
/*----------------------------------------------------------------------------------------------*/


/*! Print out command arguments to std_out */
/*----------------------------------------------------------------------------------------------*/
void usage(char *_str)
{
	fprintf(stdout,"\n");
	fprintf(stdout,"usage: [-c] [-v] [-gr] [-gi] [-d] [-l] [-w] [-x] [-s]\n");
	fprintf(stdout,"[-c] log high rate channel data\n");
	fprintf(stdout,"[-v] be verbose \n");
	fprintf(stdout,"[-gr] <gain> set rf gain in dB (DBSRX only)\n");
	fprintf(stdout,"[-gi] <gain> set if gain in dB (DBSRX only)\n");
	fprintf(stdout,"[-d] operate in two antenna mode, A & B as L1\n");
	fprintf(stdout,"[-l] operate in L1-L2 mode, A as L1, B as L2\n");
	fprintf(stdout,"[-w] <bandwidth> bandwidth of lowpass filter\n");
	fprintf(stdout,"[-x] the USRP samples at a modified 65.536 MHz (default is 64 MHz)\n");
	fprintf(stdout,"[-s] output over /dev/ttyS0 instead of the named pipe\n");
	fflush(stdout);
	exit(1);
}
/*----------------------------------------------------------------------------------------------*/


/*! Print out command arguments to std_out */
/*----------------------------------------------------------------------------------------------*/
void echo_options()
{
	FILE *fp;

	if(gopt.verbose)
	{
		fprintf(stdout,"\n");
		fprintf(stdout,"Verbose:          %13d\n",gopt.verbose);
		fprintf(stdout,"Log channel:      %13d\n",gopt.log_channel);
		fprintf(stdout,"Telemetry:        %13d\n",gopt.tlm_type);
		fprintf(stdout,"USRP Decimation:  %13d\n",gopt.decimate);
		fprintf(stdout,"USRP Sample Rate: %13.2f\n",gopt.f_sample);
		fprintf(stdout,"DBSRX LO A:       %13.2f\n",gopt.f_lo_a);
		fprintf(stdout,"DBSRX LO B:       %13.2f\n",gopt.f_lo_b);
		fprintf(stdout,"RF Gain:          %13.2f\n",gopt.gr);
		fprintf(stdout,"IF Gain:          %13.2f\n",gopt.gi);
		fprintf(stdout,"DBSRX Bandwidth:  %13.2f\n",gopt.bandwidth);
		fprintf(stdout,"\n");
		fflush(stdout);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*! Parse command line arguments to setup functionality */
/*----------------------------------------------------------------------------------------------*/
void Parse_Arguments(int32 argc, char* argv[])
{

	char *parse;
	int32 lcv;

	/* Set default options */
	gopt.verbose 		= 0;
	gopt.log_channel 	= 0;
	gopt.tlm_type		= TELEM_NAMED_PIPE;
	gopt.mode 			= 0;		//!< Single board L1 mode by default
	gopt.decimate		= 16;		//!< Default to work with both 65.536 and 64 MHz clocks
	gopt.gr 			= 40; 		//!< 40 dB of RF gain
	gopt.gi 			= 10; 		//!< 10 dB of IF gain
	gopt.f_lo_a 		= L1 - IF_FREQUENCY;	//!< Board A L1 by default
	gopt.f_ddc_a 		= 0;		//!< no DDC correction
	gopt.f_lo_b			= L2 - IF_FREQUENCY;	//!< Board B L2 by default
	gopt.f_ddc_b 		= 0;		//!< no DDC correction
	gopt.bandwidth 		= 6.0e6; 	//!< DBS-RX is set to 10 MHz wide
	gopt.f_sample 		= 64.0e6;	//!< Nominal sample rate
	gopt.realtime		= 1;

	for(lcv = 1; lcv < argc; lcv++)
	{
		switch (argv[lcv][1])
		{

			case 'c':
				gopt.log_channel = 1;
				break;
			case 'v':
				gopt.verbose = 1;
				break;
			case 'g':
				if(argv[lcv][2] == 'r')
				{
					if(++lcv >= argc)
						usage (argv[0]);

					if(isdigit(argv[lcv][0]))
						gopt.gr = strtod(argv[lcv], &parse);
					else
						usage (argv[0]);
					break;
				}
				else if(argv[lcv][2] == 'i')
				{
					if(++lcv >= argc)
						usage (argv[0]);

					if(isdigit(argv[lcv][0]))
						gopt.gi = strtod(argv[lcv], &parse);
					else
						usage (argv[0]);
					break;
				}
				else
					usage(argv[0]);
				break;
			case 'd':
				gopt.mode = 1;
				break;
			case 'l':
				gopt.mode = 2;
				gopt.f_lo_b = L2; /* L2C center frequency */
				break;
			case 'w':
				if(++lcv >= argc)
					usage (argv[0]);

				if(isdigit(argv[lcv][0]))
					gopt.bandwidth = strtod(argv[lcv], &parse);
				else
					usage (argv[0]);
				break;
			case 'x':
				gopt.f_sample = 65.536e6;
				break;
			case 's':
				gopt.tlm_type = TELEM_SERIAL;
				break;
			default:
				usage(argv[0]);
		}
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
			fprintf(stdout,"Detected MMX\n");
	}
	else
		return(false);

	if(CPU_SSE())
	{
		if(gopt.verbose)
			fprintf(stdout,"Detected SSE\n");
	}
	else
		return(false);

	if(CPU_SSE2())
	{
		if(gopt.verbose)
			fprintf(stdout,"Detected SSE2\n");
	}
	else
		return(false);

	if(CPU_SSE3())
	{
		if(gopt.verbose)
			fprintf(stdout,"Detected SSE3\n");
	}

	if(CPU_SSSE3())
	{
		if(gopt.verbose)
			fprintf(stdout,"Detected SSSE3\n");
	}

	if(CPU_SSE41())
	{
		if(gopt.verbose)
			fprintf(stdout,"Detected SSE4.1\n");
	}

	if(CPU_SSE42())
	{
		if(gopt.verbose)
			fprintf(stdout,"Detected SSE4.2\n");
	}

	return(1);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! Initialize all threaded objects and global variables */
int32 Object_Init(void)
{
	int32 lcv;

	/* Get start of receiver */
	gettimeofday(&starttime, NULL);

	/* Create Keyboard object to handle user input */
	pKeyboard = new Keyboard();

	/* Now do the hard work? */
	pAcquisition = new Acquisition(IF_SAMPLE_FREQUENCY, IF_FREQUENCY);

	/* Decode the almanac and ephemerides */
	pEphemeris = new Ephemeris;

	/* Drive the acquisition process */
	pSV_Select = new SV_Select;

	/* Output info to the GUI */
	pTelemetry = new Telemetry();

	/* Serial or named pipe */
	pTelemetry->SetType(gopt.tlm_type);

	/* execute user commands */
	pCommando = new Commando();

	/* Form a nav solution */
	pPVT = new PVT();

	/* Create the tracking channels */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		pChannels[lcv] = new Channel(lcv);

	/* Draw the GPS data from somewhere */
	pSource = new GPS_Source(&gopt);

	/* Get data from either the USRP or disk */
	pFIFO = new FIFO;

	pCorrelator = new Correlator();

	if(gopt.verbose)
	{
		fprintf(stdout,"Cleared Object Init\n");
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

	/* Create all of the pipes */
	pipe((int *)SVS_2_COR_P);
	pipe((int *)CHN_2_EPH_P);
	pipe((int *)PVT_2_TLM_P);
	pipe((int *)SVS_2_TLM_P);
	pipe((int *)EKF_2_TLM_P);
	pipe((int *)CMD_2_TLM_P);
	pipe((int *)ACQ_2_SVS_P);
	pipe((int *)EKF_2_SVS_P);
	pipe((int *)PVT_2_SVS_P);
	pipe((int *)TLM_2_CMD_P);
	pipe((int *)SVS_2_ACQ_P);
	pipe((int *)COR_2_ACQ_P);
	pipe((int *)ISRP_2_PVT_P);
	pipe((int *)ISRM_2_PVT_P);

	/* Setup some of the non-blocking pipes */
	fcntl(COR_2_ACQ_P[WRITE], F_SETFL, O_NONBLOCK);
	fcntl(EKF_2_SVS_P[WRITE], F_SETFL, O_NONBLOCK);
	fcntl(SVS_2_TLM_P[WRITE], F_SETFL, O_NONBLOCK);
	fcntl(PVT_2_SVS_P[WRITE], F_SETFL, O_NONBLOCK);
	fcntl(EKF_2_SVS_P[READ],  F_SETFL, O_NONBLOCK);
	fcntl(SVS_2_COR_P[READ],  F_SETFL, O_NONBLOCK);
	fcntl(SVS_2_TLM_P[READ],  F_SETFL, O_NONBLOCK);
	fcntl(PVT_2_TLM_P[READ],  F_SETFL, O_NONBLOCK);
	fcntl(SVS_2_TLM_P[READ],  F_SETFL, O_NONBLOCK);
	fcntl(EKF_2_TLM_P[READ],  F_SETFL, O_NONBLOCK);
	fcntl(CMD_2_TLM_P[READ],  F_SETFL, O_NONBLOCK);

	if(gopt.verbose)
	{
		fprintf(stdout,"Cleared Pipes Init\n");
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

	/* Start the keyboard thread to handle user input from stdio */
	pKeyboard->Start();

	/* Startup the PVT sltn */
	pPVT->Start();

	/* Start up the correlators */
	pCorrelator->Start();

	/* Start up the acquistion */
	pAcquisition->Start();

	/* Start up the ephemeris */
	pEphemeris->Start();

	/* Start up the command interface */
	pCommando->Start();

	/* Start the SV select thread */
	pSV_Select->Start();

	/* Last thing to do */
	pTelemetry->Start();

	/* Start up the FIFO */
	pFIFO->Start();

	return(1);

}
/*----------------------------------------------------------------------------------------------*/
