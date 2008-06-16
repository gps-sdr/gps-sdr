/*! \file Protos.h
	Default includes for each source file in the project.
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


/* These are all found in Main() */
/*----------------------------------------------------------------------------------------------*/
void Parse_Arguments(int32 _argc, char* _argv[]);	//!< Parse command line arguments to setup functionality
int32 Hardware_Init(void);							//!< Initialize any hardware (for realtime mode)
int32 Object_Init(void);								//!< Initialize all threaded objects and global variables
int32 Pipes_Init(void);								//!< Initialize all pipes
int32 Thread_Init(void);								//!< Finally start up the threads
void Thread_Shutdown(void);							//!< First step to shutdown, stopping the threads
void Pipes_Shutdown(void);							//!< Close all the pipes
void Object_Shutdown(void);							//!< Delete/free all objects
void Hardware_Shutdown(void);						//!< Shutdown any hardware
/*----------------------------------------------------------------------------------------------*/

/* Found in Misc.cpp */
/*----------------------------------------------------------------------------------------------*/
int32 code_gen(CPX *_dest, int32 _prn);
void sine_gen(CPX *_dest, double _f, double _fs, int32 _samps);
void sine_gen(CPX *_dest, double _f, double _fs, int32 _samps, double _p);
void wipeoff_gen(MIX *_dest, double _f, double _fs, int32 _samps);
void resample(CPX *_dest, CPX *_source, double _fdest, double _fsource, int32 _samps);
void downsample(CPX *_dest, CPX *_source, double _fdest, double _fsource, int32 _samps);
void init_agc(CPX *_buff, int32 _samps, int32 bits, int32 *scale);
int32 run_agc(CPX *_buff, int32 _samps, int32 bits, int32 *scale);
int32 AtanApprox(int32 y, int32 x);
int32 Atan2Approx(int32 y, int32 x);
int32 Invert4x4(double A[4][4], double B[4][4]);
/*----------------------------------------------------------------------------------------------*/

