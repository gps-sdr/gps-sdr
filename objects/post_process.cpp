/*! \file Post_Process.cpp
	Implements member functions of Post_Process class.
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

#include "post_process.h"

/*----------------------------------------------------------------------------------------------*/
void *Post_Process_Thread(void *_arg)
{

	Post_Process *aPost_Process = pPost_Process;

	aPost_Process->Open();

	while(grun)
	{
		aPost_Process->Import();
		aPost_Process->Export();
	}

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Post_Process::Start()
{
	pthread_create(&thread, NULL, Post_Process_Thread, NULL);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Post_Process::Stop()
{
	pthread_cancel(thread);
	pthread_join(thread, NULL);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Post_Process::Post_Process(char *_fname)
{
	int32 bytes;

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_unlock(&mutex);

	buff = new CPX[310*SAMPS_MS];

	/* Create named pipe */
	fifo = mkfifo("/tmp/GPSPIPE", 0666);

	/* Open the source file */
	strcpy(fname, _fname);
	fp = fopen(fname, "rb");
	if(fp == NULL)
		printf("Could not open %s for reading\n",fname);

	bytes = -1000*60*IF_SAMPS_MS*sizeof(CPX);
	fseek(fp, bytes, SEEK_END);

	if(gopt.verbose)
		printf("Creating Post_Process\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Post_Process::~Post_Process()
{

	if(fp)
		fclose(fp);

	close(npipe);
	delete [] buff;
	delete [] buff_in;

	if(gopt.verbose)
		printf("Destructing Post_Process\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Post_Process::Import()
{

	int32 bytes;

	bytes = fread(&buff[0], sizeof(CPX), IF_SAMPS_MS, fp);

	bytes = -1000*60*IF_SAMPS_MS*sizeof(CPX);

	/* Reset to loop forever */
	if(feof(fp))
		fseek(fp, bytes, SEEK_END);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Post_Process::Export()
{
	write(npipe, &buff[0], sizeof(CPX)*IF_SAMPS_MS);
	usleep(1000);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Post_Process::Open()
{

	npipe = open("/tmp/GPSPIPE", O_WRONLY);

}
/*----------------------------------------------------------------------------------------------*/



