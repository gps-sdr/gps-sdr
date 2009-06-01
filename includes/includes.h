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
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <limits.h>
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
#include "globals.h"			//!< Global objects live here
#include "threaded_object.h"	//!< Base class for threaded object
/*----------------------------------------------------------------------------------------------*/
