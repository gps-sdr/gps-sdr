/*! \file Telemetry.h
	Defines the class Telemetry
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

#ifndef TELEMETRY_H
#define TELEMETRY_H

#include "includes.h"

/*! \ingroup CLASSES
 *
 */
class Telemetry
{

	private:

		/* Default object variables */
		uint32 				execution_tic;	//!< Execution counter
		uint32 				start_tic;		//!< OS tic at start of function
		uint32 				stop_tic;		//!< OS tic at end of function
		pthread_t 			thread;			//!< For the thread
		pthread_mutex_t		mutex;			//!< Protect the following variable

		/* Stuff for ncurses display */
		WINDOW *mainwnd;
		WINDOW *screen;
		WINDOW *my_win;

		FIFO_M 		tFIFO;
		PVT_2_Telem_S 		tNav;
		Channel_Health_M 	tChan[MAX_CHANNELS];
		Acq_Result_S		tAcq;
		Ephemeris_Status_M 	tEphem;
		SV_Select_2_Telem_S tSelect;

		int32 active[MAX_CHANNELS];
		int32 line;
		int32 ncurses_on;
		int32 count;
		int32 display;

		FILE *fp_nav;		//!< Navigation data
		FILE *fp_chan;		//!< Channel tracking data
		FILE *fp_pseudo;	//!< Pseudoranges
		FILE *fp_meas;		//!< Raw measurements
		FILE *fp_sv;		//!< SV position/velocity output
		FILE *fp_ge;		//!< Google Earth Output
		uint32 fp_ge_end;	//!< Hold place of last Google earth pointer, minus the header

	public:

		Telemetry();
		~Telemetry();
		void Start();								//!< Start the thread
		void Stop();								//!< Stop the thread
		void Import();								//!< Get data into the thread
		void Export();								//!< Get data out of the thread
		void Lock(){pthread_mutex_lock(&mutex);};	//!< Lock the object's mutex
		void Unlock(){pthread_mutex_unlock(&mutex);};//!< Unlock the object's mutex
		uint32 GetExecTic(){return(execution_tic);};//!< Get the execution counter
		uint32 GetStartTic(){return(start_tic);};	//!< Get the Nucleus tic at start of function
		uint32 GetStopTic(){return(execution_tic);};//!< Get the Nucleus tic at end of function

		void SetDisplay(int32 _type){display = _type;}

		void Init();
		void InitScreen();
		void UpdateScreen();
		void EndScreen();

		void PrintChan();
		void PrintNav();
		void PrintSV();
		void PrintEphem();
		void PrintAlmanac();
		void PrintHistory();
		void LogNav();
		void LogPseudo();
		void LogTracking();
		void LogSV();
		void LogGoogleEarth();
		void GoogleEarthFooter();
		void GoogleEarthHeader();

};

#endif /* TELEMETRY_H */
