/*----------------------------------------------------------------------------------------------*/
/*! \file commands.h
//
// FILENAME: commands.g
//
// DESCRIPTION: Define commands used for RS422 interface
//
// USED ON:  GPS Receiver named "Navigator"
//
// DEVELOPED BY:  GSFC Components and Hardware Systems Branch (Code 596)
//
// HISTORY: first used on RNS to fly on Hubble SM4 (STS125) Developed 2007,2008
//          elevated to Class B for use on GPM  development 2009-
//          added spinner capabilities for use on MMS  development 2009-
//          GPM and MMS variants concurrently developed.
//
// DEVELOPERS: Greg Heckler (2008-2009)  RNS,GPM,MMS (original development team)
//
// PROPRIETARY NOTICE:  Copyright (c) NASA GSFC 2008,2009.  All rights reserved.
//
// VERSION CONTROL:  This code is configuration managed using Subversion.
//      The Subversion server and tool is maintained as part of the FFTB.
//      The detailed change history is available within Subversion.
//
// Subversion Id: $Id$
//
// Note:  Comments within this file follow a syntax that is compatible with
//        DOXYGEN and are utilized for automated document extraction
//
// Reference:
*/
/*----------------------------------------------------------------------------------------------*/

#ifndef COMMANDS_H_
#define COMMANDS_H_

#define CCSDS_APID_BASE	(0x0)	//!< The CCSDS APID Base number for our receiver

/*! @ingroup COMMANDS
 *  @brief Enum the command #s, DO NOT REORDER, ONLY APPEND! */
enum CCSDS_COMMAND_IDS
{
	FIRST_C_ID,
	NULL_C_ID,
	RESET_ALL_C_ID,
	RESET_PVT_C_ID,
	RESET_EKF_C_ID,
	RESET_CHANNEL_C_ID,
	RESET_ALMANAC_C_ID,
	RESET_EPHEMERIS_C_ID,
	RESET_PPS_C_ID,
	RESET_WATCHDOG_C_ID,
	SET_PERIODIC_C_ID,
	SET_ALMANAC_C_ID,
	SET_EPHEMERIS_C_ID,
	SET_EEPROM_C_ID,
	SET_MEMORY_C_ID,
	GET_ALMANAC_C_ID,
	GET_EPHEMERIS_C_ID,
	GET_SV_PREDICTION_C_ID,
	GET_EPHEMERIS_STATUS_C_ID,
	GET_SV_SELECT_STATUS_C_ID,
	GET_EEPROM_C_ID,
	GET_EEPROM_CHKSUM_C_ID,
	GET_MEMORY_C_ID,
	GET_MEMORY_CHKSUM_C_ID,
	LAST_C_ID
};


/*! @ingroup COMMANDS
 *  @brief Enum the command acknowledge codes */
enum CCSDS_ACKNOWLEDGE_IDS
{
	SUCCESS_A_ID,			//!< Generic success
	FAILURE_A_ID,			//!< Generic failure
	INVALID_COMMAND_A_ID,	//!< Bad command ID
	INVALID_ADDRESS_A_ID, 	//!< Bad address
	INVALID_LENGTH_A_ID,	//!< Length invalid for given command
	INVALID_EEPROM_A_ID,	//!< Invalid eeprom choice
	INVALID_SV_A_ID,		//!< Invalid SV choice
	INVALID_CHECKSUM_A_ID,	//!< Failed checksum for an EEPROM burn
	INVALID_MESSAGE_A_ID,	//!< Invalid message id
	NULL_A_ID,				//!< Null command
};


/*! @ingroup COMMANDS
 *  @brief Null command, does nothing!
*/
typedef struct Null_C
{
	int32 command_id;	//!< Command identifier
	int32 flag;			//!< Flag for no reason
} Null_C;


/*! @ingroup COMMANDS
 *  @brief Dump everything (PVT/Ephemeris/Almanac/Channels/EKF)
*/
typedef struct Reset_All_C
{
	int32 command_id;	//!< Command identifier
	int32 flag;			//!< Flag for no reason
} Reset_All_C;

