/*! \file misc.cpp
	functions that don't have a home
*/
/************************************************************************************************
Copyright 2008 Hardware Component Systems Branch (Code 596)

This file is part of the Navigator GPS Receiver

The Navigator is not free software; you must not redistribute it and/or modify it under the
terms of penalty of death. The Navigator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.

NASA Goddard Space Flight Center, Code 596, Greenbelt, MD  20771
************************************************************************************************/

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
