/*! \file Signal.h
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

#ifndef SIGNALDEF_H
#define SIGNALDEF_H

/* Data from the Universal Software Radio Peripheral */
/*----------------------------------------------------------------------------------------------*/
#if USRP_RECORDER

	#define IF_SAMPLE_FREQUENCY	(2048000)				//!< How many CPX samples in a second
	#define IF_FREQUENCY		(604000)				//!< IF frequency
	#define IF_SAMPS_MS 		(2048)					//!< Samples per millisecond

#endif

#if GRS_RECORDER

	#define IF_SAMPLE_FREQUENCY	(10000000)				//!< How many CPX samples in a second
	#define IF_FREQUENCY		(420000)				//!< IF frequency
	#define IF_SAMPS_MS 		(10000)					//!< Samples per millisecond

#endif
/*----------------------------------------------------------------------------------------------*/


#endif



