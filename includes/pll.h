///*----------------------------------------------------------------------------------------------*/
///*! \file pll.h
////
//// FILENAME: pll.h
////
//// DESCRIPTION: Fixed point loop defines
////
//// DEVELOPERS: Gregory W. Heckler (2003-2009)
////
//// LICENSE TERMS: Copyright (c) Gregory W. Heckler 2009
////
//// This file is part of the GPS Software Defined Radio (GPS-SDR)
////
//// The GPS-SDR is free software; you can redistribute it and/or modify it under the terms of the
//// GNU General Public License as published by the Free Software Foundation; either version 2 of
//// the License, or (at your option) any later version. The GPS-SDR is distributed in the hope that
//// it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
//// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
//// more details.
////
//// Note:  Comments within this file follow a syntax that is compatible with
////        DOXYGEN and are utilized for automated document extraction
////
//// Reference:
//*/
///*----------------------------------------------------------------------------------------------*/
//
//#ifndef _PLL_H
//#define _PLL_H
//
///* Tracking Defines for Fixed Point Loops */
///*----------------------------------------------------------------------------------------------*/
//#define LOGS1				12
//#define TWO32TSAMP 			2097
//#define TWO32TSAMP_T3		16777
//#define ZERO_DOPPLER_RATE	(604000)	//!< 604 kHz
//#define NCO_CARR_INCR_NOM	(0x4B800000)//!< 604000 Hz
//#define NCO_CODE_INCR_NOM	(0x3FF00000)//!< = 1.023e6 Mcps, 1 chips/s = 1049 in terms of the code NCO
//#define HZ_2_NCO_CARR_INCR	(2.097152000000000e+03) //!< These should correspond to the above values
//#define NCO_CARR_INCR_2_HZ	(4.768371582031250e-04) //!< These should correspond to the above values
///*----------------------------------------------------------------------------------------------*/
//
///* 1ms Integration Constants */
///*----------------------------------------------------------------------------------------------*/
////PLL filter coeffs for 18 Hz bandwidth and 0.001 update period
//#define B1 7874
//#define GPS_B2 378
//#define B3 35898
//#define BT 4
//
////FLL filter coeffs for 7 Hz bandwidth and 0.001 update period
//#define BF1 113717
//#define BF2 12175
//
///*//PLL filter coeffs for 15 Hz bandwidth and 0.001 update period
//#define B1 4557
//#define GPS_B2 262
//#define B3 29915
//#define BT 4
//
////FLL filter coeffs for 5 Hz bandwidth and 0.001 update period
//#define BF1 58019
//#define BF2 8696*/
//
//
//////2nd order DLL filter coeffs for 20 Hz bandwidth and 0.001 update period
////#  define B1_DLL_2ND 5833
////#  define B2_DLL_2ND 218556
//////2nd order DLL filter coeffs for 10 Hz bandwidth and 0.001 update period
////#  define B1_DLL_2ND 1458
////#  define B2_DLL_2ND 109278
//
////2nd order DLL filter coeffs for 2 Hz bandwidth and 0.001 update period
//#  define B1_DLL_2ND 58
//#  define B2_DLL_2ND 21856
///*----------------------------------------------------------------------------------------------*/
//
//
///* 20ms Integration Constants */
///*----------------------------------------------------------------------------------------------*/
//////PLL filter coeffs for 7 Hz bandwidth and 0.020 update period
////#define PLLSCALE 12
////#define B1_20 9262
////#define B2_20 1142
////#define B3_20 13960
////#define BT_20 82
//
//
////PLL filter coeffs for 5 Hz bandwidth and 0.02 update period, scaled by 1024
//#  define PLLSCALE 10
//#  define B1_20 844
//#  define B2_20 146
//#  define B3_20 2493
//#  define BT_20 20
//
//
////FLL filter coeffs for 5 Hz bandwidth and 0.020 update period
//#define BF1_20 58019
//#define BF2_20 8696
//
//
////2nd order DLL filter coeffs for 2 Hz bandwidth and 0.02 update period
//#  define B1_DLL_2ND_20 1167
//#  define B2_DLL_2ND_20 21856
///*----------------------------------------------------------------------------------------------*/
//
//#endif
