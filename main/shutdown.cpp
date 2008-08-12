/*! \file Shutdown.cpp
	Shuts down the receiver gracefully
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

/*----------------------------------------------------------------------------------------------*/
/*! First stop all threads */
void Thread_Shutdown(void)
{
	char buff[1024*1024];
	int32 lcv;

	/* Start the keyboard thread to handle user input from stdio */
	pKeyboard->Stop();

	/* Stop the telemetry */
	pTelemetry->Stop();

	/* Stop the FIFO */
	pFIFO->Stop();

	/* Uh-oh */
	pPVT->Stop();

	/* Start up the correlators */
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		pCorrelators[lcv]->Stop();

	/* Stop the acquistion */
	pAcquisition->Stop();

	/* Stop the ephemeris */
	pEphemeris->Stop();

	/* Stop the tracking */
	pSV_Select->Stop();

	/* Stop spoofing my named pipe yo */
	if(gopt.post_process)
		pPost_Process->Stop();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! Shutdown all pipes */
void Pipes_Shutdown(void)
{

	int32 lcv;

	/* Acq and track play together */
	close(Trak_2_Acq_P[READ]);
	close(Trak_2_Acq_P[WRITE]);
	close(Acq_2_Trak_P[READ]);
	close(Acq_2_Trak_P[WRITE]);
	close(FIFO_2_Telem_P[READ]);
	close(FIFO_2_Telem_P[WRITE]);
	close(FIFO_2_PVT_P[READ]);
	close(FIFO_2_PVT_P[WRITE]);
	close(Acq_2_Telem_P[READ]);
	close(Acq_2_Telem_P[WRITE]);
	close(PVT_2_Telem_P[READ]);
	close(PVT_2_Telem_P[WRITE]);
	close(Chan_2_Ephem_P[READ]);
	close(Chan_2_Ephem_P[WRITE]);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		close(Trak_2_Corr_P[lcv][READ]);
		close(Trak_2_Corr_P[lcv][WRITE]);
		close(Corr_2_PVT_P[lcv][READ]);
		close(Corr_2_PVT_P[lcv][WRITE]);
		close(PVT_2_Corr_P[lcv][READ]);
		close(PVT_2_Corr_P[lcv][WRITE]);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! Shutdown all threaded objects and global variables */
void Object_Shutdown(void)
{
	int32 lcv;

	pthread_mutex_destroy(&mAcq);
	pthread_mutex_destroy(&mInterrupt);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		delete pCorrelators[lcv];

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		delete pChannels[lcv];

	if(gopt.post_process)
		delete pPost_Process;

	delete pKeyboard;
	delete pAcquisition;
	delete pEphemeris;
	delete pFIFO;
	delete pSV_Select;
	delete pTelemetry;
	delete pPVT;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! Close out any hardware */
void Hardware_Shutdown(void)
{



}
/*----------------------------------------------------------------------------------------------*/
