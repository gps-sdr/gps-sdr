/*! \file Serial_Telemetry.h
	Defines the class Serial_Telemetry
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

#ifndef SERIAL_TELEMETRY_H
#define SERIAL_TELEMETRY_H

#include "includes.h"

/*! \ingroup CLASSES
 *
 */
class Serial_Telemetry
{

	private:

		/* Default object variables */
		uint32 				execution_tic;	//!< Execution counter
		uint32 				start_tic;		//!< OS tic at start of function
		uint32 				stop_tic;		//!< OS tic at end of function
		pthread_t 			thread;			//!< For the thread
		pthread_mutex_t		mutex;			//!< Protect the following variable


		uint32				packet_tic;
		uint32				command_tic;

		int32				serial;			//!< false = use named pipe, true = use serial port

		int32 				fifo[2];		//!< Named pipe to import/export data with the GUI
		int32	  			npipe[2];		//!< Named pipe to import/export data with the GUI
		int32				npipe_open;		//!< Is this named pipe connected?

		int32				serial_open;	//!< Has the serial port been configured

		CCSDS_PH 			pheader;						//!< CCSDS Packet header
		CCSDS_CH 			cheader;						//!< CCSDS Command header

		Board_Health_M 		board_health;					//!< Board health message
		Task_Health_M		task_health;					//!< Task health message
		Channel_Health_M 	channel_health[MAX_CHANNELS]; 	//!< Channel health message

		SPS_M				sps;							//!< SPS message
		Clock_M				clock;							//!< Clock message
		SV_Position_M		sv_positions[MAX_CHANNELS];		//!< SV Positions
		Pseudorange_M 		pseudoranges[MAX_CHANNELS];		//!< Pseudoranges
		Measurement_M 		measurements[MAX_CHANNELS];		//!< Measurements

		Ephemeris_M			ephemeris;						//!< Ephemeris message
		Almanac_M			almanac;						//!< Almanac message
		Ephemeris_Status_M	ephemeris_status;				//!< Status of ephemeris
		FIFO_M				fifo_status;

		int32 active[MAX_CHANNELS];

	public:

		Serial_Telemetry(int32 _serial);
		~Serial_Telemetry();
		void Start();								//!< Start the thread
		void Stop();								//!< Stop the thread
		void Import();								//!< Get data into the thread
		void Export();								//!< Get data out of the thread
		void Lock();								//!< Lock the object's mutex
		void Unlock();								//!< Unlock the object's mutex
		uint32 GetExecTic(){return(execution_tic);};//!< Get the execution counter
		uint32 GetStartTic(){return(start_tic);};	//!< Get the Nucleus tic at start of function
		uint32 GetStopTic(){return(execution_tic);};//!< Get the Nucleus tic at end of function

		void OpenGUIPipe();
		void SetGUIPipe(bool _status);
		void ExportGUI();
		void OpenSerial();
		void ExportSerial();

		/* Object specific methods */
		void FormCCSDSPacketHeader(uint32 _apid, uint32 _sf, uint32 _pl); 	//!< Form the CCSDS packet header
		void EmitCCSDSPacket(void *_buff, uint32 _len);						//!< Emit a CCSDS packet
		void FormCCSDSCommandHeader();										//!< Form the CCSDS command header

		/* Types of output messages */
		void SendBoardHealth();						//!< Emit hardware health values
		void SendTaskHealth();						//!< Emit task health values
		void SendChannelHealth();					//!< Emit channel health
		void SendSPS();								//!< Emit a PVT
		void SendClock();							//!< Emit a Clock state
		void SendSVPosition();						//!< Send the SV Position
		void SendEKF();								//!< Emit a GEONS solution
		void SendMeasurement();						//!< Emit a raw measurement
		void SendPseudorange();						//!< Emit a pseudorange
		void SendEphemeris();						//!< Emit a decoded ephemeris
		void SendAlamanac();						//!< Emit a decoded almanac
		void SendCommandAck();						//!< Send a command acknowledge

};

#endif /* Serial_Telemetry_H */
