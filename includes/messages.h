/*! \file messages.h
	Define messages used for RS422 interface
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

#ifndef MESSAGES_H
#define MESSAGES_H

#define CCSDS_APID_BASE	(0x0)	//!< The CCSDS APID Base number for our receiver

/* Enum the packet ID #s */
enum CCSDS_PACKET_IDS
{
	FIRST_M_ID,
	BOARD_HEALTH_M_ID,
	TASK_HEALTH_M_ID,
	CHANNEL_HEALTH_M_ID,
	SPS_M_ID,
	CLOCK_M_ID,
	SV_POSITION_M_ID,
	EKF_M_ID,
	MEASUREMENT_M_ID,
	PSEUDORANGE_M_ID,
	EPHEMERIS_M_ID,
	ALMANAC_M_ID,
	EPHEMERIS_VALID_M_ID,
	FIFO_M_ID,
	COMMAND_ACK_M_ID,
	LAST_M_ID
};


/*! \ingroup MESSAGES
 * Packet dumped to telemetry and to disk to keep track of each channel
 */
typedef struct CCSDS_Packet_Header
{

	uint16 pid;	//!< Packet ID
	uint16 psc;	//!< Packet sequence control
	uint16 pdl; //!< Packet data length

} CCSDS_Packet_Header;


/*! \ingroup MESSAGES
 * Decoded header
 */
typedef struct CCSDS_Decoded_Header
{
	uint32 id;
	uint32 type;
	uint32 tic;
	uint32 length;
} CCSDS_Decoded_Header;


/*! \ingroup MESSAGES
 * Packet dumped to telemetry and to disk to keep track of each channel
 */
typedef struct Board_Health_M
{

	//!< Grab FPGA versions
	uint32 trk_version;		//!< Tracking FPGA version
	uint32 acq_version;		//!< Acquisition FPGA version

	//!< DSA Info
	uint32 dsa0;			//!< Current state of RF DSA on RF chain 0
	uint32 dsa1;			//!< Current state of IF DSA on RF chain 0
	uint32 dsa2;			//!< Current state of RF DSA on RF chain 1
	uint32 dsa3;			//!< Current state of IF DSA on RF chain 1

	//!< LO lock indicator
	uint32 lo_locked;		//!< Is the LO (1.54 GHz downmix clock) locked to the synthesizer

	//!< Overflows on A/Ds
	uint32 ovrflw0;			//!< Overflow counter on A/D 0
	uint32 ovrflw1;			//!< Overflow counter on A/D 1
	uint32 ovrflw2;			//!< Overflow counter on A/D 2
	uint32 ovrflw3;			//!< Overflow counter on A/D 3

	//!< Acquisition SRAM
	uint32 sram_bad_mem;	//!< Debug info from Steve's POST
	uint32 sram_bad_hi;		//!< Debug info from Steve's POST
	uint32 sram_bad_lo;		//!< Debug info from Steve's POST

	//!< Missed interrupts
	uint32 missed_interrupts;//!< Missed interrupts

	//!< Software revision
	uint32 software_major;	//!< Major version
	uint32 software_minor;	//!< Minor version
	uint32 software_postfix;//!< Alpha, beta, or gold

} Board_Health_M;


/*! \ingroup MESSAGES
 * Packet dumped to telemetry and to disk to keep track of each channel
 */
typedef struct Task_Health_M
{

	uint32 execution_tic[MAX_TASKS];	//!< Execution counters
	uint32 start_tic[MAX_TASKS];		//!< Nucleus tic at function entry
	uint32 stop_tic[MAX_TASKS];			//!< Nucleus tic at function exit

} Task_Health_M;


/*! \ingroup MESSAGES
 * Packet dumped to telemetry and to disk to keep track of each channel
 */
