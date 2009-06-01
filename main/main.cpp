/*----------------------------------------------------------------------------------------------*/
/*! \file main.cpp
//
// FILENAME: main.cpp
//
// DESCRIPTION: Entry point of the receiver
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

#define GLOBALS_HERE
#include "includes.h"

/*----------------------------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	FILE* fp;
	int success;
	int state;
	int fin[2];
	int fout[2];
	pid_t pid;

	Parse_Arguments(argc, argv);

	success = Hardware_Init();
	if(success == false)
	{
		Hardware_Shutdown();
		fprintf(stderr,"Hardware_Init() failed, aborting.\n");
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
		fprintf(stderr,"Pipes_Init() failed, aborting.\n");
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
		fprintf(stderr,"Object_Init() failed, aborting.\n");
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
		fprintf(stderr,"Thread_Init() failed, aborting.\n");
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

