/*----------------------------------------------------------------------------------------------*/
/*! \file commando.h
//
// FILENAME: commando.h
//
// DESCRIPTION: Define the class Commando
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


#ifndef COMMANDO_H_
#define COMMANDO_H_

#include "includes.h"
//#include "ekf.h"			//!< The EKF driver
#include "telemetry.h"		//!< Handle the 422 telemetry
#include "ephemeris.h"		//!< Decode almanac/ephemeris/utc
#include "sv_select.h"		//!< Maintain state of GPS constellation using almanac data
#include "pvt.h"			//!< Least squares PVT solution
//#include "pps.h"			//!< Control the PPS

/*! @ingroup CLASSES
	@brief The Commando class executes any commands. It does so by accessing the other threaded objects
 * within the receiver, and executing the appropriate class methods.
 */
class Commando : public Threaded_Object
{

	private:

 		CCSDS_Packet_Header  packet_header;			//!< CCSDS Packet header
		CCSDS_Decoded_Header decoded_header;		//!< Decoded header
		Command_Union command_body;						//!< Body of a command
		Message_Union message_body;						//!< Body of a message
		uint32 command_tic;							//!< Count the number of executed commands

		void (Commando::*cmd_handlers[LAST_C_ID+1])(void);	//!< Function pointers for command handlers

	public:

		/* Default object methods */
		Commando();									//!< Constructor
		~Commando();								//!< Destructor
		void Start();								//!< Start the thread
		void Import();								//!< Get data into the thread
		void Export();								//!< Get data out of the thread

		/* Object specific methods */
		void ParseCommand();						//!< Parse command body and execute proper servicing function
		void SendAck();								//!< Send ack of command

		void doNull();								//!< Service a bad command

		/* Reset stuff */
		void resetAll();							//!< Reset everything
		void resetPVT();							//!< Reset PVT
		void resetEKF();							//!< Reset EKF
		void resetChannel();						//!< Reset Channel
		void resetEphemeris();						//!< Clear an ephemeris
		void resetAlmanac();						//!< Clear an almanac
		void resetPPS();							//!< Clear the pps state
		void resetWatchdog();						//!< Cause a soft reset by witholding the watchdog service

		/* Control periodic message rates */
		void setPeriodic();							//!< Set decimation rate of periodic messages

		/* EEPROM servicing */
		void setEEPROM();							//!< Service an EEPROM burn command
		void getEEPROM();							//!< Dump a block of EEPROM
		void getEEPROMChksum();						//!< Compute and return an EEPROM checksum
		void PendEEPROM(uint32 _bank);				//!< Wait for the EEPROM's ready bit to go high
		void PageWrite(uint32 _bank, uint32 *_addr, uint32 *_data, uint32 _words); //!< Write a page in the EEPROM

		/* Memory servicing */
		void setMemory();							//!< Overwrite an area of memory
		void getMemory();							//!< Dump a block of memory
		void getMemoryChksum();						//!< Compute and return a memory checksum

		/* Ephemeris/almanac */
		void setEphemeris();						//!< Stuff a decoded ephemeris
		void setAlmanac();							//!< Stuff a decoded almanac
		void getEphemeris();						//!< Output a decoded ephemeris
		void getAlmanac();							//!< Output a decoded almanac
		void getEphemerisStatus();					//!< Get status of ephemeris decoding
		void getSVSelectStatus();					//!< Get status of sv select
		void getSVPrediction();						//!< Get predicted state of sv

		void EmitCCSDSPacket(void *_buff, uint32 _len);	//!< Dump packet header and body to Telemtry via pipe

};

#endif /* COMMANDO_H */
