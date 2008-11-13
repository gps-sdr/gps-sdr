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
/* Form the CCSDS packet header with the given _apid, sequence flag, and packet length, and command bit */
void FormCCSDSPacketHeader(CCSDS_Packet_Header *_p, uint32 _apid, uint32 _sf, uint32 _pl, uint32 _cm, uint32 _tic)
{

	_p->pid = ((_cm & 0x1) << 12) + (_apid + CCSDS_APID_BASE) & 0x7FF;
	_p->psc = (_sf & 0x3) + ((_tic & 0x3FFF) << 2);
	_p->pdl = _pl & 0xFFFF;

}

/* Decode a command into its components */
void DecodeCCSDSPacketHeader(CCSDS_Decoded_Header *_d, CCSDS_Packet_Header *_p)
{

	_d->id 		= _p->pid - CCSDS_APID_BASE;
	_d->type 	= _p->psc & 0x3;
	_d->tic		= (_p->psc >> 2) & 0x3FFF;
	_d->length 	= _p->pdl & 0xFFFF;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/* This is global so it can be handled by the signal handler */
int gpipe_open[2];
int gpipe[2];
int grun;

void openPipe()
{

	gpipe[READ] = gpipe[WRITE] = -1;

	while((gpipe[WRITE] == -1) && grun)
	{
		gpipe[WRITE] = open("/tmp/GUI2GPS",O_WRONLY);
		usleep(1000000);
	}

	while((gpipe[READ] == -1) && grun)
	{
		gpipe[READ] = open("/tmp/GPS2GUI",O_RDONLY);
		usleep(1000000);
	}

	gpipe_open[READ] = true;
}


void gui_pipe_close(int _sig)
{
	gpipe_open[READ] = false;
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

	grun = 1;
	execution_tic = 0;
	start_tic = 0;
	stop_tic = 0;
	message_sync = 0;
	pipe_open = 0;
	byte_count = 0;
	gpipe_open[READ] = false;
	gpipe_open[WRITE] = false;
	packet_count[LAST_M_ID] = 0;
	decoded_header.tic = decoded_header.id = decoded_header.length = 0;
	memset(&packet_count[0], 0x0, (LAST_M_ID+1)*sizeof(int));
	memset(&messages.fifo, 0x0, sizeof(FIFO_M));

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_unlock(&mutex);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI_Serial::~GUI_Serial()
{

	close(gpipe[READ]);
	close(gpipe[WRITE]);
	pthread_mutex_destroy(&mutex);
	grun = false;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::Import()
{

	pipe_open = gpipe_open[READ];

	/* Open the pipe */
	if(!gpipe_open[READ])
		openPipe();
	else
		readPipe();

	execution_tic++;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::Export()
{

	/* Open the pipe */
//	if(!gpipe_open[WRITE])
//		openPipe();
//	else
//		writePipe();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
int GUI_Serial::pipeRead(void *_b, int32 _bytes)
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
	}

	return(nbytes);

}
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
//int GUI_Serial::pipeWrite(void *_b, int32 _bytes)
//{
//
//
//
//}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::readPipe()
{
	int32 nbytes, bread, k, chan;
	uint8 v;

	Message_Struct *m = &messages;

	if(message_sync)
	{

		/* Check to make sure the first 4 bytes are "0xAAAAAAAA" */
		byte_count += pipeRead(&syncword, sizeof(uint32));

		if(syncword == 0xAAAAAAAA)
		{
			message_sync++;
		}
		else
		{
			message_sync = 0;
			packet_count[LAST_M_ID]++;
			return;
		}


		/* If so get the packet header */
		byte_count += pipeRead(&packet_header, sizeof(CCSDS_Packet_Header));
		DecodeCCSDSPacketHeader(&decoded_header, &packet_header);

		/* Catch some failures */
		if(decoded_header.length < 0 ||  decoded_header.length > 512)
		{
			message_sync = 0;
			packet_count[LAST_M_ID]++;
			return;
		}

		if(decoded_header.id < FIRST_M_ID ||  decoded_header.id > LAST_M_ID)
		{
			message_sync = 0;
			packet_count[LAST_M_ID]++;
			return;
		}

		/* For the bytes/sec calculation */
		byte_count += decoded_header.length;

		packet_count[decoded_header.id]++;

		Lock();

		/* Now copy in the body */
		switch(decoded_header.id)
		{
			case BOARD_HEALTH_M_ID:
				pipeRead(&m->board_health, sizeof(Board_Health_M));
				break;
			case TASK_HEALTH_M_ID:
				pipeRead(&m->task_health, sizeof(Task_Health_M));
				break;
			case CHANNEL_HEALTH_M_ID:
				pipeRead(&m->channel_health[MAX_CHANNELS], sizeof(Channel_Health_M));
				chan = m->channel_health[MAX_CHANNELS].chan;
				memcpy(&m->channel_health[chan], &m->channel_health[MAX_CHANNELS], sizeof(Channel_Health_M));
				break;
			case SPS_M_ID:
				pipeRead(&m->sps, sizeof(SPS_M));
				break;
			case CLOCK_M_ID:
				pipeRead(&m->clock, sizeof(Clock_M));
				break;
			case SV_POSITION_M_ID:
				pipeRead(&m->sv_positions[MAX_CHANNELS], sizeof(SV_Position_M));
				chan = m->sv_positions[MAX_CHANNELS].chan;
				memcpy(&m->sv_positions[chan], &m->sv_positions[MAX_CHANNELS], sizeof(SV_Position_M));
				break;
			case EKF_M_ID:
				pipeRead(&m->task_health, sizeof(Task_Health_M));
				break;
			case MEASUREMENT_M_ID:
				pipeRead(&m->measurements[MAX_CHANNELS], sizeof(Measurement_M));
				chan = m->measurements[MAX_CHANNELS].chan;
				memcpy(&m->measurements[chan], &m->measurements[MAX_CHANNELS], sizeof(Measurement_M));
				break;
			case PSEUDORANGE_M_ID:
				pipeRead(&m->pseudoranges[MAX_CHANNELS], sizeof(Pseudorange_M));
				chan = m->pseudoranges[MAX_CHANNELS].chan;
				memcpy(&m->pseudoranges[chan], &m->pseudoranges[MAX_CHANNELS], sizeof(Pseudorange_M));
				break;
			case EPHEMERIS_M_ID:
				pipeRead(&m->ephemeris[NUM_CODES], sizeof(Ephemeris_M));
				chan = m->ephemeris[NUM_CODES].sv;
				memcpy(&m->ephemeris[chan], &m->ephemeris[NUM_CODES], sizeof(Ephemeris_M));
				break;
			case ALMANAC_M_ID:
				pipeRead(&m->almanac[NUM_CODES], sizeof(Almanac_M));
				chan = m->almanac[NUM_CODES].sv;
				memcpy(&m->almanac[chan], &m->almanac[NUM_CODES], sizeof(Almanac_M));
				break;
			case EPHEMERIS_VALID_M_ID:
				pipeRead(&m->ephemeris_status, sizeof(Ephemeris_Status_M));
				break;
			case FIFO_M_ID:
				pipeRead(&m->fifo, sizeof(FIFO_M));
				break;
//			case LAST_M_ID:
//				pipeRead(&buff[0], decoded_header.length);
//				message_sync = 0;
//				packet_count[LAST_M_ID]++;
//				break;
			default:
				packet_count[LAST_M_ID]++;
				pipeRead(&buff[0], decoded_header.length);
				break;
		}

		Unlock();

	}
	else
	{
		v = 0;
		byte_count += pipeRead(&v, 1);

		syncword <<= 8;
		syncword += (uint32)v;

		if(syncword == 0xAAAAAAAA)
		{
			message_sync = 1;

			/* If so get the packet header */
			byte_count += pipeRead(&packet_header, sizeof(CCSDS_Packet_Header));
			DecodeCCSDSPacketHeader(&decoded_header, &packet_header);

			/* Get the body */
			byte_count += pipeRead(&buff[0], decoded_header.length);
		}
	}

}
/*----------------------------------------------------------------------------------------------*/

///*----------------------------------------------------------------------------------------------*/
//void GUI_Serial::writePipe()
//{
//
//
//
//}
///*----------------------------------------------------------------------------------------------*/
//
///*----------------------------------------------------------------------------------------------*/
//void GUI_Serial::QueueCommand()
//{
//
//
//
//
//
//
//}
///*----------------------------------------------------------------------------------------------*/


