/*! \file threaded_object.cpp
	Implements member functions of threaded_object class.
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

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
void Threaded_Object::Start_Thread(void *(*_start_routine)(void*), void *_arg)
{
	pthread_create(&thread, NULL, _start_routine, _arg);
}
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
void Threaded_Object::Stop()
{
	pthread_cancel(thread);
	pthread_join(thread, NULL);
}
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
Threaded_Object::Threaded_Object()
{

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_unlock(&mutex);
	execution_tic = 0;
	start_tic = 0;
	stop_tic = 0;

}
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
Threaded_Object::~Threaded_Object()
{
	pthread_mutex_destroy(&mutex);
}
/*----------------------------------------------------------------------------------------------*/


