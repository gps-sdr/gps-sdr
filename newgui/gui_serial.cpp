/*! \file GUI_Serial.cpp
	Implements member functions of GUI_Serial class.
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

#include "gui_serial.h"

/*----------------------------------------------------------------------------------------------*/
/* This is global so it can be handled by the signal handler */
int gpipe_open;
int gpipe[2];
int grun;

void openPipe()
{

	gpipe[READ] = gpipe[WRITE] = -1;

	while((gpipe[WRITE] == -1) && grun)
	{
		gpipe[WRITE] = open("/tmp/GUI2GPS",O_WRONLY);
		usleep(100000);
	}

	while((gpipe[READ] == -1) && grun)
	{
		gpipe[READ] = open("/tmp/GPS2GUI",O_RDONLY);
		usleep(100000);
	}

	gpipe_open = true;
}


void gui_pipe_close(int _sig)
{
	gpipe_open = false;
	openPipe();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void *GUI_Serial_Thread(void *_arg)
{

	GUI_Serial *aGUI_Serial = (GUI_Serial *)_arg;

	while(grun)
	{
		aGUI_Serial->Import();
		aGUI_Serial->Export();
	}

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::Start()
{
	pthread_create(&thread, NULL, GUI_Serial_Thread, this);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::Stop()
{
	pthread_cancel(thread);
	pthread_join(thread, NULL);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI_Serial::GUI_Serial()
{

	//memset(this, 0x0, sizeof(GUI_Serial));
	grun = true;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI_Serial::~GUI_Serial()
{
	grun = false;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::Import()
{

	pipe_open = gpipe_open;

	/* Open the pipe */
	if(!gpipe_open)
		openPipe();
	else
		readPipe();

	execution_tic++;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::Export()
{

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::pipeRead(void *_b, int32 _bytes)
{
	int32 nbytes, bread;
	uint8 *buff;

	nbytes = 0; bread = 0;
	buff = (uint8 *)_b;
	while((nbytes < _bytes) && grun)
	{
		signal(SIGPIPE, gui_pipe_close);
		bread = read(gpipe[READ], &buff[nbytes], _bytes - nbytes);
		if(bread > 0)
			nbytes += bread;
		else
			usleep(10);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::readPipe()
{
	int nbytes, bread, k, chan;
	char q[9] = "01234567";
	char v;

	char buff[2048];

	if(message_sync)
	{

		/* Check to make sure the first 8 bytes are "AAAAAAAA" */
		pipeRead(&syncword[0], 8);
		syncword[9] = '\0';

		if(strcmp(syncword, "AAAAAAAA") == 0)
		{
			message_sync++;
		}
		else
		{
			message_sync = 0;
			return;
		}

		/* If so get the packet header */
		pipeRead(&pheader, sizeof(CCSDS_PH));
		DecodeCCSDSPacketHeader(&ccsds_header, &pheader);

		/* For the bytes/sec calculation */
		byte_count += 6;
		byte_count += ccsds_header.length;

		/* Now copy in the body */
		switch(ccsds_header.id)
		{
			case BOARD_HEALTH_M_ID:
				pipeRead(&board_health, sizeof(Board_Health_M));
				break;
			case TASK_HEALTH_M_ID:
				pipeRead(&task_health, sizeof(Task_Health_M));
				break;
			case CHANNEL_HEALTH_M_ID:
				pipeRead(&channel_health[MAX_CHANNELS], sizeof(Channel_Health_M));
				chan = channel_health[MAX_CHANNELS].chan;
				memcpy(&channel_health[chan], &channel_health[MAX_CHANNELS], sizeof(Channel_Health_M));
				break;
			case SPS_M_ID:
				pipeRead(&sps, sizeof(SPS_M));
				break;
			case CLOCK_M_ID:
				pipeRead(&clock, sizeof(Clock_M));
				break;
			case SV_POSITION_M_ID:
				pipeRead(&sv_positions[MAX_CHANNELS], sizeof(SV_Position_M));
				chan = sv_positions[MAX_CHANNELS].chan;
				memcpy(&sv_positions[chan], &sv_positions[MAX_CHANNELS], sizeof(SV_Position_M));
				break;
			case EKF_M_ID:
				pipeRead(&task_health, sizeof(Task_Health_M));
				break;
			case MEASUREMENT_M_ID:
				pipeRead(&measurements[MAX_CHANNELS], sizeof(Measurement_M));
				chan = measurements[MAX_CHANNELS].chan;
				memcpy(&measurements[chan], &measurements[MAX_CHANNELS], sizeof(Measurement_M));
				break;
			case PSEUDORANGE_M_ID:
				pipeRead(&pseudoranges[MAX_CHANNELS], sizeof(Pseudorange_M));
				chan = pseudoranges[MAX_CHANNELS].chan;
				memcpy(&pseudoranges[chan], &pseudoranges[MAX_CHANNELS], sizeof(Pseudorange_M));
				break;
			case EPHEMERIS_M_ID:
				pipeRead(&ephemeris[NUM_CODES], sizeof(Ephemeris_M));
				chan = ephemeris[NUM_CODES].sv;
				memcpy(&ephemeris[chan], &ephemeris[NUM_CODES], sizeof(Ephemeris_M));
				break;
			case ALMANAC_M_ID:
				pipeRead(&almanac[NUM_CODES], sizeof(Almanac_M));
				chan = almanac[NUM_CODES].sv;
				memcpy(&almanac[chan], &almanac[NUM_CODES], sizeof(Almanac_M));
				break;
			case EPHEMERIS_VALID_M_ID:
				pipeRead(&ephemeris_status, sizeof(Ephemeris_Status_M));
				break;
			case FIFO_M_ID:
				pipeRead(&fifo_status, sizeof(FIFO_M));
				break;
//			case LAST_M_ID:
//				pipeRead(&buff[0], ccsds_header.length);
//				message_sync = 0;
//				packet_count[LAST_M_ID]++;
//				break;
			default:
				packet_count[LAST_M_ID]++;
				pipeRead(&buff[0], ccsds_header.length);
				break;
		}

	}
	else
	{
		v = '0';
		pipeRead(&v, 1);

		memcpy(&q[0], &syncword[0], 7*sizeof(char));
		syncword[0] = v;
		memcpy(&syncword[1], &q[0], 7*sizeof(char));
		syncword[9] = '\0';

		if(strcmp(syncword, "AAAAAAAA") == 0)
		{
			message_sync = 1;

			/* If so get the packet header */
			pipeRead(&pheader, sizeof(CCSDS_PH));
			DecodeCCSDSPacketHeader(&ccsds_header, &pheader);

			/* Get the body */
			pipeRead(&buff[0], ccsds_header.length);
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::DecodeCCSDSPacketHeader(CCSDS_Header *_h, CCSDS_PH *_p)
{

	_h->id 		= _p->pid - CCSDS_APID_BASE;
	_h->type 	= _p->psc & 0x3;
	_h->tic		= (_p->psc >> 2) & 0x3FFF;
	_h->length 	= _p->pdl & 0xFFFF;

}
/*----------------------------------------------------------------------------------------------*/



