/*----------------------------------------------------------------------------------------------*/
/*! \file telemetry.h
//
// FILENAME: telemetry.h
//
// DESCRIPTION: Defines the class Telemetry.
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

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include "includes.h"
//#include "ekf.h"				//!< The GEONS driver
//#include "idle.h"				//!< Idle CPU counter
#include "telemetry.h"			//!< Handle the 422 telemetry
//#include "patience.h"			//!< Service the watchdog timer
//#include "commando.h"			//!< Actually services the commands
#include "correlator.h"
#include "acquisition.h"		//!< Interact with and direct acquisition engine
#include "ephemeris.h"			//!< Decode almanac/ephemeris/utc
#include "sv_select.h"			//!< Maintain state of GPS constellation using almanac data
#include "pvt.h"				//!< Least squares PVT solution
//#include "pps.h"				//!< Control the PPS
#include "channel.h"			//!< Tracking loops

/*! @ingroup CLASSES
	@brief The Telemetry services the RS422 port.
 */
class Telemetry : public Threaded_Object
{

	private:

		/* Object specific variables */
		uint32 packet_tic;							//!< Count the number of messages
		uint32 command_tic;							//!< Count the number of commands
		uint32 export_messages;						//!< New data from PVT is here, now transmit
		uint32 export_ekf;							//!< New data from the EKF is here, now transmit
		uint32 export_tic;							//!< Count number of exports
		uint32 last_start_tic;						//!< Special variable to allow the telemetry to time itself
		uint32 last_stop_tic;						//!< Special variable to allow the telemetry to time itself
		uint32 missed_interrupts;					//!< Keep track of missed interrupts
		uint32 adc_values[SHU_SIGNALS];				//!< This must be kept track of
		uint32 msg_rates[LAST_M_ID+1];				//!< Control output of periodic messages, 0 == off, !0 == decimation rate
		void (Telemetry::*msg_handlers[LAST_M_ID+1])(void);	//!< Function pointers for periodic message handlers

		int32 fifo[2];
		int32 npipe[2];								//!< Named pipe and/or serial interface
		int32 npipe_open;							//!< Is the named pipe open!?

		CCSDS_Packet_Header  packet_header;			//!< CCSDS Packet header
		CCSDS_Packet_Header  command_header;		//!< CCSDS Command header
		CCSDS_Decoded_Header decoded_header;		//!< Decoded header

		Command_Union command_body;					//!< Body of a command
		Message_Union message_body;					//!< Body of a message
		SV_Prediction_M sv_predictions[MAX_SV];		//!< Buffer SV predictions

		/* Variables for serial input */
		uint32 syncstate;							//!< Flag to indicate synchronization step 0 for nothing, 1 for preamble, 2 for CCSDS header, 3 for packet
		uint32 syncword;							//!< Synchronize to the 0xAAAAAAAA preamble

		uint8 *pheader;								//!< Pointer to header
		uint32 header_bytes;						//!< Bytes left to read in header

		uint8 *pcommand;							//!< Pointer to command
		uint32 command_bytes;						//!< Bytes read in command
		uint32 command_bytes_2_read;				//!< Total bytes in body of command

		uint32 bytes_read;							//!< Count number of received bytes
		uint32 bytes_sent;							//!< Count number of transmitted bytes

		PVT_2_TLM_S pvt_s;							//!< Receive data from PVT
		PPS_2_TLM_S	pps_s;							//!< Receive data from PPS
		EKF_2_TLM_S	ekf_s;							//!< Receive data from EKF
		SVS_2_TLM_S svs_s;							//!< Receive data from SV Select

	public:

		/* Default object methods */
		Telemetry();								//!< Constructor
		~Telemetry();
		void OpenPipe();
		void OpenSerial();
		void ClosePipe();
		void Start();								//!< Start the thread
		void Import();								//!< Get data into the thread
		void Export();								//!< Get data out of the thread
		void ImportPVT();							//!< Get data from the PVT
		void ImportEKF();							//!< Get data from the EKF
		void ImportCommando();						//!< Bent pipe info from Commando

		void ImportSerial();						//!< Parse on serial input
		void StateZero();							//!< State zero, waiting for 0xAAAAAAAA preamble
		void StateOne();							//!< State one, collecting CCSDS header
		void StateTwo();							//!< State two, collecting body of command

		uint32 ReadSHU(uint32 _shu);				//!< Read the status and health A/D

		/* Object specific methods */
		//void setSerial(NU_SERIAL_PORT *_sport){sport = _sport;};	//!< Grab the serial pointer
		void EmitCCSDSPacket(void *_buff, int32 _len);				//!< Emit a CCSDS packet

		/* Types of output messages */
		void SendBoardHealth();						//!< Emit hardware health values
		void SendTaskHealth();						//!< Emit task health values
		void SendChannelHealth();					//!< Emit channel health
		void SendSPS();								//!< Emit a PVT
		void SendClock();							//!< Emit a Clock state
		void SendTOT();								//!< Emit Time of Tone (TOT) message
		void SendPseudoranges();					//!< Emit pseudoranges
		void SendMeasurements();					//!< Emit measurements
		void SendSVPositions();						//!< Emit SV positions
		void SendSVPredictions();					//!< Emit SV predictions
		void SendPPS();								//!< Emit pps state
		void SendEKFState();						//!< Emit EKF state
		void SendEKFResidual();						//!< Emit EKF residuals
		void SendEKFCovariance();					//!< Emit EKF covariance
		void setMessageRate(uint32 _mid, uint32 _val);	//!< Set the message rate
		uint32 getMessageRate(uint32 _mid); 			//!< Get the message rate

};

#endif /* TELEMETRY_H */
