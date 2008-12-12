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
	RESET_ALL_C_ID,
	RESET_PVT_C_ID,
	RESET_EKF_C_ID,
	RESET_CHANNEL_C_ID,
	RESET_ALMANAC_C_ID,
	RESET_EPHEMERIS_C_ID,
	SET_PVT_C_ID,
	SET_ALMANAC_C_ID,
	SET_EPHEMERIS_C_ID,
	SET_ACQ_CONFIG_C_ID,
	GET_MEASUREMENT_C_ID,
	GET_PSEUDORANGE_C_ID,
	GET_ALMANAC_C_ID,
	GET_EPHEMERIS_C_ID,
	GET_ACQ_CONFIG_C_ID,
	GET_SV_PREDICTION_C_ID,
	GET_EPHEMERIS_VALID_C_ID,
	GET_BOARD_HEALTH_C_ID,
	GET_ACQ_COMMAND_C_ID,
	GET_SV_POSITION_C_ID,
	GET_CHANNEL_C_ID,
	LAST_C_ID
};

/*! \ingroup COMMANDS
	Dump everything (PVT/Ephemeris/Almanac/Channels/EKF)
*/
typedef struct Reset_All_C
{
	int32 command_id;	//!< Command identifier
	int32 flag;			//!< Flag for no reason
} Reset_All_C;

/*! \ingroup COMMANDS
	Reset the PVT
*/
typedef struct Reset_PVT_C
{
	int32 command_id;	//!< Command identifier
	int32 flag;			//!< Flag for no reason
} Reset_PVT_C;


/*! \ingroup COMMANDS
	Reset the EKF
*/
typedef struct Reset_EKF_C
{
	int32 command_id;	//!< Command identifier
	int32 flag;			//!< Flag for no reason
} Reset_EKF_C;


/*! \ingroup COMMANDS
	Dump the desired channel
*/
typedef struct Reset_Channel_C
{
	int32 command_id;	//!< Command identifier
	int32 chan;			//!< Channel #, or all if chan >= MAX_CHANNELS
} Reset_Channel_C;


/*! \ingroup COMMANDS
	Clear the given ephemeris
*/
typedef struct Reset_Ephemeris_C
{
	int32 command_id;	//!< Command identifier
	int32 sv;			//!< SV #, or all if sv >= NUM_CODES
} Reset_Ephemeris_C;


/*! \ingroup COMMANDS
	Clear the given almanac
*/
typedef struct Reset_Almanac_C
{
	int32 command_id;	//!< Command identifier
	int32 sv;			//!< SV #, or all if sv >= NUM_CODES
} Reset_Almanac_C;


/*! \ingroup COMMANDS
	Get a raw measurement
*/
typedef struct Get_Measurement_C
{
	int32 command_id;	//!< Command identifier
	int32 flag;			//!< On/off
} Get_Measurement_C;


/*! \ingroup COMMANDS
	Get a pseudorange
*/
typedef struct Get_Pseudorange_C
{
	int32 command_id;	//!< Command identifier
	int32 flag;			//!< On/off
} Get_Pseudorange_C;


/*! \ingroup COMMANDS
	Emit the given ephemerides
*/
typedef struct Get_Ephemeris_C
{
	int32 command_id;	//!< Command identifier
	int32 sv;			//!< SV #, or all if sv >= NUM_CODES || sv < 0
} Get_Ephemeris_C;


/*! \ingroup COMMANDS
	Emit the given almanac
*/
typedef struct Get_Almanac_C
{
	int32 command_id;	//!< Command identifier
	int32 sv;			//!< SV #, or all if sv >= NUM_CODES || sv < 0
} Get_Almanac_C;


/*! \ingroup COMMANDS
	Stuff the given almanac
*/
typedef struct Set_Almanac_C
{
	int32 command_id;		//!< Command identifier
	int32 sv;				//!< SV #
	Almanac_M almanac;		//!< Data to load
} Set_Almanac_C;


