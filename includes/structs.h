/*! \file Structs.h
	Default includes for each source file in the project.
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


#ifndef STRUCTS_H_
#define STRUCTS_H_


//!< IF data format
/*----------------------------------------------------------------------------------------------*/
/*! \ingroup STRUCTS
 * Format of IF data
 */
typedef struct CPX {

	int16 i;	//!< Inphase (real)
	int16 q;	//!< Quadrature (imaginary)

} CPX;


/*! \ingroup STRUCTS
 * Format of complex accumulations
 */
typedef struct CPX_ACCUM {

	int32 i;	//!< Inphase (real)
	int32 q;	//!< Quadrature (imaginary)

} CPX_ACCUM;


/*! \ingroup STRUCTS
 *
 */
typedef struct MIX {

	int16 i;	//!< Inphase (real)
	int16 nq;	//!< Quadrature (imaginary)
	int16 q;	//!< Quadrature (imaginary)
	int16 ni;	//!< Inphase (real)

} MIX;
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
/*! \ingroup STRUCTS
 * Options parsed from command line to start the receiver
 */
typedef struct _Options_S
{

	int32	verbose;					//!< Do a lot of extra printing
	int32	realtime;					//!< Run off of USRP
	int32	post_process; 				//!< Use recorded data
	int32 	ocean;						//!< Do reflected waveforms
	int32 	log_channel;				//!< Log low-level tracking loop info
	int32	log_nav;					//!< Log nav solution output to disk
	int32	log_decimate;				//!< Decimate log output by this factor
	int32	google_earth;				//!< Output .klm file for google earth
	int32	ncurses;					//!< Use ncurses display
	int32	doppler_min;				//!< Set minimum Doppler
	int32	doppler_max;				//!< Set maximum Doppler
	int32	corr_sleep;					//!< How long to correlators should sleep
	int32	startup;					//!< Startup warm/cold
	int32	gui;						//!< Run with the GUI program (disables ncurses)
	int32	serial;						//!< Output telemetry over the serial port (disables ncurses)
	int32	usrp_internal;				//!< Run usrp-gps as a child process of receiver
	char	filename_direct[1024];		//!< Skyview filename
	char	filename_reflected[1024];	//!< Reflected filename

} Options_S;
/*----------------------------------------------------------------------------------------------*/

//!< FIFO structure for linked list?
/*----------------------------------------------------------------------------------------------*/
/*! \ingroup STRUCTS
 *  linked list structure for circular FIFO buffer
 */
typedef struct ms_packet {

	ms_packet *next;
	int32 measurement;				//!< This packet is flagged for a measurement
	int32 count;					//!< number of packets
	int32 accessed[MAX_CHANNELS+1];	//!< keep track of accesses
	CPX data[SAMPS_MS];				//!< payload size

} ms_packet;
/*----------------------------------------------------------------------------------------------*/


//!< Correlator and channel structs
/*----------------------------------------------------------------------------------------------*/
/*! \ingroup STRUCTS
 * Feedback applied from channel object to correlator object
 */
typedef struct _NCO_Command
{

	double carrier_nco;	//!< New carr_freq
	double code_nco;	//!< New code_freq
	int32 kill;			//!< Stop or start the channel
	int32 reset_1ms;	//!< Reset the 1ms counter
	int32 reset_20ms;	//!< Reset the 20ms counter
	int32 set_z_count;	//!< Set the z count
	int32 z_count;		//!< Actual value
	int32 length;		//!< Integrate for this many ms
	int32 navigate;		//!< Use this correlator to navigate

} NCO_Command_S;


/*! \ingroup STRUCTS
 * Raw correlation values, early, prompt, and late
 */
typedef struct _Correlation_S
{

	int32 I[3];
	int32 Q[3];

} Correlation_S;


/*! \ingroup STRUCTS
 * Hold state information of the correlator
 */
