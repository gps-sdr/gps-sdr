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

	while(grun)
	{
		aCommando->Import();
		aCommando->Export();
	}

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
void Commando::Start()
{
	pthread_create(&thread, NULL, Commando_Thread, NULL);

	if(gopt.verbose)
		printf("Commando thread started\n");
}
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
void Commando::Stop()
{
	pthread_cancel(thread);
	pthread_join(thread, NULL);

	if(gopt.verbose)
		printf("Commando thread stopped\n");
}
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
Commando::Commando()
{

	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_unlock(&mutex);

	execution_tic = 0;	//!< Execution counter
	start_tic = 0;		//!< OS tic at start of function
	stop_tic = 0;		//!< OS tic at end of function
}
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
Commando::~Commando()
{

	pthread_mutex_destroy(&mutex);

	if(gopt.verbose)
		printf("Destructing Commando\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Import()
{

	uint32 bread;

	bread = read(Telem_2_Cmd_P[READ], &command_header, sizeof(CCSDS_Packet_Header));//!< Read in the head
	DecodeCCSDSPacketHeader(&decoded_header, &command_header);						//!< Decode the commmand
	bread = read(Telem_2_Cmd_P[READ], &command_body, decoded_header.length);			//!< Read in the body

	/* Now do something based on the command */
	switch(decoded_header.id)
	{
		case RESET_PVT_C_ID:
			Reset_PVT();
			break;
		case RESET_EKF_C_ID:
			Reset_EKF();
			break;
		case RESET_CHANNEL_C_ID:
			Reset_Channel();
			break;
		case RESET_EPHEMERIS_C_ID:
			Reset_Ephemeris();
			break;
		case RESET_ALMANAC_C_ID:
			Reset_Almanac();
			break;
		case GET_MEASUREMENT_C_ID:
			Get_Measurement();
			break;
		case GET_PSEUDORANGE_C_ID:
			Get_Pseudorange();
			break;
		case GET_EPHEMERIS_C_ID:
			Get_Ephemeris();
			break;
		case GET_ALMANAC_C_ID:
			Get_Alamanc();
			break;
		default:
			break;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Export()
{

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Send_Ack()
{

	uint32 preamble = 0xAAAAAAAA;

	command_ack.command_tic = command_tic;
	command_ack.command_id = decoded_header.id;
	FormCCSDSPacketHeader(&packet_header, COMMAND_ACK_M_ID, 0, sizeof(Command_Ack_M), 1, command_tic++);

	/* Dump to the telemetry */
	write(Cmd_2_Telem_P[WRITE], &preamble, sizeof(uint32));
	write(Cmd_2_Telem_P[WRITE], &packet_header, sizeof(CCSDS_Packet_Header));
	write(Cmd_2_Telem_P[WRITE], &command_ack, sizeof(Command_Ack_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Reset_PVT()
{

	pPVT->Lock();
	pPVT->Reset();
	pPVT->Unlock();

	Send_Ack();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Reset_EKF()
{


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Reset_Channel()
{

	int32 chan;

	chan = command_body.reset_channel.chan;

	if((chan >= 0) && (chan < MAX_CHANNELS))
	{
		pChannels[chan]->Lock();
		pChannels[chan]->setActive(false);
		pChannels[chan]->Unlock();
	}
	else
	{
		for(chan = 0; chan < MAX_CHANNELS; chan++)
		{
			pChannels[chan]->Lock();
			pChannels[chan]->setActive(false);
			pChannels[chan]->Unlock();
		}

	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Reset_Ephemeris()
{

	pEphemeris->Lock();
	pEphemeris->ClearEphemeris(command_body.reset_ephemeris.sv);
	pEphemeris->Unlock();

	Send_Ack();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Reset_Almanac()
{

	pEphemeris->Lock();
	pEphemeris->ClearAlmanac(command_body.reset_almanac.sv);
	pEphemeris->Unlock();

	Send_Ack();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Get_Measurement()
{


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Get_Pseudorange()
{


}

/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Get_Ephemeris()
{


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Commando::Get_Alamanc()
{


}
/*----------------------------------------------------------------------------------------------*/
