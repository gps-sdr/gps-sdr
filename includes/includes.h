/*! \file Includes.h
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


#ifdef GLOBALS_HERE
	#define EXTERN /**/
#else
	#define EXTERN extern
#endif


/* Include standard headers, OS stuff */
/*----------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sched.h>
#include <curses.h>
#include <limits.h>
#include <termios.h>
/*----------------------------------------------------------------------------------------------*/

/*Herein Lies Many Important File, note thy order is important!*/
/*----------------------------------------------------------------------------------------------*/
#include "config.h"				//!< Configure receiver
#include "signaldef.h"			//!< Define attributes of input data
#include "defines.h"			//!< Defines from IS-GPS-200D and some other things
#include "macros.h"				//!< Macros
#include "messages.h"
#include "commands.h"
#include "structs.h"			//!< Structs used for interprocess communication
#include "protos.h"				//!< Functions & thread prototypes
#include "simd.h"				//!< Include the SIMD functionality
/*----------------------------------------------------------------------------------------------*/

/* Include the "Threaded Objects" */
/*----------------------------------------------------------------------------------------------*/
#include "fft.h"				//!< Fixed point FFT object
#include "fifo.h"				//!< Circular buffer for Importing IF data
#include "keyboard.h"			//!< Handle user input via keyboard
#include "correlator.h"			//!< Correlator
#include "channel.h"			//!< Tracking channels
#include "acquisition.h"		//!< Acquisition
#include "pvt.h"				//!< PVT solution
#include "ephemeris.h"			//!< Ephemeris decode
#include "telemetry.h"			//!< Ncurses telemetry
#include "serial_telemetry.h"	//!< Serial/GUI telemetry
#include "sv_select.h"			//!< Drives acquisition/reacquisition process
//#include "ocean.h"			//!< Ocean reflection waveforms
#include "post_process.h"		//!< Run the receiver from a file
/*----------------------------------------------------------------------------------------------*/

/* This must go last */
/*----------------------------------------------------------------------------------------------*/
#include "globals.h"			//!< Global variables and objects
/*----------------------------------------------------------------------------------------------*/