typedef struct _Correlator_State_S
{

	int32	sv;
	int32	navigate;			//!< Is this correlator sending out valid measurements
	int32	active;				//!< Active flag
	int32  	count;				//!< How long has this been active (ms)
	int32   scount;				//!< Number of samples in current accumulation
	double 	code_phase; 		//!< Code phase (chips)
	double 	carrier_phase;		//!< Carrier phase (cycles)
	double  carrier_phase_prev;	//!< Used for phase correction to correlations
	double 	code_phase_mod;		//!< Code phase (chips), mod 1023
	double 	carrier_phase_mod;	//!< Carrier phsae (cycles), mod 1
	double 	code_nco;			//!< Code NCO
	double 	carrier_nco;		//!< Carrier NCO
	int32  	_1ms_epoch;			//!< _1ms_epoch
	int32  	_20ms_epoch;		//!< _20ms_epoch
	int32 	_z_count;			//!< Keep track of the z count
	int32  	rollover;			//!< rollover point of C/A code in next ms packet
	int32	cbin[3];			//!< Code bins
	int32	sbin;				//!< Carriers bins
	MIX		*pcode[3];			//!< pointer to early-prompt-late codes
	CPX		*psine;				//!< pointer to Doppler removal vector
	int32	nav_history[MEASUREMENT_DELAY]; //!< keep track of the navigate flag

} Correlator_State_S;


/*! \ingroup STRUCTS
 * State of phase lock loop
 */
typedef struct _Phase_lock_loop {

	float PLLBW;				//!< PLL Bandwidth (Hz)
	float FLLBW;				//!< FLL Bandwidth (Hz)
	float a3;
	float b3;
	float w0p;
	float w0p2;
	float w0p3;
	float a2;
	float w0f;
	float w0f2;
	float gain;
	float w;					//!< Acceleration accumulator
	float x;					//!< Velocity accumulator
	float z;					//!< P feedback
	float pll_lock;				//!< PLL Lock Indicator
	float fll_lock;				//!< FLL lock indicator
	float t;					//!< Integration length (ms)
	int32 fll_lock_ticks;		//!< Is the fll locked?

} Phase_lock_loop;


/*! \ingroup STRUCTS
 * State of delay lock loop
 */
typedef struct _Delay_lock_loop
{

	float DLLBW;				//!< Bandwidth
	float x;					//!< Velocity accumulator
	float z;					//!< Proportional feedback
	float a;					//!< Acceleration accumulator
	float w0;					//!< w0
	float w02;					//!< w0^2
	float t;					//!< Integration length

} Delay_lock_loop;



/*! \ingroup STRUCTS
 * Raw subframes sent from channel to ephemeris object
 */
typedef struct _Chan_2_Ephem_S {

	int32 sv;					//!< SV number
	int32 subframe;				//!< Subframe number
	uint32 word_buff[FRAME_SIZE_PLUS_2]; //!< Raw binary values

} Chan_2_Ephem_S;
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/


/*! \ingroup STRUCTS
	Raw ephemeris struct
*/
typedef struct _Ephem_Data_S
{

	uint32 subframe_1[FRAME_SIZE_PLUS_2];	//!< Subframe 1
	uint32 subframe_2[FRAME_SIZE_PLUS_2];	//!< Subframe 2
	uint32 subframe_3[FRAME_SIZE_PLUS_2];	//!< Subframe 3
	uint32 subframe_4[FRAME_SIZE_PLUS_2];	//!< Subframe 4
	uint32 subframe_5[FRAME_SIZE_PLUS_2];	//!< Subframe 5
	bool   valid[5];						//!< Good subframes, at determined by the Channel object


} Ephem_Data_S;

/*! \ingroup STRUCTS
	Raw almanac struct
*/
typedef struct _Almanac_Data_S
{

	uint32 page[FRAME_SIZE_PLUS_2];		//!< Store raw binary for all the data
	bool   good_page;					//!< Mark whether the page is good or not

} 	Almanac_Data_S;
/*----------------------------------------------------------------------------------------------*/


/* Structs associated with SV_Select and Acquisition object */
/*----------------------------------------------------------------------------------------------*/
/*! \ingroup STRUCTS
 * Get the result of an acquisition
 */