typedef struct Channel_Health_M
{

	float chan;			//!< The channel number
	float state;		//!< channel's state
	float sv;			//!< SV/PRN number the channel is tracking
	float antenna;		//!< Antenna channel is tracking off of
	float len;			//!< acummulation length (1 or 20 ms)
	float w;			//!< 3rd order PLL state
	float x;			//!< 3rd order PLL state
	float z;			//!< 3rd order PLL state
	float code_nco;		//!< State of code_nco
	float carrier_nco;	//!< State of carrier_nco
	float CN0;			//!< CN0 estimate
	float p_avg;		//!< Filtered version of I^2+Q^2
	float bit_lock;		//!< Bit lock?
	float frame_lock;	//!< Frame lock?
	float navigate;		//!< Navigate on this channel flag
	float count;		//!< Number of accumulations that have been processed
	float subframe;		//!< Current subframe number
	float best_epoch;	//!< Best estimate of bit edge position

} Channel_Health_M;


/*! \ingroup MESSAGES
	Raw PVT navigation solution
*/
typedef struct SPS_M
 {

	double x;			//!< ECEF x coordinate (meters)
	double y;			//!< ECEF y coordinate (meters)
	double z;			//!< ECEF z coordinate (meters)
	double vx;			//!< ECEF x velocity (meters/sec)
	double vy;			//!< ECEF x velocity (meters/sec)
	double vz;			//!< ECEF x velocity (meters/sec)
	double ax;			//!< ECEF x acceleration (meters/sec/sec)
	double ay;			//!< ECEF x acceleration (meters/sec/sec)
	double az;			//!< ECEF x acceleration (meters/sec/sec)

	double time;		//!< time in seconds
	double clock_bias;	//!< clock bias in seconds
	double clock_rate;  //!< clock rate in meters/second
	double latitude;	//!< latitude in decimal radians
	double longitude;	//!< longitude in decimal radians
	double altitude;	//!< height in meters

	double gdop;		//!< geometric dilution of precision
	double pdop;		//!< position dilution of precision
	double tdop;		//!< time dilution of precision
	double hdop;		//!< hdop diultion of precision
	double vdop;		//!< vertical dilution of precision

	uint32 nsvs;		//!< This is a mask, not a number
	uint32 converged;	//!< declare convergence
	uint32 tic;			//!< global_tic associated with this solution

	uint32 stale_ticks;			//!< count the number of tics since the last good sltn
	uint32 converged_ticks;		//!< count number of converged tics
	uint32 nav_channels;		//!< count number of SVs used in last PVT estimation
	uint32 initial_convergence;	//!< Flag set ONCE if the first convergence has occured

	uint32 chanmap[MAX_CHANNELS];

} SPS_M;


/*! \ingroup MESSAGES
	Contains the clock state
*/
typedef struct Clock_M
{

	double receiver_time;		//!< Elapsed receiver time
	double rate;	 			//!< Clock rate
	double bias; 				//!< Clock bias
	double time0;				//!< Guess of gps second at initialization
	double time;				//!< Best estimate of GPStime
	double time_raw;			//!< Uncorrected time
	double week;				//!< GPS week
	uint32 state;				//!< Clock state

} Clock_M;


/*! \ingroup MESSAGES
	Contains the SV position used for the PVT solution
*/
typedef struct SV_Position_M
{
	uint32 chan;
	double x;				//!< ECEF x coordinate (meters)
	double y;				//!< ECEF y coordinate (meters)
	double z;				//!< ECEF z coordinate (meters)
	double vx;				//!< ECEF x velocity (meters/sec)
	double vy;				//!< ECEF x velocity (meters/sec)
	double vz;				//!< ECEF x velocity (meters/sec)
	double ax;				//!< ECEF x acceleration (meters/sec/sec)
	double ay;				//!< ECEF x acceleration (meters/sec/sec)
	double az;				//!< ECEF x acceleration (meters/sec/sec)
	double elev;			//!< Satellite elevation (radians)
	double azim;			//!< Satellite azimuth (radians)
	double clock_bias;		//!< SV clock bias
	double frequency_bias;	//!< SV clock rate bias
	double transit_time;	//!< Time of flight from SV to receiver
	double time;			//!< Time used in SV position calculation
	double latitude;		//!< Latitude using WGS-84 ellipsoid in decimal radians
	double longitude;		//!< Longitude using WGS-84 ellipsoid in decimal radians
	double altitude;		//!< height in meters

} SV_Position_M;


