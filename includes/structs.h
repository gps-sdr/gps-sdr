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
typedef struct CPX {
	
	int16 i;	//!< Inphase (real)
	int16 q;	//!< Quadrature (imaginary)
	
} CPX;


typedef struct MIX {
	
	int16 i;	//!< Inphase (real)
	int16 nq;	//!< Quadrature (imaginary)
	int16 q;	//!< Quadrature (imaginary)
	int16 ni;	//!< Inphase (real)
	
} MIX;
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
typedef struct _Options_S
{
	
	int32	verbose;					//!< Do a lot of extra printing
	int32	realtime;					//!< Run off of USRP
	int32	post_process; 				//!< Use recorded data
	int32 	ocean;						//!< Do reflected waveforms 
	int32 	log_channel;				//!< Log low-level tracking loop info
	int32	log_nav;					//!< Log nav solution output to disk
	int32	ncurses;					//!< Use ncurses display
	int32	doppler_min;				//!< Set minimum Doppler
	int32	doppler_max;				//!< Set maximum Doppler
	char	filename_direct[1024];		//!< Skyview filename
	char	filename_reflected[1024];	//!< Reflected filename
	
} Options_S; 
/*----------------------------------------------------------------------------------------------*/

//!< FIFO structure for linked list?
/*----------------------------------------------------------------------------------------------*/
/*! linked list structure for circular buffer */

typedef struct ms_packet {
	
	ms_packet *next;
	int32 measurement;				//!< This packet is flagged for a measurement
	int32 count;					//!< number of packets
	int32 accessed[MAX_CHANNELS+1];	//!< keep track of accesses
	CPX data[SAMPS_MS];				//!< payload size
	
} ms_packet;
/*----------------------------------------------------------------------------------------------*/


//!< Options
/*----------------------------------------------------------------------------------------------*/
typedef struct _Options {
	
	bool realtime;			//!< Run realtime off of USRP
	bool post_process;		//!< Run off of a recorded file
	bool ocean;				//!< Produce reflected waveforms
	bool occultation;		//!< Produce occulatating measurements from open loop tracking
	char fname[1024];		//!< Recorded filename
	
} Options;
/*----------------------------------------------------------------------------------------------*/


//!< Acquisition results
/*----------------------------------------------------------------------------------------------*/
typedef struct _Acq_Result_S
{
	
	int32 count;		//!< packet tag
	int32 chan;			//!< The channel this SV will be assigned to
	int32 sv;			//!< SV number
	int32 type;			//!< Strong, medium, or weak
	int32 success;		//!< Did we declare detection?
	float delay;		//!< Delay in chips
	float doppler;		//!< Doppler in Hz
	float magnitude;	//!< Magnitude
	float nf;			//!< Noise floor
	float snr;			//!< SNR in dB
	
} Acq_Result_S;


typedef struct _Acq_Request_S
{
	
	int32 corr;			//!< which correlator
	int32 count;		//!< packet tag
	int32 state;		//!< request started, IF data collected, request compelete
	int32 sv;			//!< look for this SV
	int32 mindopp;		//!< minimum Doppler
	int32 maxdopp;		//!< maximum Doppler
	int32 type;			//!< type (STRONG/MEDIUM/WEAK)
	
} Acq_Request_S;
/*----------------------------------------------------------------------------------------------*/

//!< Correlator and channel structs
/*----------------------------------------------------------------------------------------------*/
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


typedef struct _Correlation_S
{
	
	int32 I[3];
	int32 Q[3];
	
} Correlation_S;


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
	CPX		*pcode[3];			//!< pointer to early-prompt-late codes
	CPX		*psine;				//!< pointer to Doppler removal vector
	int32	nav_history[MEASUREMENT_DELAY]; //!< keep track of the navigate flag
		
} Correlator_State_S;


typedef struct _Measurement_S
{
	
	double	code_time;			//!< The code time
	double 	code_phase; 		//!< Code phase (chips) 
	double 	carrier_phase;		//!< Carrier phase (cycles)
	double 	carrier_phase_prev;	//!< Carrier phase prev (cycles)
	double 	carrier_phase_prev_prev;	//!< Carrier phase prev prev (cycles)
	double 	code_phase_mod;		//!< Code phase (chips), mod 1023
	double 	carrier_phase_mod;	//!< Carrier phase (cycles), mod 1
	double 	code_nco;			//!< Code NCO
	double 	carrier_nco;		//!< Carrier NCO
	int32  	_1ms_epoch;			//!< _1ms_epoch
	int32  	_20ms_epoch;		//!< _20ms_epoch
	int32	_z_count;			//!< The z count
	int32 	navigate;			//!< This has been tagged as a good measurement
	int32	sv;					//!< For this sv
	int32	chan;				//!< For this channel
	int32 	count;				//!< Corresponds to this tic
	
} Measurement_S;


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


