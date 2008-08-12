/*! \file Keyboard.cpp
	Implements member functions of Keyboard class.
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

#include "keyboard.h"

/*----------------------------------------------------------------------------------------------*/
void *Keyboard_Thread(void *_arg)
{

	Keyboard *aKeyboard = pKeyboard;
	int32 key;

	while(grun)
	{

		key = getchar();
		printf("%c",(char)key);

		if((char)key == 'Q')
		{
			grun = 0x0;
		}

		if((char)key == 'n') //Navigation
		{
			pTelemetry->Lock();
			pTelemetry->SetDisplay(0);
			pTelemetry->Unlock();
		}

		if((char)key == 'a') //Almanac & SV predict
		{
			pTelemetry->Lock();
			pTelemetry->SetDisplay(1);
			pTelemetry->Unlock();
		}

		if((char)key == 'h') //Acquisition history
		{
			pTelemetry->Lock();
			pTelemetry->SetDisplay(2);
			pTelemetry->Unlock();
		}


	}

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Keyboard::Start()
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
	param.sched_priority = KEY_PRIORITY;

	/* Setting the new scheduling param */
	ret = pthread_attr_setschedparam(&tattr, &param);
	ret = pthread_attr_setschedpolicy(&tattr, SCHED_RR);

	/* With new priority specified */
	pthread_create(&thread, &tattr, Keyboard_Thread, NULL);

	if(gopt.verbose)
		printf("Keyboard thread started\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Keyboard::Stop()
{

	if(gopt.verbose)
		printf("Keyboard thread stopped\n");

	pthread_cancel(thread);
	pthread_join(thread, NULL);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Keyboard::Keyboard()
{

	if(gopt.verbose)
		printf("Creating Keyboard\n");

	temp = 0;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Keyboard::~Keyboard()
{
	printf("Destructing Keyboard\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Keyboard::Inport()
{

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Keyboard::Parse()
{

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Keyboard::Export()
{

}
/*----------------------------------------------------------------------------------------------*/


