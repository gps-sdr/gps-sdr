/*! \file commands.h
	Define commands used for RS422 interface
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

#ifndef COMMANDS_H
#define COMMANDS_H

#define CCSDS_APID_BASE	(0x0)	//!< The CCSDS APID Base number for our receiver

/* Enum the command #s */
enum CCSDS_COMMAND_IDS
{
	FIRST_C_ID,
	RESET_PVT_C_ID,
	RESET_EKF_C_ID,
	RESET_CHANNEL_C_ID,
	RESET_EPHEMERIS_C_ID,
	RESET_ALMANAC_C_ID,
	GET_MEASUREMENT_C_ID,
	GET_PSEUDORANGE_C_ID,
	GET_EPHEMERIS_C_ID,
	GET_ALMANAC_C_ID,
	LAST_C_ID
};

typedef struct Reset_PVT_C
{
	int32 flag;		//!< Flag for no reason
} Reset_PVT_C;

typedef struct Reset_EKF_C
{
	int32 flag;		//!< Flag for no reason
} Reset_EKF_C;

typedef struct Reset_Channel_C
{
	int32 chan;		//!< Channel #, or all if chan > MAX_CHANNELS
} Reset_Channel_C;

typedef struct Reset_Ephemeris_C
{
	int32 sv;		//!< SV #, or all if sv > NUM_CODES
} Reset_Ephemeris_C;

typedef struct Reset_Almanac_C
{
	int32 sv;		//!< SV #, or all if sv > NUM_CODES
} Reset_Almanac_C;

typedef struct Get_Measurement_C
{
	int32 chan;		//!< Channel #, or all if chan > MAX_CHANNELS
} Get_Measurement_C;

typedef struct Get_Pseudorange_C
{
	int32 chan;		//!< Channel #, or all if chan > MAX_CHANNELS
} Get_Pseudorange_C;

typedef struct Get_Ephemeris_C
{
	int32 sv;		//!< SV #, or all if sv > NUM_CODES
} Get_Ephemeris_C;

typedef struct Get_Almanac_C
{
	int32 sv;		//!< SV #, or all if sv > NUM_CODES
} Get_Almanac_C;

#endif /* COMMANDS_H_ */
