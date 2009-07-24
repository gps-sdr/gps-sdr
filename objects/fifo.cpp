/*----------------------------------------------------------------------------------------------*/
/*! \file fifo.h
//
// FILENAME: fifo.h
//
// DESCRIPTION: Implements member functions of the FIFO class.
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

#include "fifo.h"

/*----------------------------------------------------------------------------------------------*/
void *FIFO_Thread(void *_arg)
{

	FIFO *aFIFO = pFIFO;

	while(grun)
	{
		aFIFO->Import();
		aFIFO->IncExecTic();
	}

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void FIFO::Start()
{

	Start_Thread(FIFO_Thread, NULL);

	if(gopt.verbose)
		fprintf(stdout,"FIFO thread started\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
FIFO::FIFO():Threaded_Object("FIFTASK")
{
	int32 lcv;

	/* Create the buffer */
	buff = new ms_packet[FIFO_DEPTH];
	memset(buff, 0x0, sizeof(ms_packet)*FIFO_DEPTH);
	head = &buff[0];
	tail = &buff[0];

	/* Create circular linked list */
	for(lcv = 0; lcv < FIFO_DEPTH-1; lcv++)
		buff[lcv].next = &buff[lcv+1];

	buff[FIFO_DEPTH-1].next = &buff[0];

	tic = count = 0;

	sem_init(&sem_full, NULL, 0);
	sem_init(&sem_empty, NULL, FIFO_DEPTH);

	if(gopt.verbose)
		fprintf(stdout,"Creating FIFO\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
FIFO::~FIFO()
{
	int32 lcv;

	sem_destroy(&sem_full);
	sem_destroy(&sem_empty);

	delete [] buff;

	if(gopt.verbose)
		fprintf(stdout,"Destructing FIFO\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void FIFO::Import()
{

	IncStartTic();

	/* Read from the GPS source */
	pSource->Read(head);

	Enqueue();

	IncStopTic();

	count++;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void FIFO::Enqueue()
{

	sem_wait(&sem_empty);

	head->count = count;

	/* Send a packet to the acquisition (nonblocking) */
	write(COR_2_ACQ_P[WRITE], head, sizeof(ms_packet));

	head = head->next;

	sem_post(&sem_full);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void FIFO::Dequeue(ms_packet *p)
{

	sem_wait(&sem_full);

	memcpy(p, tail, sizeof(ms_packet));
	tail = tail->next;

	sem_post(&sem_empty);

}
/*----------------------------------------------------------------------------------------------*/

