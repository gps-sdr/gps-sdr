/*----------------------------------------------------------------------------------------------*/
/*! \file ephemeris.h
//
// FILENAME: ephemeris.h
//
// DESCRIPTION: Define the ephemeris object.
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

#ifndef EPHEMERIS_H_
#define EPHEMERIS_H_

#include "includes.h"

/*! @ingroup CLASSES
	@brief The Ephemeris class takes the raw gps subframes (in binary) and parses out the decimal values.
	Note the class does not error check the dating being passed in, hence the frame should be
	VALID before being passed into the object.
*/
class Ephemeris : public Threaded_Object
{

	private:

		Ephemeris_M			ephemerides[MAX_SV];	//!< The decoded ephemerides
		Ephem_Data_S		ephem_data[MAX_SV];		//!< Raw binary data
		Almanac_M			almanacs[MAX_SV];		//!< The decoded almanacs
		Almanac_Data_S		almanac_data[MAX_SV];	//!< Raw binary data
		Almanac_Data_S		health_data[2];			//!< Store page 25 of subframes 4 and 5 for SV health info
		Almanac_Data_S		utc_data;				//!< Store page 18 of subframes 4 for UTC and Klobuchar info
		UTC_Parameter_S 	utc;					//!< Decode the UTC parameters
		Klobuchar_Model_S 	klobuchar;				//!< Deocde the Klobuchar ionospheric model
		Channel_2_Ephemeris_S ephem_packet;			//!< Data from channels
		Ephemeris_Status_M	output_s;				//!< Status to the telemetry
		int32 				iode_master[MAX_SV];	//!< IODE flags

	public:

		Ephemeris();
		~Ephemeris();
		void Import();							//!< Read data from channels
		void Start();							//!< Start the thread
		void Parse(int32 _sv);					//!< Parse data message into decimal values
		void ParseUTC();						//!< Parse the UTC values
		void ParseHealth();						//!< Parse page 24 of SF 4 and 5 for SV Health codes
		void ParsePage(int32 _sv_id);			//!< Parse almanac page
		void ClearEphemeris(int32 _sv);			//!< Dump an ephemeris
		void ClearAlmanac(int32 _sv);			//!< Dump an almanac
		void ClearUTC();						//!< Clear UTC & Klobuchar info
		void WriteEphemeris();					//!< Write ephemerides to a txt file
		void ReadEphemeris();					//!< Read ephemerides from the same txt file
		void WriteAlmanac();					//!< Write alamanacs to a YUMA file
		void ReadAlmanac();						//!< Write alamanacs from a YUMA file

		/* Accessors */
		Ephemeris_M getEphemeris(int32 _sv);	//!< Get this SV's ephemeris values
		Almanac_M getAlmanac(int32 _sv);		//!< Get this SV's almanac values
		UTC_Parameter_S getUTC();				//!< Return the utc structure
		Klobuchar_Model_S getKlobuchar();		//!< Return the Klobuchar structure
		int32 getIODE(int32 _sv);				//!< Get the current IODE for this SV
		int32 getEphemerisValidity(int32 _sv);	//!< Get the decoded status
		int32 getAlmanacValidity(int32 _sv);	//!< Get the decoded status
		uint32 getAlmanacHealth(int32 _sv);		//!< Get the health status
		Ephemeris_Status_M getEphemerisStatus();//!< Return all of the status

		void setEphemeris(Ephemeris_M *_e);		//!< Set an ephemeris
		void setAlmanac(Almanac_M *_a);			//!< Set an almanac

};

#endif
