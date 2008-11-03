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

		/* Default object variables */
		uint32 				execution_tic;	//!< Execution counter
		uint32 				start_tic;		//!< OS tic at start of function
		uint32 				stop_tic;		//!< OS tic at end of function
		pthread_t 			thread;			//!< For the thread
		pthread_mutex_t		mutex;			//!< Protect the following variable

		int32 				fifo;
		int32	  			gpipe;			//!< Named pipe to export data to the GUI
		int32				gpipe_open;		//!< Is this named pipe connected?

		/* Stuff for ncurses display */
		WINDOW *mainwnd;
		WINDOW *screen;
		WINDOW *my_win;

		FIFO_2_Telem_S 		tFIFO;
		PVT_2_Telem_S 		tNav;
		Chan_Packet_S 		tChan[MAX_CHANNELS];
		Acq_Result_S		tAcq;
		Ephem_2_Telem_S 	tEphem;
		SV_Select_2_Telem_S tSelect;
		Telem_2_GUI_S 		tGUI;

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

		Telemetry(int32 _ncurses);
		~Telemetry();
		void Start();								//!< Start the thread
		void Stop();								//!< Stop the thread
		void Import();								//!< Get data into the thread
		void Export();								//!< Get data out of the thread
		void Lock();								//!< Lock the object's mutex
		void Unlock();								//!< Unlock the object's mutex
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

		void OpenGUIPipe();
		void SetGUIPipe(bool _status);
		void ExportGUI();

		/* Object specific methods */
		void FormCCSDSPacketHeader(uint32 _apid, uint32 _sf, uint32 _pl); 	//!< Form the CCSDS packet header
		void EmitCCSDSPacket(void *_buff, uint32 _len);						//!< Emit a CCSDS packet
		void FormCCSDSCommandHeader();										//!< Form the CCSDS command header

		/* Types of output messages */
		void SendBoardHealth();						//!< Emit hardware health values
		void SendTaskHealth();						//!< Emit task health values
		void SendChannelHealth();					//!< Emit channel health
		void SendSPS();								//!< Emit a PVT
		void SendClock();							//!< Emit a Clock state
		void SendSVPosition();						//!< Send the SV Position
		void SendEKF();								//!< Emit a GEONS solution
		void SendMeasurement();						//!< Emit a raw measurement
		void SendPseudorange();						//!< Emit a Pseudorange
		void SendEphemeris();						//!< Emit a decoded ephemeris
		void SendAlamanac();						//!< Emit a decoded almanac
		void SendCommandAck();						//!< Send a command acknowledge

};

#endif /* Telemetry_H */
