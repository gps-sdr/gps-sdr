/*! \file SV_Select.h
	Defines the class SV_Select
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

#ifndef SV_Select_H
#define SV_Select_H

#include "includes.h"

#define COLD_START 		(0)
#define WARM_START		(1)
#define HOT_START		(2)

/*! \ingroup CLASSES
 *
 */
class SV_Select : public Threaded_Object
{

	private:

		/* Default object variables */
		//SPS_M 			master_nav;
		//Clock_M 			master_clock;
		PVT_2_SV_Select_S	pvt;
		Acq_Command_M		request;						//!< Acquisition transaction
		Acq_Command_M		result;							//!< Acquisition transaction
		Acq_Command_M		result_history[NUM_CODES];		//!< Acquisition transaction history per SV
		Acq_Config_M		config;							//!< How does the acquisition behave
		SPS_M		 		*pnav;							//!< Pointer to nav sltn
		Clock_M 			*pclock;						//!< Point to clock sltn
		Almanac_M			almanacs[NUM_CODES];			//!< The decoded almanacs
		SV_Position_M		sv_positions[NUM_CODES];		//!< The GPS positions calculated from the almanac
		SV_Prediction_M 	sv_prediction[NUM_CODES];		//!< Predicated delay/doppler visibility, etc
		Acq_History_S		sv_history[NUM_CODES];			//!< Keep track of acquisition attempts for each SV
		SV_Select_2_Telem_S	output_s;						//!< Send predicted states to telemetry
		int32				mode;							//!< SV select mode (COLD, WARM, HOT)
		int32				sv;								//!< Current SV
		int32				acq_ticks;
		float				mask_angle;						//!< Elevation mask angle

	public:

		SV_Select();
		~SV_Select();
		void Start();								//!< Start the thread
		void Import();								//!< Get data into the thread
		void Export();								//!< Get data out of the thread

 		void UpdateState();							//!< Update acq type
 		void Acquire();								//!< Run the acquisition
		void GetAlmanac(int32 _sv);					//!< Get the most up-to-date almanacs from the ephemeris

		void SV_Predict(int32 _sv);					//!< Predict states of SVs
		void SV_Position(int32 _sv);				//!< Compute SV positions from almanac
		void SV_LatLong(int32 _sv);					//!< Compute SV's lat and long
 		bool SetupRequest();						//!< Setup the acq request
 		void ProcessResult();						//!< Take the result and do something with it!
		void MaskAngle();							//!< Calculate elevation mask angle

		void setConfig(Acq_Config_M *_cfg);			//!< Set the acquisition behaviour
		Acq_Config_M getConfig(){return(config);};	//!< Return the acquisition behaviour
		SV_Prediction_M getSVPrediction(int32 _sv){return(sv_prediction[_sv]);};	//!< Return given SV predicition
		Acq_Command_M getAcqCommand(int32 _sv){return(result_history[_sv]);};		//!< Return acquisition result per sv

};

#endif /* SV_Select_H */
