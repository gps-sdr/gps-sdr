/*! \file COMMANDO.h
	Defines the class COMMANDO
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

#ifndef COMMANDO_H
#define COMMANDO_H

#include "includes.h"

/*! \ingroup CLASSES
 *
 */
class Commando
{

	private:

		/* Default object variables */
		uint32 				execution_tic;	//!< Execution counter
		uint32 				start_tic;		//!< OS tic at start of function
		uint32 				stop_tic;		//!< OS tic at end of function
		pthread_t 			thread;			//!< For the thread
		pthread_mutex_t		mutex;			//!< Protect the following variable

		/* Nondefault variables */
		CCSDS_Packet_Header  packet_header;			//!< CCSDS Packet header
		CCSDS_Packet_Header  command_header;		//!< CCSDS Command header
		CCSDS_Decoded_Header decoded_header;		//!< Decoded header

		Union_C				command_body;			//!< Body of the command
		Union_M				message_buff;			//!< Transmit sheit over
		Command_Ack_M		command_ack;			//!< Dump the acknowledge
		uint32				command_tic;

	public:

		/* Default object methods */
		Commando();									//!< Constructor
		~Commando();								//!< Destructor
		void Start();								//!< Start the thread
		void Stop();								//!< Stop the thread
		void Import();								//!< Get data into the thread
		void Export();								//!< Get data out of the thread
		void Lock(){pthread_mutex_lock(&mutex);};	//!< Lock the object's mutex
		void Unlock(){pthread_mutex_unlock(&mutex);};//!< Unlock the object's mutex
		uint32 GetExecTic(){return(execution_tic);};//!< Get the execution counter
		uint32 GetStartTic(){return(start_tic);};	//!< Get the Nucleus tic at start of function
		uint32 GetStopTic(){return(execution_tic);};//!< Get the Nucleus tic at end of function

		/* Nondefault methods */
		void Send_Ack();
		void Reset_PVT();
		void Reset_EKF();
		void Reset_Channel();
		void Reset_Ephemeris();
		void Reset_Almanac();
		void Get_Measurement();
		void Get_Pseudorange();
		void Get_Ephemeris();
		void Get_Alamanc();

};

#endif /* COMMANDO_H */
