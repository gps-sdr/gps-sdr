/*! \file Commando.cpp
	Implements member functions of Commando class.
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

#include "commando.h"

/*----------------------------------------------------------------------------------------------*/
void *Commando_Thread(void *_arg)
{

	Commando *aCommando = pCommando;

	aCommando->SetPid();

	while(grun)
	{
		aCommando->Import();
		aCommando->Export();
		aCommando->IncExecTic();
	}

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Start()
{
	Start_Thread(Commando_Thread, NULL);

	if(gopt.verbose)
		printf("Commando thread started\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Commando::Commando()
{

	execution_tic = 0;	//!< Execution counter
	start_tic = 0;		//!< OS tic at start of function
	stop_tic = 0;		//!< OS tic at end of function
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Commando::~Commando()
{

	if(gopt.verbose)
		printf("Destructing Commando\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Import()
{

	uint32 bread, id;

	bread = read(Telem_2_Cmd_P[READ], &command_header, sizeof(CCSDS_Packet_Header));//!< Read in the head
	DecodeCCSDSPacketHeader(&decoded_header, &command_header);						//!< Decode the commmand
	bread = read(Telem_2_Cmd_P[READ], &command_body, decoded_header.length);		//!< Read in the body

	IncStartTic();

	/* Make sure the command is in the correct format */
	if(decoded_header.id == COMMAND_M_ID)
		id = command_body.reset_all.command_id;
	else
		return;

	/* Now do something based on the command */
	switch(id)
	{
		case RESET_ALL_C_ID:
			resetReset();
			break;
		case RESET_PVT_C_ID:
			resetPVT();
			break;
		case RESET_EKF_C_ID:
			resetEKF();
			break;
		case RESET_CHANNEL_C_ID:
			resetChannel();
			break;
		case RESET_EPHEMERIS_C_ID:
			resetEphemeris();
			break;
		case RESET_ALMANAC_C_ID:
			resetAlmanac();
			break;
		case GET_MEASUREMENT_C_ID:
			getMeasurement();
			break;
		case GET_PSEUDORANGE_C_ID:
			getPseudorange();
			break;
		case GET_EPHEMERIS_C_ID:
			getEphemeris();
			break;
		case GET_ALMANAC_C_ID:
			getAlmanac();
			break;
		case SET_EPHEMERIS_C_ID:
			setEphemeris();
			break;
		case SET_ALMANAC_C_ID:
			setAlmanac();
			break;
		case SET_ACQ_CONFIG_C_ID:
			setAcq_Config();
			break;
		case SET_PVT_C_ID:
			setPVT();
			break;
		case GET_ACQ_CONFIG_C_ID:
			getAcq_Config();
			break;
		case GET_SV_PREDICTION_C_ID:
			getSV_Prediction();
			break;
		case GET_EPHEMERIS_VALID_C_ID:
			getEphemeris_Valid();
			break;
		case GET_BOARD_HEALTH_C_ID:
			getBoard_Health();
			break;
		case GET_ACQ_COMMAND_C_ID:
			getAcq_Command();
			break;
		case GET_SV_POSITION_C_ID:
			getSV_Position();
			break;
		case GET_CHANNEL_C_ID:
			getChannel();
			break;
		default:
			break;
	}

	Send_Ack();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Export()
{

	IncStopTic();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Send_Ack()
{

	uint32 preamble = 0xAAAAAAAA;

	command_ack.command_tic = decoded_header.tic;
	command_ack.command_id = decoded_header.id;
	FormCCSDSPacketHeader(&packet_header, COMMAND_ACK_M_ID, 0, sizeof(Command_Ack_M), 1, command_tic++);

	/* Dump to the telemetry */
	write(Cmd_2_Telem_P[WRITE], &preamble, sizeof(uint32));
	write(Cmd_2_Telem_P[WRITE], &packet_header, sizeof(CCSDS_Packet_Header));
	write(Cmd_2_Telem_P[WRITE], &command_ack, sizeof(Command_Ack_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetReset()
{

	resetPVT();
	resetEKF();

	command_body.reset_channel.chan = MAX_CHANNELS;
	resetChannel();

	command_body.reset_ephemeris.sv = NUM_CODES;
	resetEphemeris();

	command_body.reset_almanac.sv = NUM_CODES;
	resetAlmanac();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetPVT()
{

	pPVT->Lock();
	pPVT->Reset();
	pPVT->Unlock();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetEKF()
{


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetChannel()
{

	int32 chan;

	chan = command_body.reset_channel.chan;

	if((chan >= 0) && (chan < MAX_CHANNELS))
	{
		pChannels[chan]->Lock();
		pChannels[chan]->Kill();
		pChannels[chan]->Unlock();
	}
	else
	{
		for(chan = 0; chan < MAX_CHANNELS; chan++)
		{
			pChannels[chan]->Lock();
			pChannels[chan]->Kill();
			pChannels[chan]->Unlock();
		}

	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetEphemeris()
{

	pEphemeris->Lock();
	pEphemeris->ClearEphemeris(command_body.reset_ephemeris.sv);
	pEphemeris->Export();
	pEphemeris->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::resetAlmanac()
{

	pEphemeris->Lock();
	pEphemeris->ClearAlmanac(command_body.reset_almanac.sv);
	pEphemeris->Export();
	pEphemeris->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getMeasurement()
{

	pSerial_Telemetry->Lock();

	if(command_body.get_measurement.flag)
		pSerial_Telemetry->startStream(MEASUREMENT_M_ID);
	else
		pSerial_Telemetry->stopStream(MEASUREMENT_M_ID);

	pSerial_Telemetry->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getPseudorange()
{

	pSerial_Telemetry->Lock();

	if(command_body.get_pseudorange.flag)
		pSerial_Telemetry->startStream(PSEUDORANGE_M_ID);
	else
		pSerial_Telemetry->stopStream(PSEUDORANGE_M_ID);

	pSerial_Telemetry->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getSV_Position()
{

	pSerial_Telemetry->Lock();

	if(command_body.get_sv_position.flag)
		pSerial_Telemetry->startStream(SV_POSITION_M_ID);
	else
		pSerial_Telemetry->stopStream(SV_POSITION_M_ID);

	pSerial_Telemetry->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getChannel()
{

	pSerial_Telemetry->Lock();

	if(command_body.get_channel.flag)
		pSerial_Telemetry->startStream(CHANNEL_M_ID);
	else
		pSerial_Telemetry->stopStream(CHANNEL_M_ID);

	pSerial_Telemetry->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getEphemeris()
{
	int32 lcv;

	/* Get the ephemeris */
	if((command_body.get_ephemeris.sv >=0) && (command_body.get_ephemeris.sv < NUM_CODES))
	{
		pEphemeris->Lock();
		message_buff.ephemeris = pEphemeris->getEphemeris(command_body.get_ephemeris.sv);
		pEphemeris->Unlock();

		/* Form the packet header */
		FormCCSDSPacketHeader(&packet_header, EPHEMERIS_M_ID, 0, sizeof(Ephemeris_M), 0, command_tic++);

		/* Emit the packet */
		EmitCCSDSPacket((void *)&message_buff, sizeof(Ephemeris_M));
	}
	else
	{
		for(lcv = 0; lcv < NUM_CODES; lcv++)
		{
			pEphemeris->Lock();
			message_buff.ephemeris = pEphemeris->getEphemeris(lcv);
			pEphemeris->Unlock();

			/* Form the packet header */
			FormCCSDSPacketHeader(&packet_header, EPHEMERIS_M_ID, 0, sizeof(Ephemeris_M), 0, command_tic++);

			/* Emit the packet */
			EmitCCSDSPacket((void *)&message_buff, sizeof(Ephemeris_M));
		}

	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getAlmanac()
{

	int32 lcv;

	/* Get the ephemeris */
	if((command_body.get_almanac.sv >=0) && (command_body.get_almanac.sv < NUM_CODES))
	{
		pEphemeris->Lock();
		message_buff.almanac = pEphemeris->getAlmanac(command_body.get_almanac.sv);
		pEphemeris->Unlock();

		/* Form the packet header */
		FormCCSDSPacketHeader(&packet_header, ALMANAC_M_ID, 0, sizeof(Almanac_M), 0, command_tic++);

		/* Emit the packet */
		EmitCCSDSPacket((void *)&message_buff, sizeof(Almanac_M));


	}
	else
	{
		for(lcv = 0; lcv < NUM_CODES; lcv++)
		{
			pEphemeris->Lock();
			message_buff.almanac = pEphemeris->getAlmanac(lcv);
			pEphemeris->Unlock();

			/* Form the packet header */
			FormCCSDSPacketHeader(&packet_header, ALMANAC_M_ID, 0, sizeof(Almanac_M), 0, command_tic++);

			/* Emit the packet */
			EmitCCSDSPacket((void *)&message_buff, sizeof(Almanac_M));
		}

	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::setAlmanac()
{

	pEphemeris->Lock();
	pEphemeris->setAlmanac(&command_body.set_almanac.almanac);
	pEphemeris->Export();
	pEphemeris->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::setEphemeris()
{

	pEphemeris->Lock();
	pEphemeris->setEphemeris(&command_body.set_ephemeris.ephemeris);
	pEphemeris->Export();
	pEphemeris->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::setAcq_Config()
{

	pSV_Select->Lock();
	pSV_Select->setConfig(&command_body.set_acq_config.acq_config);
	pSV_Select->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::setPVT()
{

	pPVT->Lock();
	pPVT->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getAcq_Config()
{

	pSV_Select->Lock();
	message_buff.acq_config = pSV_Select->getConfig();
	pSV_Select->Unlock();

	/* Form the packet header */
	FormCCSDSPacketHeader(&packet_header, ACQ_CONFIG_M_ID, 0, sizeof(Acq_Config_M), 0, command_tic++);

	/* Emit the packet */
	EmitCCSDSPacket((void *)&message_buff, sizeof(Acq_Config_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getSV_Prediction()
{
	int32 lcv;

	if((command_body.get_sv_prediction.sv >=0) && (command_body.get_sv_prediction.sv < NUM_CODES))
	{
		pSV_Select->Lock();
		message_buff.sv_prediction = pSV_Select->getSVPrediction(command_body.get_sv_prediction.sv);
		pSV_Select->Unlock();

		/* Form the packet header */
		FormCCSDSPacketHeader(&packet_header, SV_PREDICTION_M_ID, 0, sizeof(SV_Prediction_M), 0, command_tic++);

		/* Emit the packet */
		EmitCCSDSPacket((void *)&message_buff, sizeof(SV_Prediction_M));


	}
	else
	{
		for(lcv = 0; lcv < NUM_CODES; lcv++)
		{
			pSV_Select->Lock();
			message_buff.sv_prediction = pSV_Select->getSVPrediction(lcv);
			pSV_Select->Unlock();

			/* Form the packet header */
			FormCCSDSPacketHeader(&packet_header, SV_PREDICTION_M_ID, 0, sizeof(SV_Prediction_M), 0, command_tic++);

			/* Emit the packet */
			EmitCCSDSPacket((void *)&message_buff, sizeof(SV_Prediction_M));
		}

	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getEphemeris_Valid()
{

	pSerial_Telemetry->Lock();
	pSerial_Telemetry->SendEphemerisStatus();
	pSerial_Telemetry->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getBoard_Health()
{

	pSerial_Telemetry->Lock();
	pSerial_Telemetry->SendBoardHealth();
	pSerial_Telemetry->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::getAcq_Command()
{

	int32 lcv;

	if((command_body.get_acq_command.sv >=0) && (command_body.get_acq_command.sv < NUM_CODES))
	{
		pSV_Select->Lock();
		message_buff.acq_command = pSV_Select->getAcqCommand(command_body.get_acq_command.sv);
		pSV_Select->Unlock();

		/* Form the packet */
		FormCCSDSPacketHeader(&packet_header, ACQ_COMMAND_M_ID, 0, sizeof(Acq_Command_M), 0, command_tic++);

		/* Emit the packet */
		EmitCCSDSPacket((void *)&message_buff, sizeof(Acq_Command_M));


	}
	else
	{
		for(lcv = 0; lcv < NUM_CODES; lcv++)
		{
			pSV_Select->Lock();
			message_buff.acq_command = pSV_Select->getAcqCommand(lcv);
			pSV_Select->Unlock();

			/* Form the packet */
			FormCCSDSPacketHeader(&packet_header, ACQ_COMMAND_M_ID, 0, sizeof(Acq_Command_M), 0, command_tic++);

			/* Emit the packet */
			EmitCCSDSPacket((void *)&message_buff, sizeof(Acq_Command_M));
		}


	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::EmitCCSDSPacket(void *_buff, uint32 _len)
{

	uint32 preamble = 0xAAAAAAAA;


	/* Dump to the telemetry */
	write(Cmd_2_Telem_P[WRITE], &preamble, sizeof(uint32));
	write(Cmd_2_Telem_P[WRITE], &packet_header, sizeof(CCSDS_Packet_Header));
	write(Cmd_2_Telem_P[WRITE], _buff, _len);

}
/*----------------------------------------------------------------------------------------------*/
