/*! \file Post_Process.h
	Defines the class Post_Process
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

#ifndef Post_Process_H
#define Post_Process_H

#include "includes.h"

/*! \ingroup CLASSES
 * 
 */
typedef class Post_Process
{

	private:

		pthread_t	thread;	//!< For the thread
		FILE 		*fp;	//!< file pointer to source GPS data
		int32 		npipe;	//!< spoof the FIFO's named pipe
		int32		fifo;
		char		fname[1024];
		CPX			*buff;
		CPX 		*buff_in;
		Acq_Result_S results[NUM_CODES];

	public:

		Post_Process(char *);
		~Post_Process();
		void Open();
		void Inport();
		void Parse();
		void Export();
		void Start();
		void Stop();

};

#endif /* Post_Process_H */
