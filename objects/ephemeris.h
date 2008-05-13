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

#ifndef EPHEMERIS_H
#define EPHEMERIS_H

#include "includes.h"

class Ephemeris
{
	private:

		pthread_t 			thread;	 						//!< For the thread
		pthread_mutex_t		mutex;							//!< Protect the following variable
		Ephemeris_S			ephemerides[NUM_CODES];			//!< The decoded ephemerides
		Ephem_Data_S		ephem_data[NUM_CODES];			//!< Raw binary data
		Almanac_S			almanacs[NUM_CODES];			//!< The decoded almanacs
		Almanac_Data_S		almanac_data[NUM_CODES];		//!< Raw binary data
		Chan_2_Ephem_S		ephem_packet;
		Ephem_2_Telem_S		output_s;	
		int32 				iode_master[NUM_CODES];			//!< IODE flags

	public:

		Ephemeris();
		~Ephemeris();
		void Parse(int32 _sv);						//!< Parse data message into decimal values
		void ParsePage(int32 _sv_id);				//!< Parse almanac page		
		void Import();								//!< Read data from channels
		void Export();
		void Start();								//!< Start the thread
		void Stop();								//!< Stop the thread
		Ephemeris_S getEphemeris(int32 _sv);		//!< Get this SV's ephemeris values
		int32  		getIODE(int32 _sv);				//!< Get the current IODE for this SV
		void Lock();
		void Unlock();
		void ClearSV(int32 _sv);					//!< Dump an ephemeris (usually from a detected cross-correlation)
		void WriteEpehemeris();						//!< Write ephemerides to a txt file
		void ReadEphemeris();						//!< Read ephemerides from the same txt file
		void WriteAlmanac();						//!< Write alamanacs to a YUMA file
		void ReadAlmanac();						//!< Write alamanacs from a YUMA file

};

#endif // EPHEMERIS_H
