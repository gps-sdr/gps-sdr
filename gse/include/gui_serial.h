/*! \file GUI_Serial.h
	Defines the class GUI_Serial
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

#ifndef GUI_SERIAL_H
#define GUI_SERIAL_H

#include "gui.h"

#define COMMAND_BUFFER_DEPTH	(256)

/*! \ingroup CLASSES
 *
 */
class GUI_Serial : public Threaded_Object
{

	private:


	public:

		uint32 				execution_tic;					//!< Execution counter
		uint32				packet_tic;						//!< Received packets
		uint32				command_tic;					//!< Transmitted commands

		int32				message_sync;					//!< Are we synched to the packets
		uint32 				syncword;						//!< Sync to the 0xAAAAAAAA preamble
		uint32				postword;						//!< Word 0xBBBBBBBB after each message

		/* Serial port flag */
		int32				serial;							//!< Serial or named pipe

		/* Named pipe */
		int32				npipe_open;						//!< Flag to see if the pipes are open
		int32				npipe[2];						//!< Read and write files

		/* Headers for CCSDC packets */
		CCSDS_Packet_Header  packet_header;					//!< CCSDS Packet header
		CCSDS_Decoded_Header decoded_packet;				//!< Decoded header
		int32 				packet_count[LAST_M_ID + 1];	//!< Count the packets
		int32				byte_count;						//!< Count the bytes

		/* Buffer for commands */
		Command_Union		 	command_body;				//!< Union for commands
		CCSDS_Packet_Header		command_header;				//!< CCSDS Command header
		CCSDS_Decoded_Header	decoded_command;			//!< Decoded header
		uint32					command_ready;				//!< Flag to indicate command needs to be sent
		uint32					command_sent;				//!< Flag to indicate command was transmitted
		uint32					command_ack;				//!< Flag to indicate command was executed
		uint32					command_count;				//!< Used to detect a command ack

		Message_Struct			messages;					//!< Hold all the messages
		Message_Union			message_body;				//!< Union for messages

		char buff[2048];									//!< Dump wasted data

		/* Logging variables */
		int32 logging_on;									//!< Control overall logging
		char filepath[1024];								//!< Log to this path
		FILE *robsfile;										//!< Pointer to Rinex observation file
		FILE *rephemfile;									//!< Pointer to Rinex ephemeris file
		FILE *lfile;										//!< Pointer to log file
		FILE *gfile;										//!< Pointer to google earth file
		int32 gfile_end;									//!< Pointer to end of gfile
		int32 log_flag[LAST_M_ID + 20];						//!< Control messages on/off

	public:

		/* Default GUI_Serial methods */
		GUI_Serial();										//!< Constructor
		~GUI_Serial();										//!< Destructor
		void Start();										//!< Start the thread
		void Import();										//!< Get data into the thread
		void Export();										//!< Get data out of the thread

		uint32 GetPipeOpen(){return(npipe_open);};
		uint32 GetByteCount(){return(byte_count);};
		uint32 GetPacketTic(){return(packet_tic);};
		uint32 GetCommandTic(){return(messages.command_ack.command_tic);};
		Message_Struct *GetMessages(){return(&messages);};	//!< Dump the messages
		void FixDoubles(void *_b, int32 _num);
		void setIO(int32 _serial);
		bool CheckPacket(CCSDS_Decoded_Header *p);			//!< Make sure packet's ID and length match

		/* Nondefault methods */
		void setPipe(bool _status);							//!< Signal handler
		void openSerial();									//!< Attempt to open the serial port
		void openPipe();									//!< Attempt to open the named pipe
		void closePipe();
		void readGPS();										//!< Read from the receiver
		void writeGPS();									//!< Write to the receiver
		int Read(void *_b, int32 _bytes);					//!< Read bytes from given interface
		int Write(void *_b, int32 _bytes);					//!< Write bytes to given interface
		void formCommand(int32 _id, void *_p);

		/* Control the logging */
		void logOn(int32 _apid, bool _on){log_flag[_apid] = _on;};
		int32 getLog(int32 _apid){return(log_flag[_apid]);};
		void logStart();
		void logStop();
		void logClear();
		void setLogFile(const char *_str);
		char *getLogFile(){return(&filepath[0]);};

		void printPVT();
		void printClock();
		void printEKFState();
		void printEKFCovariance();
		void printEKFResidual();
		void printPPS();
		void printPseudo(int32 _chan);
		void printMeas(int32 _chan);
		void printChan(int32 _chan);
		void printSVPos(int32 _chan);
		void printSVPred(int32 _sv);
		void printTask();
		void printBoard();
		void printGoogleEarth();
		void printGoogleEarthHeader();
		void printGoogleEarthFooter();
		void printRinexObs();
		void printRinexObsHeader();
		void printRinexEphem();
		void printRinexEphemHeader();

		void pendCommand();									//!< Wait for a free command
		int32 peekCommand();								//!< Poll for a command

};

#endif /* GUI_Serial_H */
