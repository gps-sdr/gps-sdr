/*----------------------------------------------------------------------------------------------*/
/*! \file threaded_object.h
//
// FILENAME: threaded_object.h
//
// DESCRIPTION: Implements member functions of the threaded_object class.
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

#include "threaded_object.h"

/*----------------------------------------------------------------------------------------------*/
Threaded_Object::Threaded_Object(const char _task_name[8])
{
	int32 lcv;

	for(lcv = 0; lcv < 8; lcv++)
		task_name[lcv] = _task_name[lcv];

	execution_tic = 0;
	start_tic = 0;
	temp_start_tic = 0;
	stop_tic = 0;
	size = 0;
	stack = TASK_STACK_SIZE;
	task_mem = NULL;
	object_mem = NULL;

	#ifdef LINUX_OS
		pthread_mutex_init(&mutex, NULL);
		pthread_mutex_unlock(&mutex);
	#endif

	#ifdef NUCLEUS_OS
		STATUS status;

		status = NU_Create_Semaphore(&mutex, task_name, 1, NU_FIFO);
		CHECK_STATUS(status);

		status = NU_Allocate_Memory(&System_Memory, &task_mem, TASK_STACK_SIZE, NU_NO_SUSPEND);
		CHECK_STATUS(status);
	#endif
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Threaded_Object::~Threaded_Object()
{
	#ifdef LINUX_OS
		pthread_mutex_destroy(&mutex);
	#endif

	#ifdef NUCLEUS_OS
		NU_Obtain_Semaphore(&mutex,NU_SUSPEND);
	#endif
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Threaded_Object::Lock()
{

	#ifdef LINUX_OS
		pthread_mutex_lock(&mutex);
	#endif

	#ifdef NUCLEUS_OS
		NU_Obtain_Semaphore(&mutex,NU_SUSPEND);
	#endif

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Threaded_Object::Unlock()
{

	#ifdef LINUX_OS
		pthread_mutex_unlock(&mutex);
	#endif

	#ifdef NUCLEUS_OS
		NU_Release_Semaphore(&mutex);
	#endif

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 Threaded_Object::Trylock()
{

	#ifdef LINUX_OS
		return(FALSE);
	#endif

	#ifdef NUCLEUS_OS
		return(FALSE);
	#endif

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
#ifdef LINUX_OS
void Threaded_Object::Start_Thread(void *(*_start_routine)(void*), void *_arg)
{
	pthread_create(&task, NULL, _start_routine, _arg);
}
#endif

#ifdef NUCLEUS_OS
void Threaded_Object::Start_Thread(void (*_start_routine)(UNSIGNED)(void*))
{
	STATUS status;

	status = NU_Create_Task(&task, task_name, _start_routine, 0, NU_NULL,
		task_mem, TASK_STACK_SIZE, ACQUISITION_PRIORITY, 1, NU_PREEMPT, NU_START);
	CHECK_STATUS(status);
}
#endif
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Threaded_Object::Stop()
{
	#ifdef LINUX_OS
		pthread_cancel(task);
		pthread_join(task, NULL);
	#endif


	#ifdef NUCLEUS_OS

	#endif
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 Threaded_Object::getExecTic()
{
	return(execution_tic);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 Threaded_Object::getStartTic()
{
	return(start_tic);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 Threaded_Object::getStopTic()
{
	return(stop_tic);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 Threaded_Object::getSize()
{
	return(size);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 Threaded_Object::getStack()
{
	return(stack);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void *Threaded_Object::getTaskMem()
{
	return(task_mem);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void *Threaded_Object::getObjectMem()
{
	return(object_mem);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Threaded_Object::IncExecTic()
{
	execution_tic++;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Threaded_Object::IncStartTic()
{
	#ifdef LINUX_OS
		gettimeofday(&tv, NULL);
		temp_start_tic = 100*(tv.tv_sec - starttime.tv_sec) + (tv.tv_usec - starttime.tv_usec)/10000;
	#endif

	#ifdef NUCLEUS_OS
		temp_start_tic = tic_tracking_isr;
	#endif
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Threaded_Object::IncStopTic()
{
	#ifdef LINUX_OS
		gettimeofday(&tv, NULL);
		stop_tic = 100*(tv.tv_sec - starttime.tv_sec) + (tv.tv_usec - starttime.tv_usec)/10000;
		start_tic = temp_start_tic;
	#endif

	#ifdef NUCLEUS_OS
		stop_tic = tic_tracking_isr;
		start_tic = temp_start_tic;
	#endif
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Threaded_Object::setStack()
{
	uint32 t;

	#ifdef LINUX_OS
		stack = 0;
	#endif

	#ifdef NUCLEUS_OS
		t = NU_Check_Stack();

		if(t < stack)
		{
			stack = t;
		}
	#endif
}
/*----------------------------------------------------------------------------------------------*/
