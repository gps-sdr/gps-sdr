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

/* Enum the command #s, DO NOT REORDER, ONLY APPEND! */
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
	SET_ALMANAC_C_ID,
	SET_EPHEMERIS_C_ID,
	ACQ_CONFIG_C_ID,
	CLOCK_CONFIG_C_ID,
	LAST_C_ID
};

/*! \ingroup COMMANDS
	Reset the PVT
*/
typedef struct Reset_PVT_C
{
	int32 flag;		//!< Flag for no reason
} Reset_PVT_C;


/*! \ingroup COMMANDS
	Reset the EKF
*/
typedef struct Reset_EKF_C
{
	int32 flag;		//!< Flag for no reason
} Reset_EKF_C;


/*! \ingroup COMMANDS
	Dump the desired channel
*/
typedef struct Reset_Channel_C
{
	int32 chan;		//!< Channel #, or all if chan >= MAX_CHANNELS
} Reset_Channel_C;


/*! \ingroup COMMANDS
	Clear the given ephemeris
*/
typedef struct Reset_Ephemeris_C
{
	int32 sv;		//!< SV #, or all if sv >= NUM_CODES
} Reset_Ephemeris_C;


/*! \ingroup COMMANDS
	Clear the given almanac
*/
typedef struct Reset_Almanac_C
{
	int32 sv;		//!< SV #, or all if sv >= NUM_CODES
} Reset_Almanac_C;


/*! \ingroup COMMANDS
	Get a raw measurement
*/
typedef struct Get_Measurement_C
{
	int32 chan;		//!< Channel #, or all if chan >= MAX_CHANNELS, stop with chan < 0
} Get_Measurement_C;


/*! \ingroup COMMANDS
	Get a pseudorange
*/
typedef struct Get_Pseudorange_C
{
	int32 chan;		//!< Channel #, or all if chan >= MAX_CHANNELS, stop with chan < 0
} Get_Pseudorange_C;


/*! \ingroup COMMANDS
	Emit the given ephemerides
*/
typedef struct Get_Ephemeris_C
{
	int32 sv;		//!< SV #, or all if sv >= NUM_CODES || sv < 0
} Get_Ephemeris_C;


/*! \ingroup COMMANDS
	Emit the given almanac
*/
typedef struct Get_Almanac_C
{
	int32 sv;		//!< SV #, or all if sv >= NUM_CODES || sv < 0
} Get_Almanac_C;


/*! \ingroup COMMANDS
	Stuff the given almanac
*/
typedef struct Set_Almanac_C
{
	int32 sv;				//!< SV #
	Almanac_M almanac;		//!< Data to load
} Set_Almanac_C;


/*! \ingroup COMMANDS
	Stuff the given ephemeris
*/
typedef struct Set_Ephemeris_C
{
	int32 sv;				//!< SV #
	Ephemeris_M ephemeris;	//!< Data to load
} Set_Ephemeris_C;


/*! \ingroup COMMANDS
	Configure the acquisition
*/
typedef struct _Acquisition_Config_C
{
	int32 min_doppler;		//!< Minimum doppler
	int32 max_doppler;		//!< Maximum doppler
	int32 doppler_range;	//!< Doppler range to search when almanac is valid
	int32 acq_strong;		//!< 0 for off, 1 for on, 2 for on IF almanac is valid
	int32 acq_medium;		//!< 0 for off, 1 for on, 2 for on IF almanac is valid
	int32 acq_weak;			//!< 0 for off, 1 for on, 2 for on IF almanac is valid
} Acquisition_Config_C;


/*! \ingroup COMMANDS
	Stuff the Clock
*/
typedef struct _Clock_Config_C
{
	double second;			//!< Stuff the gps second
	double week;			//!< Stuff the gps week
} Clock_Config_C;

typedef union Union_C
{
	Reset_PVT_C			reset_pvt;
	Reset_EKF_C			reset_ekf;
	Reset_Channel_C		reset_channel;
	Reset_Ephemeris_C	reset_ephemeris;
	Reset_Almanac_C		reset_almanac;
	Get_Measurement_C	get_measurement;
	Get_Pseudorange_C	get_pseudorange;
	Get_Ephemeris_C		get_ephemeris;
	Get_Almanac_C		get_almanac;
	Set_Ephemeris_C		set_ephemeris;
	Set_Almanac_C		set_almanac;

} Union_C;

#endif /* COMMANDS_H_ */
