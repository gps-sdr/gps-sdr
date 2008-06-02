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

#ifndef Telemetry_H
#define Telemetry_H

#include "includes.h"

/*! \ingroup CLASSES
 * 
 */
typedef class Telemetry
{

	private:

		int32	  gpipe;	//!< Named pipe to export data to the prop
		pthread_t thread;	//!< For the thread
		pthread_mutex_t		mutex;	//!< Protect the following variable
			
		/* Stuff for ncurses display */
		WINDOW *mainwnd;
		WINDOW *screen;
		WINDOW *my_win;
		
		FIFO_2_Telem_S 	tFIFO;
		PVT_2_Telem_S 	tNav;
		Chan_Packet_S 	tChan[MAX_CHANNELS];
		Acq_Result_S	tAcq;
		Ephem_2_Telem_S tEphem;
		SV_Select_2_Telem_S tSelect;
		
		int32 active[MAX_CHANNELS];
		int32 line;
		int32 ncurses_on;
		int32 count;
		int32 display;
		
		FILE *fp_nav;
		FILE *fp_chan;
		FILE *fp_pseudo;
		FILE *fp_meas;
		FILE *fp_sv;

	public:

		Telemetry(int32 _ncurses);
		~Telemetry();
		void Lock();
		void Unlock();
		void SetDisplay(int32 _type){display = _type;}
		void Inport();
		void Export();
		void Start();
		void Stop();
		
		void Init();
		void InitScreen();
		void UpdateScreen();
		void EndScreen();

		void PrintChan();
		void PrintNav();
		void PrintSV();
		void PrintEphem();
		void PrintAlmanac();		
		void LogNav();
		void LogPseudo();
		void LogTracking();
		void LogSV();
};

#endif /* Telemetry_H */
