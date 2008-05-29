/*! \file Main.cpp
	Entry point of program
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

#define GLOBALS_HERE

#include "includes.h"

/*----------------------------------------------------------------------------------------------*/
int main(int32 argc, char* argv[])
{

	int32 success;

	Parse_Arguments(argc, argv);
	
	success = Hardware_Init();
	if(success == false)
	{
		Hardware_Shutdown();
		printf("Hardware_Init() failed, aborting.\n");
		return(-1);
	}

	if(success)
	{
		success = Pipes_Init();
	}
	else
	{
		Pipes_Shutdown();
		Hardware_Shutdown();
		printf("Pipes_Init() failed, aborting.\n");
		return(-1);
	}
			
	if(success)
	{
		success = Object_Init();
	}
	else
	{
		Pipes_Shutdown();
		Object_Shutdown();
		Hardware_Shutdown();
		printf("Object_Init() failed, aborting.\n");
		return(-1);
	}
		
	if(success)
	{
		success = Thread_Init();
	}
	else
	{
		Thread_Shutdown();
		Pipes_Shutdown();
		Object_Shutdown();
		Hardware_Shutdown();
		printf("Thread_Init() failed, aborting.\n");
		return(-1);
	}


	while(grun)
	{
		usleep(10000);
	}


	Thread_Shutdown();
	
	Pipes_Shutdown();

	Object_Shutdown();

	Hardware_Shutdown();

	return(1);

}
/*----------------------------------------------------------------------------------------------*/

