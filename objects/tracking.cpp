/*! \file Tracking.cpp
	Implements member functions of Tracking class.
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

#include "tracking.h"

/*----------------------------------------------------------------------------------------------*/
void *Tracking_Thread(void *_arg)
{

	Tracking *aTracking = pTracking;

	while(grun)
	{
		aTracking->Track();
		aTracking->Export();
	}
	
	pthread_exit(0);
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Tracking::Tracking()
{

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Tracking::~Tracking()
{

	printf("Destructing Tracking\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Tracking::Inport()
{


}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void Tracking::Track()
{
	int32 lcv, chan;
	chan = 666;

	/* See if any correlators are available */
	pthread_mutex_lock(&mInterrupt);

	/* If an empty channel exists, ask for an acquisition */	
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		if(gInterrupt[lcv] == 0)
		{
			chan = lcv;
			RequestAcq();
			break;
		}	

	pthread_mutex_unlock(&mInterrupt);
	
	/* Do something with acquisition */
	if(chan != 666)
	{
		/* Tell the acquisition to try */
		request.state = 1;
		write(Trak_2_Acq_P[WRITE], &request, sizeof(Acq_Request_S));
	
		/* Read an acquisition result, do stuff depending on the state */
		read(Acq_2_Trak_P[READ], &result, sizeof(Acq_Result_S));
		
		/* See if the SV is already being tracked */	
		for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
			if((pChannels[lcv]->active) && (pChannels[lcv]->sv == result.sv))
				return;
	
		if(result.success)
		{
			/* Else start a channel */
			result.chan = chan;
		
			/* Map receiver channels to channels on correlator */
			write(Trak_2_Corr_P[result.chan][WRITE], &result, sizeof(Acq_Result_S));
		}
	}
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Tracking::Export()
{


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Tracking::RequestAcq()
{
	
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Tracking::StartChannel()
{

		

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Tracking::Start()
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
	pthread_create(&thread, NULL, Tracking_Thread, NULL);
	
	if(gopt.verbose)
		printf("Tracking thread started\n");	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Tracking::Stop()
{
	pthread_join(thread, NULL);
	
	if(gopt.verbose)
		printf("Tracking thread stopped\n");
}
/*----------------------------------------------------------------------------------------------*/

