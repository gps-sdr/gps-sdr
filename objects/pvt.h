/*----------------------------------------------------------------------------------------------*/
/*! \file pvt.h
//
// FILENAME: pvt.h
//
// DESCRIPTION: Define the PVT object.
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

#ifndef _PVT_H
#define _PVT_H

#include "includes.h"
#include "ephemeris.h"
#include "channel.h"

enum PVT_CLOCK_STATE
{
	PVT_CLOCK_UNINITIALIZED,
	PVT_CLOCK_NOMINAL
};

enum PVT_ERROR_CODES
{
	PVT_ERROR_INACTIVE,
	PVT_ERROR_NOMINAL,
	PVT_ERROR_EPHEM,
	PVT_ERROR_POSITION,
	PVT_ERROR_VELOCITY,
	PVT_ERROR_PSEUDO,
	PVT_ERROR_PSEUDO_RATE,
};

/*! @ingroup CLASSES
	@brief Performs least squares point solution.
*/
class PVT : public Threaded_Object
{

	private:

		uint32 val;

		/* Satellite related stuff */
		Ephemeris_M		ephemerides[MAX_CHANNELS];				//!< Decoded ephemerides
		SV_Position_M	sv_positions[MAX_CHANNELS];				//!< Calculated SV positions
		Pseudorange_M	pseudoranges[MAX_CHANNELS];				//!< Pseudoranges
		Measurement_M	measurements[MAX_CHANNELS];				//!< Raw measurements

		int32 good_channels[MAX_CHANNELS];						//!< Is this a good channel (used to navigate)
		int32 master_sv[MAX_CHANNELS];							//!< Channel->SV map
		int32 sv_codes[MAX_CHANNELS];							//!< Error codes
		int32 doppler_suspect[MAX_CHANNELS][MAX_CHANNELS];		//!< For the cross-corr check

		/* Position and clock solutions */
		SPS_M master_nav;										//!< Master nav sltn
		SPS_M temp_nav;											//!< Temp nav sltn
		Clock_M master_clock;									//!< Master clock
		TOT_M tot;												//!< Time of tone message
		UTC_Parameter_S utc;									//!< UTC parameter
		Preamble_2_PVT_S preamble;								//!< Preamble from tracking isr
		PVT_2_TLM_S tlm_s;										//!< Dump stuff to telemetry, sv_select, pps, and ekf

		/* Matrices used in nav solution */
		double *alpha[MAX_CHANNELS];
		double alpha_2[4][4];
		double alpha_inv[4][4];
		double alpha_pinv[4][MAX_CHANNELS];

		double dircos[MAX_CHANNELS][4];
		double pseudorangeres[MAX_CHANNELS];					//!< Pseudorange residuals
		double pseudorangerateres[MAX_CHANNELS];				//!< Pseudorange rate residuals
		double dr[4];

	public:

		PVT();
		~PVT();
		void Start();							//!< Start the thread
		void Import();							//!< Get data into the thread
		void Export();							//!< Get data out of the thread
		void PipeCheck();						//!< Make sure the pipes are EMPTY

		void Navigate();						//!< main navigation task, call the following tabbed functions
			void ProjectState();				//!< project state to current measurement epoch
			void Update_Time();					//!< estimate GPS time at current tic
			void Get_Ephemerides();				//!< get the current ephemeris from Ephemeris_Thread
			void SV_TransitTime();				//!< Transit time to each SV
			void SV_Positions();				//!< calculate SV Positions (for each channel with valid ephemeris)
			void SV_Elevations();				//!< calculate SV Elevation angles for elevation mask
			void SV_Correct();					//!< correct SV positions for transit time
			void PseudoRange();					//!< calculate the pseudo ranges
			void FormModel();					//!< form direction cosine matrix and prediction residuals
			uint32 PreErrorCheck();				//!< check all SV's for bad measurements, etc
			void ErrorCheckCrossCorr();			//!< check for cross-correlation problem via ephemeris match
			uint32 PostErrorCheck();			//!< check all SV's for bad measurements, etc
			uint32 Converged();					//!< declare convergence
			void Residuals();					//!< compute resdiuals
			double PVT_Estimation();			//!< estimate PVT
			void ClockUpdate();					//!< update the clock
			void UTCTime();						//!< form time of tone (tot) message
			void LatLong();						//!< convert ECEF coordinates to Lat,Long,Height
			void DOP();							//!< calculate DOP terms
			void ClockInit();					//!< initialize clock

		void Reset();							//!< Reset the navigation solution to naught values
		void ResetChannel(int32 _chan);			//!< Reset individual channel
		void ResetNav();						//!< Reset navigation state (master_nav)
		void ResetClock();						//!< Reset clock state (master_clock)
		void ZeroNav();							//!< Zero out nav

};


#endif



