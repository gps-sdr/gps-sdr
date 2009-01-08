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

		uint32	execution_tic;					//!< Execution counter
		uint32	packet_tic;						//!< Received packets
		uint32	command_tic;					//!< Transmitted commands

		/* Sync to receiver packets */
		int32	message_sync;					//!< Are we synched to the packets
		uint32 	syncstate;						//!< Flag to indicate synchronization step 0 for nothing, 1 for preamble, 2 for CCSDS header, 3 for packet
		uint32 	syncword;						//!< Synchronize to the 0xAAAAAAAA preamble

		uint8 	*pheader;						//!< Pointer to header
		uint32 	header_bytes;					//!< Bytes left to read in header

		uint8 	*pmessage;						//!< Pointer to message
		uint32 	message_bytes;					//!< Bytes read in command
		uint32 	message_bytes_2_read;			//!< Total bytes in body of command


		/* Serial port flag */
		int32				serial;				//!< Serial or named pipe

		/* Named pipe */
		int32				npipe_open;			//!< Flag to see if the pipes are open
		int32				npipe[2];			//!< Read and write files

		/* Serial port */
		int32				spipe_open;			//!< Has the serial port been configured?
		int32				spipe;				//!< Serial port file
		struct termios		tty; 	     		//!< Port settings

		/* USB port */

		/* Headers for CCSDC packets */
		CCSDS_Packet_Header  packet_header;		//!< CCSDS Packet header
		CCSDS_Packet_Header  message_header;	//!< CCSDS Packet header
		CCSDS_Decoded_Header decoded_header;	//!< Decoded header
		int32 				 packet_count[LAST_M_ID+1];	//!< Count the packets
		int32				 byte_count;				//!< Count the bytes

		Union_M				message_body;				//!< Union for messages

		/* Buffer for commands */
		Union_C				 	command_body[COMMAND_BUFFER_DEPTH];		//!< Union for commands
		CCSDS_Packet_Header		command_header[COMMAND_BUFFER_DEPTH];	//!< CCSDS Command header
		CCSDS_Decoded_Header	decoded_command[COMMAND_BUFFER_DEPTH];	//!< Decoded header
		uint32					command_ready[COMMAND_BUFFER_DEPTH];	//!< Flag to indicate command needs to be sent
		uint32					command_sent[COMMAND_BUFFER_DEPTH];		//!< Flag to indicate command was transmitted
		uint32					command_ack[COMMAND_BUFFER_DEPTH];		//!< Flag to force pend until an ack from the receiver
		uint32					command_count[COMMAND_BUFFER_DEPTH];	//!< Store command_tic
		uint32					command_head;							//!< Command head
		uint32					command_tail;							//!< Command tail
		int32					command_free;							//!< Commands free

		Message_Struct			messages;					//!< Hold all the messages
		char buff[2048];									//!< Dump wasted data

		/* Logging variables */
		int32 logging_on;									//!< Control overall logging
		char filename[1024];								//!< Log to this file
		FILE *lfile;										//!< Pointer to log file
		int32 log_flag[LAST_M_ID];							//!< Control messages on/off

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
		uint32 GetCommandTic(){return(command_tic);};
		uint32 GetCommandFree(){return(command_free);};
		Message_Struct *GetMessages(){return(&messages);};	//!< Dump the messages

		void setIO(int32 _serial);							//!< Set IO, named pipe, serial, or usb

		/* Nondefault methods */
		void setPipe(bool _status);							//!< Signal handler
		void openSerial();									//!< Attempt to open the serial port
		void openPipe();									//!< Attempt to open the named pipe
		void openUSB()	;									//!< Attempt to open the usb
		void readGPS();										//!< Read from the receiver
		void writeGPS();									//!< Write to the receiver
		int32 Read(uint8 *_b);								//!< Read a byte from given interface
		int32 Write(void *_b, int32 _bytes);				//!< Write bytes to given interface
		void parseMessage();								//!< Parse the incoming message
		void stateZero();									//!< Search for syncword
		void stateOne();									//!< Download message header
		void stateTwo();									//!< Download message body
		void packetFailure();								//!< Loss of sync
		void formCommand(int32 _id, void *_p, bool _pend);	//!< Form a command, _pend will make the function pend until it is acked */
		void processAck();

		/* Control the logging */
		void logOn(int32 _apid, bool _on){log_flag[_apid] = _on;};
		int32 getLog(int32 _apid){return(log_flag[_apid]);};
		void logStart();
		void logStop();
		void logClear();
		void setLogFile(const char *_str);
		char *getLogFile(){return(&filename[0]);};

		void printChan(int32 _chan);
		void printPVT();
		void printTask();
		void printEKF();
		void printClock();
		void printPseudo(int32 _chan);

};

#endif /* GUI_Serial_H */
