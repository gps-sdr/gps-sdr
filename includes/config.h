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

/* The most important thing, the NUMBER OF CORRELATORS IN THE RECEIVER and the NUMBER OF CPUs */
/*----------------------------------------------------------------------------------------------*/
#define MAX_CHANNELS			(12)						//!< Number of channel objects
#define CPU_CORES				(2)							//!< 1 for a single core, 2 for a dual core system, etc
#define CORR_PER_CPU			(MAX_CHANNELS/CPU_CORES)	//!< Distribute them up evenly (this should be an INTEGER!)
#define MAX_ANTENNAS			(1)							//!< The number of antennas
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


/* Work on what kind of Data?? */
/*----------------------------------------------------------------------------------------------*/
#define USRP_RECORDER			(1)			//!< Data collected with USRP
 /*----------------------------------------------------------------------------------------------*/


/* MISC GPS Constants */
/*----------------------------------------------------------------------------------------------*/
#define MAX_SV					(32)		//!< Number of PRN codes
#define SHU_SIGNALS				(16)		//!< Status and health signals
/*! WAAS PRNs will be referred to internally as PRN 32...50 (zero indexed) */
#define NON_ALLOCATED_PRN		(36)		//!< A non-allocated PRN number, used to generate acquisition thresholds
/*----------------------------------------------------------------------------------------------*/


/* Acquisition defines */
/*----------------------------------------------------------------------------------------------*/
#define MASK_ANGLE				(5)			//!< Add this many degrees to altitude dependant mask
#define ACQ_STRONG				(0)			//!< Acq strong type
#define ACQ_MEDIUM				(1)			//!< Acq medium type
#define ACQ_WEAK 				(2)			//!< Acq weak type

#define ACQ_STRONG_STATE		(1)			//!< 0 for off, 1 for on, 2 for hot acquisition only
#define ACQ_MEDIUM_STATE		(2)			//!< 0 for off, 1 for on, 2 for hot acquisition only
#define ACQ_WEAK_STATE			(2)			//!< 0 for off, 1 for on, 2 for hot acquisition only

#define ACQ_ITERATIONS			(1)			//!< Do this many acqs at a given type before moving to next type
#define THRESH_STRONG			(1.3e7)		//!< Threshold for strong signal detection
#define THRESH_MEDIUM			(1.5e7)		//!< Threshold for medium signal detection
#define THRESH_WEAK				(1.5e7)		//!< Threshold for weak signal detection
#define MAX_DOPPLER				(45000)		//!< Set the maximum Doppler frequency
#define DOPPLER_RANGE			(1000)		//!< Search this Doppler range for hot acquisitions
/*----------------------------------------------------------------------------------------------*/


/* Correlator  Defines */
/*----------------------------------------------------------------------------------------------*/
#define CORR_DELAYS				(1)			//!< Number of delays to calculate (plus-minus)
#define CORR_SPACING			(.5)		//!< How far should the correlators be spaced (chips)
#define FRAME_SIZE_PLUS_2		(12)		//!< 10 words per frame, 12 = 10 + 2
#define MEASUREMENT_INT			(100)		//!< Packets of ~1ms data
#define CODE_BINS				(20)		//!< Partial code offset bins code resolution -> 1 chip/X bins
#define CARRIER_SPACING			(20)		//!< Spacing of bins (Hz)
#define CARRIER_BINS			(MAX_DOPPLER/CARRIER_SPACING) //!< Number of pre-sampled carrier wipeoff bins
#define ICP_TICS				(5)			//!< Number of measurement ints (plus-minus) to calculate ICP,
											//!< this cannot exceed TICS_PER_SECOND/2 !!!!

#define TICS_2_SECONDS			(MEASUREMENT_INT/1000)
#define TICS_PER_SECOND			(1000/MEASUREMENT_INT)
#define	ACQS_PER_SECOND			(20)
/*----------------------------------------------------------------------------------------------*/


/* Channel defines */
/*----------------------------------------------------------------------------------------------*/
#define CARRIER_AIDING			(1)			//!< Carrier aid the DLL
#define PLL_BN					(15)		//!< PLL Bandwidth
#define FLL_BN					(10)		//!< FLL Bandwidth
#define AGC_BITS				(5)			//!< AGC to this bit depth
#define AGC_LOW					(4)			//!< Overflow low
#define AGC_HIGH				(16)		//!< Overflow high
#define MEASUREMENT_DELAY		(10)		//!< Number of measurements that have to be marked as navigate to allow PVT
/*----------------------------------------------------------------------------------------------*/


/* Associate each task with a enum */
/*----------------------------------------------------------------------------------------------*/
#define	MAX_TASKS				(16)		//!< Max task number (used to allocate arrays)
enum TASK_IDS
{
	CORRELATOR_TASK_ID = CPU_CORES,			//!< Bottom tasks are the correlators, which is a variable
	POST_PROCESS_TASK_ID,
	FIFO_TASK_ID,
	COMMANDO_TASK_ID,
	TELEMETRY_TASK_ID,
	SERIAL_TELEMETRY_TASK_ID,
	KEYBOARD_TASK_ID,
	EPHEMERIS_TASK_ID,
	SV_SELECT_TASK_ID,
	ACQUISITION_TASK_ID,
	PVT_TASK_ID,
	EKF_TASK_ID,
	LAST_TASK_ID
};
/*----------------------------------------------------------------------------------------------*/


#endif
