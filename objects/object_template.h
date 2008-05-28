/*! \file OBJECT.h
	Defines the class OBJECT
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

#ifndef OBJECT_H
#define OBJECT_H

#include "Includes.h"

/*! \ingroup CLASSES
 * 
 */
typedef class OBJECT
{

	private:

		pthread_t thread;	//!< For the thread
		int32 temp;

	public:

		OBJECT();
		~OBJECT();
		void Inport();
		void Parse();
		void Export();
		void Start();
		void Stop();

};

#endif /* OBJECT_H */
