/*----------------------------------------------------------------------------------------------*/
/*! \file commando.cpp
//
// FILENAME: commando.cpp
//
// DESCRIPTION: Implement the Commando class.
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

#include "commando.h"

/*----------------------------------------------------------------------------------------------*/
void *Commando_Thread(void *_arg)
{

	while(grun)
	{
		pCommando->Import();
	}

	pthread_exit(0);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Start()
{
	Start_Thread(Commando_Thread, NULL);

	if(gopt.verbose)
		fprintf(stdout,"Commando thread started\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Commando::Commando():Threaded_Object("CMDTASK")
{
	object_mem = this;
	size = sizeof(Commando);

	/* Build the function pointer array */
	cmd_handlers[FIRST_C_ID] 				= NULL;
	cmd_handlers[NULL_C_ID] 				= &Commando::doNull;
	cmd_handlers[RESET_ALL_C_ID] 			= &Commando::resetAll;
	cmd_handlers[RESET_PVT_C_ID] 			= &Commando::resetPVT;
	cmd_handlers[RESET_EKF_C_ID] 			= &Commando::resetEKF;
	cmd_handlers[RESET_CHANNEL_C_ID] 		= &Commando::resetChannel;
	cmd_handlers[RESET_ALMANAC_C_ID] 		= &Commando::resetAlmanac;
	cmd_handlers[RESET_EPHEMERIS_C_ID] 		= &Commando::resetEphemeris;
	cmd_handlers[RESET_PPS_C_ID] 			= &Commando::resetPPS;
	cmd_handlers[RESET_WATCHDOG_C_ID] 		= &Commando::resetWatchdog;
	cmd_handlers[SET_PERIODIC_C_ID] 		= &Commando::setPeriodic;
	cmd_handlers[SET_ALMANAC_C_ID] 			= &Commando::setAlmanac;
	cmd_handlers[SET_EPHEMERIS_C_ID] 		= &Commando::setEphemeris;
	cmd_handlers[SET_EEPROM_C_ID] 			= &Commando::setEEPROM;
	cmd_handlers[SET_MEMORY_C_ID] 			= &Commando::setMemory;
	cmd_handlers[GET_ALMANAC_C_ID] 			= &Commando::getAlmanac;
	cmd_handlers[GET_EPHEMERIS_C_ID] 		= &Commando::getEphemeris;
	cmd_handlers[GET_SV_PREDICTION_C_ID]	= &Commando::getSVPrediction;
	cmd_handlers[GET_EPHEMERIS_STATUS_C_ID] = &Commando::getEphemerisStatus;
	cmd_handlers[GET_SV_SELECT_STATUS_C_ID] = &Commando::getSVSelectStatus;
	cmd_handlers[GET_EEPROM_C_ID] 			= &Commando::getEEPROM;
	cmd_handlers[GET_EEPROM_CHKSUM_C_ID] 	= &Commando::getEEPROMChksum;
	cmd_handlers[GET_MEMORY_C_ID] 			= &Commando::getMemory;
	cmd_handlers[GET_MEMORY_CHKSUM_C_ID] 	= &Commando::getMemoryChksum;
	cmd_handlers[LAST_C_ID] 				= NULL;

	if(gopt.verbose)
		fprintf(stdout,"Creating Commando\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Commando::~Commando()
{
	if(gopt.verbose)
		fprintf(stdout,"Destructing Commando\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Import()
{

	uint32 bread;

	/* Wait for data from the serial port */
	read(TLM_2_CMD_P[READ], &packet_header, sizeof(CCSDS_Packet_Header));

	/* Decode the header */
	DecodeCCSDSPacketHeader(&decoded_header, &packet_header);

	/* Get the body */
	read(TLM_2_CMD_P[READ], &command_body, decoded_header.length);

	/* Get the start of execution */
	IncStartTic();

	/* Now execute the command */
	ParseCommand();

	/* Get the stop of execution */
	IncStopTic();

	/* Increment execution count */
	IncExecTic();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::ParseCommand()
{

	uint32 id;

	/* Make sure the command is in the correct format */
	id = command_body.reset_all.command_id;

	if((id > FIRST_M_ID) && (id < LAST_M_ID))
	{
		if(cmd_handlers[id] != NULL) /* Error check */
		{
			(this->*cmd_handlers[id])();
		}
		else
		{
			message_body.command_ack.command_status = INVALID_COMMAND_A_ID;
		}
	}
	else
	{
		message_body.command_ack.command_status = INVALID_COMMAND_A_ID;
	}

	SendAck();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::SendAck()
{

	/* message_body.command_ack.command_status should have been set by previous function! */
	message_body.command_ack.command_tic = command_tic++;
	message_body.command_ack.command_id = decoded_header.id;

	/* Form the ack header */
	FormCCSDSPacketHeader(&packet_header, COMMAND_ACK_M_ID, 0, sizeof(Command_Ack_M), 0, 0);

	/* Dump to the telemetry */
	EmitCCSDSPacket(&message_body, sizeof(Command_Ack_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::doNull()
{

	message_body.command_ack.command_status = SUCCESS_A_ID;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetAll()
{

	resetPVT();
	resetEKF();
	resetPPS();

	command_body.reset_channel.chan = MAX_CHANNELS;
	resetChannel();

	command_body.reset_ephemeris.sv = MAX_SV;
	resetEphemeris();

	command_body.reset_almanac.sv = MAX_SV;
	resetAlmanac();

	message_body.command_ack.command_status = SUCCESS_A_ID;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetPVT()
{

	pPVT->Lock();
	pPVT->Reset();
	pPVT->Unlock();

	message_body.command_ack.command_status = SUCCESS_A_ID;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetEKF()
{
//
//	pEKF->Lock();
//	pEKF->Reset();
//	pEKF->Unlock();
//
	message_body.command_ack.command_status = SUCCESS_A_ID;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetWatchdog()
{

//	pPatience->Lock();
//	pPatience->ResetWatchdog();
//	pPatience->Unlock();
//

	message_body.command_ack.command_status = SUCCESS_A_ID;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetChannel()
{

	int32 chan, lcv;

	chan = command_body.reset_channel.chan;

	if((chan >= 0) && (chan < MAX_CHANNELS))
	{
		pChannels[chan]->Lock();
		pChannels[chan]->Kill();
		pChannels[chan]->Unlock();
	}
	else
	{
		for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		{
			pChannels[lcv]->Lock();
			pChannels[lcv]->Kill();
			pChannels[lcv]->Unlock();
		}
	}

	message_body.command_ack.command_status = SUCCESS_A_ID;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetEphemeris()
{

	pEphemeris->Lock();
	pEphemeris->ClearEphemeris(command_body.reset_ephemeris.sv);
	pEphemeris->Unlock();

	message_body.command_ack.command_status = SUCCESS_A_ID;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetAlmanac()
{

	pEphemeris->Lock();
	pEphemeris->ClearAlmanac(command_body.reset_almanac.sv);
	pEphemeris->Unlock();

	message_body.command_ack.command_status = SUCCESS_A_ID;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetPPS()
{

	message_body.command_ack.command_status = SUCCESS_A_ID;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::setPeriodic()
{

	uint32 mid;		//!< Message id
	uint32 decim; 	//!< Decimation rate

	mid = command_body.set_periodic.message_id;
	decim = command_body.set_periodic.decimation;

	if((mid <= FIRST_PERIODIC_M_ID) || (mid >= LAST_PERIODIC_M_ID))
	{
		message_body.command_ack.command_status = INVALID_MESSAGE_A_ID;
	}
	else
	{

		pTelemetry->Lock();
		pTelemetry->setMessageRate(mid, decim);
		pTelemetry->Unlock();

		message_body.command_ack.command_status = SUCCESS_A_ID;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getEphemeris()
{
	/* Get the ephemeris */
	if((command_body.get_ephemeris.sv >= 0) && (command_body.get_ephemeris.sv < MAX_SV))
	{
		pEphemeris->Lock();
		message_body.ephemeris = pEphemeris->getEphemeris(command_body.get_ephemeris.sv);
		pEphemeris->Unlock();

		/* Form the packet header */
		FormCCSDSPacketHeader(&packet_header, EPHEMERIS_M_ID, 0, sizeof(Ephemeris_M), 0, 0);

		/* Emit the packet */
		EmitCCSDSPacket((void *)&message_body, sizeof(Ephemeris_M));

		message_body.command_ack.command_status = SUCCESS_A_ID;
	}
	else
	{
		message_body.command_ack.command_status = INVALID_SV_A_ID;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getAlmanac()
{

	/* Get the ephemeris */
	if((command_body.get_almanac.sv >=0) && (command_body.get_almanac.sv < MAX_SV))
	{
		pEphemeris->Lock();
		message_body.almanac = pEphemeris->getAlmanac(command_body.get_almanac.sv);
		pEphemeris->Unlock();

		/* Form the packet header */
		FormCCSDSPacketHeader(&packet_header, ALMANAC_M_ID, 0, sizeof(Almanac_M), 0, 0);

		/* Emit the packet */
		EmitCCSDSPacket((void *)&message_body, sizeof(Almanac_M));

		message_body.command_ack.command_status = SUCCESS_A_ID;
	}
	else
	{
		message_body.command_ack.command_status = INVALID_SV_A_ID;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::setAlmanac()
{

	pEphemeris->Lock();
	pEphemeris->setAlmanac(&command_body.set_almanac.almanac);
	pEphemeris->Unlock();

	message_body.command_ack.command_status = SUCCESS_A_ID;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::setEphemeris()
{

	pEphemeris->Lock();
	pEphemeris->setEphemeris(&command_body.set_ephemeris.ephemeris);
	pEphemeris->Unlock();

	message_body.command_ack.command_status = SUCCESS_A_ID;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getEphemerisStatus()
{

	pEphemeris->Lock();
	message_body.ephemeris_status = pEphemeris->getEphemerisStatus();
	pEphemeris->Unlock();

	/* Form the packet */
	FormCCSDSPacketHeader(&packet_header, EPHEMERIS_STATUS_M_ID, 0, sizeof(Ephemeris_Status_M), 0, 0);

	/* Emit the packet */
	EmitCCSDSPacket((void *)&message_body, sizeof(Ephemeris_Status_M));

	message_body.command_ack.command_status = SUCCESS_A_ID;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getSVSelectStatus()
{

	pSV_Select->Lock();
	message_body.sv_select_status = pSV_Select->getStatus();
	pSV_Select->Unlock();

	/* Form the packet */
	FormCCSDSPacketHeader(&packet_header, SV_SELECT_STATUS_M_ID, 0, sizeof(SV_Select_Status_M), 0, 0);

	/* Emit the packet */
	EmitCCSDSPacket((void *)&message_body, sizeof(SV_Select_Status_M));

	message_body.command_ack.command_status = SUCCESS_A_ID;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getSVPrediction()
{

	/* Get the ephemeris */
	if((command_body.get_sv_prediction.sv >= 0) && (command_body.get_sv_prediction.sv < MAX_SV))
	{

		pSV_Select->Lock();
		message_body.sv_prediction = pSV_Select->getPrediction(command_body.get_sv_prediction.sv);
		pSV_Select->Unlock();

		/* Form the packet header */
		FormCCSDSPacketHeader(&packet_header, SV_PREDICTION_M_ID, 0, sizeof(SV_Prediction_M), 0, 0);

		/* Emit the packet */
		EmitCCSDSPacket((void *)&message_body, sizeof(SV_Prediction_M));

		message_body.command_ack.command_status = SUCCESS_A_ID;
	}
	else
	{
		message_body.command_ack.command_status = INVALID_SV_A_ID;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! Burn some data to EEPROM */
void Commando::setEEPROM()
{


	message_body.command_ack.command_status = SUCCESS_A_ID;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! Return some EEPROM data */
void Commando::getEEPROM()
{


	message_body.command_ack.command_status = SUCCESS_A_ID;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getEEPROMChksum()
{


	message_body.command_ack.command_status = SUCCESS_A_ID;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::PageWrite(uint32 _bank, uint32 *_addr, uint32 *_data, uint32 _words)
{


	message_body.command_ack.command_status = SUCCESS_A_ID;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::PendEEPROM(uint32 _bank)
{


	message_body.command_ack.command_status = SUCCESS_A_ID;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::setMemory()
{


	message_body.command_ack.command_status = SUCCESS_A_ID;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getMemory()
{


	message_body.command_ack.command_status = SUCCESS_A_ID;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getMemoryChksum()
{


	message_body.command_ack.command_status = SUCCESS_A_ID;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::EmitCCSDSPacket(void *_buff, uint32 _len)
{
	write(CMD_2_TLM_P[WRITE], &packet_header, sizeof(CCSDS_Packet_Header));
	write(CMD_2_TLM_P[WRITE], _buff, _len);
}
/*----------------------------------------------------------------------------------------------*/

