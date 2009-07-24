/*----------------------------------------------------------------------------------------------*/
/*! \file telemetry.cpp
//
// FILENAME: telemetry.cpp
//
// DESCRIPTION: Implement the Telemetry class.
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

#include "telemetry.h"

/*----------------------------------------------------------------------------------------------*/
void lost_gui_pipe(int _sig)
{
	pTelemetry->ClosePipe();
	fprintf(stderr,"GUI disconnected\n");
}
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
void *Telemetry_Thread(void *_arg)
{

	while(grun)
	{
		pTelemetry->Import();
		pTelemetry->Export();
		usleep(10000);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::Start()
{
	Start_Thread(Telemetry_Thread, NULL);

	if(gopt.verbose)
		fprintf(stdout,"Telemetry thread started\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Telemetry::Telemetry():Threaded_Object("TLMTASK")
{

	object_mem = this;
	size = sizeof(Telemetry);

	npipe_open = false;
	npipe[READ] = npipe[WRITE] = -1;
	pheader = (uint8 *)&command_header;
	pcommand = (uint8 *)&command_body;

	signal(SIGPIPE, lost_gui_pipe);

	remove("/tmp/GPS2GUI");
	fifo[WRITE] = mkfifo("/tmp/GPS2GUI", S_IRWXU|S_IRWXG|S_IRWXO|S_IROTH|S_IWOTH);
	if(fifo[WRITE] == -1)
		fprintf(stderr,"Error creating the named pipe /tmp/GPS2GUI\n");
	chmod("/tmp/GPS2GUI", S_IRWXU|S_IRWXG|S_IRWXO|S_IROTH|S_IWOTH);


	remove("/tmp/GUI2GPS");
	fifo[READ] = mkfifo("/tmp/GUI2GPS", S_IRWXU|S_IRWXG|S_IRWXO|S_IROTH|S_IWOTH);
	if(fifo[READ] == -1)
		fprintf(stderr,"Error creating the named pipe /tmp/GUI2GPS\n");
	chmod("/tmp/GUI2GPS", S_IRWXU|S_IRWXG|S_IRWXO|S_IROTH|S_IWOTH);

	/* Build the function pointer array */
	msg_handlers[FIRST_M_ID] 				= NULL;
	msg_handlers[FIRST_PERIODIC_M_ID]		= NULL;
	msg_handlers[BOARD_HEALTH_M_ID] 		= &Telemetry::SendBoardHealth;
	msg_handlers[TASK_HEALTH_M_ID] 			= &Telemetry::SendTaskHealth;
	msg_handlers[SPS_M_ID] 					= &Telemetry::SendSPS;
	msg_handlers[TOT_M_ID] 					= &Telemetry::SendTOT;
	msg_handlers[PPS_M_ID] 					= &Telemetry::SendPPS;
	msg_handlers[CLOCK_M_ID] 				= &Telemetry::SendClock;
	msg_handlers[CHANNEL_M_ID] 				= &Telemetry::SendChannelHealth;
	msg_handlers[SV_POSITION_M_ID] 			= &Telemetry::SendSVPositions;
	msg_handlers[MEASUREMENT_M_ID] 			= &Telemetry::SendMeasurements;
	msg_handlers[PSEUDORANGE_M_ID] 			= &Telemetry::SendPseudoranges;
	msg_handlers[SV_PREDICTION_M_ID] 		= &Telemetry::SendSVPredictions;
	msg_handlers[LAST_PERIODIC_M_ID]		= NULL;
	msg_handlers[EKF_STATE_M_ID] 			= &Telemetry::SendEKFState;
	msg_handlers[EKF_COVARIANCE_M_ID] 		= &Telemetry::SendEKFCovariance;
	msg_handlers[EKF_RESIDUAL_M_ID] 		= &Telemetry::SendEKFResidual;
	msg_handlers[COMMAND_M_ID] 				= NULL;
	msg_handlers[COMMAND_ACK_M_ID] 			= NULL;
	msg_handlers[EPHEMERIS_M_ID] 			= NULL;
	msg_handlers[ALMANAC_M_ID] 				= NULL;
	msg_handlers[EPHEMERIS_STATUS_M_ID]		= NULL;
	msg_handlers[SV_SELECT_STATUS_M_ID]		= NULL;
	msg_handlers[EEPROM_M_ID]				= NULL;
	msg_handlers[EEPROM_CHKSUM_M_ID] 		= NULL;
	msg_handlers[MEMORY_M_ID] 				= NULL;
	msg_handlers[MEMORY_CHKSUM_M_ID] 		= NULL;
	msg_handlers[LAST_M_ID] 				= NULL;

	/* Build the message rates */
	msg_rates[FIRST_M_ID] 				= 0;
	msg_rates[FIRST_PERIODIC_M_ID]		= 0;
	msg_rates[BOARD_HEALTH_M_ID] 		= 1;
	msg_rates[TASK_HEALTH_M_ID] 		= 1;
	msg_rates[SPS_M_ID] 				= 1;
	msg_rates[TOT_M_ID] 				= 1;
	msg_rates[PPS_M_ID] 				= 1;
	msg_rates[CLOCK_M_ID] 				= 1;
	msg_rates[CHANNEL_M_ID] 			= 1;
	msg_rates[SV_POSITION_M_ID] 		= 0;
	msg_rates[MEASUREMENT_M_ID] 		= 0;
	msg_rates[PSEUDORANGE_M_ID] 		= 1;
	msg_rates[SV_PREDICTION_M_ID] 		= 1;
	msg_rates[LAST_PERIODIC_M_ID]		= 0;
	msg_rates[EKF_STATE_M_ID] 			= 1;
	msg_rates[EKF_COVARIANCE_M_ID] 		= 1;
	msg_rates[EKF_RESIDUAL_M_ID] 		= 1;
	msg_rates[COMMAND_M_ID] 			= 0;
	msg_rates[COMMAND_ACK_M_ID] 		= 0;
	msg_rates[EPHEMERIS_M_ID] 			= 0;
	msg_rates[ALMANAC_M_ID] 			= 0;
	msg_rates[EPHEMERIS_STATUS_M_ID]	= 0;
	msg_rates[SV_SELECT_STATUS_M_ID]	= 0;
	msg_rates[EEPROM_M_ID]				= 0;
	msg_rates[EEPROM_CHKSUM_M_ID] 		= 0;
	msg_rates[MEMORY_M_ID] 				= 0;
	msg_rates[MEMORY_CHKSUM_M_ID] 		= 0;
	msg_rates[LAST_M_ID] 				= 0;

	if(gopt.verbose)
		fprintf(stdout,"Creating Telemetry\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Telemetry::~Telemetry()
{

	close(npipe[READ]);
	close(npipe[WRITE]);
	remove("/tmp/GPS2GUI");
	remove("/tmp/GUI2GPS");

	if(gopt.verbose)
		fprintf(stdout,"Destructing Telemetry\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SetType(int32 _type)
{
	if(_type == TELEM_NAMED_PIPE)
		tlm_type = TELEM_NAMED_PIPE;
	else
		tlm_type = TELEM_SERIAL;

	ClosePipe();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::OpenSerial()
{
	int32 spipe;
	struct termios tty;

	spipe = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(spipe < 0)
    {
		npipe[READ] = -1;
		npipe[WRITE] = -1;
		npipe_open = false;
    	return;
    }

    memset(&tty, 0x0, sizeof(tty));		//!< Initialize the port settings structure to all zeros
    tty.c_cflag =  B57600 | CLOCAL | CREAD | CS8;
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;
    tty.c_cc[VMIN] = 0;      			//!< 0 means use-vtime
    tty.c_cc[VTIME] = 1;      			//!< Time to wait until exiting read (tenths of a second)

    tcflush(spipe, TCIFLUSH);				//!< Flush old data
    tcsetattr(spipe, TCSANOW, &tty);		//!< Apply new settings
    fcntl(spipe, F_SETFL, FASYNC);			//!< ???
	fcntl(spipe, F_SETFL, O_NONBLOCK);		//!< Nonblocking reads and writes

	/* Alias the serial port */
	npipe[READ] = npipe[WRITE] = spipe;
	npipe_open = true;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::OpenPipe()
{

	npipe[READ] = npipe[WRITE] = -1;
	npipe[READ] = open("/tmp/GUI2GPS", O_RDONLY | O_NONBLOCK);
	npipe[WRITE] = open("/tmp/GPS2GUI", O_WRONLY | O_NONBLOCK);

	if((npipe[READ] != -1) && (npipe[WRITE] != -1))
	{
		fcntl(npipe[READ] , F_SETFL, O_NONBLOCK);
		fcntl(npipe[WRITE] , F_SETFL, O_NONBLOCK);
		npipe_open = true;
		fprintf(stdout,"GUI connected\n");
	}
	else
	{
		npipe_open = false;
		close(npipe[READ]);
		close(npipe[WRITE]);
		npipe[READ] = -1;
		npipe[WRITE] = -1;
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::ClosePipe()
{
	npipe_open = false;
	close(npipe[READ]);
	close(npipe[WRITE]);
	npipe[READ] = -1;
	npipe[WRITE] = -1;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::Import()
{

	/* Wait for data from the PVT */
	ImportPVT();

	/* Get data from EKF */
	ImportEKF();

	/* Get data from serial port */
	ImportSerial();

	/* Bent pipe anything from Commando */
	ImportCommando();

	/* Increment execution counter */
	IncExecTic();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::ImportPVT()
{

	uint32 bread, sv;

	bread = read(PVT_2_TLM_P[READ], &pvt_s, sizeof(PVT_2_TLM_S));

	if(bread == sizeof(PVT_2_TLM_S))
	{
		export_messages = true;
		if(npipe_open == false)
		{
			if(tlm_type == TELEM_NAMED_PIPE)
			{
				OpenPipe();
			}
			else
			{
				OpenSerial();
			}
		}
	}

	bread = read(SVS_2_TLM_P[READ], &svs_s, sizeof(SVS_2_TLM_S));
	if(bread == sizeof(SVS_2_TLM_S))
	{
		sv = svs_s.sv;
		if((sv >= 0) && (sv < MAX_SV))
		{
			memcpy(&sv_predictions[sv], &svs_s, sizeof(SVS_2_TLM_S));
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::ImportEKF()
{
	uint32 bread;

	bread = read(EKF_2_TLM_P[READ], &ekf_s, sizeof(EKF_2_TLM_S));

	if(bread == sizeof(EKF_2_TLM_S))
		export_ekf = true;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::ImportSerial()
{

	int32 bread;
	uint8 abyte;

	if(npipe_open)
	{
		bread = read(npipe[READ], &abyte, sizeof(uint8));
		while((bread > 0) && npipe_open && grun)
		{
			bytes_read++;

			switch(syncstate)
			{
				case 0: /* Get the preamble */
				{
					syncword <<= 8;
					syncword += (uint32)abyte;

					if(syncword == 0xAAAAAAAA)
						StateZero();

					break;
				}
				case 1: /* Get the CCSDS header */
				{
					pheader[header_bytes] = abyte; header_bytes++;

					if(header_bytes >= sizeof(CCSDS_Packet_Header))
						StateOne();

					break;
				}
				case 2: /* Get the command payload */
				{
					pcommand[command_bytes] = abyte; command_bytes++;

					if(command_bytes >= command_bytes_2_read)
						StateTwo();

					break;
				}
				default:
				{
					syncstate = 0;
					syncword = 0;
				}
			}

			/* Get the byte to process */
			bread = read(npipe[READ], &abyte, sizeof(uint8));
		}
	}


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::StateZero()
{

	syncstate = 1;
	syncword = 0;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::StateOne()
{

	/* Decode the header */
	DecodeCCSDSPacketHeader(&decoded_header, &command_header);

	/* Do some error checking */
	if((decoded_header.length > 512) || (decoded_header.length < 1))
	{
		syncstate = 0;
	}
	else if(decoded_header.id != COMMAND_M_ID)
	{
		syncstate = 0;
	}
	else
	{
		command_bytes_2_read = decoded_header.length;
		syncstate = 2;
	}

	/* Reset header and bytes */
	header_bytes = 0;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::StateTwo()
{

	/* Decode the header */
	DecodeCCSDSPacketHeader(&decoded_header, &command_header);

	/* Bent pipe data to Commando */
	write(TLM_2_CMD_P[WRITE], &command_header, sizeof(CCSDS_Packet_Header));
	write(TLM_2_CMD_P[WRITE], &command_body, decoded_header.length);

	syncstate = 0;

	command_bytes = 0;
	command_bytes_2_read = 0;

	memset(&command_body, 0x0, sizeof(Command_Union));
	memset(&command_header, 0x0, sizeof(CCSDS_Packet_Header));
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::ImportCommando()
{

	uint32 bread, status;

	bread = read(CMD_2_TLM_P[READ], &command_header, sizeof(CCSDS_Packet_Header));

	if(bread == sizeof(CCSDS_Packet_Header))
	{
		DecodeCCSDSPacketHeader(&decoded_header, &command_header);
		FormCCSDSPacketHeader(&packet_header, decoded_header.id, 0, decoded_header.length, 0, packet_tic++);
		read(CMD_2_TLM_P[READ], &message_body, decoded_header.length);
		EmitCCSDSPacket(&message_body, decoded_header.length);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::Export()
{

	int32 lcv;

	/* Dump EKF ASAP */
	if(export_ekf == true)
	{
		(this->*msg_handlers[EKF_STATE_M_ID])();
		(this->*msg_handlers[EKF_COVARIANCE_M_ID])();
		(this->*msg_handlers[EKF_RESIDUAL_M_ID])();
		export_ekf = false;
	}

	/* Now transmit the normal once/pvt stuff */
	if(export_messages == true)
	{
		export_tic++;

		/* Get the start of execution */
		IncStartTic();

		/* Dump info */
		for(lcv = FIRST_PERIODIC_M_ID; lcv < LAST_PERIODIC_M_ID; lcv++)
		{
			if((msg_handlers[lcv] != NULL) && (msg_rates[lcv] != 0))
			{
				if((export_tic % msg_rates[lcv]) == 0)
				{
					(this->*msg_handlers[lcv])();
				}
			}
		}

		export_messages = false;

		/* Get the stop of execution */
		IncStopTic();

		last_start_tic = start_tic;
		last_stop_tic = stop_tic;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendBoardHealth()
{

	int32 lcv;
	uint32 shu;

	Board_Health_M *board_health = &message_body.board_health;

	/* FPGA Version Numbers */
	board_health->acq_version = 0;
	board_health->trk_version = 0;
	board_health->fft_version = 0;

	/* DSA Values */
	board_health->dsa0 = pSource->getScale();
	board_health->dsa1 = 0;
	board_health->dsa2 = 0;
	board_health->dsa3 = 0;

	/* Overflow on A/Ds */
	board_health->ovrflw0 = pSource->getOvrflw();
	board_health->ovrflw1 = 0;
	board_health->ovrflw2 = 0;
	board_health->ovrflw3 = 0;

	/* LO Status Bit */
	board_health->lo_locked = 0;

	/* Acquisition SRAM Health (Stuck bits, etc) */
	board_health->sram_bad_mem = 0;
	board_health->sram_bad_lo = 0;
	board_health->sram_bad_hi = 0;

	/* Software Revision */
	board_health->sft_version = SOFTWARE_VERSION_MAJOR << 16;
	board_health->sft_version += SOFTWARE_VERSION_MINOR << 8;
	board_health->sft_version += SOFTWARE_VERSION_POSTFIX;

	/* Update only 1 shu at a time */
	shu = execution_tic % SHU_SIGNALS;
	adc_values[shu] = ReadSHU(shu);

	for(lcv = 0; lcv < SHU_SIGNALS; lcv++)
		board_health->adc_values[lcv] = adc_values[lcv];

	board_health->tic = pvt_s.sps.tic;

	/* Form the packet header */
	FormCCSDSPacketHeader(&packet_header, BOARD_HEALTH_M_ID, 0, sizeof(Board_Health_M), 0, packet_tic++);

	/* Emit the packet */
	EmitCCSDSPacket((void *)&message_body.board_health, sizeof(Board_Health_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendTaskHealth()
{

	Task_Health_M *task_health = &message_body.task_health;

	/* Get execution counters */
	task_health->execution_tic[TRACKING_ISR_TASK_ID]= pCorrelator->getExecTic();
	task_health->execution_tic[COMMANDO_TASK_ID]  	= pCommando->getExecTic();
	task_health->execution_tic[ACQUISITION_TASK_ID] = pAcquisition->getExecTic();
	task_health->execution_tic[SV_SELECT_TASK_ID] 	= pSV_Select->getExecTic();
	task_health->execution_tic[EPHEMERIS_TASK_ID] 	= pEphemeris->getExecTic();
	task_health->execution_tic[TELEMETRY_TASK_ID]  	= execution_tic;
	task_health->execution_tic[PATIENCE_TASK_ID]  	= 0;
	task_health->execution_tic[EKF_TASK_ID]  		= 0;
	task_health->execution_tic[PVT_TASK_ID]  		= pPVT->getExecTic();
	task_health->execution_tic[PPS_TASK_ID]  		= 0;
	task_health->execution_tic[IDLE_TASK_ID]  		= 0;

	/* get execution counters */
	task_health->start_tic[TRACKING_ISR_TASK_ID]	= pCorrelator->getStartTic();
	task_health->start_tic[COMMANDO_TASK_ID]  		= pCommando->getStartTic();
	task_health->start_tic[ACQUISITION_TASK_ID] 	= pAcquisition->getStartTic();
	task_health->start_tic[SV_SELECT_TASK_ID] 		= pSV_Select->getStartTic();
	task_health->start_tic[EPHEMERIS_TASK_ID] 		= pEphemeris->getStartTic();
	task_health->start_tic[TELEMETRY_TASK_ID]  		= last_start_tic;
	task_health->start_tic[PATIENCE_TASK_ID]  		= 0;
	task_health->start_tic[EKF_TASK_ID]  			= 0;
	task_health->start_tic[PVT_TASK_ID]  			= pPVT->getStartTic();
	task_health->start_tic[PPS_TASK_ID]  			= 0;
	task_health->start_tic[IDLE_TASK_ID]  			= 0;

	/* get execution counters */
	task_health->stop_tic[TRACKING_ISR_TASK_ID]		= pCorrelator->getStopTic();
	task_health->stop_tic[COMMANDO_TASK_ID]  		= pCommando->getStopTic();
	task_health->stop_tic[ACQUISITION_TASK_ID] 		= pAcquisition->getStopTic();
	task_health->stop_tic[SV_SELECT_TASK_ID] 		= pSV_Select->getStopTic();
	task_health->stop_tic[EPHEMERIS_TASK_ID] 		= pEphemeris->getStopTic();
	task_health->stop_tic[TELEMETRY_TASK_ID]  		= last_stop_tic;
	task_health->stop_tic[PATIENCE_TASK_ID]  		= 0;
	task_health->stop_tic[EKF_TASK_ID]  			= 0;
	task_health->stop_tic[PVT_TASK_ID]  			= pPVT->getStopTic();
	task_health->stop_tic[PPS_TASK_ID]  			= 0;
	task_health->stop_tic[IDLE_TASK_ID]  			= 0;

	/* Check for missed interrupts */
	task_health->missed_interrupts = 0;
	task_health->tic_fpu_mul = 0;
	task_health->tic_fpu_div = 0;
	task_health->tic = pvt_s.sps.tic;

	/* Form the packet header */
	FormCCSDSPacketHeader(&packet_header, TASK_HEALTH_M_ID, 0, sizeof(Task_Health_M), 0, packet_tic++);

	/* Emit the packet */
	EmitCCSDSPacket((void *)&message_body.task_health, sizeof(Task_Health_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendChannelHealth()
{

	int32 lcv;
	Channel_M *channel = &message_body.channel;
	Channel *aChannel;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		aChannel = pChannels[lcv];

		/* Only emit active channels */
		channel->chan 		= lcv;
		channel->tic		= pvt_s.sps.tic;
		channel->state 		= aChannel->state;		//!< Channel's state
		channel->sv 		= aChannel->sv;			//!< SV/PRN number the channel is tracking
		channel->antenna 	= aChannel->antenna;	//!< Antenna channel is tracking off of
		channel->len 		= aChannel->len;		//!< Accumulation length (1 or 20 ms)
		channel->cn0 		= aChannel->cn0 * 128;	//!< CN0 estimate
		channel->p_avg		= aChannel->P_avg;		//!< Filtered version of I^2+Q^2
		channel->bit_lock 	= aChannel->bit_lock;	//!< Bit lock?
		channel->frame_lock = aChannel->frame_lock;	//!< Frame lock?
		channel->navigate 	= aChannel->navigate;	//!< Navigate on this channel flag
		channel->count 		= aChannel->count;		//!< Number of accumulations that have been processed
		channel->subframe	= aChannel->subframe;	//!< Current subframe number
		channel->best_epoch = aChannel->best_epoch;	//!< Best estimate of bit edge position
		channel->w 			= aChannel->aPLL.w*4096.0;						//!< 3rd order PLL state
		channel->x 			= aChannel->aPLL.x*4096.0;						//!< 3rd order PLL state
		channel->z 			= aChannel->aPLL.z*4096.0;						//!< 3rd order PLL state
		channel->code_nco 	= aChannel->code_nco*HZ_2_NCO_CODE_INCR;		//!< State of code_nco
		channel->carrier_nco = aChannel->carrier_nco*HZ_2_NCO_CARR_INCR;	//!< State of carrier_nco

		/* Form the packet */
		FormCCSDSPacketHeader(&packet_header, CHANNEL_M_ID, 0, sizeof(Channel_M), 0, packet_tic++);

		/* Emit the packet */
		EmitCCSDSPacket((void *)&message_body.channel, sizeof(Channel_M));
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendSPS()
{

	/* Form the packet */
	FormCCSDSPacketHeader(&packet_header, SPS_M_ID, 0, sizeof(SPS_M), 0, packet_tic++);

	/* Emit the packet */
	EmitCCSDSPacket((void *)&pvt_s.sps, sizeof(SPS_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendClock()
{

	/* Form the packet */
	FormCCSDSPacketHeader(&packet_header, CLOCK_M_ID, 0, sizeof(Clock_M), 0, packet_tic++);

	/* Emit the packet */
	EmitCCSDSPacket((void *)&pvt_s.clock, sizeof(Clock_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendTOT()
{

	/* Form the packet */
	FormCCSDSPacketHeader(&packet_header, TOT_M_ID, 0, sizeof(TOT_M), 0, packet_tic++);

	/* Emit the packet */
	EmitCCSDSPacket((void *)&pvt_s.tot, sizeof(TOT_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendPseudoranges()
{

	int32 lcv;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		/* Form the packet */
		FormCCSDSPacketHeader(&packet_header, PSEUDORANGE_M_ID, 0, sizeof(Pseudorange_M), 0, packet_tic++);

		/* Emit the packet */
		pvt_s.pseudoranges[lcv].chan = lcv;
		EmitCCSDSPacket((void *)&pvt_s.pseudoranges[lcv], sizeof(Pseudorange_M));
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendMeasurements()
{

	int32 lcv;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		/* Form the packet */
		FormCCSDSPacketHeader(&packet_header, MEASUREMENT_M_ID, 0, sizeof(Measurement_M), 0, packet_tic++);

		/* Emit the packet */
		pvt_s.pseudoranges[lcv].chan = lcv;
		EmitCCSDSPacket((void *)&pvt_s.measurements[lcv], sizeof(Measurement_M));
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendSVPositions()
{

	int32 lcv;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		/* Form the packet */
		FormCCSDSPacketHeader(&packet_header, SV_POSITION_M_ID, 0, sizeof(SV_Position_M), 0, packet_tic++);

		/* Emit the packet */
		pvt_s.sv_positions[lcv].chan = lcv;
		EmitCCSDSPacket((void *)&pvt_s.sv_positions[lcv], sizeof(SV_Position_M));
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendSVPredictions()
{

	int32 sv;

	sv = export_tic % MAX_SV;

	/* Form the packet */
	FormCCSDSPacketHeader(&packet_header, SV_PREDICTION_M_ID, 0, sizeof(SV_Prediction_M), 0, packet_tic++);

	/* Emit the packet */
	sv_predictions[sv].sv = sv;
	EmitCCSDSPacket((void *)&sv_predictions[sv], sizeof(SV_Prediction_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendPPS()
{

	/* Form the packet */
	FormCCSDSPacketHeader(&packet_header, PPS_M_ID, 0, sizeof(PPS_M), 0, packet_tic++);

	/* Emit the packet */
	EmitCCSDSPacket((void *)&pps_s, sizeof(PPS_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendEKFState()
{

	/* Form the packet */
	FormCCSDSPacketHeader(&packet_header, EKF_STATE_M_ID, 0, sizeof(EKF_State_M), 0, packet_tic++);

	/* Emit the packet */
	EmitCCSDSPacket((void *)&ekf_s.state, sizeof(EKF_State_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendEKFResidual()
{

	/* Form the packet */
	FormCCSDSPacketHeader(&packet_header, EKF_RESIDUAL_M_ID, 0, sizeof(EKF_Residual_M), 0, packet_tic++);

	/* Emit the packet */
	EmitCCSDSPacket((void *)&ekf_s.residual, sizeof(EKF_Residual_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::SendEKFCovariance()
{

	/* Form the packet */
	FormCCSDSPacketHeader(&packet_header, EKF_COVARIANCE_M_ID, 0, sizeof(EKF_Covariance_M), 0, packet_tic++);

	/* Emit the packet */
	EmitCCSDSPacket((void *)&ekf_s.covariance, sizeof(EKF_Covariance_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 Telemetry::ReadSHU(uint32 _shu)
{

//	uint32 value = 0;
//	uint32 mux = 0;
//
//	/* The address bits are inverted on the board */
//	mux = (~_shu) & 0xf;
//
//	/* Also the address bits are discontinuous */
//	if(mux < 8)
//	{
//		mux = mux << 13;
//	}
//	else
//	{
//		mux = (mux & 0x7) << 13; /* Fixed & value 1/15/2008 */
//		mux += 0x80000000;
//	}
//
//	/* Must not clear out vital bits in PADAT */
//	value = cpu_iord_32(PADAT_ADDR);
//	mux |= (value & 0x7FFF1FFF);
//
//	cpu_iowr_32(PADAT_ADDR,mux);		/* Set mux on PADAT */
//	NU_Sleep(4);						/* After talking to R. Butler, sleeping for 10 ms should be enough */
//										/* Ugh this is a cluster fuck, the ADC needs 4 clocks to latch data,
//										however it is being fed the OS Timer!!! as a clock, hence you must
//										sleep 40 ms to latch data, however this is too much sleeping for
//										the task, so for now we accept sheit data out of the SHU */
//	value = ~cpu_iord_32(PADAT_ADDR);	/* Read from PADAT */
//	value = value & 0x00000FFF;			/* Only bottom 12 bits are important? */
//
//	return value;

	return(0);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::EmitCCSDSPacket(void *_buff, int32 _len)
{

	int32 lcv;
	uint8 *sbuff;
	uint8 pre = 0xAA;
	uint8 post = 0xBB;

	/* Write the sync header to the UART */
	if(npipe_open)
	{
		for(lcv = 0; lcv < 4; lcv++)
			write(npipe[WRITE], &pre, sizeof(uint8));

		/* Write the CCSDS header to the UART */
		sbuff = (uint8 *)&packet_header;
		for(lcv = 0; lcv < 6; lcv++)
		{
			write(npipe[WRITE], sbuff, sizeof(uint8));
			sbuff++;
		}

		/* Now write the body to the UART */
		sbuff = (uint8 *)_buff;
		for(lcv = 0; lcv < _len; lcv++)
		{
			write(npipe[WRITE], sbuff, sizeof(uint8));
			sbuff++;
		}

		/* Write the sync postfix to the UART */
		for(lcv = 0; lcv < 4; lcv++)
			write(npipe[WRITE], &post, sizeof(uint8));
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Telemetry::setMessageRate(uint32 _mid, uint32 _val)
{
	if((_mid > FIRST_PERIODIC_M_ID) && (_mid < LAST_PERIODIC_M_ID))
	{
		msg_rates[_mid] = _val;
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 Telemetry::getMessageRate(uint32 _mid)
{
	if((_mid > FIRST_PERIODIC_M_ID) && (_mid < LAST_PERIODIC_M_ID))
	{
		return(msg_rates[_mid]);
	}
	else
	{
		return(0);
	}
}
/*----------------------------------------------------------------------------------------------*/


