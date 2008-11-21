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

/*! \ingroup CLASSES
 *
 */
class Correlator : public Threaded_Object
{

	private:

		FILE *crap;

		/* Default object variables */
		int32				packet_count;						//!< Count 1ms packets
		ms_packet			packet;								//!< 1ms of data

		int32 				chan;			 					//!< Which channel is this?
		Acq_Command_M 		result; 							//!< An acquisition result has been returned!
		NCO_Command_S  		feedback;							//!< NCO feedback commands
		Correlation_S  		corr;								//!< Resulting correlation
		Correlator_State_S	state;								//!< Correlator states
		Measurement_M		meas;								//!< Measurements to dump
		Measurement_M		meas_buff[TICS_PER_SECOND];			//!< Measurements to dump
		class Channel 		*aChannel;							//!< Get this correlators channel

		/* This  is important, the following array is large and is constant, so it is
		 * shared among all instances of this class */
		static CPX 			*sine_table;						//!< Hold the sine wipeoff table
		static CPX 			**sine_rows;						//!< Row pointers to above
		static MIX  		*main_code_table;					//!< Hold the PRN lookup table for all 32 SVs  [2*CODE_BINS+1][2*SAMPS_MS];
		static MIX 			**main_code_rows;					//!< Row pointers to above

		MIX					*code_table;						//!< Local code table
		MIX					**code_rows;						//!< Row pointers to above
		CPX					scratch[2*SAMPS_MS];				//!< Scratch data
		CPX					lookup[SAMPS_MS];					//!< Hold the sine lookup
		uint32				nco_phase_inc;						//!< For dynamically generating the wipeoff
		uint32				nco_phase;							//!< For dynamically generating the wipeoff

	public:

		Correlator(int32 _chan);
		~Correlator();
		void Import();											//!< Get IF data, NCO commands, and acq results
		void Export();											//!< Dump results to channels and Navigation
		void Start();											//!< Start the thread

		void Correlate();										//!< Run the actual correlation
		void TakeMeasurement();									//!< Take some measurements
		void SamplePRN();										//!< Sample all 32 PRN codes and put it into the code table
		void GetPRN(int32 _sv);									//!< Get row pointers to specific PRN
		void InitCorrelator();									//!< Initialize a correlator/channel with an acquisition result
		void DumpAccum(Correlation_S *c);						//!< Dump accumulation to channel for processing
		void UpdateState(int32 samps);							//!< Update correlator state
		void ProcessFeedback(NCO_Command_S *f);					//!< Process the feedback
		void Accum(Correlation_S *c, CPX *data, int32 samps);	//!< Do the actual accumulation
		void SineGen(int32 samps);								//!< Dynamic wipeoff generation
};

#endif /* Correlator_H */