/*! @ingroup COMMANDS
 *  @brief Reset the PVT
*/
typedef struct Reset_PVT_C
{
	int32 command_id;	//!< Command identifier
	int32 flag;			//!< Flag for no reason
} Reset_PVT_C;


/*! @ingroup COMMANDS
 *  @brief Reset the EKF
*/
typedef struct Reset_EKF_C
{
	int32 command_id;	//!< Command identifier
	int32 flag;			//!< Flag for no reason
} Reset_EKF_C;


/*! @ingroup COMMANDS
 *  @brief Reset the desired channel
*/
typedef struct Reset_Channel_C
{
	int32 command_id;	//!< Command identifier
	int32 chan;			//!< Channel #, or all if chan >= MAX_CHANNELS
} Reset_Channel_C;


/*! @ingroup COMMANDS
 *  @brief Clear the given ephemeris
*/
typedef struct Reset_Ephemeris_C
{
	int32 command_id;	//!< Command identifier
	int32 sv;			//!< SV #, or all if sv >= NUM_CODES
} Reset_Ephemeris_C;


/*! @ingroup COMMANDS
 *  @brief Clear the given almanac
*/
typedef struct Reset_Almanac_C
{
	int32 command_id;	//!< Command identifier
	int32 sv;			//!< SV #, or all if sv >= NUM_CODES
} Reset_Almanac_C;


/*! @ingroup COMMANDS
 *  @brief Clear the PPS sate
*/
typedef struct Reset_PPS_C
{
	int32 command_id;	//!< Command identifier
	int32 flag;			//!< Flag for no reason
} Reset_PPS_C;


/*! @ingroup COMMANDS
 *  @brief Reset the board by witholding the watchdog
*/
typedef struct Reset_Watchdog_C
{
	int32 command_id;	//!< Command identifier
	int32 flag;			//!< Flag for no reason
} Reset_Watchdog_C;


/*! @ingroup COMMANDS
 *  @brief Control the rates of periodic messages
*/
typedef struct Set_Periodic_C
{
	int32 command_id;		//!< Command identifier
	uint32 message_id;		//!< Which message id should I set
	uint32 decimation;		//!< Decimation rate (0 for OFF)
} Set_Periodic_C;


/*! @ingroup COMMANDS
 *  @brief Stuff the given almanac
*/
typedef struct Set_Almanac_C
{
	int32 command_id;		//!< Command identifier
	int32 sv;				//!< SV #
	Almanac_M almanac;		//!< Data to load
} Set_Almanac_C;


/*! @ingroup COMMANDS
 *  @brief Stuff the given ephemeris
*/
typedef struct Set_Ephemeris_C
{
	int32 command_id;		//!< Command identifier
	int32 sv;				//!< SV #
	Ephemeris_M ephemeris;	//!< Data to load
} Set_Ephemeris_C;


/*! @ingroup COMMANDS
 *  @brief Burn a block (or subblock) of EEPROM, up to 256 bytes
*/
typedef struct Set_EEPROM_C
{
	int32 command_id;		//!< Command identifier
	int32 bank;				//!< 0 for EEPROM0, 1 for EEPROM 1
	int32 offset;			//!< Offset from base (dwords)
	int32 dwords;			//!< Number of 32 bit dwords
	uint32 checksum;		//!< Checksum of payload
	uint32 payload[64];		//!< Data to be written
} Set_EEPROM_C;


/*! @ingroup COMMANDS
 *  @brief Write an area of SRAM, up to 256 bytes
*/
typedef struct Set_Memory_C
{
	int32 command_id;		//!< Command identifier
	int32 address;			//!< Raw address
	int32 bytes;			//!< Number of bytes
	uint32 checksum;		//!< Checksum of payload
	uint8 payload[256];		//!< Data to be written
} Set_Memory_C;


/*! @ingroup COMMANDS
 *  @brief Emit the given almanac
*/
typedef struct Get_Almanac_C
{
	int32 command_id;	//!< Command identifier
	int32 sv;			//!< SV #, or all if sv >= NUM_CODES || sv < 0
} Get_Almanac_C;


