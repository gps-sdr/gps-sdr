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

	/* If the command line indicates that the usrp should be called
	 * do that stuff here
	 */
	if(gopt.usrp_internal)
	{

		pipe(fin);
		pipe(fout);

		pid = fork();
		if(pid == 0)
		{
		    close(fin[1]);
		    dup2(fin[0], STDIN_FILENO);		/* Connect the read end of the pipe to standard input.  */
		    close(STDOUT_FILENO);			/* Kill the stdout so not to screw up the ncurses display */

		    /* Replace the child process with the gps-usrp client */
			execl("gps-usrp", NULL, NULL);
		}
		else
		{
			while(grun)
			{
				usleep(10000);
			}

			Thread_Shutdown();

			Pipes_Shutdown();

			Object_Shutdown();

			Hardware_Shutdown();

			/* Kill the usrp-gps child */
			close(fin[0]);
		    fp = fdopen(fin[1], "w");
		    fprintf(fp, "Q\n");
		    fflush(fp);
		    close(fin[1]);
			pid = waitpid(-1, &state, 0);
		}
	}
	else
	{

		while(grun)
		{
			usleep(10000);
		}

		Thread_Shutdown();

		Pipes_Shutdown();

		Object_Shutdown();

		Hardware_Shutdown();

	}

	return(1);

}
/*----------------------------------------------------------------------------------------------*/

