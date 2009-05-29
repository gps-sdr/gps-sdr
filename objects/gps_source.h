/*----------------------------------------------------------------------------------------------*/
/*! \file gps_source.h
//
// FILENAME: gps_source.h
//
// DESCRIPTION: Defines the GPS_Source class.
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

#ifndef GPS_SOURCE_H_
#define GPS_SOURCE_H_

#include "includes.h"
#include "db_dbs_rx.h"

enum GPS_SOURCE_TYPE
{
	USRP_V1,
	USRP_V2,
	SIGE_GN3S
};

class GPS_Source
{

	private:

		/* Generic variables */
		int32 source_open;
		int32 source_type;
		int32 sample_mode;
		int32 leftover;
		int32 bwrite;
		CPX buff[16384]; 		//!< Base buffer
		CPX buff_out[4096]; 	//!< Output buffer
		CPX db_a[16384]; 		//!< Buffer for double buffering
		Options_S opt;			//!< Options

		/* USRP V1 Handles */
		usrp_standard_rx *urx;
		db_dbs_rx *dbs_rx_a;
		db_dbs_rx *dbs_rx_b;

		/* USRP V2 Handles */

		/* SIGE_GN3S Handles */

		/* File handles */


	private:

		void Read_USRP_V1();		//!< Read from the USRP Version 1
		void Read_USRP_V2();		//!< Read from the USRP Version 2
		void Read_GN3S();			//!< Read from the SparkFun GN3S Sampler
		void Read_File();			//!< Read from a file
		void Open_USRP_V1();		//!< Open the USRP Version 1
		void Open_USRP_V2();		//!< Open the USRP Version 2
		void Open_GN3S();			//!< Open the SparkFun GN3S Sampler
		void Open_File();			//!< Open the file
		void Close_USRP_V1();		//!< Close the USRP Version 1
		void Close_USRP_V2();		//!< Close the USRP Version 2
		void Close_GN3S();			//!< Close the SparkFun GN3S Sampler
		void Close_File();			//!< Close the file
		void Resample_USRP_V1(CPX *_in, CPX *_out);

	public:
			GPS_Source(Options_S *_opt);	//!< Create the GPS source with the proper hardware type
			~GPS_Source();					//!< Kill the object
			void Read(ms_packet *_p);		//!< Read in a single ms of data

};

#endif /* GPS_SOURCE_H_ */
