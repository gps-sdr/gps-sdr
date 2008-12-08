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

#define COMMANDO_BUFF_SIZE	(10240)

/*! \ingroup CLASSES
 *
 */
class Serial_Telemetry : public Threaded_Object
{

	private:

		/* Default object variables */
		uint32				packet_tic;

		int32				serial;			//!< false = use named pipe, true = use serial port

		int32 				fifo[2];		//!< Named pipe to import/export data with the GUI
		int32	  			npipe[2];		//!< Named pipe to import/export data with the GUI
		int32				npipe_open;		//!< Is this named pipe connected?

		int32				serial_open;	//!< Has the serial port been configured?

		int32				stream[LAST_M_ID];				//!< Control the streaming messages

		CCSDS_Packet_Header  packet_header;					//!< CCSDS Packet header
		CCSDS_Packet_Header  command_header;				//!< CCSDS Command header
		CCSDS_Decoded_Header decoded_header;				//!< Decoded header

		Union_C				command_body;						//!< Body of the command
		uint8				commando_buff[COMMANDO_BUFF_SIZE];	//!< Bent pipe size

		Board_Health_M 		board_health;					//!< Board health message
		Task_Health_M		task_health;					//!< Task health message
		Channel_M 			channel[MAX_CHANNELS]; 			//!< Channel health message

		SPS_M				sps;							//!< SPS message
		Clock_M				clock;							//!< Clock message
		SV_Position_M		sv_positions[MAX_CHANNELS];		//!< SV Positions
		Pseudorange_M 		pseudoranges[MAX_CHANNELS];		//!< Pseudoranges
		Measurement_M 		measurements[MAX_CHANNELS];		//!< Measurements

		Ephemeris_M			ephemeris;						//!< Ephemeris message
		Almanac_M			almanac;						//!< Almanac message
		Ephemeris_Status_M	ephemeris_status;				//!< Status of ephemeris
		FIFO_M				fifo_status;

		Acq_Command_M		acq_command;
		SV_Select_2_Telem_S tSelect;

		int32 active[MAX_CHANNELS];

	public:

		Serial_Telemetry(int32 _serial);
		~Serial_Telemetry();
		void Start();								//!< Start the thread
		void Import();								//!< Get data into the thread
		void Export();								//!< Get data out of the thread

		void OpenGUIPipe();
		void SetGUIPipe(bool _status);
		void ExportGUI();
		void ImportGUI();
		void OpenSerial();
		void ExportSerial();
		void ImportSerial();

		/* Object specific methods */
		void EmitCCSDSPacket(void *_buff, uint32 _len);	//!< Emit a CCSDS packet

		/* Types of output messages */
		void SendBoardHealth();						//!< Emit hardware health values
		void SendTaskHealth();						//!< Emit task health values
		void SendChannelHealth();					//!< Emit channel health
		void SendSPS();								//!< Emit a PVT
		void SendClock();							//!< Emit a Clock state
		void SendSVPositions();						//!< Send the SV Position
		void SendFIFO();							//!< Send the FIFO status
		void SendEphemerisStatus();					//!< Decoded ephemerides, etc
		void SendSVPrediction();					//!< Send SV prediction
		void SendAcqCommand();						//!< Send a recent acquisition attempt
		void SendPseudoranges();					//!< Send the pseudoranges
		void SendMeasurements();					//!< Send the raw measurements

		void stopStream(int32 _id){stream[_id] = 0;};
		void startStream(int32 _id){stream[_id] = 1;};
};

#endif /* Serial_Telemetry_H */
