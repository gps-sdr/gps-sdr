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

#ifndef EPHEMERIS_H_
#define EPHEMERIS_H_

#include "includes.h"

/*! \ingroup CLASSES
 *
 */
class Ephemeris : public Threaded_Object
{
	private:

		Ephemeris_M			ephemerides[NUM_CODES];			//!< The decoded ephemerides
		Ephem_Data_S		ephem_data[NUM_CODES];			//!< Raw binary data
		Almanac_M			almanacs[NUM_CODES];			//!< The decoded almanacs
		Almanac_Data_S		almanac_data[NUM_CODES];		//!< Raw binary data
		Chan_2_Ephem_S		ephem_packet;
		Ephemeris_Status_M	output_s;
		int32 				iode_master[NUM_CODES];			//!< IODE flags

	public:

		Ephemeris();
		~Ephemeris();
		void Import();							//!< Read data from channels
		void Export();							//!< Send stuff to the telemetry thread
		void Start();							//!< Start the thread
		void Parse(int32 _sv);					//!< Parse data message into decimal values
		void ParsePage(int32 _sv_id);			//!< Parse almanac page
		void ClearEphemeris(int32 _sv);			//!< Dump an ephemeris
		void ClearAlmanac(int32 _sv);			//!< Dump an almanac
		void WriteEphemeris();					//!< Write ephemerides to a txt file
		void ReadEphemeris();					//!< Read ephemerides from the same txt file
		void WriteAlmanac();					//!< Write alamanacs to a YUMA file
		void ReadAlmanac();						//!< Write alamanacs from a YUMA file
		Ephemeris_M getEphemeris(int32 _sv){return(ephemerides[_sv]);}	//!< Get this SV's ephemeris values
		Almanac_M getAlmanac(int32 _sv){return(almanacs[_sv]);}			//!< Get this SV's almanac values
		void setEphemeris(Ephemeris_M *_e);
		void setAlmanac(Almanac_M *_a);
		int32 getIODE(int32 _sv){return(iode_master[_sv]);}				//!< Get the current IODE for this SV
};

#endif // EPHEMERIS_H
