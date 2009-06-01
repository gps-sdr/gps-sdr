/*----------------------------------------------------------------------------------------------*/
/*! \file sv_select.h
//
// FILENAME: sv_select.h
//
// DESCRIPTION: Defines the class sv_select
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

#ifndef SV_SELECT_H_
#define SV_SELECT_H_

#include "includes.h"
#include "ephemeris.h"
#include "channel.h"
#define EKF_STATE_INITIALIZED (1)

enum SV_SELECT_MODE
{
	ACQ_MODE_COLD,			//!< Cold start
	ACQ_MODE_WARM,			//!< Warm start (EKF)
	ACQ_MODE_HOT			//!< Hot start (PVT)
};

enum SV_SELECT_TYPE
{
	ACQ_TYPE_STRONG,		//!< Strong acquisition
	ACQ_TYPE_MEDIUM,		//!< Medium acquisition
	ACQ_TYPE_WEAK			//!< Weak acquisition
};

enum SV_SELECT_OPERATION
{
	ACQ_OPERATION_DISABLED,	//!< Type turned off
	ACQ_OPERATION_WARM,		//!< Warm only
	ACQ_OPERATION_COLD		//!< Cold and warm
};

enum SV_PREDICTION_STATE
{
	SV_NOT_PREDICTED,		//!< Not almanac == no prediction
	SV_NOT_VISIBLE,			//!< Predicted, but not visible
	SV_NOT_TRACKED,			//!< Predicted, visible, not tracked
	SV_TRACKED,				//!< Predicted, visible, tracked
};

/*! @ingroup CLASSES
	@brief SV_Select uses state information from the PVT/EKF and the almanac to
 * predict the state of the GPS constellation. This information is used to aid
 * the acquisition process. */
class SV_Select : public Threaded_Object
{

	private:

		EKF_2_SVS_S			ekf_s;							//!< Allow EKF to drive SV select if no PVT
		PVT_2_SVS_S			pvt_s;							//!< Get nav state from PVT
		SV_Select_Status_M	status;							//!< Dump out sv select status
		SV_Select_Config_S	config;							//!< Configure the SV_Select process
		Acq_Command_S		command;						//!< Interface to acquisition thread
		SPS_M				*pnav;							//!< Pointer to nav sltn
		Clock_M 			*pclock;						//!< Point to clock sltn
		Almanac_M			almanacs[MAX_SV];				//!< The decoded almanacs
		SV_Position_M		sv_positions[MAX_SV];			//!< The GPS positions calculated from the almanac
		SV_Prediction_M 	sv_prediction[MAX_SV];			//!< Predicated delay/doppler visibility, etc
		int32				mode;							//!< SV select mode (COLD, WARM, HOT)
		int32				type;							//!< WEAK or STRONG
		int32				strong_sv;						//!< The current strong SV
		int32				weak_sv;						//!< The current weak SV
		int32				acq_ticks;						//!< Number of acq ticks
		float				mask_angle;						//!< Elevation mask angle

	public:

		SV_Select();
		~SV_Select();
		void Start();					//!< Start the thread
		void Import();					//!< Get info from PVT
		void Export(int32 _sv);			//!< Export state info for the given SV
 		void UpdateState();				//!< Update acq type
 		void Acquire();					//!< Run the acquisition
		void GetAlmanac(int32 _sv);		//!< Get the most up-to-date almanacs from the ephemeris
		void SV_Predict(int32 _sv);		//!< Predict states of SVs
		void SV_Position(int32 _sv);	//!< Compute SV positions from almanac
 		uint32 SetupRequest(int32 _sv);	//!< Setup the acq request
		void MaskAngle();				//!< Calculate elevation mask angle
		void EKF_2_Nav();				//!< Get the GEONS data into the proper structure

		void setConfig(SV_Select_Config_S *_config);
		SV_Select_Config_S getConfig();
		SV_Select_Status_M getStatus();
		SV_Prediction_M getPrediction(uint32 _sv); //!< Get the SV prediction

};

#endif /* SV_SELECT_H_ */