/*! \ingroup MESSAGES
	EKF navigation solution
*/
typedef struct EKF_M
 {

	double x;			//!< ECEF x coordinate (meters)
	double y;			//!< ECEF y coordinate (meters)
	double z;			//!< ECEF z coordinate (meters)
	double vx;			//!< ECEF x velocity (meters/sec)
	double vy;			//!< ECEF x velocity (meters/sec)
	double vz;			//!< ECEF x velocity (meters/sec)
	double ax;			//!< ECEF x acceleration (meters/sec/sec)
	double ay;			//!< ECEF x acceleration (meters/sec/sec)
	double az;			//!< ECEF x acceleration (meters/sec/sec)

	double time;		//!< time in seconds
	double clock_bias;	//!< clock bias in seconds
	double clock_rate;  //!< clock rate in meters/second
	double latitude;	//!< latitude in decimal radians
	double longitude;	//!< longitude in decimal radians
	double altitude;	//!< height in meters

	double gdop;		//!< geometric dilution of precision
	double pdop;		//!< position dilution of precision
	double tdop;		//!< time dilution of precision
	double hdop;		//!< hdop diultion of precision
	double vdop;		//!< vertical dilution of precision

	uint32 nsvs;		//!< This is a mask, not a number
	uint32 converged;	//!< declare convergence
	uint32 tic;			//!< global_tic associated with this solution

	uint32 stale_ticks;			//!< count the number of tics since the last good sltn
	uint32 converged_ticks;		//!< count number of converged tics
	uint32 nav_channels;		//!< count number of SVs used in last PVT estimation
	uint32 initial_convergence;	//!< Flag set ONCE if the first convergence has occured

	uint32 chanmap[MAX_CHANNELS];

} EKF_M;


/*! \ingroup STRUCTS
 * The measurement dumped to the PVT object
 */
typedef struct Measurement_M
{

	double	code_time;					//!< The code time
	double 	code_phase; 				//!< Code phase (chips)
	double 	carrier_phase;				//!< Carrier phase (cycles)
	double 	carrier_phase_prev;			//!< Carrier phase prev (cycles)
	double 	carrier_phase_prev_prev;	//!< Carrier phase prev prev (cycles)
	double 	code_phase_mod;				//!< Code phase (chips), mod 1023
	double 	carrier_phase_mod;			//!< Carrier phase (cycles), mod 1
	double 	code_nco;					//!< Code NCO
	double 	carrier_nco;				//!< Carrier NCO
	uint32  _1ms_epoch;					//!< _1ms_epoch
	uint32  _20ms_epoch;				//!< _20ms_epoch
	uint32	_z_count;					//!< The z count
	uint32 	navigate;					//!< This has been tagged as a good measurement
	uint32	sv;							//!< For this sv
	uint32	chan;						//!< For this channel
	uint32 	count;						//!< Corresponds to this tic

} Measurement_M;


/*! \ingroup MESSAGES
	Pseudoranges structure, holds both time and meters
*/
typedef struct Pseudorange_M
{

	double gpstime;			//!< Time tag associated with pseudorange
	double time;			//!< pseudorange in seconds
	double time_rate;		//!< pseudorange rate in sec/sec
	double meters;			//!< pseudorange in meters
	double meters_rate;		//!< pseudorange rate in meters/sec
	double residual;		//!< residual in meters
	double rate_residual;	//!< rate residual (m/s)
	double time_uncorrected;//!< raw pseudorange measurements
	double previous;		//!< from previous step, used for err check

	uint32 chan;

} Pseudorange_M;


