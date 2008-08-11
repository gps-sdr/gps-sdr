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

#include "gui.h"

enum
{
    ID_Quit = 1,
    ID_About,
};

IMPLEMENT_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
bool GUI_App::OnInit()
{
	frame = new GUI( wxT("GPS-SDR"), wxPoint(50,50), wxSize(800,600));
    frame->Show(TRUE);
    SetTopWindow(frame);

	/* Start up the render loop */
    activateRenderLoop(true);
    return TRUE;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_App::activateRenderLoop(bool on)
{
    if(on and !render_loop_on)
    {
        Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(GUI_App::onIdle) );
        render_loop_on = true;
    }
    else if(!on and render_loop_on)
    {
        Disconnect( wxEVT_IDLE, wxIdleEventHandler(GUI_App::onIdle) );
        render_loop_on = false;
    }
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_App::onIdle(wxIdleEvent& evt)
{
    if(render_loop_on)
    {
        frame->paintNow();
        evt.RequestMore(); //render continuously, not only once on idle
    }
}
/*----------------------------------------------------------------------------------------------*/

BEGIN_EVENT_TABLE(GUI, wxFrame)
    EVT_MENU(ID_Quit, GUI::OnQuit)
    EVT_MENU(ID_About, GUI::OnAbout)
    EVT_PAINT(GUI::paintEvent)
    EVT_CLOSE(GUI::onClose)
END_EVENT_TABLE()

/*----------------------------------------------------------------------------------------------*/
GUI::GUI(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{

	/* Create the Menu */
    wxMenu *menuFile = new wxMenu;
    menuFile->Append( ID_About, wxT("&About...") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, wxT("E&xit") );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, wxT("&File") );
    SetMenuBar( menuBar );

    /* Create a status bar */
    CreateStatusBar();
    SetStatusText( wxT("Welcome to the GPS-SDR!") );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	Main = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	Navigation = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );

	tNavigation = new wxStaticText( Navigation, -1, wxT("Crap"),  wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT, wxT("Huh"));

	Main->AddPage( Navigation, wxT("Navigation"), true );
	Acquisition = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( Acquisition, wxT("Acquisition"), false );
	Ephemeris = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( Ephemeris, wxT("Ephemeris"), false );
	Constellation = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( Constellation, wxT("Constellation"), false );
	EKF = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( EKF, wxT("EKF"), false );
	Threads = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( Threads, wxT("Threads"), false );
	Commands = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( Commands, wxT("Commands"), false );

	bSizer1->Add( Main, 1, wxEXPAND | wxALL, 5 );
	this->SetSizer( bSizer1 );
	this->Layout();

	tGUI = new Telem_2_GUI_S;

	k = 0;

	//gpipe_open = openPipe();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
bool GUI::openPipe()
{

	gpipe = open("/tmp/GUIPIPE",O_RDONLY | O_NONBLOCK);

	if(gpipe != -1)
	{
		SetStatusText(wxT("GPS pipe open"));
		return(true);
	}
	else
	{
		SetStatusText(wxT("Could not open GPS pipe"));
		return(false);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(TRUE);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::OnAbout(wxCommandEvent& WXUNUSED(event))
{

    wxMessageBox(wxT("Welcome to the GPS-SDR"),
        wxT("About GPS-SDR"), wxOK | wxICON_INFORMATION, this);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::onClose(wxCloseEvent& evt)
{
    wxGetApp().activateRenderLoop(false);
    evt.Skip(); // don't stop event, we still want window to close
    Close(TRUE);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    render(dc);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::readPipe()
{

	char *p;
	int32 nbytes, bread;

	/* Get data from pipe (1 ms) */
	nbytes = 0; p = (char *)&tGUI;
	while(nbytes < sizeof(Telem_2_GUI_S))
	{
		//signal(SIGPIPE, kill_program);
		bread = read(gpipe, &p[nbytes], PIPE_BUF);
		if(bread >= 0)
			nbytes += bread;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::render(wxDC& dc)
{

    wxString str;


	bool flag;

	/* Open the pipe */
//	if(!gpipe_open)
//		gpipe_open = openPipe();

//	readPipe();
	k++;

	flag = wxGetApp().getRenderLoop();

	str.Printf(wxT("Pipe Reads %d"),k);
	//if(flag)
	tNavigation->SetLabel(str);
	//else
		//tNavigation->SetLabel(wxT("Render Loop Off"));

	/* First try to get data from the named pipe */



	/* Next find out which panel is active */




	/* Now render the specific panel */

	usleep(10000);

}
/*----------------------------------------------------------------------------------------------*/


