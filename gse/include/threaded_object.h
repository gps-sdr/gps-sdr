/*! \file Threaded_Object.h
	Defines the class Threaded_Object
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

#ifndef THREADED_OBJECT_H_
#define THREADED_OBJECT_H_

#include <pthread.h>
#include <time.h>
#include "defines.h"

/*! \ingroup CLASSES
 *
 */
class Threaded_Object
{

	protected:

		/* Default object variables */
		uint32				pid;			//!< Pid value
		uint32 				execution_tic;	//!< Execution counter
		uint32 				start_tic;		//!< OS tic at start of function
		uint32 				stop_tic;		//!< OS tic at end of function
		pthread_t 			thread;			//!< For the thread
		pthread_mutex_t		mutex;			//!< Protect the following variable

	public:

		/* Default object methods */
		Threaded_Object();										//!< Constructor
		~Threaded_Object();										//!< Destructor

		void Start_Thread(void *(*_start_routine)(void*), void *_arg);	//!< Start the thread
		void Stop();											//!< Stop the thread
		void Lock(){pthread_mutex_lock(&mutex);};				//!< Lock the object's mutex
		int32 TryLock(){return(pthread_mutex_trylock(&mutex));};//!< Try the lock
		void Unlock(){pthread_mutex_unlock(&mutex);};			//!< Unlock the object's mutex

		void SetPid(){pid = getpid();};							//!< Set the pid
		uint32 GetPid(){return(pid);};							//!< Get the pid

		uint32 GetExecTic(){return(execution_tic);};			//!< Get the execution counter
		uint32 GetStartTic(){return(start_tic);};				//!< Get the Nucleus tic at start of function
		uint32 GetStopTic(){return(stop_tic);};					//!< Get the Nucleus tic at	end of function

		void IncExecTic(){execution_tic++;};							//!< Increment execution tic
		void IncStartTic(){start_tic = 100*clock()/CLOCKS_PER_SEC;};	//!< Use OS method to set the start tic
		void IncStopTic(){stop_tic = 100*clock()/CLOCKS_PER_SEC;};		//!< Use OS method to set the stop tic

};

#endif /* Threaded_Object_H */