/*! \ingroup MESSAGES
	Decoded ephemeris struct
*/
typedef struct Ephemeris_M
{
	double	tgd;                    //!< Group delay parameter.
	double	toc;					//!< Reference time of clock data parameter set.
	double	af0;					//!< Clock correction polynomial coefficient.
	double	af1;					//!< Clock correction polynomial coefficient.
	double	af2;					//!< Clock correction polynomial coefficient.
	double	crs;					//!< Sine harmonic correction to orbital radius.
	double	deltan;					//!< Mean motion delta from computed value.
	double	m0;                     //!< Mean anomaly at TOE.
	double	cuc;					//!< Cosine harmonic correction to orbital radius.
	double	ecc;                    //!< Eccentricity.
	double	cus;					//!< Sine harmonic corr to argument of latitude.
	double	sqrta;                  //!< Square root of semimajor axis.
	double	toe;					//!< Reference time of ephemeris data set.
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

	uint32	sv;						//!< SV #
	uint32	zcount;					//!< zcount?
	uint32	toewk;                  //!< GPS week corresponding to toe.
	uint32	fti;                    //!< Fit interval.
	uint32	iode;                   //!< Issue of data, ephemeris.
	uint32	tocwk;					//!< GPS week corresponding to toc.
	uint32	valid;					//!< 0=No valid data.
	uint32	tofxmission;			//!< Time of subframe 1 transmission, sec of week.
	uint32	tow;					//!< Truncated TOW count
	uint32	subframe_1_health;      //!< Subframe 1 health code.
	uint32	code_on_L2;             //!< Code on L2 flag.
	uint32	week_number;			//!< GPS week at time of subframe 1 reception.
	uint32	L2pdata;                //!< L2 P data flag.
	uint32	ura;                    //!< Satellite's URA code.
	uint32	iodc;                   //!< Issue of data, clock.


} Ephemeris_M;


/*! \ingroup MESSAGES
	Decoded almanac struct
*/
typedef struct Almanac_M
{

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

	uint32	sv;
	uint32	week;					//!< Week number
	uint32	decoded;				//!< Has this been decoded yet
	uint32	health;					//!< Health code

} Almanac_M;


/*! \ingroup MESSAGES
 *
 */
typedef struct Ephemeris_Status_M
{

	uint32 valid[NUM_CODES];		//!< Valid ephemeris
	uint32 iode[NUM_CODES];		//!< Corresponding IODE
	uint32 avalid[NUM_CODES];	//!< Valid almanac

} Ephemeris_Status_M;


/*! \ingroup MESSAGES
 * Data from the FIFO to the Telemetry
 */
typedef struct FIFO_M
{

	uint32 tic;
	uint32 count;		//!< Number of 1 ms packets processed
	uint32 head;		//!< Head pointer number
	uint32 tail;		//!< Tail pointer number
	uint32 agc_scale;	//!< Value used for AGC scale
	uint32 overflw;		//!< Overflows in last ms
	uint32 nactive;		//!< Number of channels to process the measurment packet

} FIFO_M;


/*! \ingroup MESSAGES
	Acknowledge processing of command
*/
typedef struct Command_Ack_M
{

	uint32 command_id;
	uint32 command_tic;

} Command_Ack_M;


/* Holds all the relevant messages */
typedef struct Message_Struct
{

	/* Data gets stored here! */
	Board_Health_M 		board_health;					//!< Board health message
	Task_Health_M		task_health;					//!< Task health message
	Channel_Health_M 	channel_health[MAX_CHANNELS+1]; //!< Channel health message, last element is used as a buffer

	SPS_M				sps;							//!< SPS message
	Clock_M				clock;							//!< Clock message
	SV_Position_M		sv_positions[MAX_CHANNELS+1];	//!< SV Positions, last element is used as a buffer
	Pseudorange_M 		pseudoranges[MAX_CHANNELS+1];	//!< Pseudoranges, last element is used as a buffer
	Measurement_M 		measurements[MAX_CHANNELS+1];	//!< Measurements, last element is used as a buffer

	Ephemeris_M			ephemerides[NUM_CODES+1];		//!< Ephemeris message, last element is used as a buffer
	Almanac_M			almanacs[NUM_CODES+1];			//!< Almanac message, last element is used as a buffer
	Ephemeris_Status_M	ephemeris_status;				//!< Status of ephemeris
	FIFO_M				fifo;							//!< FIFO status
	Command_Ack_M		command_ack;

} Message_Struct;


/* Unionize the structures */
typedef union Union_M
{
	Board_Health_M		board_health;
	Task_Health_M		task_health;
	Channel_Health_M	channel_health;
	SPS_M				sps;
	Clock_M				clock;
	SV_Position_M		sv_position;
	EKF_M				ekf;
	Measurement_M		measurement;
	Pseudorange_M		pseudorange;
	Ephemeris_M			ephemeris;
	Almanac_M			almanac;
	Ephemeris_Status_M	ephemeris_status;
	FIFO_M				fifo;
	Command_Ack_M		command_ack;
} Union_M;


#endif /* MESSAGES_H */

