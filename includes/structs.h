/*----------------------------------------------------------------------------------------------*/
/*! \file structs.h
//
// FILENAME: structs.h
//
// DESCRIPTION: Basic internal structures
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

#ifndef STRUCTS_H_
#define STRUCTS_H_


/*! @ingroup STRUCTS
 *  @brief The boot log is used to indicate causes of soft resets, uses
	a linked list structure. */
typedef struct Boot_Log_S
{
	struct Boot_Log_S *next;	//!< Next log (NULL if this is the end)
	uint32 bank;		//!< Bank booted from
	uint32 reset;		//!< Rest code
} Boot_Log_S;


/*! @ingroup STRUCTS
 *  @brief Tell the PVT how many measurements are in the pipe */
typedef struct Preamble_2_PVT_S
{
	uint32  tic_measurement;
	uint32 	pps_accum;

} Preamble_2_PVT_S;


/*! @ingroup STRUCTS
 *  @brief Use an alias to maintain compatability, and naming conventions */
typedef Measurement_M Measurement_2_PVT_S;


/*! @ingroup STRUCTS
 *  @brief Raw subframes sent from channel to ephemeris object */
typedef struct Channel_2_Ephemeris_S {

	int32 sv;					//!< SV number
	int32 subframe;				//!< Subframe number
	uint32 word_buff[FRAME_SIZE_PLUS_2]; //!< Raw binary values

} Channel_2_Ephemeris_S;


/*! @ingroup STRUCTS
 *  @brief Raw ephemeris struct */
typedef struct Ephem_Data_S
{

	uint32 subframe_1[FRAME_SIZE_PLUS_2];	//!< Subframe 1
	uint32 subframe_2[FRAME_SIZE_PLUS_2];	//!< Subframe 2
	uint32 subframe_3[FRAME_SIZE_PLUS_2];	//!< Subframe 3
	uint32 subframe_4[FRAME_SIZE_PLUS_2];	//!< Subframe 4
	uint32 subframe_5[FRAME_SIZE_PLUS_2];	//!< Subframe 5
	uint32 valid[5];						//!< Good subframes, at determined by the Channel object


} Ephem_Data_S;


/*! @ingroup STRUCTS
 *  @brief Raw almanac struct */
typedef struct Almanac_Data_S
{

	uint32 page[FRAME_SIZE_PLUS_2];		//!< Store raw binary for all the data
	uint32 good_page;					//!< Mark whether the page is good or not

} 	Almanac_Data_S;


/*! @ingroup STRUCTS
 *  @brief UTC Parameters from the almanac */
typedef struct UTC_Parameter_S
{
	double	a0;		//!< Seconds
	double	a1;		//!< Seconds/second
	int32	dtls;	//!< Seconds
	uint32	tot;	//!< Seconds
	uint32	wnt;	//!< Weeks
	uint32	wnlsf;	//!< Weeks
	uint32	dn;		//!< Days
	int32	dtlsf;	//!< Seconds
	uint32  valid;
} UTC_Parameter_S;


/*! @ingroup STRUCTS
 *  @brief Klobuchar model from the almanac */
typedef struct Klobuchar_Model_S
{
	double a0;		//!< Seconds
	double a1;		//!< Sec/semi-circle
	double a2;		//!< Sec/(semi-circle^2)
	double a3;		//!< Sec/(semi-circle^3)
	double b0;		//!< Seconds
	double b1;		//!< Sec/semi-circle
	double b2;		//!< Sec/(semi-circle^2)
	double b3;		//!< Sec/(semi-circle^3)
	uint32 valid;
} Klobuchar_Model_S;


/*! @ingroup STRUCTS
 *  @brief Get the result of an acquisition */
