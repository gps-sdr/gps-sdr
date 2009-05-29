/*----------------------------------------------------------------------------------------------*/
/*! \file globals.h
//
// FILENAME: globals.h
//
// DESCRIPTION: All the global objects in the receiver live here
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

#ifdef GLOBALS_HERE
	#define EXTERN
#else
	#define EXTERN extern
#endif

#ifndef GLOBALS_H_
#define GLOBALS_H_

/* Part 1, Threaded and Semi-threaded objects */
/*----------------------------------------------------------------------------------------------*/
EXTERN class Keyboard		*pKeyboard;						//!< Handle user input
EXTERN class FIFO			*pFIFO;							//!< Get data and pass it into the receiver
EXTERN class PVT			*pPVT;							//!< Do the PVT solution
EXTERN class Ephemeris		*pEphemeris;					//!< Extract the ephemeris
EXTERN class Acquisition	*pAcquisition;					//!< Perform acquisitions
EXTERN class Correlator		*pCorrelator;					//!< Correlator
EXTERN class Channel		*pChannels[MAX_CHANNELS];		//!< Channels (uses correlations to close the loops)
EXTERN class SV_Select		*pSV_Select;					//!< Contains the channels and drives the channel objects
EXTERN class Telemetry		*pTelemetry;					//!< Simple ncurses interface
EXTERN class Commando		*pCommando;						//!< Process and execute commands
EXTERN class GPS_Source		*pSource;						//!< Get the GPS data from somewhere
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
EXTERN int32 grun;									//!< Keep all the threads active (technically, this should be mutex protected, but eh, who cares? )
EXTERN Options_S gopt;								//!< Global receiver options
EXTERN struct timeval starttime;					//!< Get receiver start time
/*----------------------------------------------------------------------------------------------*/


/* Part 3, Pipes */
/*----------------------------------------------------------------------------------------------*/
/* Interplay between acquisition and tracking */
EXTERN int32 SVS_2_COR_P[2];						//!< \ingroup PIPES Send an acquisition result to the correlator to start a channel
EXTERN int32 CHN_2_EPH_P[2];						//!< \ingroup PIPES Output raw subframes to Ephemeris
EXTERN int32 PVT_2_TLM_P[2];						//!< \ingroup PIPES Output PVT state to Telemetry
EXTERN int32 SVS_2_TLM_P[2];						//!< \ingroup PIPES Output predicted SV states to Telemetry
EXTERN int32 EKF_2_TLM_P[2];						//!< \ingroup PIPES Output EKF state to Telemetry
EXTERN int32 CMD_2_TLM_P[2];						//!< \ingroup PIPES Output results of commands to Telemetry
EXTERN int32 ACQ_2_SVS_P[2];						//!< \ingroup PIPES Request an acquisition because some of the channels are empty
EXTERN int32 EKF_2_SVS_P[2];						//!< \ingroup PIPES Output EKF state to SV Select
EXTERN int32 PVT_2_SVS_P[2];						//!< \ingroup PIPES Output PVT state to SV Select
EXTERN int32 TLM_2_CMD_P[2];						//!< \ingroup PIPES Output received commands to Commando
EXTERN int32 SVS_2_ACQ_P[2];						//!< \ingroup PIPES Request an acquisition because some of the channels are empty
EXTERN int32 COR_2_ACQ_P[2];						//!< \ingroup PIPES Output packets of IF data to the Acquisition
EXTERN int32 ISRP_2_PVT_P[2];						//!< \ingroup PIPES Output measurement preamble to PVT
EXTERN int32 ISRM_2_PVT_P[2];						//!< \ingroup PIPES Output measurements to PVT
/*----------------------------------------------------------------------------------------------*/


/* Part 4, Anything else */
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/


#endif /* GLOBALS_H */
