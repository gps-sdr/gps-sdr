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

/* wxWidgets headers */
/*----------------------------------------------------------------------------------------------*/
#include "wx/wx.h"
#include <wx/app.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
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
#include "includes.h"
#include "gui_classes.h"
#include "gui_serial.h"
#include "gui_default.h"
#include "gui_almanac.h"
#include "gui_ephemeris.h"
#include "gui_channel.h"
#include "gui_commands.h"
#include "gui_toplevel.h"
/*----------------------------------------------------------------------------------------------*/


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
