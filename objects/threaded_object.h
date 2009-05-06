/*----------------------------------------------------------------------------------------------*/
/*! \file threaded_object.h
//
// FILENAME: threaded_object.h
//
// DESCRIPTION: Defines the threaded_object class.
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

#ifndef THREADED_OBJECT_H_
#define THREADED_OBJECT_H_

#include "includes.h"

#ifdef LINUX_OS
	#include <pthread.h>
	#include <time.h>
#endif

/*! @ingroup CLASSES
	@brief The Threaded_Object class provides the base functionality to monitor each tasks' status
	 and health. This should be OS transparent, and will depend on the #defines LINUX_OS or NUCLEUS_OS
	 to control compilation */
class Threaded_Object
{

	protected:

		/* Default object variables */
		char				task_name[8];	//!< 8 character task name
		uint32 				execution_tic;	//!< Execution counter
		uint32 				start_tic;		//!< ISR tic at start of function
		uint32				temp_start_tic;	//!< ISR tic at start of function (temp)
		uint32 				stop_tic;		//!< ISR tic at end of function
		uint32				size;			//!< Size of object
		uint32				stack;			//!< Number of bytes on stack
		void 				*task_mem;		//!< Pointer to the task memory
		void				*object_mem;	//!< Pointer to the class memory

		#ifdef NUCLEUS_OS
			NU_TASK 		task;			//!< Nucleus task variable
			NU_SEMAPHORE 	mutex;			//!< Lock and unlock the object
		#endif

		#ifdef LINUX_OS
			pthread_t 		task;			//!< pthread task variable
			pthread_mutex_t	mutex;			//!< Lock and unlock the object
			struct timeval 	tv;				//!< To emulate 500 us tic
		#endif

	public:

		/* Default object methods */
		Threaded_Object(char _task_name[8]);//!< Constructor
		~Threaded_Object();		//!< Destructor

		void Lock();			//!< Lock the object's mutex
		void Unlock();			//!< Unlock the object's mutex
		uint32 Trylock();		//!< Trylock, locks the mutex and returns true, else returns false
		void Stop();			//!< Stop the thread

		uint32 getExecTic();	//!< Get the execution counter
		uint32 getStartTic();	//!< Get the 500 us ISR tic at start of function
		uint32 getStopTic();	//!< Get the 500 us ISR tic at end of function
		uint32 getSize();		//!< Get the size of the object
		uint32 getStack();		//!< Get the stack size
		void *getTaskMem();		//!< Get the task memory
		void *getObjectMem();	//!< Get the class memory

		void IncExecTic();		//!< Increment execution tic
		void IncStartTic();		//!< Get the 500 us ISR tic
		void IncStopTic();		//!< Get the 500 us ISR tic
		void setStack();		//!< Set the stack depth

		#ifdef LINUX_OS
			void Start_Thread(void *(*_start_routine)(void*), void *_arg);	//!< Start the thread
		#endif

		#ifdef NUCLEUS_OS
			void Start_Thread(void (*_start_routine)(UNSIGNED)(void*));	//!< Start the thread
		#endif

};

#endif /* Threaded_Object_H */