typedef struct _Acq_Result_S
{

	int32 count;		//!< Packet tag
	int32 chan;			//!< The channel this SV will be assigned to
	int32 sv;			//!< SV number
	int32 type;			//!< Strong, medium, or weak
	int32 success;		//!< Did we declare detection?
	int32 antenna;		//!< antenna number
	float delay;		//!< Delay in chips
	float doppler;		//!< Doppler in Hz
	float magnitude;	//!< Magnitude
	float nf;			//!< Noise floor
	float snr;			//!< SNR in dB

} Acq_Result_S;


/*! \ingroup STRUCTS
 * Informs the acquisition in what mode to perform the next acquisition
 */
typedef struct _Acq_Request_S
{

	int32 corr;			//!< which correlator
	int32 count;		//!< packet tag
	int32 state;		//!< request started, IF data collected, request complete
	int32 sv;			//!< look for this SV
	int32 mindopp;		//!< minimum Doppler
	int32 maxdopp;		//!< maximum Doppler
	int32 type;			//!< type (STRONG/MEDIUM/WEAK)
	int32 antenna;		//!< antenna number

} Acq_Request_S;


/*! \ingroup STRUCTS
 * The predicted state of an SV via the almanac
 */
typedef struct _Acq_Predicted_S
{

	int32 sv;					//!< SV number
	int32 visible;				//!< Should the SV be visible?
	int32 tracked;				//!< Is it being tracked?
	float elev;					//!< Predicted elev (degrees)
	float azim;					//!< Predicted azim (degrees)
	float v_elev;				//!< Elevation of vehicle relative to SV
	float v_azim;				//!< Azimuth of vehicle relative to SV
	float delay;				//!< Predicted delay (seconds)
	float doppler;				//!< Predicted Doppler (Hz)
	float doppler_rate;			//!< Predicted Doppler rate (Hz/sec)

} Acq_Predicted_S;


/*! \ingroup STRUCTS
 * Keep track of latest acquisition attempts
 */
typedef struct _Acq_History_S
{

	int32 sv;			//!< SV number
	int32 type;			//!< Last acquisiton was what type (STRONG, MEDIUM, WEAK)
	int32 antenna;		//!< Which antenna last acq was on
	int32 count[3];		//!< Successive counts for each type
	int32 failures[3];	//!< Number of failed acquisitions
	int32 attempts[3];	//!< Number of attempts
	int32 successes[3];	//!< Number of successes
	int32 mindopp;		//!< Mindopp
	int32 maxdopp;		//!< Maxdopp
	int32 magnitude;	//!< Last magnitude
	int32 doppler;		//!< Last doppler

} Acq_History_S;
/*----------------------------------------------------------------------------------------------*/


/* Structs associated with the telemetry object */
/*----------------------------------------------------------------------------------------------*/
/*! \ingroup STRUCTS
 *
 */
typedef Acq_Result_S Acq_2_Telem_S;


/*! \ingroup STRUCTS
 *
 */
typedef Channel_Health_M Trak_2_Telem_S;

/*! \ingroup STRUCTS
 * Information sent from PVT to SV_Select to drive warm and hot starts
 */
typedef struct _PVT_2_SV_Select_S
{

	SPS_M 			master_nav;
	Clock_M 		master_clock;

} PVT_2_SV_Select_S;


/*! \ingroup STRUCTS
 *
 */
typedef struct _SV_Select_2_Telem_S
{

	int32 type;
	int32 mode;
	float mask_angle;
	Acq_Predicted_S	sv_predicted[NUM_CODES];
	Acq_History_S	sv_history[NUM_CODES];

} SV_Select_2_Telem_S;

/*! \ingroup STRUCTS
 * Information sent from PVT to telemetry
 */
typedef struct _PVT_2_Telem_S
{

	SPS_M 			master_nav;
	Clock_M 		master_clock;
	SV_Position_M	sv_positions[MAX_CHANNELS];
	Pseudorange_M	pseudoranges[MAX_CHANNELS];
	Measurement_M	measurements[MAX_CHANNELS];

} PVT_2_Telem_S;
/*----------------------------------------------------------------------------------------------*/


#endif /* STRUCTS_H_ */
