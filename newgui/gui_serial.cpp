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

void openPipe(int32 _pipe)
{

	gpipe[_pipe] = -1;

	if(_pipe == WRITE)
	{

		while((gpipe[_pipe] == -1) && grun)
		{
			gpipe[_pipe] = open("/tmp/GUI2GPS",O_WRONLY);
			usleep(100000);
		}

		gpipe_open[_pipe] = true;

	}
	else
	{

		while((gpipe[_pipe] == -1) && grun)
		{
			gpipe[_pipe] = open("/tmp/GPS2GUI",O_RDONLY);
			usleep(100000);
		}

		gpipe_open[_pipe] = true;
	}

}


void gps_2_gui_pipe_close(int _sig)
{
	gpipe_open[READ] = false;
	openPipe(READ);
}

void gui_2_gps_pipe_close(int _sig)
{
	gpipe_open[WRITE] = false;
	openPipe(WRITE);
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
	Start_Thread(GUI_Serial_Thread, this);
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
	command_ready = 0;
	command_sent = 0;
	command_ack = 0;
	gpipe_open[READ] = false;
	gpipe_open[WRITE] = false;
	packet_count[LAST_M_ID] = 0;
	decoded_packet.tic = decoded_packet.id = decoded_packet.length = 0;
	memset(&packet_count[0], 0x0, (LAST_M_ID+1)*sizeof(int));
	memset(&messages.fifo, 0x0, sizeof(FIFO_M));
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI_Serial::~GUI_Serial()
{

	close(gpipe[READ]);
	close(gpipe[WRITE]);
	grun = false;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::Import()
{

	pipe_open = gpipe_open[READ];

	/* Open the pipe */
	if(!gpipe_open[READ])
		openPipe(READ);
	else
		readPipe();

	execution_tic++;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::Export()
{

	/* Open the pipe */
	if(!gpipe_open[WRITE])
		openPipe(WRITE);
	else
		writePipe();

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
		signal(SIGPIPE, gps_2_gui_pipe_close);
		bread = read(gpipe[READ], &buff[nbytes], _bytes - nbytes);
		if(bread > 0)
			nbytes += bread;
	}

	return(nbytes);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
int GUI_Serial::pipeWrite(void *_b, int32 _bytes)
{

	uint32 bwrote;
	bwrote = write(gpipe[WRITE], _b, _bytes);
	return(bwrote);

}
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
		DecodeCCSDSPacketHeader(&decoded_packet, &packet_header);

		/* Catch some failures */
		if(decoded_packet.length < 0 ||  decoded_packet.length > 512)
		{
			message_sync = 0;
			packet_count[LAST_M_ID]++;
			return;
		}

		if(decoded_packet.id < FIRST_M_ID ||  decoded_packet.id > LAST_M_ID)
		{
			message_sync = 0;
			packet_count[LAST_M_ID]++;
			return;
		}

		/* For the bytes/sec calculation */
		byte_count += decoded_packet.length;

		packet_count[decoded_packet.id]++;

		Lock();

		/* Now copy in the body */
		switch(decoded_packet.id)
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
				pipeRead(&m->ephemerides[NUM_CODES], sizeof(Ephemeris_M));
				chan = m->ephemerides[NUM_CODES].sv;
				memcpy(&m->ephemerides[chan], &m->ephemerides[NUM_CODES], sizeof(Ephemeris_M));
				break;
			case ALMANAC_M_ID:
				pipeRead(&m->almanacs[NUM_CODES], sizeof(Almanac_M));
				chan = m->almanacs[NUM_CODES].sv;
				memcpy(&m->almanacs[chan], &m->almanacs[NUM_CODES], sizeof(Almanac_M));
				break;
			case EPHEMERIS_VALID_M_ID:
				pipeRead(&m->ephemeris_status, sizeof(Ephemeris_Status_M));
				break;
			case FIFO_M_ID:
				pipeRead(&m->fifo, sizeof(FIFO_M));
				break;
			case COMMAND_ACK_M_ID:
				pipeRead(&m->command_ack, sizeof(Command_Ack_M));
				break;
//			case LAST_M_ID:
//				pipeRead(&buff[0], decoded_packet.length);
//				message_sync = 0;
//				packet_count[LAST_M_ID]++;
//				break;
			default:
				packet_count[LAST_M_ID]++;
				pipeRead(&buff[0], decoded_packet.length);
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
			DecodeCCSDSPacketHeader(&decoded_packet, &packet_header);

			/* Get the body */
			byte_count += pipeRead(&buff[0], decoded_packet.length);
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::writePipe()
{

	Lock();

	uint32 preamble = 0xAAAAAAAA;

	if(command_ready && (command_sent == 0))
	{
		pipeWrite(&preamble, sizeof(uint32));
		pipeWrite(&command_header, sizeof(CCSDS_Packet_Header));
		pipeWrite(&command_body, decoded_command.length);
		command_sent = 1;
	}

	Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::formCommand(int32 _id, void *_p)
{

	Lock();

	memset(&command_body, 0x0, sizeof(Union_C));

	command_ready = 1;
	command_sent = 0;
	command_ack = 0;

	switch(_id)
	{
		case RESET_PVT_C_ID:
			command_body.reset_pvt.flag = 1;
			FormCCSDSPacketHeader(&command_header, _id, 0, sizeof(Reset_PVT_C), 1, command_tic++);
			break;
		case RESET_CHANNEL_C_ID:
			command_body.reset_channel.chan = *(int32 *)_p;
			FormCCSDSPacketHeader(&command_header, _id, 0, sizeof(Reset_Channel_C), 1, command_tic++);
			break;
		case RESET_EPHEMERIS_C_ID:
			command_body.reset_ephemeris.sv = *(int32 *)_p;
			FormCCSDSPacketHeader(&command_header, _id, 0, sizeof(Reset_Ephemeris_C), 1, command_tic++);
			break;
		case RESET_ALMANAC_C_ID:
			command_body.reset_almanac.sv = *(int32 *)_p;
			FormCCSDSPacketHeader(&command_header, _id, 0, sizeof(Reset_Almanac_C), 1, command_tic++);
			break;
		case GET_MEASUREMENT_C_ID:
			command_body.get_measurement.chan = *(int32 *)_p;
			FormCCSDSPacketHeader(&command_header, _id, 0, sizeof(Get_Measurement_C), 1, command_tic++);
			break;
		case GET_PSEUDORANGE_C_ID:
			command_body.get_pseudorange.chan = *(int32 *)_p;
			FormCCSDSPacketHeader(&command_header, _id, 0, sizeof(Get_Pseudorange_C), 1, command_tic++);
			break;
		case GET_EPHEMERIS_C_ID:
			command_body.get_ephemeris.sv = *(int32 *)_p;
			FormCCSDSPacketHeader(&command_header, _id, 0, sizeof(Get_Ephemeris_C), 1, command_tic++);
			break;
		case GET_ALMANAC_C_ID:
			command_body.get_almanac.sv = *(int32 *)_p;
			FormCCSDSPacketHeader(&command_header, _id, 0, sizeof(Get_Almanac_C), 1, command_tic++);
			break;
		default:
			command_ready = 0;
			command_sent = 0;
			command_ack = 0;
			break;
	}

	DecodeCCSDSPacketHeader(&decoded_command, &command_header);

	Unlock();

}
/*----------------------------------------------------------------------------------------------*/
