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
/*----------------------------------------------------------------------------------------------*/
/*! \defgroup STRUCTS Structures */
/*! \defgroup CLASSES Classes */
/*! \defgroup PIPES Pipes */

/*! \mainpage Main Page

\section version Current Version

	The currently release is considered <b><i>GPS-SDR 3.0.16a</i></b>

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

	GCC 			\n
	ncurses  		\n
	USRP library  	\n
	CPU with MMX & SSE2 extensions (Pentium 4 or later) \n

\section soft-inst Install Guide

\subsection step1 Step 1: Get Source

	GPS-SDR is version controlled using the program Git. Git was written by Linus Tvoralds to version control
	the Linux kernel. Git can be installed on your Linux box via the following command:
	
	sudo apt-get install git git-core git-cvs git-gui gitk gitweb
	
	Next clone the source:
	
	git clone git://github.com/gps-sdr/gps-sdr.git
	
	Or alternatively goto:
	
	http://github.com/gps-sdr/gps-sdr/tree/master
	
	and download the latest tarball.
	
\subsection step2 Step 2: Install Dependancies

	Dependancy 1, ncurses
	
	sudo apt-get install ncurses-dev
	
\subsection step3 Step 3: (OPTIONAL) Install GNURadio

	This is only necessary if you want to build the C++ code that drives the USRP and DBS-RX daughtercard.	
	
	Goto http://gnuradio.org/trac/wiki/BuildGuide , build and install the latest version of GNURadio. Note where you
	installed GNURadio. Open up the makefile in ./gps-sdr/usrp and edit the Makefile, set the GNURADIO variable
	to where GNURadio was build and installed. CD to the ./gps-sdr/usrp directory and type "make", if everything
	is configured correctly the executable "usrp_sdr" should have been built. Invoking this program should 
	start the USRP and configure the DBS-RX daughtercard, tuning the USRP to receive GPS L1 C/A code. 
	
\subsection step4 Step 4: Build gps-sdr	
	
	CD to the ./gps-sdr directory, type "make". The source should compile into 4 executables, simd_test, fft_test
	acq_test, and gps-sdr. Just to make sure, run simd_test and verify that all the simd functions return PASSED.
	acq_test and fft_test are NOT supported, they may or may not work at any time! acq_test can be used to perform
	an acquisition on a recorded set of data. Type "acq_test --help" for details. 

\subsection step5 Step 5: Run gps-sdr

	To run the receiver first start the usrp_sdr executable to startup the USRP and DBS-RX. This will also
	create a named pipe over which the IF samples will be streamed to the receiver. Open a new shell window
	and invoke the gps-sdr program. If successful you will see a screen like this:

\verbatim
	
  FIFO:  999     9801    2325    7

  Ch#  SV   CL       Faccel          Doppler     CN0   BE       Locks        Power   Active
  -----------------------------------------------------------------------------------------
   0    1    4       -4.258          999.625   45.37    8   fB-------      2812044        9
   1    3    4       -4.295         1290.562   46.28   11   fB-------      2570315        9
   2    8    4       -0.532          973.312   45.72   10   fB-------      2583762        9
   3   14    4      -12.436        -2956.562   45.04   15   fB-------      2343204        9
   4   16    4       -3.151        -1337.938   45.52   14   fB-------      2400923        9
   5   18    4       -2.017         2830.062   45.70   15   fB-------      2314470        9
   6   19    4       -1.934         2103.938   45.98    0   fB-------      2053206        9
   7   22    4       -3.039        -2604.812   46.16   10   fB-------      2590615        9
   8   25    4        0.162        -2269.750   45.38    1   fB-------      1986522        9
   9   31    1        0.581         3650.375   40.01   14   f--------       201520        9
  10    1    4       -4.337          999.562   45.08    7   fB-------      2811940        9
  11   10    1     -515.769       -34540.562   26.64   16   f--------        10170        0

  Ch#  SV         SV Time        VX        VY        VZ    Transit Time        Residual
  -------------------------------------------------------------------------------------
   0   --  --------------  --------  --------  --------  --------------  --------------
   1   --  --------------  --------  --------  --------  --------------  --------------
   2   --  --------------  --------  --------  --------  --------------  --------------
   3   --  --------------  --------  --------  --------  --------------  --------------
   4   --  --------------  --------  --------  --------  --------------  --------------
   5   --  --------------  --------  --------  --------  --------------  --------------
   6   --  --------------  --------  --------  --------  --------------  --------------
   7   --  --------------  --------  --------  --------  --------------  --------------
   8   --  --------------  --------  --------  --------  --------------  --------------
   9   --  --------------  --------  --------  --------  --------------  --------------
  10   --  --------------  --------  --------  --------  --------------  --------------
  11   --  --------------  --------  --------  --------  --------------  --------------

  Last Acq: STRONG, 11,  818.20,  -35500,   14893789

  Nav SVs:       0
  Receiver Tic:  99
                               X               Y               Z
  Position (m):             0.00            0.00            0.00
  Vel (cm/s):               0.00            0.00            0.00

                             Lat            Long             Alt
                     0.000000000     0.000000000          0.0000

                      Clock Bias      Clock Rate        GPS Time
                        0.000000       0.0000000        9.900000

  EPH:
  ALM:
  
\endverbatim

  Pressing the "a" key will bring you to another screen showing the almanac prediction routine, press "n" to return
  to the main display.
	
\subsection step6 Step 6: Get HELP!

	Please visit my website at http://www.gps-sdr.com and post a question on the latest topic, I will try to answer your
	questions as soon as possible!


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

	
	
/*----------------------------------------------------------------------------------------------*/