typedef struct _Delay_lock_loop
{

	float x;
	float z;
	float d1;
	float d2;

} Delay_lock_loop;


typedef struct _Chan_Packet_S
{
	
	float header;
	float chan;
	float sv;
	float bit_lock;
	float frame_lock;
	float subframe;
	float best_epoch;
	float count;
	float len;	
	float I[3];
	float Q[3];
	float I_avg;
	float Q_var;
	float P_avg;
	float CN0;
	float code_nco;
	float carrier_nco;
	float fll_lock;
	float pll_lock;
	float fll_lock_ticks;
	float w;
	float x;
	float z;	

} Chan_Packet_S;


typedef struct _Chan_2_Ephem_S {

	int32 sv;
	int32 subframe;
	uint32 word_buff[FRAME_SIZE_PLUS_2];

} Chan_2_Ephem_S;
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/*! \ingroup STRUCTS
	Decoded ephemeris struct
*/
typedef struct _Ephemeris_S
{

	int32	valid;					//!< 0=No valid data. 
	int32	tofxmission;			//!< Time of subframe 1 transmission, sec of week. 
	int32	tow;					//!< Truncated TOW count 
	int32	subframe_1_health;      //!< Subframe 1 health code. 
	int32	code_on_L2;             //!< Code on L2 flag. 
	int32	week_number;			//!< GPS week at time of subframe 1 reception. 
	int32	L2pdata;                //!< L2 P data flag. 
	int32	ura;                    //!< Satellite's URA code. 
	int32	iodc;                   //!< Issue of data, clock. 
	double	tgd;                    //!< Group delay parameter. 
	int32	tocwk;					//!< GPS week corresponding to toc. 
	double	toc;					//!< Reference time of clock data parameter set. 
	double	af0;					//!< Clock correction polynomial coefficient. 
	double	af1;					//!< Clock correction polynomial coefficient. 
	double	af2;					//!< Clock correction polynomial coefficient. 
	int32	iode;                   //!< Issue of data, ephemeris. 
	double	crs;					//!< Sine harmonic correction to orbital radius. 
	double	deltan;					//!< Mean motion delta from computed value. 
	double	m0;                     //!< Mean anomaly at TOE. 
	double	cuc;					//!< Cosine harmonic correction to orbital radius. 
	double	ecc;                    //!< Eccentricity. 
	double	cus;					//!< Sine harmonic corr to argument of latitude. 
	double	sqrta;                  //!< Square root of semimajor axis. 
	int32	toewk;                  //!< GPS week corresponding to toe. 
	double	toe;					//!< Reference time of ephemeris data set. 
	int32	fti;                    //!< Fit interval. 
	double	cic;					//!< Cosine harmonic corr to inclination. 
	double	om0;                    //!< Right ascension at TOE. 
	double	cis;					//!< Sine harmonic corr to inclination. 
	double	in0;                    //!< Inclination at TOE. 
	double	crc;					//!< Cosine harmonic correction to orbital radius. 
	double	argp;                   //!< Argument of perigee at TOE. 
	double	omd;                    //!< Rate of right ascension. 
	double	idot;                   //!< Rate of inclination. 
	double	a;						//!< Derived qty: a = sqrta**2. 
	double	n0;						//!< Derived qty: n0 = sqrt(GravConstant/(a*a*a)). 
	double	relativistic;			//!< Relativistic correction 
	int32	zcount;
	int32	sv;

} Ephemeris_S;


/*! \ingroup STRUCTS
	Decoded almanac struct
*/
typedef struct _Almanac_S
{

	uint32	decoded;				//!< Has this been decoded yet
	int32	health;					//!< Health code
	double	ecc;					//!< Eccentricity
	double	toa;					//!< Time of Almanac
	double	in0;					//!< Inclination
	double	omd;					//!< Rate of right ascention
	double	sqrta;					//!< Sqrt of Semi-Major Axis
	double	om0;					//!< Longitude of Ascending Node
	double	argp;					//!< Argument of perigee
	double	m0;						//!< Mean Anomaly
	double	af0;					//!< Clock parameter 0
	double	af1;					//!< Clock parameter 1
	int32	week;					//!< Week number

} Almanac_S;

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

