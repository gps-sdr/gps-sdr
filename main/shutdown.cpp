/*----------------------------------------------------------------------------------------------*/
/*! \file shutdown.cpp
//
// FILENAME: shutdown.cpp
//
// DESCRIPTION: Terminates the receiver software gracefully
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
//#include "telemetry.h"		//!< Ncurses telemetry
//#include "serial_telemetry.h"	//!< Serial/GUI telemetry
//#include "commando.h"			//!< Command interface
#include "sv_select.h"			//!< Drives acquisition/reacquisition process
//#include "post_process.h"		//!< Run the receiver from a file
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! First stop all threads */
void Thread_Shutdown(void)
{
	char buff[1024*1024];
	int32 lcv;

	/* Start the keyboard thread to handle user input from stdio */
	pKeyboard->Stop();

	/* Stop the FIFO */
	pFIFO->Stop();

	/* Stop the telemetry */
//	if(gopt.ncurses)
//		pTelemetry->Stop();
//	else
//		pSerial_Telemetry->Stop();

	/* Uh-oh */
	pPVT->Stop();

	/* Stop the correlator */
	pCorrelator->Stop();

	/* Stop the acquistion */
	pAcquisition->Stop();

	/* Stop the ephemeris */
	pEphemeris->Stop();

//	/* Stop the command interface */
//	pCommando->Stop();

	/* Stop the tracking */
	pSV_Select->Stop();

//	/* Stop spoofing my named pipe yo */
//	if(gopt.post_process)
//		pPost_Process->Stop();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! Shutdown all pipes */
void Pipes_Shutdown(void)
{

	close(SVS_2_COR_P[READ]);
	close(CHN_2_EPH_P[READ]);
	close(PVT_2_TLM_P[READ]);
	close(SVS_2_TLM_P[READ]);
	close(EKF_2_TLM_P[READ]);
	close(CMD_2_TLM_P[READ]);
	close(ACQ_2_SVS_P[READ]);
	close(EKF_2_SVS_P[READ]);
	close(PVT_2_SVS_P[READ]);
	close(TLM_2_CMD_P[READ]);
	close(SVS_2_ACQ_P[READ]);
	close(COR_2_ACQ_P[READ]);
	close(ISRP_2_PVT_P[READ]);
	close(ISRM_2_PVT_P[READ]);

	close(SVS_2_COR_P[WRITE]);
	close(CHN_2_EPH_P[WRITE]);
	close(PVT_2_TLM_P[WRITE]);
	close(SVS_2_TLM_P[WRITE]);
	close(EKF_2_TLM_P[WRITE]);
	close(CMD_2_TLM_P[WRITE]);
	close(ACQ_2_SVS_P[WRITE]);
	close(EKF_2_SVS_P[WRITE]);
	close(PVT_2_SVS_P[WRITE]);
	close(TLM_2_CMD_P[WRITE]);
	close(SVS_2_ACQ_P[WRITE]);
	close(COR_2_ACQ_P[WRITE]);
	close(ISRP_2_PVT_P[WRITE]);
	close(ISRM_2_PVT_P[WRITE]);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! Shutdown all threaded objects and global variables */
void Object_Shutdown(void)
{
	int32 lcv;

	delete pCorrelator;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		delete pChannels[lcv];

//	if(gopt.post_process)
//		delete pPost_Process;

	delete pKeyboard;
	delete pAcquisition;
	delete pEphemeris;
//	delete pFIFO;
	delete pSV_Select;
//	delete pTelemetry;
	delete pPVT;
//	delete pCommando;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! Close out any hardware */
void Hardware_Shutdown(void)
{



}
/*----------------------------------------------------------------------------------------------*/