/*! \ingroup COMMANDS
	Stuff the given ephemeris
*/
typedef struct Set_Ephemeris_C
{
	int32 command_id;		//!< Command identifier
	int32 sv;				//!< SV #
	Ephemeris_M ephemeris;	//!< Data to load
} Set_Ephemeris_C;


/*! \ingroup COMMANDS
	Configure the acquisition
*/
typedef struct Set_Acq_Config_C
{
	int32 command_id;		//!< Command identifier
	Acq_Config_M acq_config;
} Set_Acq_Config_C;


/*! \ingroup COMMANDS
	Do a warm start.
*/
typedef struct Set_PVT_C
{
	int32 command_id;		//!< Command identifier
	double x;				//!< ECEF X position (meters)
	double y;				//!< ECEF Y position (meters)
	double z;				//!< ECEF Z position (meters)
	double vx;				//!< ECEF X velocity (meters/second)
	double vy;				//!< ECEF Y velocity (meters/second)
	double vz;				//!< ECEF Z velocity (meters/second)
	double second;			//!< GPS second
	double week;			//!< GPS week
} Set_PVT_C;


/*! \ingroup COMMANDS
	Emit an acquisition config message.
*/
typedef struct Get_Acq_Config_C
{
	int32 command_id;		//!< Command identifier
	int32 flag; 			//!< Not used
} Get_Acq_Config_C;


/*! \ingroup COMMANDS
	Emit a SV prediction from SV_Select
*/
typedef struct Get_SV_Prediction_C
{
	int32 command_id;		//!< Command identifier
	int32 sv; 				//!< SV $
} Get_SV_Prediction_C;


/*! \ingroup COMMANDS
	Emit ephemeris/acquisition decode status
*/
typedef struct Get_Ephemeris_Valid_C
{
	int32 command_id;		//!< Command identifier
	int32 flag; 			//!< Not used
} Get_Ephemeris_Valid_C;


/*! \ingroup COMMANDS
	Emit a board health message
*/
typedef struct Get_Board_Health_C
{
	int32 command_id;		//!< Command identifier
	int32 flag; 			//!< Not used
} Get_Board_Health_C;


/*! \ingroup COMMANDS
	Emit an acquisition result
*/
typedef struct Get_Acq_Command_C
{
	int32 command_id;	//!< Command identifier
	int32 sv; 			//!< SV #
} Get_Acq_Command_C;


/*! \ingroup COMMANDS
	Emit an acquisition result
*/
typedef struct Get_SV_Position_C
{
	int32 command_id;	//!< Command identifier
	int32 flag; 		//!< On/off
} Get_SV_Position_C;


/*! \ingroup COMMANDS
	Emit the channel status
*/
typedef struct Get_Channel_C
{
	int32 command_id;	//!< Command identifier
	int32 flag; 		//!< On/off
} Get_Channel_C;


typedef union Union_C
{
	Reset_All_C			reset_all;
	Reset_PVT_C			reset_pvt;
	Reset_EKF_C			reset_ekf;
	Reset_Channel_C		reset_channel;
	Reset_Ephemeris_C	reset_ephemeris;
	Reset_Almanac_C		reset_almanac;
	Set_PVT_C			set_pvt;
	Set_Almanac_C		set_almanac;
	Set_Ephemeris_C		set_ephemeris;
	Set_Acq_Config_C	set_acq_config;
	Get_Measurement_C	get_measurement;
	Get_Pseudorange_C	get_pseudorange;
	Get_Ephemeris_C		get_ephemeris;
	Get_Almanac_C		get_almanac;
	Get_Acq_Config_C	get_acq_config;
	Get_SV_Prediction_C 	get_sv_prediction;
	Get_Ephemeris_Valid_C 	get_ephemeris_valid;
	Get_Board_Health_C 		get_board_health;
	Get_Acq_Command_C		get_acq_command;
	Get_SV_Position_C		get_sv_position;
	Get_Channel_C			get_channel;
} Union_C;

#endif /* COMMANDS_H_ */
