/*----------------------------------------------------------------------------------------------*/
/*! \file config.h
//
// FILENAME: config.h
//
// DESCRIPTION: Control the configuration of the receiver.
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

#ifndef CONFIG_H_
#define CONFIG_H_

#define LINUX_OS

/* Software Version */
/*----------------------------------------------------------------------------------------------*/
#define SOFTWARE_VERSION_MAJOR		(2)
#define SOFTWARE_VERSION_MINOR		(6)
#define SOFTWARE_VERSION_POSTFIX	(1)
/*----------------------------------------------------------------------------------------------*/

/* The most important thing, the NUMBER OF CORRELATORS IN THE RECEIVER and the NUMBER OF CPUs */
/*----------------------------------------------------------------------------------------------*/
#define MAX_CHANNELS			(12)						//!< Number of channel objects
#define CPU_CORES				(2)							//!< 1 for a single core, 2 for a dual core system, etc
#define CORR_PER_CPU			(MAX_CHANNELS/CPU_CORES)	//!< Distribute them up evenly (this should be an INTEGER!)
#define MAX_ANTENNAS			(2)							//!< The number of antennas
#define TASK_STACK_SIZE			(2048)						//!< For Nucleus/Linux compatibility
/*----------------------------------------------------------------------------------------------*/


/* What stuff to log */
/*----------------------------------------------------------------------------------------------*/
#define CHANNEL_DEBUG			(1)			//!< Log channel data to HD
#define CHANNEL_INVESTIGATE		(0)			//!< Try to figure out Nav bug
#define LOG_PSEUDO				(0)			//!< Pseudoranges
#define LOG_SV					(0)			//!< SV Navigation data
#define LOG_NAV					(1)			//!< Nav Output
/*----------------------------------------------------------------------------------------------*/


/* MISC GPS Constants */
/*----------------------------------------------------------------------------------------------*/
#define MAX_SV					(32)		//!< Number of PRN codes
#define SHU_SIGNALS				(16)		//!< Status and health signals
/*! WAAS PRNs will be referred to internally as PRN 32...50 (zero indexed) */
#define NON_ALLOCATED_PRN		(36)		//!< A non-allocated PRN number, used to generate acquisition thresholds
/*----------------------------------------------------------------------------------------------*/


/*! Config the SV Select */
/*----------------------------------------------------------------------------------------------*/
#define MASK_ANGLE				(0)				//!< Add this many degrees to altitude dependant mask
#define ACQ_MODULO_WEAK			(8)				//!< Do this many weak acqs per 32 PRN strong search
#define MAX_DOPPLER_ABSOLUTE	(15000)			//!< Limit any and all Dopplers the be within this range
#define MAX_DOPPLER_STRONG		(15000)			//!< Cold Doppler search space for strong signal
#define MAX_DOPPLER_MEDIUM		(15000)			//!< Cold Doppler search space for strong signal
#define MAX_DOPPLER_WEAK		(15000)			//!< Cold Doppler search space for weak signal
#define MAX_DOPPLER_WARM		(1000)			//!< Search this much Doppler space if state information is available
#define ACQ_OPERATION_STRONG	(ACQ_OPERATION_COLD)	//!< Acq strong mode (2 = cold & warm)
#define ACQ_OPERATION_MEDIUM	(ACQ_OPERATION_DISABLED)//!< Acq strong mode (2 = cold & warm)
#define ACQ_OPERATION_WEAK		(ACQ_OPERATION_DISABLED)//!< Acq weak mode (2 = warm only)
#define THRESH_STRONG			(0)						//!< Thats right zero! 40 dB-Hz and above acquisition threshold
#define THRESH_MEDIUM			(0)						//!< Thats right zero! 30 dB-Hz and above acquisition threshold
#define THRESH_WEAK				(0)						//!< 30 dB-Hz and below (down to ~22 dB-Hz <-- LIAR!) acquisition threshold
/*----------------------------------------------------------------------------------------------*/


/* Correlator Defines */
/*----------------------------------------------------------------------------------------------*/
#define CORR_DELAYS				(1)			//!< Number of delays to calculate (plus-minus)
#define CORR_SPACING			(.5)		//!< How far should the correlators be spaced (chips)
#define FRAME_SIZE_PLUS_2		(12)		//!< 10 words per frame, 12 = 10 + 2
#define CODE_BINS				(20)		//!< Partial code offset bins code resolution -> 1 chip/X bins
#define CARRIER_SPACING			(20)		//!< Spacing of bins (Hz)
#define CARRIER_BINS			(MAX_DOPPLER_ABSOLUTE/CARRIER_SPACING) //!< Number of pre-sampled carrier wipeoff bins
/*----------------------------------------------------------------------------------------------*/


/* Measurement/PVT Defines */
/*----------------------------------------------------------------------------------------------*/
#define MEASUREMENT_INT			(100)		//!< Packets of ~1ms data
#define ICP_TICS				(1)			//!< Number of measurement ints (plus-minus) to calculate ICP,
											//!< this cannot exceed TICS_PER_SECOND/2 !!!!
#define MEASUREMENTS_PER_SECOND	(10)
#define SECONDS_PER_TICK		(.1)
#define PVT_ITERATIONS			(10)		//!< Max number of PVT iterations
#define MEASUREMENT_MOD			(1)			//!< Slow down measurement transmission to the PVT
/*----------------------------------------------------------------------------------------------*/


/* Channel defines */
/*----------------------------------------------------------------------------------------------*/
#define CARRIER_AIDING			(1)			//!< Carrier aid the DLL
#define PLL_BN					(15)		//!< PLL Bandwidth
#define FLL_BN					(10)		//!< FLL Bandwidth
#define AGC_BITS				(6)			//!< AGC to this bit depth
#define OVERFLOW_LOW			(4)			//!< Overflow low
#define OVERFLOW_HIGH			(64)		//!< Overflow high
/*----------------------------------------------------------------------------------------------*/


/* Associate each task with a enum */
/*----------------------------------------------------------------------------------------------*/
#define	MAX_TASKS				(14)			//!< Max task number (used to allocate arrays)
enum TASK_IDS
{
	TRACKING_ISR_TASK_ID,
	PVT_TASK_ID,
	PPS_TASK_ID,
	COMMANDO_TASK_ID,
	ACQUISITION_TASK_ID,
	EPHEMERIS_TASK_ID,
	SV_SELECT_TASK_ID,
	TELEMETRY_TASK_ID,
	EKF_TASK_ID,
	PATIENCE_TASK_ID,
	IDLE_TASK_ID
};
/*----------------------------------------------------------------------------------------------*/


/* Account for GPS rollover here */
/*----------------------------------------------------------------------------------------------*/
#define CURRENT_GPS_WEEK_ROLLOVER	(1)			//!< The number of GPS week rollovers
/*----------------------------------------------------------------------------------------------*/


#endif
