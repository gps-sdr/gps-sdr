/*! \file Correlator.h
	Defines the class Correlator
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

#ifndef Correlator_H
#define Correlator_H

#include "includes.h"

class Correlator
{

	private:

		FILE *crap;
	
		pthread_t 			thread;	 				//!< For the thread
		
		int32				packet_count;			//!< Count 1ms packets
		ms_packet			packet;					//!< 1ms of data
		
		int32 				chan;			 		//!< Which channel is this?
		Acq_Result_S 		result; 				//!< An acquisition result has been returned!
		NCO_Command_S  		feedback;				//!< NCO feedback commands
		Correlation_S  		corr;					//!< Resulting correlation
		Correlator_State_S	state;					//!< Correlator states
		Measurement_S		meas;					//!< Measurements to dump
		Channel 			*aChannel;				//!< Get this correlators channel
		
		/* This  is important, the following array is large and is constant, so it is 
		 * shared among all instances of this class */
		 static CPX *sine_table;
		 static CPX **sine_rows;
//		static CPX			sine_table[(2*CARRIER_BINS+1)*2*SAMPS_MS];	//!< Hold the sine lookup table [2*CARRIER_BINS+1][2*SAMPS_MS];
//		static CPX			*sine_rows[2*CARRIER_BINS+1];				//!< Row pointers to above
		
		CPX					*code_table;			//!< Hold the PRN lookup table  [2*CODE_BINS+1][2*SAMPS_MS];
		CPX					**code_rows;			//!< Row pointers to above
		CPX					scratch[2*SAMPS_MS];	//!< Scratch data
		CPX					lookup[SAMPS_MS];
		uint32				nco_phase_inc;
		uint32				nco_phase;
		
		
	public:

		Correlator(int32 _chan);
		~Correlator();
		void Inport();												//!< Get IF data, NCO commands, and acq results
		void Correlate();											//!< Run the actual correlation
		void Export();												//!< Dump results to channels and Navigation
		void Start();												//!< Start the thread
		void Stop();												//!< Stop the thread
		void TakeMeasurement();										//!< Take some measurements
		void SamplePRN();											//!< Sample the PRN code and put it into the code table
		void InitCorrelator();										//!< Initialize a correlator/channel with an acquisition result
		void DumpAccum(Correlation_S *c);							//!< Dump accumulation to channel for processing
		void UpdateState(int32 samps);								//!< Update correlator state
		void ProcessFeedback(NCO_Command_S *f);		
		void Accum(Correlation_S *c, CPX *data, int32 samps);		//!< Do the actual accumulation
		void SineGen(int32 samps);									//!< Dynamic wipeoff generation
};

#endif /* Correlator_H */