/*! @ingroup COMMANDS
 *  @brief Emit the given ephemerides
*/
typedef struct Get_Ephemeris_C
{
	int32 command_id;	//!< Command identifier
	int32 sv;			//!< SV #, or all if sv >= NUM_CODES || sv < 0
} Get_Ephemeris_C;


/*! @ingroup COMMANDS
 *  @brief Emit a SV prediction from SV_Select
*/
typedef struct Get_SV_Prediction_C
{
	int32 command_id;		//!< Command identifier
	int32 sv; 				//!< SV #
} Get_SV_Prediction_C;


/*! @ingroup COMMANDS
 *  @brief Emit ephemeris/acquisition decode status
*/
typedef struct Get_Ephemeris_Status_C
{
	int32 command_id;		//!< Command identifier
	int32 flag; 			//!< Not used
} Get_Ephemeris_Status_C;


/*! @ingroup COMMANDS
 *  @brief Emit sv select status
*/
typedef struct Get_SV_Select_Status_C
{
	int32 command_id;		//!< Command identifier
	int32 flag; 			//!< Not used
} Get_SV_Select_Status_C;


/*! @ingroup COMMANDS
 *  @brief Get a block of EEPROM, up to 256 bytes
*/
typedef struct Get_EEPROM_C
{
	int32 command_id;		//!< Command identifier
	int32 bank;				//!< 0 for EEPROM0, 1 for EEPROM 1
	int32 offset;			//!< Offset from base (dwords)
	int32 dwords;			//!< Number of 32 bit dwords
} Get_EEPROM_C;


/*! @ingroup COMMANDS
 *  @brief Compute checksum of EEPROM at address
*/
typedef struct Get_EEPROM_Chksum_C
{
	int32 command_id;		//!< Command identifier
	int32 bank;				//!< 0 for EEPROM0, 1 for EEPROM 1
	int32 offset;			//!< Offset from base (dwords)
	int32 dwords;			//!< Number of 32 bit dwords
} Get_EEPROM_Chksum_C;


/*! @ingroup COMMANDS
 *  @brief Get a block of SRAM, up to 256 bytes
*/
typedef struct Get_Memory_C
{
	int32 command_id;		//!< Command identifier
	int32 address;			//!< Raw address
	int32 bytes;			//!< Number of bytes
} Get_Memory_C;


/*! @ingroup COMMANDS
 *  @brief Compute checksum of Memory at address
*/
typedef struct Get_Memory_Chksum_C
{
	int32 command_id;		//!< Command identifier
	int32 address;			//!< Raw address
	int32 bytes;			//!< Number of bytes
} Get_Memory_Chksum_C;


/*! @ingroup COMMANDS
 *  @brief Union containing all commands
*/
typedef union Command_Union
{
	Null_C				null;
	Reset_All_C			reset_all;
	Reset_PVT_C			reset_pvt;
	Reset_EKF_C			reset_ekf;
	Reset_Channel_C		reset_channel;
	Reset_Almanac_C		reset_almanac;
	Reset_Ephemeris_C	reset_ephemeris;
	Reset_PPS_C			reset_pps;
	Reset_Watchdog_C	reset_watchdog;
	Set_Periodic_C		set_periodic;
	Set_Almanac_C		set_almanac;
	Set_Ephemeris_C		set_ephemeris;
	Set_EEPROM_C		set_eeprom;
	Set_Memory_C		set_memory;
	Get_Almanac_C		get_almanac;
	Get_Ephemeris_C		get_ephemeris;
	Get_SV_Prediction_C 	get_sv_prediction;
	Get_Ephemeris_Status_C 	get_ephemeris_status;
	Get_SV_Select_Status_C 	get_sv_select_status;
	Get_EEPROM_C			get_eeprom;
	Get_EEPROM_Chksum_C		get_eeprom_chksum;
	Get_Memory_C			get_memory;
	Get_Memory_Chksum_C		get_memory_chksum;
} Command_Union;

#endif /* COMMANDS_H_ */
