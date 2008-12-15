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

GUI_Serial *aGUI_Serial;

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
int grun;

void pipe_closed(int _sig)
{
	aGUI_Serial->setPipe(false);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void *GUI_Serial_Thread(void *_arg)
{

	aGUI_Serial = (GUI_Serial *)_arg;

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
	byte_count = 0;
	command_head = 0;
	command_tail = 0;
	command_free = COMMAND_BUFFER_DEPTH;
	npipe_open = false;
	spipe_open = false;
	serial = 0;
	spipe = NULL;
	npipe[READ] = NULL;
	npipe[WRITE] = NULL;
	lfile = NULL;
	packet_count[LAST_M_ID] = 0;
	decoded_packet.tic = decoded_packet.id = decoded_packet.length = 0;
	memset(&packet_count[0], 0x0, (LAST_M_ID+1)*sizeof(int));
	memset(&messages.fifo, 0x0, sizeof(FIFO_M));
	memset(&command_ready[0], 0x0, COMMAND_BUFFER_DEPTH*sizeof(uint32));
	memset(&command_sent[0], 0x0, COMMAND_BUFFER_DEPTH*sizeof(uint32));
	memset(&command_ack[0], 0x0, COMMAND_BUFFER_DEPTH*sizeof(uint32));
	memset(&command_body[0], 0x0, COMMAND_BUFFER_DEPTH*sizeof(Union_C));
	memset(&command_header[0], 0x0, COMMAND_BUFFER_DEPTH*sizeof(CCSDS_Packet_Header));
	memset(&decoded_command[0], 0x0, COMMAND_BUFFER_DEPTH*sizeof(CCSDS_Decoded_Header));
	memset(&filename[0], 0x0, 1024*sizeof(char));
	memset(&log_flag[0], 0x0, LAST_M_ID*sizeof(char));

	signal(SIGPIPE, pipe_closed);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI_Serial::~GUI_Serial()
{
	if(lfile)
		fclose(lfile);

	if(npipe[READ])
		close(npipe[READ]);

	if(npipe[WRITE])
		close(npipe[WRITE]);

	if(spipe)
		close(spipe);

	grun = false;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::openSerial()
{

	spipe = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(spipe < 0)
    {
    	usleep(1000000);
    	spipe_open = false;
    	spipe = NULL;
    	return;
    }

    memset(&tty, 0x0, sizeof(tty));			//!< Initialize the port settings structure to all zeros
    tty.c_cflag =  B115200 | CS8 | CLOCAL | CREAD | CRTSCTS;	//!< 8N1
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;
    tty.c_cc[VMIN] = 0;      				//!< 0 means use-vtime
    tty.c_cc[VTIME] = 1;      				//!< Time to wait until exiting read (tenths of a second)

    tcflush(spipe, TCIFLUSH);				//!< flush old data
    tcsetattr(spipe, TCSANOW, &tty);		//!< apply new settings
    fcntl(spipe, F_SETFL, FASYNC);
	fcntl(spipe, F_SETFL, O_NONBLOCK);
	spipe_open = true;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::openPipe()
{

	npipe[READ] = npipe[WRITE] = -1;
	npipe[READ] = open("/tmp/GPS2GUI", O_RDONLY | O_NONBLOCK);
	npipe[WRITE] = open("/tmp/GUI2GPS", O_WRONLY | O_NONBLOCK);

	if((npipe[READ] != -1) && (npipe[WRITE] != -1))
	{
		npipe_open = true;
	}
	else
	{
		npipe_open = false;
		usleep(1000000);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::setPipe(bool _status)
{
	npipe_open = _status;
	if(_status == false)
	{
		close(npipe[READ]);
		close(npipe[WRITE]);
		npipe[READ] = NULL;
		npipe[WRITE] = NULL;
	}
}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::setIO(int32 _serial)
{

	if(spipe_open)
	{
		spipe_open = false;
		close(spipe);
		spipe = NULL;
	}

	if(npipe_open)
	{
		npipe_open = false;
		close(npipe[READ]);
		close(npipe[WRITE]);
		npipe[READ] = NULL;
		npipe[WRITE] = NULL;
	}

	serial = _serial;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::Import()
{
	/* Open the pipe */
	if(serial)
	{
		if(spipe_open)
			readGPS();
		else
			openSerial();
	}
	else
	{
		if(npipe_open == true)
			readGPS();
		else
			openPipe();
	}

	execution_tic++;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::Export()
{

	/* Open the pipe */
	if(serial)
	{
		if(spipe_open)
			writeGPS();
		else
			openSerial();
	}
	else
	{
		if(npipe_open == true)
			writeGPS();
		else
			openPipe();
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
int GUI_Serial::Read(void *_b, int32 _bytes)
{

	int32 nbytes, bread;
	uint8 *buff;

	nbytes = 0; bread = 0;
	buff = (uint8 *)_b;
	while((nbytes < _bytes) && grun)
	{
		if(serial && spipe_open)
			bread = read(spipe, &buff[nbytes], _bytes - nbytes);
		else if(npipe_open)
			bread = read(npipe[READ], &buff[nbytes], _bytes - nbytes);
		else
			nbytes = _bytes;

		if(bread > 0)
			nbytes += bread;

		usleep(1000);
	}

	return(nbytes);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
int GUI_Serial::Write(void *_b, int32 _bytes)
{

	uint32 bwrote;

	if(serial && spipe_open)
		bwrote = write(spipe, _b, _bytes);
	else if(npipe_open)
		bwrote = write(npipe[WRITE], _b, _bytes);
	else
		bwrote = 0;

	return(bwrote);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::readGPS()
{
	int32 nbytes, bread, k, chan;
	uint8 v;

	Message_Struct *m = &messages;

	if(message_sync)
	{

		/* Check to make sure the first 4 bytes are "0xAAAAAAAA" */
		byte_count += Read(&syncword, sizeof(uint32));

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
		byte_count += Read(&packet_header, sizeof(CCSDS_Packet_Header));
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

		Lock();

		/* For the bytes/sec calculation */
		byte_count += decoded_packet.length;

		packet_count[decoded_packet.id]++;

		/* Now copy in the body */
		switch(decoded_packet.id)
		{
			case BOARD_HEALTH_M_ID:
				Read(&m->board_health, sizeof(Board_Health_M));
				break;
			case TASK_HEALTH_M_ID:
				Read(&m->task_health, sizeof(Task_Health_M));
				if(log_flag[PSEUDORANGE_M_ID]) printTask();
				break;
			case CHANNEL_M_ID:
				Read(&m->channel[MAX_CHANNELS], sizeof(Channel_M));
				chan = m->channel[MAX_CHANNELS].chan;
				if((chan >= 0) && (chan < MAX_CHANNELS))
				{
					memcpy(&m->channel[chan], &m->channel[MAX_CHANNELS], sizeof(Channel_M));
					if(log_flag[CHANNEL_M_ID]) printChan(chan);
				}
				else
				{
					message_sync = 0;
					packet_count[LAST_M_ID]++;
				}
				break;
			case SPS_M_ID:
				Read(&m->sps, sizeof(SPS_M));
				if(log_flag[SPS_M_ID]) printPVT();
				break;
			case CLOCK_M_ID:
				Read(&m->clock, sizeof(Clock_M));
				if(log_flag[CLOCK_M_ID]) printClock();
				break;
			case SV_POSITION_M_ID:
				Read(&m->sv_positions[MAX_CHANNELS], sizeof(SV_Position_M));
				chan = m->sv_positions[MAX_CHANNELS].chan;
				if((chan >= 0) && (chan < MAX_CHANNELS))
				{
					memcpy(&m->sv_positions[chan], &m->sv_positions[MAX_CHANNELS], sizeof(SV_Position_M));
				}
				else
				{
					message_sync = 0;
					packet_count[LAST_M_ID]++;
				}
				break;
			case EKF_M_ID:
				Read(&m->task_health, sizeof(Task_Health_M));
				if(log_flag[EKF_M_ID]) printEKF();
				break;
			case MEASUREMENT_M_ID:
				Read(&m->measurements[MAX_CHANNELS], sizeof(Measurement_M));
				chan = m->measurements[MAX_CHANNELS].chan;
				if((chan >= 0) && (chan < MAX_CHANNELS))
				{
					memcpy(&m->measurements[chan], &m->measurements[MAX_CHANNELS], sizeof(Measurement_M));
				}
				else
				{
					message_sync = 0;
					packet_count[LAST_M_ID]++;
				}
				break;
			case PSEUDORANGE_M_ID:
				Read(&m->pseudoranges[MAX_CHANNELS], sizeof(Pseudorange_M));
				chan = m->pseudoranges[MAX_CHANNELS].chan;
				if((chan >= 0) && (chan < MAX_CHANNELS))
				{
					memcpy(&m->pseudoranges[chan], &m->pseudoranges[MAX_CHANNELS], sizeof(Pseudorange_M));
					if(log_flag[PSEUDORANGE_M_ID]) printPseudo(chan);
				}
				else
				{
					message_sync = 0;
					packet_count[LAST_M_ID]++;
				}
				break;
			case EPHEMERIS_M_ID:
				Read(&m->ephemerides[NUM_CODES], sizeof(Ephemeris_M));
				chan = m->ephemerides[NUM_CODES].sv;
				if((chan >= 0) && (chan < NUM_CODES))
				{
					memcpy(&m->ephemerides[chan], &m->ephemerides[NUM_CODES], sizeof(Ephemeris_M));
				}
				else
				{
					message_sync = 0;
					packet_count[LAST_M_ID]++;
				}
				break;
			case ALMANAC_M_ID:
				Read(&m->almanacs[NUM_CODES], sizeof(Almanac_M));
				chan = m->almanacs[NUM_CODES].sv;
				if((chan >= 0) && (chan < NUM_CODES))
				{
					memcpy(&m->almanacs[chan], &m->almanacs[NUM_CODES], sizeof(Almanac_M));
				}
				else
				{
					message_sync = 0;
					packet_count[LAST_M_ID]++;
				}
				break;
			case EPHEMERIS_VALID_M_ID:
				Read(&m->ephemeris_status, sizeof(Ephemeris_Status_M));
				break;
			case FIFO_M_ID:
				Read(&m->fifo, sizeof(FIFO_M));
				break;
			case SV_PREDICTION_M_ID:
				Read(&m->sv_predictions[NUM_CODES], sizeof(SV_Prediction_M));
				chan = m->sv_predictions[NUM_CODES].sv;
				if((chan >= 0) && (chan < NUM_CODES))
				{
					memcpy(&m->sv_predictions[chan], &m->sv_predictions[NUM_CODES], sizeof(SV_Prediction_M));
				}
				else
				{
					message_sync = 0;
					packet_count[LAST_M_ID]++;
				}
				break;
			case ACQ_COMMAND_M_ID:
				Read(&m->acq_command[NUM_CODES], sizeof(Acq_Command_M));
				chan = m->acq_command[NUM_CODES].sv;
				if((chan >= 0) && (chan < NUM_CODES))
				{
					memcpy(&m->acq_command[chan], &m->acq_command[NUM_CODES], sizeof(Acq_Command_M));
				}
				else
				{
					message_sync = 0;
					packet_count[LAST_M_ID]++;
				}

				break;
			case COMMAND_ACK_M_ID:
				Read(&m->command_ack, sizeof(Command_Ack_M));
				break;
//			case LAST_M_ID:
//				Read(&buff[0], decoded_packet.length);
//				message_sync = 0;
//				packet_count[LAST_M_ID]++;
//				break;
			default:
				packet_count[LAST_M_ID]++;
				Read(&buff[0], decoded_packet.length);
				break;
		}

		Unlock();

	}
	else
	{
		v = 0;
		byte_count += Read(&v, 1);

		syncword <<= 8;
		syncword += (uint32)v;

		if(syncword == 0xAAAAAAAA)
		{
			message_sync = 1;

			/* If so get the packet header */
			byte_count += Read(&packet_header, sizeof(CCSDS_Packet_Header));
			DecodeCCSDSPacketHeader(&decoded_packet, &packet_header);

			/* Get the body */
			byte_count += Read(&buff[0], decoded_packet.length);
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::writeGPS()
{

	uint32 preamble = 0xAAAAAAAA;
	Union_C *cb;
	CCSDS_Packet_Header *ch;
	CCSDS_Decoded_Header *dc;

	Lock();

	while(command_free < COMMAND_BUFFER_DEPTH)
	{

		cb = &command_body[command_tail];
		ch = &command_header[command_tail];
		dc = &decoded_command[command_tail];

		if(command_ready[command_tail] && (command_sent[command_tail] == 0))
		{
			Write(&preamble, sizeof(uint32));
			Write(ch, sizeof(CCSDS_Packet_Header));
			Write(cb, dc->length);
			command_sent[command_tail] = 1;
			command_tail++;
			command_tail %= COMMAND_BUFFER_DEPTH;
			command_free++;
		}
	}

	Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::formCommand(int32 _id, void *_p)
{

	int32 pend;
	Union_C *cb;
	CCSDS_Packet_Header *ch;
	CCSDS_Decoded_Header *dc;

	/* Wait for a free node */
	pend = 0;
	while(pend == 0)
	{
		Lock();
		pend = command_free > 0;
		Unlock();
		usleep(1000);
	}

	Lock();

	command_ready[command_head] = 1;
	command_sent[command_head] = 0;
	command_ack[command_head] = command_tic;
	dc = &decoded_command[command_head];
	ch = &command_header[command_head];
	cb = &command_body[command_head];
	memset(cb, 0x0, sizeof(Union_C));

	switch(_id)
	{
		case RESET_ALL_C_ID:
			cb->reset_all.flag = 1;
			cb->reset_all.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Reset_All_C), 1, command_tic++);
			break;
		case RESET_PVT_C_ID:
			cb->reset_pvt.flag = 1;
			cb->reset_pvt.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Reset_PVT_C), 1, command_tic++);
			break;
		case RESET_CHANNEL_C_ID:
			cb->reset_channel.chan = *(int32 *)_p;
			cb->reset_channel.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Reset_Channel_C), 1, command_tic++);
			break;
		case RESET_EPHEMERIS_C_ID:
			cb->reset_ephemeris.sv = *(int32 *)_p;
			cb->reset_all.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Reset_Ephemeris_C), 1, command_tic++);
			break;
		case RESET_ALMANAC_C_ID:
			cb->reset_almanac.sv = *(int32 *)_p;
			cb->reset_almanac.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Reset_Almanac_C), 1, command_tic++);
			break;
		case GET_MEASUREMENT_C_ID:
			cb->get_measurement.flag = *(int32 *)_p;
			cb->get_measurement.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Get_Measurement_C), 1, command_tic++);
			break;
		case GET_PSEUDORANGE_C_ID:
			cb->get_pseudorange.flag = *(int32 *)_p;
			cb->get_pseudorange.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Get_Pseudorange_C), 1, command_tic++);
			break;
		case GET_EPHEMERIS_C_ID:
			cb->get_ephemeris.sv = *(int32 *)_p;
			cb->get_ephemeris.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Get_Ephemeris_C), 1, command_tic++);
			break;
		case GET_ALMANAC_C_ID:
			cb->get_almanac.sv = *(int32 *)_p;
			cb->get_almanac.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Get_Almanac_C), 1, command_tic++);
			break;
		case SET_EPHEMERIS_C_ID:
			memcpy(&cb->set_ephemeris, _p, sizeof(Set_Ephemeris_C));
			cb->set_ephemeris.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Set_Ephemeris_C), 1, command_tic++);
			break;
		case SET_ALMANAC_C_ID:
			memcpy(&cb->set_almanac, _p, sizeof(Set_Almanac_C));
			cb->set_almanac.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Set_Almanac_C), 1, command_tic++);
			break;
		case SET_ACQ_CONFIG_C_ID:
			memcpy(&cb->set_acq_config, _p, sizeof(Set_Acq_Config_C));
			cb->set_acq_config.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Set_Acq_Config_C), 1, command_tic++);
			break;
		case SET_PVT_C_ID:
			memcpy(&cb->set_pvt, _p, sizeof(Set_PVT_C));
			cb->set_pvt.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Set_PVT_C), 1, command_tic++);
			break;
		case GET_ACQ_CONFIG_C_ID:
			cb->get_acq_config.flag = 1;
			cb->get_acq_config.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Get_Acq_Config_C), 1, command_tic++);
			break;
		case GET_SV_PREDICTION_C_ID:
			cb->get_sv_prediction.sv = *(int32 *)_p;
			cb->get_sv_prediction.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Get_SV_Prediction_C), 1, command_tic++);
			break;
		case GET_EPHEMERIS_VALID_C_ID:
			cb->get_ephemeris_valid.flag = 1;
			cb->get_ephemeris_valid.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Get_Ephemeris_Valid_C), 1, command_tic++);
			break;
		case GET_BOARD_HEALTH_C_ID:
			cb->get_board_health.flag = 1;
			cb->get_board_health.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Get_Board_Health_C), 1, command_tic++);
			break;
		case GET_ACQ_COMMAND_C_ID:
			cb->get_acq_command.sv = *(int32 *)_p;
			cb->get_acq_command.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Get_Acq_Command_C), 1, command_tic++);
			break;
		case GET_SV_POSITION_C_ID:
			cb->get_sv_position.flag = *(int32 *)_p;
			cb->get_sv_position.command_id = _id;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Get_SV_Position_C), 1, command_tic++);
			break;
		case GET_CHANNEL_C_ID:
			cb->get_channel.command_id = _id;
			cb->get_channel.flag = *(int32 *)_p;
			FormCCSDSPacketHeader(ch, COMMAND_M_ID, 0, sizeof(Get_Channel_C), 1, command_tic++);
			break;
		default:
			command_ready[command_head] = 0;
			command_sent[command_head] = 0;
			command_ack[command_head] = 0;
			Unlock();
			return;
	}

	DecodeCCSDSPacketHeader(dc, ch);

	command_head++;
	command_head %= COMMAND_BUFFER_DEPTH;
	command_free--;

	Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::setLogFile(const char *_str)
{
	if(lfile) fclose(lfile);
	strcpy(filename, _str);
	lfile = fopen(filename, "wt");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::logStart()
{
	if(lfile) fclose(lfile);
	lfile = NULL;
	lfile = fopen(filename, "wt");

	if(lfile)
		logging_on = true;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::logStop()
{
	if(lfile) fclose(lfile);
	lfile = NULL;
	logging_on = false;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::logClear()
{
	if(lfile) fclose(lfile);
	lfile = NULL;
	lfile = fopen(filename, "wt");
	if(lfile) fclose(lfile);
	lfile = NULL;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::printChan(int32 _chan)
{
	SPS_M		*pNav = &messages.sps;
	Channel_M	*pChan = &messages.channel[_chan];

	if(logging_on && lfile)
	{
		fprintf(lfile,"%02d,%.16e,%02d,%02d,%08d,%01d,%01d,%01d,%02d,%.16e,%.16e\n",
		CHANNEL_M_ID,
		pNav->time,
		_chan,
		(int32)pChan->sv,
		(int32)pChan->count,
		(int32)pChan->bit_lock,
		(int32)pChan->frame_lock,
		(int32)pChan->subframe,
		(int32)pChan->len,
		pChan->p_avg,
		pChan->CN0);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::printPVT()
{
	SPS_M *pNav = &messages.sps; /* Navigation Solution */
	int32 lcv, nsvs;

	if(logging_on && lfile)
	{
		nsvs = 0;
		for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		{
			if((pNav->nsvs >> lcv) & 0x1)
				nsvs++;
		}
			/* Nav solution */
		fprintf(lfile,"%02d,%.16e,%02d,%02d,%08d,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e\n",
			SPS_M_ID,
			pNav->time,
			pNav->converged,
			nsvs,
			pNav->tic,
			pNav->x,
			pNav->y,
			pNav->z,
			pNav->vx,
			pNav->vy,
			pNav->vz,
			pNav->gdop,
			pNav->hdop,
			pNav->tdop,
			pNav->vdop,
			pNav->pdop);
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::printTask()
{
	if(logging_on && lfile)
	{

	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::printEKF()
{
	if(logging_on && lfile)
	{

	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::printClock()
{
	SPS_M		*pNav		= &messages.sps;		/* Navigation Solution */
	Clock_M		*pClock		= &messages.clock;	/* Clock solution */

	if(logging_on && lfile)
	{
		/* Clock state */
		fprintf(lfile,"%02d,%.16e,%04d,%04d,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e\n",
		CLOCK_M_ID,
		pNav->time,
		(int32)pClock->week,
		pClock->state,
		pClock->receiver_time,
		pClock->bias,
		pClock->rate,
		pClock->time0,
		pClock->time,
		pClock->time_raw);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Serial::printPseudo(int32 _chan)
{

	Pseudorange_M *pPseudo = &messages.pseudoranges[_chan];
	SPS_M *pNav	= &messages.sps;

	if(logging_on && lfile)
	{
		/* Pseudorange */
		fprintf(lfile,"%02d,%.16e,%02d,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e\n",
			PSEUDORANGE_M_ID,
			pNav->time,
			_chan,
			pPseudo->time,
			pPseudo->time_rate,
			pPseudo->meters,
			pPseudo->meters_rate,
			pPseudo->residual,
			pPseudo->rate_residual,
			pPseudo->time_uncorrected);
	}
}
/*----------------------------------------------------------------------------------------------*/
