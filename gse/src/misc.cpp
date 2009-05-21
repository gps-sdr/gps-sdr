/*----------------------------------------------------------------------------------------------*/
/*! \file misc.cpp
//
// FILENAME: misc.cpp
//
// DESCRIPTION: Functions that dont have a home
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

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
double icn0_2_fcn0(uint32 _cn0)
{

//	double cn0, NP;
//
//	if(_cn0 < 4866)
//		_cn0 = (uint32)4866;
//
//	NP = (double)_cn0 / 4096.0;
//
//	cn0 = 10*log10((NP - 1.0)/(20.0 - NP)) + 30.25;

	return(_cn0 / 128.0);

}
/*----------------------------------------------------------------------------------------------*/
