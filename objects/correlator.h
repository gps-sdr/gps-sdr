/*----------------------------------------------------------------------------------------------*/
/*! \file correlator.h
//
// FILENAME: correlator.h
//
// DESCRIPTION: Defines the Correlator class.
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

#ifndef CORRELATOR_H_
#define CORRELATOR_H_

#include "includes.h"
#include "sv_select.h"
#include "channel.h"
#include "fifo.h"

/*! \ingroup CLASSES
 *
 */
class Correlator : public Threaded_Object
{

	private:

		/* Default object variables */
		NCO_Command_S  		feedback[MAX_CHANNELS];				//!< NCO feedback commands
		Correlation_S  		correlations[MAX_CHANNELS];			//!< Resulting correlation
		Correlator_State_S	states[MAX_CHANNELS];				//!< Correlator states
		Measurement_M		measurements[MAX_CHANNELS];			//!< Measurements to dump
		Measurement_M		measurements_buff[MAX_CHANNELS][MEASUREMENTS_PER_SECOND];	//!< Measurements to dump
		Preamble_2_PVT_S	preamble;

		/* These variables are shared among all the channels */
		Acq_Command_S 		result; 							//!< An acquisition result has been returned!
		ms_packet			packet;								//!< 1ms of data
		int32				packet_count;						//!< Count 1ms packets
		int32				measurement_tic;					//!< Measurement tic
		CPX 				*main_sine_table;					//!< Hold the sine wipeoff table
		CPX 				**main_sine_rows;					//!< Row pointers to above
		MIX 		 		*main_code_table;					//!< Hold the PRN lookup table for all 32 SVs [2*CODE_BINS+1][2*SAMPS_MS];
		MIX	 				**main_code_rows;					//!< Row pointers to above
		CPX					scratch[2*SAMPS_MS];				//!< Scratch data
		CPX					lookup[SAMPS_MS];					//!< Hold the sine lookup
		uint32				nco_phase_inc;						//!< For dynamically generating the wipeoff
		uint32				nco_phase;							//!< For dynamically generating the wipeoff

	public:

		Correlator();
		~Correlator();
		void Import();											//!< Get IF data, NCO commands, and acq results
		void Export();											//!< Dump results to channels and Navigation
		void Start();											//!< Start the thread
		void Correlate();										//!< Run the actual correlation
		void SamplePRN();																	//!< Sample all 32 PRN codes and put it into the code table
		void GetPRN(Correlator_State_S *s);													//!< Get row pointers to specific PRN
		void InitCorrelator(Correlator_State_S *s);											//!< Initialize a correlator/channel with an acquisition result
		void UpdateState(Correlator_State_S *s, int32 samps);								//!< Update correlator state
		void ProcessFeedback(Correlator_State_S *s, NCO_Command_S *f);						//!< Process the feedback
		void DumpAccum(Correlator_State_S *s, Correlation_S *c, NCO_Command_S *f, int32 _chan);	//!< Dump accumulation to channel for processing
		void TakeMeasurements();																//!< Take some measurements
		void Accum(Correlator_State_S *s, Correlation_S *c, CPX *data, int32 samps);		//!< Do the actual accumulation
		void SineGen(int32 samps);															//!< Dynamic wipeoff generation
};

#endif /* CORRELATOR_H_ */