typedef struct Acq_Command_S
{

	/* Command sent to acquisition */
	int32	sv;						//!< PRN number
	int32	chan;					//!< Assign to this channel
	int32	type;					//!< ACQ_STRONG=0, ACQ_MEDIUM=1, ACQ_WEAK=2, ACQ_FINE=3
	int32	mode;					//!< If hot/warm use cendopp instead of acquisition doppler
	int32	antenna;				//!< Antenna Select
	int32	evenodd;				//!< Even or odd set of 10ms blocks
	int32	mindopp;				//!< Minimum doppler
	int32	cendopp;				//!< Center doppler
	int32	maxdopp;				//!< Maximum doppler
	int32	success;				//!< Did the acq say the SV was detected?
	uint32	int_length;				//!< Total integration length
	int32	fft_ovrflw;				//!< Forward FFT overflows
	int32	ifft_ovrflw;			//!< Inverse FFT overflows
	uint32	fft_scale;				//!< Fordward FFT rank scaling
	uint32	ifft_scale;				//!< Inverse FFT rank scaling

	int32 	time;					//!< Time of acq in nucleus tics
	int32 	dt;						//!< Time since last weak acq
	int32	count;					//!< 1 ms ticks

	/* These are the actual results */
	int32	code_phase;				//!< Code phase
	int32	doppler;				//!< Doppler
	int32	accel;					//!< Accel
	uint32	magnitude;				//!< Magnitude of peak

	/* For starting up a channel */
	int32 	accum_len;				//!< 1 or 20 ms
	int32 	initial_state;			//!< Bit lock, etc etc

} Acq_Command_S;


/*! @ingroup STRUCTS
 *  @brief Configure the acquisition object */
typedef struct Acq_Config_S
{

	uint32 fft_debug;				//!< Setup the FFT debug register
	uint32 acq_debug;				//!< Setup the ACQ debug register
	uint32 weak_ranks;				//!< Weak rank scaling
	uint32 strong_ranks;			//!< Strong rank scaling
	uint32 weak_thresh;				//!< Weak threshold value
	uint32 strong_thresh;			//!< Strong threshold value

} Acq_Config_S;


/*! @ingroup STRUCTS
 *  @brief Configure the acquisition object */
typedef struct SV_Select_Config_S
{

	int32 warm_doppler;				//!< Warm acquisition Doppler range
	int32 weak_doppler;				//!< Weak Doppler range, plus-minus
	int32 strong_doppler;			//!< Strong Doppler range, plus-minus
	int32 weak_operation;			//!< 0 for off, 1 for warm only, 2 for warm and cold
	int32 strong_operation;			//!< 0 for off, 1 for warm only, 2 for warm and cold
	int32 weak_modulo;				//!< This many weaks per 32 strong

} SV_Select_Config_S;


/*! @ingroup STRUCTS
 *  @brief Drive the SV Select object */
typedef struct PVT_2_SVS_S
{
	SPS_M 	sps;
	Clock_M clock;
 } PVT_2_SVS_S;


 /*! @ingroup STRUCTS
  *  @brief Dump info from the PVT to the PPS */
typedef struct PVT_2_PPS_S
{
	SPS_M 	sps;
	Clock_M clock;
 } PVT_2_PPS_S;


 /*! @ingroup STRUCTS
  *  @brief Dump info from the PVT to the EKF */
typedef struct PVT_2_EKF_S
{
	SPS_M 	sps;
	Clock_M clock;
	Pseudorange_M pseudoranges[MAX_CHANNELS];
	Measurement_M measurements[MAX_CHANNELS];
} PVT_2_EKF_S;


/*! @ingroup STRUCTS
 *  @brief Dump info from the PVT to the Telemetry */
typedef struct PVT_2_TLM_S
{
	SPS_M 	sps;
	Clock_M clock;
	Pseudorange_M pseudoranges[MAX_CHANNELS];
	Measurement_M measurements[MAX_CHANNELS];
	SV_Position_M sv_positions[MAX_CHANNELS];
	TOT_M tot;
} PVT_2_TLM_S;


/*! @ingroup STRUCTS
 *  @brief Dump EKF state to the telemetry */
typedef struct EKF_2_TLM_S
{
	EKF_State_M 		state;
	EKF_Covariance_M 	covariance;
	EKF_Residual_M		residual;
} EKF_2_TLM_S;


/*! @ingroup STRUCTS
 *  @brief Dump EKF state to the sv select */
typedef EKF_2_TLM_S EKF_2_SVS_S;


/*! @ingroup STRUCTS
 *  @brief Dump PPS state to the telemetry */
typedef PPS_M PPS_2_TLM_S;


/*! @ingroup STRUCTS
 *  @brief Dump SV Select state to the telemetry */
typedef SV_Prediction_M SVS_2_TLM_S;


#endif /* STRUCTS_H_ */
