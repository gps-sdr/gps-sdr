/*! \file globals.h
	Default includes for each source file in the project.
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

#ifndef GLOBALS_H_
#define GLOBALS_H_

#ifdef GLOBALS_HERE
	#define EXTERN
#else
	#define EXTERN extern
#endif


/* Part 1, Threaded and Semi-threaded objects */
/*----------------------------------------------------------------------------------------------*/
EXTERN class Keyboard		*pKeyboard;						//!< Handle user input
EXTERN class FIFO			*pFIFO;							//!< Get data and pass it into the receiver
EXTERN class PVT			*pPVT;							//!< Do the PVT solution
EXTERN class Ephemeris		*pEphemeris;					//!< Extract the ephemeris
EXTERN class Acquisition	*pAcquisition;					//!< Perform acquisitions
EXTERN class Correlator		*pCorrelators[MAX_CHANNELS];	//!< Bank of correlators
EXTERN class Channel		*pChannels[MAX_CHANNELS];		//!< Channels (uses correlations to close the loops)
EXTERN class SV_Select		*pSV_Select;					//!< Contains the channels and drives the channel objects
EXTERN class Telemetry		*pTelemetry;					//!< Simple ncurses interface
EXTERN class Post_Process	*pPost_Process;					//!< Drive the receiver from a recorded file
EXTERN class Serial_Telemetry *pSerial_Telemetry;			//!< Dump data to GUI over named pipe or serial
EXTERN class Commando		*pCommando;						//!< Process and execute commands
/*----------------------------------------------------------------------------------------------*/


/* Part 2, Mutexes, semaphores, and their respective protected memory locations */
/*----------------------------------------------------------------------------------------------*/
EXTERN pthread_mutex_t		mAcq;							//!< Protect the following variable
EXTERN int32				gAcq_high;						//!< This is set high when the acquisition is active
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
EXTERN int32 grun;											//!< Keep all the threads active (technically, this should be mutex protected, but eh, who cares? )
EXTERN Options_S gopt;										//!< Global receiver options
EXTERN struct timeval starttime;							//!< Get receiver start time
/*----------------------------------------------------------------------------------------------*/


/* Part 3, Pipes */
/*----------------------------------------------------------------------------------------------*/
/* Interplay between acquisition and tracking */
EXTERN int32 Acq_2_Trak_P[2];								//!< \ingroup PIPES Get an acquisition result
EXTERN int32 Trak_2_Acq_P[2];								//!< \ingroup PIPES Request an acquisition because some of the channels are empty

/* Interplay between correlator and channels */
EXTERN int32 Corr_2_PVT_P[MAX_CHANNELS][2];					//!< \ingroup PIPES Output measurements to PVT
EXTERN int32 PVT_2_Corr_P[MAX_CHANNELS][2];					//!< \ingroup PIPES Output measurements to PVT
EXTERN int32 Trak_2_Corr_P[MAX_CHANNELS][2];				//!< \ingroup PIPES Have the tracking tell the correlator to start or stop a channel

/* How do we decode the ephemerides? */
EXTERN int32 Chan_2_Ephem_P[2];								//!< \ingroup PIPES Dump raw subframes to Ephemeris

/* User feedback */
EXTERN int32 FIFO_2_Telem_P[2];								//!< \ingroup PIPES Send FIFO status (nodes empty, agc value)
EXTERN int32 FIFO_2_PVT_P[2];								//!< \ingroup PIPES Send number of incoming measurements to nav
EXTERN int32 Acq_2_Telem_P[2];								//!< \ingroup PIPES Send latest acquisition attempts to GUI
EXTERN int32 PVT_2_Telem_P[2];								//!< \ingroup PIPES Send latest nav solution to GUI
EXTERN int32 Ephem_2_Telem_P[2];							//!< \ingroup PIPES Send latest ephemeris to GUI
EXTERN int32 SV_Select_2_Telem_P[2];						//!< \ingroup PIPES Send predicted SV states to GUI
EXTERN int32 PVT_2_SV_Select_P[2];							//!< \ingroup PIPES Output nav state to sat select
EXTERN int32 Telem_2_Cmd_P[2];								//!< \ingroup PIPES Send received commands to Commando
EXTERN int32 Cmd_2_Telem_P[2];								//!< \ingroup PIPES Send results of commands to Telemetry
/*----------------------------------------------------------------------------------------------*/


/* Part 4, Anything else */
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/


#endif /* GLOBALS_H */
