/*! \file Tracking.h
	Defines the class Tracking
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

#ifndef Tracking_H
#define Tracking_H

#include "includes.h"

typedef class Tracking
{

	private:

		pthread_t 		thread;	//!< For the thread
		Acq_Result_S	result;
		Acq_Request_S	request;
		Correlation_S	corr[MAX_CHANNELS];
		NCO_Command_S 	feedback[MAX_CHANNELS];
		uint32			interrupt[MAX_CHANNELS];
		Chan_Packet_S	chan_packets[MAX_CHANNELS];

	public:

		Tracking();
		~Tracking();
		void Inport();
		void Track();
		void Export();
		void Start();
		void Stop();
		void StartChannel();
		void RequestAcq();

};

#endif /* Tracking_H */
