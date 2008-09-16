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

/* Include standard headers, OS stuff */
/*----------------------------------------------------------------------------------------------*/
#include "includes.h"
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
#include <wx/toolbar.h>
#include <wx/log.h>
#include <wx/process.h>
/*----------------------------------------------------------------------------------------------*/

#define ID_EXIT 1000


/*----------------------------------------------------------------------------------------------*/
class GUI: public wxFrame
{
public:


	private:

		/* Capture the named pipe */
		int 			k;
		int 			last_k;
		int				active_panel; /* Always hold the active panel */
		int				gpipe;
		bool			gpipe_open;
		wxString		status_str;
		Telem_2_GUI_S 	tGUI;

		/* GPS-SDR exec variables */
		wxInputStream* 	gps_in;
		wxOutputStream* gps_out;
		wxProcess*		gps_proc;
		int				gps_pid;
		int				gps_active;

		/* GPS-USRP exec variables */
		wxInputStream* 	usrp_in;
		wxOutputStream* usrp_out;
		wxProcess*		usrp_proc;
		int				usrp_pid;
		int				usrp_active;

	protected:

		wxTimer 		*timer;

		wxStatusBar*	m_statusbar;
		wxMenuBar* 		m_menubar;

		wxMenu* 		Menu;
		wxNotebook* 	Main;

		/* Navigation Tab */
		wxPanel* 		pNavigation;
		wxSizer* 		sNavigation;
		wxTextCtrl* 	tNavigation;

		/* Acquisition Tab */
		wxPanel* 		pAcquisition;
		wxSizer* 		sAcquisition;
		wxTextCtrl* 	tAcquisition;

		/* Ephemeris Tab */
		wxPanel* 		pEphemeris;
		wxSizer* 		sEphemeris;
		wxTextCtrl* 	tEphemeris;

		/* Constellation Tab */
		wxPanel* 		pConstellation;
		wxSizer* 		sConstellation;
		wxTextCtrl* 	tConstellation;

		/* EKF Tab */
		wxPanel* 		pEKF;
		wxSizer* 		sEKF;
		wxTextCtrl* 	tEKF;

		/* Threads Tab */
		wxPanel* 		pThreads;
		wxSizer* 		sThreads;
		wxTextCtrl* 	tThreads;

		/* Commands Tab */
		wxPanel* 		pCommands;
		wxSizer* 		sCommands;
		wxTextCtrl* 	tCommands;

		/* Logging Tab */
		wxPanel* 		pLogging;
		wxBoxSizer* 	sLogging;
		wxTextCtrl* 	tLogging;
		wxCheckListBox *cLogging;

		/* Feedbck Tab */
		wxPanel* 		pFeedback;
		wxSizer* 		sFeedback;
		wxTextCtrl* 	tFeedback;


	public:

		GUI(const wxString& title, const wxPoint& pos, const wxSize& size);
		~GUI();
		bool openPipe();
		void readPipe();

		void onTimer(wxTimerEvent& evt);
		void onClose(wxCloseEvent& evt);
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnStart(wxCommandEvent& event);
		void OnStop(wxCommandEvent& event);
		void OnUSRPStart(wxCommandEvent& event);
		void OnUSRPStop(wxCommandEvent& event);
	    void paintEvent(wxPaintEvent& evt);
	    void paintNow();
	    void render(wxDC& dc);

	    void initNavigation();
	    void renderNavigation();
			void PrintChan(wxTextCtrl* _text);
			void PrintNav(wxTextCtrl* _text);
			void PrintSV(wxTextCtrl* _text);

		void initAcquisition();
	    void renderAcquisition();
			void PrintHistory(wxTextCtrl* _text);

		void initEphemeris();
	    void renderEphemeris();
			void PrintEphem(wxTextCtrl* _text);
			void PrintAlmanac(wxTextCtrl* _text);

		void initConstellation();
	    void renderConstellation();

	    void initEKF();
	    void renderEKF();

	    void initThreads();
	    void renderThreads();

	    void initCommands();
	    void renderCommands();

	    void initLogging();
	    void renderLogging();

	    void initFeedback();
	    void renderFeedback();

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

#endif
