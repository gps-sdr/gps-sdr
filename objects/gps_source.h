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
#include "gn3s.h"

enum GPS_SOURCE_TYPE
{
	SOURCE_USRP_V1,
	SOURCE_USRP_V2,
	SOURCE_SIGE_GN3S,
	SOURCE_DISK_FILE,
};

/*! \ingroup CLASSES
 *
 */
class GPS_Source
{

	private:

		/* Options */
		Options_S opt;			//!< Options

		/* Generic variables */
//		int32 source_open;		//!< Is the source open?
		int32 source_type;		//!< Source type
		int32 sample_mode;		//!< Sample mode
		int32 leftover;			//!< Leftover bytes for USRP double buffering
		int32 bwrite;			//!< Bytes somthing something?
		int32 ms_count;			//!< Count the numbers of ms processed

		/* Tag overflows */
		time_t rawtime;
		struct tm * timeinfo;

		/* AGC Values */
		int32 agc_scale;		//!< To do the AGC
		int32 overflw;			//!< Overflow counter
		int32 soverflw;			//!< Overflow counter

		/* Data buffers */
		int8 gbuff[40919*2]; 	//!< Byte buffer for GN3S
		CPX buff[40932]; 		//!< Base buffer for GN3S/USRP
		CPX buff_out[10240]; 	//!< Output buffer @ 2.048 Msps
		CPX *buff_out_p; 		//!< Pointer to a spot in buff_out
		CPX dbuff[16384]; 		//!< Buffer for double buffering
		MIX gn3s_mix[10240];	//!< Mix GN3S to the same IF frequency
		int32 gdec[10240];		//!< Index array to filter & resample GN3S data to 2.048 Msps

		/* USRP V1 Handles */
		usrp_standard_rx *urx;
		db_dbs_rx *dbs_rx_a;
		db_dbs_rx *dbs_rx_b;

		/* USRP V2 Handles */

		/* SOURCE_SIGE_GN3S Handles */
		gn3s *gn3s_a;
		gn3s *gn3s_b;

		/* File handles */
		FILE *fp_a;
		FILE *fp_b;

	private:

		void Open_USRP_V1();		//!< Open the USRP Version 1
		void Open_USRP_V2();		//!< Open the USRP Version 2
		void Open_GN3S();			//!< Open the SparkFun GN3S Sampler
		void Open_File();			//!< Open the file
		void Close_USRP_V1();		//!< Close the USRP Version 1
		void Close_USRP_V2();		//!< Close the USRP Version 2
		void Close_GN3S();			//!< Close the SparkFun GN3S Sampler
		void Close_File();			//!< Close the file
		void Read_USRP_V1(ms_packet *_p);//!< Read from the USRP Version 1
		void Read_USRP_V2(ms_packet *_p);//!< Read from the USRP Version 2
		void Read_GN3S(ms_packet *_p);	//!< Read from the SparkFun GN3S Sampler
		void Read_File(ms_packet *_p);	//!< Read from a file
		void Resample_USRP_V1(CPX *_in, CPX *_out);
		void Resample_GN3S(CPX *_in, CPX *_out);

	public:

		GPS_Source(Options_S *_opt);	//!< Create the GPS source with the proper hardware type
		~GPS_Source();					//!< Kill the object
		void Read(ms_packet *_p);		//!< Read in a single ms of data
		int32 getScale(){return(agc_scale);}
		int32 getOvrflw(){return(overflw);}

};

#endif /* GPS_SOURCE_H_ */
