/*! \file serial_telemetry.cpp
	Implements member functions of Serial_Telemetry class.
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

#include "serial_telemetry.h"

/*----------------------------------------------------------------------------------------------*/
void *Serial_Telemetry_Thread(void *_arg)
{

	Serial_Telemetry *aSerial_Telemetry = pSerial_Telemetry;

	while(grun)
	{
		aSerial_Telemetry->Import();
		aSerial_Telemetry->Export();
	}

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::Start()
{
	pthread_attr_t tattr;
	sched_param param;
	int32 ret;

	/* Unitialized with default attributes */
	ret = pthread_attr_init(&tattr);

	/*Ssafe to get existing scheduling param */
	ret = pthread_attr_getschedparam(&tattr, &param);

	/* Set the priority; others are unchanged */
	param.sched_priority = TELEM_PRIORITY;

	/* Setting the new scheduling param */
	ret = pthread_attr_setschedparam(&tattr, &param);
	ret = pthread_attr_setschedpolicy(&tattr, SCHED_FIFO);

	/* With new priority specified */
	pthread_create(&thread, NULL, Serial_Telemetry_Thread, NULL);

	if(gopt.verbose)
		printf("Serial_Telemetry thread started\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::Stop()
{
	pthread_cancel(thread);
	pthread_join(thread, NULL);

	if(gopt.verbose)
		printf("Serial_Telemetry thread stopped\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Serial_Telemetry::Serial_Telemetry(int32 _serial)
{

	execution_tic = start_tic = stop_tic = 0;
	serial_open = npipe_open;
	serial = _serial;

	/* Communicate over the serial port */
	if(serial)
	{
		OpenSerial();
	}
	else /* Communicate over the named pipe */
	{
		/* Everything set, now create a disk thread & pipe, and do some recording! */
		fifo[WRITE] = mkfifo("/tmp/GPS2GUI", S_IRWXG | S_IRWXU | S_IRWXO);
		if ((fifo[WRITE] == -1) && (errno != EEXIST))
			printf("Error creating the named pipe");

		/* Everything set, now create a disk thread & pipe, and do some recording! */
		fifo[READ] = mkfifo("/tmp/GUI2GPS", S_IRWXG | S_IRWXU | S_IRWXO);
		if ((fifo[READ] == -1) && (errno != EEXIST))
			printf("Error creating the named pipe");

		OpenGUIPipe();
	}




	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_unlock(&mutex);

	if(gopt.verbose)
		printf("Creating Serial_Telemetry\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Serial_Telemetry::~Serial_Telemetry()
{

	pthread_mutex_destroy(&mutex);

	if(npipe_open)
	{
		close(npipe[READ]);
		close(npipe[WRITE]);
	}

	if(gopt.verbose)
		printf("Destroying Serial_Telemetry\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::Lock()
{
	pthread_mutex_lock(&mutex);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::Unlock()
{
	pthread_mutex_unlock(&mutex);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::Import()
{
	Channel_Health_M temp;
	int32 bread, lcv, num_chans;

	/* Pend on this pipe */
	bread = read(FIFO_2_Telem_P[READ], &fifo_status, sizeof(FIFO_M));

	/* Lock correlator status */
	pthread_mutex_lock(&mInterrupt);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(gInterrupt[lcv])
		{
			channel_health[lcv] = pChannels[lcv]->getPacket();
			active[lcv] = 1;
		}
		else
		{
			channel_health[lcv].count = 0;
			active[lcv] = 0;
		}
	}

	/* Unlock correlator status */
	pthread_mutex_unlock(&mInterrupt);

	/* Read from PVT */
	bread = read(PVT_2_Telem_P[READ], &sps,   sizeof(SPS_M));
	bread = read(PVT_2_Telem_P[READ], &clock, sizeof(Clock_M));
	bread = read(PVT_2_Telem_P[READ], &sv_positions, MAX_CHANNELS*sizeof(SV_Position_M));
	bread = read(PVT_2_Telem_P[READ], &pseudoranges, MAX_CHANNELS*sizeof(Pseudorange_M));
	bread = read(PVT_2_Telem_P[READ], &measurements, MAX_CHANNELS*sizeof(Measurement_M));

//	bread = sizeof(Acq_Result_S);
//	while(bread == sizeof(Acq_Result_S))
//		bread = read(Acq_2_Telem_P[READ],&tAcq,sizeof(Acq_Result_S));

	/* Read from Ephemeris */
	bread = read(Ephem_2_Telem_P[READ], &ephemeris_status, sizeof(Ephemeris_Status_M));
//
//	bread = sizeof(SV_Select_2_Telem_S);
//	while(bread == sizeof(SV_Select_2_Telem_S))
//		bread = read(SV_Select_2_Telem_P[READ], &tSelect, sizeof(SV_Select_2_Telem_S));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::Export()
{

	if(serial)
	{
		if(serial_open)
			ExportSerial();
		else
			OpenSerial();
	}
	else
	{
		if(npipe_open)
			ExportGUI();
		else
			OpenGUIPipe();
	}


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void lost_gui_pipe(int _sig)
{
	pSerial_Telemetry->SetGUIPipe(false);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::SetGUIPipe(bool _status)
{
	npipe_open = _status;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::ExportGUI()
{

	int32 nbytes, bwrote;
	char *pbuff;

	/* Now transmit the normal once/pvt stuff */
	SendFIFO();
	SendBoardHealth();
	SendTaskHealth();
	SendChannelHealth();
//	SendSPS();
//	SendClock();

	/* See if there is any new GEONS data */


	/* If so write out that message */
	//SendEKF();


	/* Now, get any data from GUI */



	/* Process any incoming data */



	/* Get the stop of execution */


	execution_tic++;


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::OpenGUIPipe()
{

	npipe[READ] = npipe[WRITE] = -1;
	npipe[READ] = open("/tmp/GUI2GPS", O_RDONLY | O_NONBLOCK);
	npipe[WRITE] = open("/tmp/GPS2GUI", O_WRONLY | O_NONBLOCK);

	if((npipe[READ] != -1) && (npipe[WRITE] != -1))
	{
		npipe_open = true;
		printf("GUI connected\n");
	}
	else
		npipe_open = false;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::ExportSerial()
{

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::OpenSerial()
{

}
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::SendBoardHealth()
{

	/* Form the packet header */
	FormCCSDSPacketHeader(BOARD_HEALTH_M_ID, 0, sizeof(Board_Health_M));

	/* Emit the packet */
	EmitCCSDSPacket((void *)&board_health, sizeof(Board_Health_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::SendTaskHealth()
{

	/* Get execution counters */

	/* Form the packet header */
	FormCCSDSPacketHeader(TASK_HEALTH_M_ID, 0, sizeof(Task_Health_M));

	/* Emit the packet */
	EmitCCSDSPacket((void *)&task_health, sizeof(Task_Health_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::SendChannelHealth()
{

	int32 lcv;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{

		/* Form the packet */
		FormCCSDSPacketHeader(CHANNEL_HEALTH_M_ID, 0, sizeof(Channel_Health_M));

		/* Emit the packet */
		EmitCCSDSPacket((void *)&channel_health[lcv], sizeof(Channel_Health_M));
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::SendFIFO()
{

	/* Form the packet header */
	FormCCSDSPacketHeader(FIFO_M_ID, 0, sizeof(FIFO_M));

	/* Emit the packet */
	EmitCCSDSPacket((void *)&fifo_status, sizeof(FIFO_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::SendSPS()
{


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::SendClock()
{


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::SendEKF()
{


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Serial_Telemetry::EmitCCSDSPacket(void *_buff, uint32 _len)
{

	uint32 lcv, bwrote;
	uint8 *sbuff;
	char header[9] = "AAAAAAAA";

	if(serial)
	{
		/* Write the sync header to the UART */
		for(lcv = 0; lcv < 8; lcv++)
			//NU_SD_Put_Char(header[lcv], &sport);

		/* Write the CCSDS header to the UART */
		sbuff = (uint8 *)&pheader;
		for(lcv = 0; lcv < 6; lcv++)
		{
			//NU_SD_Put_Char(*sbuff, &sport);
			sbuff++;
		}

		/* Now write the body to the UART */
		sbuff = (uint8 *)_buff;
		for(lcv = 0; lcv < _len; lcv++)
		{
			//NU_SD_Put_Char(*sbuff, &sport);
			sbuff++;
		}
	}
	else
	{

		signal(SIGPIPE, lost_gui_pipe);

		/* Only write if the client is connected */
		if(npipe_open)
		{
			bwrote = write(npipe[WRITE], &header, 8*sizeof(char));  	//!< Stuff the preamble
			bwrote = write(npipe[WRITE], &pheader, sizeof(CCSDS_PH)); 	//!< Stuff the CCSDS header
			bwrote = write(npipe[WRITE], _buff, _len);					//!< Stuff the body
		}
	}

	/* Increment counter */
	packet_tic++;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/* Form the CCSDS packet header with the given _apid, sequence flag, and packet length */
void Serial_Telemetry::FormCCSDSPacketHeader(uint32 _apid, uint32 _sf, uint32 _pl)
{

	pheader.pid = (_apid + CCSDS_APID_BASE) & 0x7FF;
	pheader.psc = (_sf & 0x3) + ((packet_tic & 0x3FFF) << 2);
	pheader.pdl = _pl & 0xFFFF;

}
/*----------------------------------------------------------------------------------------------*/
