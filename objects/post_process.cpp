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
		aPost_Process->Inport();
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
	int32 lcv, k, type, agc_scale;

	buff_in = new CPX[310*IF_SAMPS_MS];
	buff = new CPX[310*SAMPS_MS];
	memset(&results[0], 0x0, NUM_CODES*sizeof(Acq_Result_S));
	Acq_Result_S *p;

	agc_scale = 0;

	/* Create named pipe */
	fifo = mkfifo("/tmp/GPSPIPE", 0666);

	/* Open the source file */
	strcpy(fname, _fname);
	fp = fopen(fname, "rb");
	if(fp == NULL)
		printf("Could not open %s for reading\n",fname);

	/* First read in several seconds of data */
	fread(&buff_in[0], sizeof(CPX), 310*IF_SAMPS_MS, fp);

	/* Rewind the data */
	fseek(fp, 0x0, SEEK_SET);

	/* Downsample to 2048 samps/ms */
	downsample(buff, buff_in, SAMPLE_FREQUENCY, IF_SAMPLE_FREQUENCY, IF_SAMPS_MS*310);

	/* Init the AGC scale value */
	init_agc(&buff[0], SAMPS_MS, AGC_BITS, &agc_scale);

	/* Now actually run the AGC */
	for(lcv = 0; lcv < 310; lcv++)
		run_agc(&buff[lcv*SAMPS_MS], SAMPS_MS, AGC_BITS, &agc_scale);

	pFIFO->SetScale(agc_scale);
	printf("AGC Scale: %d\n",agc_scale);


	printf("Type   SV        Delay      Doppler             Power    Detected\n");
	printf("-----------------------------------------------------------------\n");

	/* Now do the acquisition(s) */
	for(type = 0; type < 1; type++)
	{
		pAcquisition->doPrepIF(type, buff);

		for(lcv = 0; lcv < NUM_CODES; lcv++)
		{
			if(results[lcv].success == 0)
			{
				if(type == 0)
					results[lcv] = pAcquisition->doAcqStrong(lcv, gopt.doppler_min, gopt.doppler_max);
				else if(type == 1)
					results[lcv] = pAcquisition->doAcqMedium(lcv, gopt.doppler_min, gopt.doppler_max);
				else
					results[lcv] = pAcquisition->doAcqWeak(lcv, gopt.doppler_min, gopt.doppler_max);

				p = &results[lcv];
				printf("  %02d,  %02d,  %10.2f,  %10.0f,  %15.0f,          %1d\n",p->type,lcv+1,p->delay,p->doppler,p->magnitude,p->success);
			}

		}
	}

	pAcquisition->Export("AcqPP.txt");

	/* For the detected SVs, start up correlators */
	k = 0;
	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		if(results[lcv].success)
		{
			/* Map receiver channels to channels on correlator */
			write(Trak_2_Corr_P[k][WRITE], &results[lcv], sizeof(Acq_Result_S));
			if(++k >= MAX_CHANNELS)
				break;
		}
	}

	if(gopt.verbose)
		printf("Creating Post_Process\n");


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Post_Process::~Post_Process()
{

	close(npipe);
	delete [] buff;
	delete [] buff_in;

	if(gopt.verbose)
		printf("Destructing Post_Process\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Post_Process::Inport()
{

	fread(&buff[0], sizeof(CPX), IF_SAMPS_MS, fp);
	if(feof(fp))
		grun = false;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Post_Process::Parse()
{

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Post_Process::Export()
{
//
//	int32 nbytes, bwrote, bread;;
//	char *pbuff;
//
//	bread = IF_SAMPS_MS*sizeof(CPX);
//
//	/* Dump to the pipe */
//	nbytes = 0; pbuff = (char *)&buff[0];
//	while((nbytes < bread) && grun)
//	{
//		bwrote = write(npipe, buff[nbytes], PIPE_BUF);
//
//		signal(SIGPIPE, SIG_IGN);
//		if(bwrote > 0)
//			nbytes += bwrote;
//	}
//
	write(npipe, &buff[0], sizeof(CPX)*IF_SAMPS_MS);
	usleep(250);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Post_Process::Open()
{

	npipe = open("/tmp/GPSPIPE", O_WRONLY);

}
/*----------------------------------------------------------------------------------------------*/



