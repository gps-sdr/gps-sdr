/*! \file GUI.h
	Define the wxWidgets GUI App and object
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

#ifndef GUI_H
#define GUI_H

/*Herein Lies Many Important Files*/
/*Note thy order is important!*/
/*----------------------------------------------------------------------------------------------*/
//#include "mcf5307.h"			//!< ColdFire stuf
#include "defines.h"			//!< GPS ICD-2000 defines
//#include "registers.h"			//!< Memmap FPGA Registers
#include "config.h"				//!< Configuration options
#include "messages.h"			//!< RS422 messages
#include "commands.h"			//!< RS422 commands
/*----------------------------------------------------------------------------------------------*/

/* Include standard headers, OS stuff */
/*----------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sched.h>
#include <curses.h>
#include <limits.h>
#include <termios.h>
/*----------------------------------------------------------------------------------------------*/

/* wxWidgets headers */
/*----------------------------------------------------------------------------------------------*/
#include "wx/wx.h"
#include <wx/app.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/dcbuffer.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/frame.h>
#include <wx/toolbar.h>
#include <wx/log.h>
#include <wx/process.h>
/*----------------------------------------------------------------------------------------------*/

/* Include standard headers, OS stuff */
/*----------------------------------------------------------------------------------------------*/
#include "threaded_object.h"
#include "pll.h"
#include "macros.h"
#include "gui_classes.h"
#include "gui_object.h"
#include "gui_serial.h"
#include "gui_pvt.h"
#include "gui_ekf.h"
#include "gui_almanac.h"
#include "gui_ephemeris.h"
#include "gui_channel.h"
#include "gui_commands.h"
#include "gui_pseudo.h"
//#include "gui_eeprom.h"
#include "gui_health.h"
#include "gui_messages.h"
#include "gui_select.h"
#include "gui_toplevel.h"
/*----------------------------------------------------------------------------------------------*/
double icn0_2_fcn0(uint32 _cn0);

#define ID_EXIT  1000
#define ID_TIMER 9999

/*----------------------------------------------------------------------------------------------*/
class GUI_App: public wxApp
{

	GUI_Toplevel *pMain;

    bool render_loop_on;
    virtual bool OnInit();
    void onIdle(wxIdleEvent& evt);

public:

    void activateRenderLoop(bool on);
    bool getRenderLoop(){return(render_loop_on);}

};
/*----------------------------------------------------------------------------------------------*/

#endif
