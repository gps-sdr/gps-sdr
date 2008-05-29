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
typedef class SV_Select
{

	private:

		pthread_t 			thread;							//!< For the thread
		PVT_2_SV_Select_S	input_s;						//!< Get nav state from PVT
		Acq_Result_S		result;							//!< Acquisition result
		Acq_Request_S		request;						//!< Acquisition request
		Nav_Solution_S 		*pnav;							//!< Pointer to nav sltn
		Clock_S 			*pclock;						//!< Point to clock sltn
		Almanac_S			almanacs[NUM_CODES];			//!< The decoded almanacs
		SV_Position_S		sv_positions[NUM_CODES];		//!< The GPS positions calculated from the almanac
		Acq_Predicted_S 	sv_prediction[NUM_CODES];		//!< Predicated delay/doppler visibility, etc
		Acq_History_S		sv_history[NUM_CODES];			//!< Keep track of acquisition attempts for each SV
		SV_Select_2_Telem_S	output_s;						//!< Send predicted states to telemetry		
		int32				mode;							//!< SV select mode (COLD, WARM, HOT)
		int32				type[NUM_CODES];				//!< Type (STRONG, MEDIUM, WEAK)
		int32				sv;								//!< Current SV
		int32				acq_ticks;
		float				mask_angle;						//!< Elevation mask angle
		 						
	public:

		SV_Select();
		~SV_Select();
		void Inport();
		void Export();					//!< Export something
		void Start();					//!< Start the thread
		void Stop();					//!< Stop the thread
 		void UpdateState();				//!< Update acq type
 		void Acquire();					//!< Run the acquisition
		void GetAlmanac(int32 _sv);		//!< Get the most up-to-date almanacs from the ephemeris
		void SV_Predict(int32 _sv);		//!< Predict states of SVs
		void SV_Position(int32 _sv);	//!< Compute SV positions from almanac
		void SV_LatLong(int32 _sv);		//!< Compute SV's lat and long
		void SetState(int32 _state); 	//!< Set state
 		bool SetupRequest();			//!< Setup the acq request
 		void ProcessResult();			//!< Take the result and do something with it!		
		void MaskAngle();				//!< Calculate elevation mask angle

};

#endif /* SV_Select_H */
