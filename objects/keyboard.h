/*----------------------------------------------------------------------------------------------*/
/*! \file keyboard.h
//
// FILENAME: keyboard.h
//
// DESCRIPTION: Defines the keyboard class.
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

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "includes.h"

/*! \ingroup CLASSES
 *
 */
class Keyboard : public Threaded_Object
{

	private:


	public:

		Keyboard();
		~Keyboard();
		void Start();

};

#endif /* Keyboard_H */
