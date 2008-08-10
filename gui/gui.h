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
#include <fcntl.h>
#include <sched.h>
#include <limits.h>
/*----------------------------------------------------------------------------------------------*/

/* wxWidgets headers */
/*----------------------------------------------------------------------------------------------*/
#include "wx/wx.h"
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
/*----------------------------------------------------------------------------------------------*/

#define ID_EXIT 1000

/*----------------------------------------------------------------------------------------------*/
class GUI: public wxFrame
{
public:


	private:

		int k;

	protected:

		wxStatusBar*	m_statusbar;
		wxMenuBar* 		m_menubar;
		wxMenu* 		file;
		wxNotebook* 	Main;
		wxPanel* 		Navigation;
		wxPanel* 		Acquisition;
		wxPanel* 		Ephemeris;
		wxPanel* 		Constellation;
		wxPanel* 		EKF;
		wxPanel* 		Threads;
		wxPanel* 		Commands;

		wxStaticText* 	tNavigation;

		int				active_panel; /* Always hold the active panel */

	public:

		GUI(const wxString& title, const wxPoint& pos, const wxSize& size);
		void onClose(wxCloseEvent& evt);
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
	    void paintEvent(wxPaintEvent& evt);
	    void paintNow();
	    void render( wxDC& dc );

	    void renderNavigation();
	    void renderAcquisition();
	    void renderEphemeris();
	    void renderConstellation();
	    void renderEKF();
	    void renderThreads();
	    void renderCommands();

    DECLARE_EVENT_TABLE()
};
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
class GUI_App: public wxApp
{

	GUI *frame;
    bool render_loop_on;
    virtual bool OnInit();
    void onIdle(wxIdleEvent& evt);

public:
    void activateRenderLoop(bool on);
    bool getRenderLoop(){return(render_loop_on);}

};
/*----------------------------------------------------------------------------------------------*/