/* Navigator Structs */
/*----------------------------------------------------------------------------------------------*/
/*! \ingroup STRUCTS
	Raw PVT Navigation Solution
*/
typedef struct _Nav_Solution_S
 {

	double x;			//!< x in meters 
	double y;			//!< y in meters 
	double z;			//!< z in meters 
	
	double vx;			//!< vx in meters/sec 
	double vy;			//!< vy in meters/sec 
	double vz;			//!< vz in meters/sec 
	
	double time;		//!< time in seconds 
	double clock_bias;	//!< clock bias 
	double clock_rate;  //!< clock rate 
	double latitude;	//!< latitude in decimal degrees 
	double longitude;	//!< longitude in decimal degrees 
	double altitude;	//!< height in meters 
	
	double gdop;		//!< geometric dilution of precision 
	double pdop;		//!< position dilution of precision 
	double tdop;		//!< time dilution of precision 
	double hdop;		//!< hdop diultion of precision 
	double vdop;		//!< vertical dilution of precision 
	
	int32 converged;	//!< declare convergence 
	int32 tic;			//!< global_tic associated with this solution
	int32 nsvs;	
	int32 chanmap[MAX_CHANNELS];

} Nav_Solution_S;

/*! \ingroup STRUCTS
	Pseudorango structure, holds both time and meters
*/
typedef struct _Pseudorange_S
{
	
	double time;			//!< pseudorange in seconds 
	double time_rate;		//!< pseudorange rate in sec/sec 

	double meters;			//!< pseudorange in meters 
	double meters_rate;		//!< pseudorange rate in meters/sec 

	double residual;		//!< residual in meters 
	double time_uncorrected;//!< raw pseudorange measurements 

} Pseudorange_S;

/*! \ingroup STRUCTS
	Contains the SV position used for the PVT solution
*/
typedef struct _SV_Position_S
{
	
	double x;				//!< ECEF x coordinate (meters)
	double y;				//!< ECEF y coordinate (meters)
	double z;				//!< ECEF z coordinate (meters)
	double vx;				//!< ECEF x velocity (meters/sec)
	double vy;				//!< ECEF x velocity (meters/sec)
	double vz;				//!< ECEF x velocity (meters/sec)
	double elev;			//!< Satellite elevation (radians)
	double azim;			//!< Satellite azimuth (radians)
	double clock_bias;		//!< SV clock bias
	double frequency_bias;	//!< SV clock rate bias
	double transit_time;	//!< Time of flight from SV to receiver
	double time;			//!< Time used in SV position calculation

} SV_Position_S;


/*! \ingroup STRUCTS
	Contains the clock state
*/
typedef struct _Clock_S
{
	
	double receiver_time;		//!< Elapsed receiver time
	double rate;	 			//!< Clock rate
	double bias; 				//!< Clock bias
	double time0;				//!< Guess of gps second at initialization
	double time;				//!< Best estimate of GPStime
	double time_raw;			//!< Uncorrected time
	double week;				//!< GPS week
	uint32 state;				//!< Clock state	

} Clock_S;
/*----------------------------------------------------------------------------------------------*/


/* Structs associated with the telemetry object */
/*----------------------------------------------------------------------------------------------*/
typedef struct _FIFO_2_Telem_S
{
	
	int32 tic;
	int32 count;		//!< Number of 1 ms packets processed
	int32 head;			//!< Head pointer number
	int32 tail;			//!< Tail pointer number
	int32 agc_scale;	//!< Value used for AGC scale
	int32 overflw;		//!< Overflows in last ms
	int32 nactive;		//!< Number of channels to process the measurment packet
	 	
} FIFO_2_Telem_S;


/* Make an alias */
typedef _Acq_Result_S Acq_2_Telem_S;

typedef _Chan_Packet_S Trak_2_Telem_S;

typedef struct _Ephem_2_Telem_S 
{
	
	int32 valid[NUM_CODES];		//!< Valid ephemeris
	int32 iode[NUM_CODES];		//!< Corresponding IODE
	int32 avalid[NUM_CODES];	//!< Valid almanac
	
} Ephem_2_Telem_S;


typedef struct _PVT_2_Telem_S
{
	
	Nav_Solution_S 	master_nav;
	Clock_S 		master_clock;
	SV_Position_S	sv_positions[MAX_CHANNELS];
	Pseudorange_S	pseudoranges[MAX_CHANNELS];
	Measurement_S	measurements[MAX_CHANNELS];
	
} PVT_2_Telem_S;
/*----------------------------------------------------------------------------------------------*/

#endif /* STRUCTS_H_ */
