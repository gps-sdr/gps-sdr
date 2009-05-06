/*----------------------------------------------------------------------------------------------*/
/*! \file includes.h
//
// FILENAME: includes.h
//
// DESCRIPTION: Default includes for all files within the project
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
#include <semaphore.h>
/*----------------------------------------------------------------------------------------------*/

/* Herein Lies Many Important File, note thy order is important! */
/*----------------------------------------------------------------------------------------------*/
#include "config.h"				//!< Configure receiver
#include "signaldef.h"			//!< Define attributes of input data
#include "defines.h"			//!< Defines from IS-GPS-200D and some other things
#include "macros.h"				//!< Macros
#include "messages.h"			//!< Defines output telemetry
#include "commands.h"			//!< Defines command input
#include "structs.h"			//!< Structs used for interprocess communication
#include "sdr_structs.h"		//!< Structs used for interprocess communication
#include "protos.h"				//!< Functions & thread prototypes
#include "simd.h"				//!< Include the SIMD functionality
#include "globals.h"
/*----------------------------------------------------------------------------------------------*/

/* Include the "Threaded Objects" */
/*----------------------------------------------------------------------------------------------*/
#include "threaded_object.h"	//!< Base class for threaded object
//#include "fft.h"				//!< Fixed point FFT object
//#include "fifo.h"				//!< Circular buffer for Importing IF data
//#include "keyboard.h"			//!< Handle user input via keyboard
//#include "channel.h"			//!< Tracking channels
//#include "correlator.h"			//!< Correlator
//#include "acquisition.h"		//!< Acquisition
//#include "pvt.h"				//!< PVT solution
//#include "ephemeris.h"			//!< Ephemeris decode
//#include "telemetry.h"			//!< Ncurses telemetry
//#include "serial_telemetry.h"	//!< Serial/GUI telemetry
//#include "commando.h"			//!< Command interface
//#include "sv_select.h"			//!< Drives acquisition/reacquisition process
//#include "post_process.h"		//!< Run the receiver from a file
/*----------------------------------------------------------------------------------------------*/
