/*! \file FIFO.h
	Defines the class FIFO
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

#ifndef FIFO_H
#define FIFO_H

#include "includes.h"

#define FIFO_DEPTH (1000)	//!< In ms

/*! \ingroup CLASSES
 *
 */
class FIFO
{

	private:

		/* Default object variables */
		uint32 				execution_tic;	//!< Execution counter
		uint32 				start_tic;		//!< OS tic at start of function
		uint32 				stop_tic;		//!< OS tic at end of function
		pthread_t 			thread;			//!< For the thread
		pthread_mutex_t		mutex;			//!< Protect the following variable

		pthread_mutex_t	mutex_head;	//!< Semaphore to protect the FIFO's head
		pthread_mutex_t	mutex_tail;	//!< Semaphore to protect the FIFO's tail
		pthread_mutex_t	chan_mutex[MAX_CHANNELS+1];

		CPX *if_buff;		//!< Get the data from the named pipe
		ms_packet *buff;	//!< 1 second buffer (in 1 ms packets)
		ms_packet *head;	//!< Pointer to the head
		ms_packet *tail;	//!< Pointer to the tail

		int32 	npipe;		//!< Get the IF data from the USRP_Uno program, and its pipe ("/tmp/GPSPIPE")
		int32 	count;		//!< Count the number of packets received
		int32	agc_scale;	//!< To do the AGC
		int32	overflw;
		int32	tic;		//!< Master receiver tic

		FIFO_2_Telem_S telem; //!< Stuff to dump to the telemetry

	public:

		FIFO();				//!< Create circular FIFO
		~FIFO();			//!< Destroy circular FIFO
		void Start();								//!< Start the thread
		void Stop();								//!< Stop the thread
		void Import();								//!< Get data into the thread
		void Export();								//!< Get data out of the thread
		void Lock();								//!< Lock the object's mutex
		void Unlock();								//!< Unlock the object's mutex
		uint32 GetExecTic(){return(execution_tic);};//!< Get the execution counter
		uint32 GetStartTic(){return(start_tic);};	//!< Get the Nucleus tic at start of function
		uint32 GetStopTic(){return(execution_tic);};//!< Get the Nucleus tic at end of function

		void Open();
		void Enqueue();
		void Dequeue(int32 _resource, ms_packet *p);
		void SetScale(int32 _agc_scale);
		void Wait(int32 _resource);
};

#endif /* FIFO_H */


