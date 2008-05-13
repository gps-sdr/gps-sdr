/*! \file Doxygen.h
	Mainpage for Doxygen documentation.
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


/* Doxygen Controls Go Here! */
/************************************************************************************************/
/*! \defgroup STRUCTS Structures */
/*! \defgroup CLASSES Classes */
/*! \defgroup PIPES Pipes */

/*! \mainpage Main Page

\section version Current Version

	The currently release is considered <b><i>GPS-SDR 3.0a</i></b>

	Releases are named as follows:		\n
	alpha (a): feature uncomplete		\n
	beta  (b): feature complete			\n
	no suffix: feature complete, tested, and locked (gold) \n

\section intro Introduction

	Welcome to the GPS-SDR source code and documentation. A new user to the receiver
	should be able to browse this documentation and the associated source code to obtain a 
	working knowledge of the GPS-SDR, allowing him to modify the behaviour of the
	receiver as seen fit.


\section soft-pre Software Prerequisites



\section soft-style Software Style Guide

	The GPS-SDR is coded in C++-- ("see plus plus minus minus"). Some of the more
	abstract, and performance inhibiting, aspects of C++ are not used. These include polymorphism, operator
	overloading, and inheritance. In fact the entire source could easily be converted to straight C by
	an ambitious co-op/intern who would change the classes to structures.

	The main point of the software to absorb is the idea of a "threaded object". Each discrete 
	task that the receiver must perform (i.e. decoding the satellite ephemerides) is given 
	a plainly named object and an associated thread. The object can be treated like a black box with
	an input and output(s). The threaded object	will be fed data through a blocking pipe, which will be
	read by the object's Import() method. The method will perform its duties on the data it received, and then
	send the resulting data to it's destination via a Export() method. Note that the blocking pipes
	achieve two things at once (schweet!). The pipes both allow data to flow to and from a process
	as well as timing the process. Since the process will block on its Import() call if the incoming pipe
	is empty no assignment of thread execution frequencies is required. Threads are executed as needed
	when data is made available to them.

	The actual thread of the threaded object should look something like this:

	\code 
	while(1)	
	{			
		pObject->Import();	
		pObject->Parse();	<-- more than one method can be here	
		pObject->Export();	
	}	
	\endcode

	Please note that the software is by design very OS generic. As long as whatever OS the software 
	is running on has:

	-# Blocking pipes
	-# Mutex and/or semaphores
	-# Thread priorities
	-# A Sleep() call
	
	any OS specific calls could easily be changed fit the user's OS of choice. 
	
\section overview Source Code Overview

	
	
\section doco Documentation
	The documentation is provided in both .html and
	.pdf format, but you know that already since you are reading this!


\section sloc Source Lines of Code

/************************************************************************************************/