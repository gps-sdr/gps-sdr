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
	frame = new GUI( wxT("GPS-SDR"), wxPoint(50,50), wxSize(1024,768));
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

	pNavigation = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pNavigation, wxT("Navigation"), true );

	pAcquisition = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pAcquisition, wxT("Acquisition"), false );

	pEphemeris = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pEphemeris, wxT("Ephemeris"), false );

	pConstellation = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pConstellation, wxT("Constellation"), false );

	pEKF = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pEKF, wxT("EKF"), false );

	pThreads = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pThreads, wxT("Threads"), false );

	pCommands = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pCommands, wxT("Commands"), false );

	bSizer1->Add( Main, 1, wxEXPAND | wxALL, 5 );
	this->SetSizer( bSizer1 );
	this->Layout();

	sNavigation = new wxBoxSizer(wxVERTICAL);
	pNavigation->SetSizer(sNavigation);

	sAcquisition = new wxBoxSizer(wxVERTICAL);
	pAcquisition->SetSizer(sAcquisition);

	sEphemeris = new wxBoxSizer(wxVERTICAL);
	pEphemeris->SetSizer(sEphemeris);

	sConstellation = new wxBoxSizer(wxVERTICAL);
	pConstellation->SetSizer(sConstellation);

	sEKF = new wxBoxSizer(wxVERTICAL);
	pEKF->SetSizer(sEKF);

	sThreads = new wxBoxSizer(wxVERTICAL);
	pThreads->SetSizer(sThreads);

	sCommands = new wxBoxSizer(wxVERTICAL);
	pCommands->SetSizer(sCommands);

	tNavigation = new wxTextCtrl(pNavigation, -1, wxT("Navigation"), sNavigation->GetPosition(), sNavigation->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sNavigation->Add(tNavigation, 1, wxEXPAND | wxALL, 4);
	tNavigation->SetFont(wxFont(12, wxTELETYPE, wxNORMAL, wxNORMAL));

	tAcquisition = new wxTextCtrl(pAcquisition, -1, wxT("Acquisition"), sAcquisition->GetPosition(), sAcquisition->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sAcquisition->Add(tAcquisition, 1, wxEXPAND | wxALL, 4);
	tAcquisition->SetFont(wxFont(12, wxTELETYPE, wxNORMAL, wxNORMAL));

	tEphemeris = new wxTextCtrl(pEphemeris, -1, wxT("Ephemeris"), sEphemeris->GetPosition(), sEphemeris->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sEphemeris->Add(tEphemeris, 1, wxEXPAND | wxALL, 4);
	tEphemeris->SetFont(wxFont(12, wxTELETYPE, wxNORMAL, wxNORMAL));

	tConstellation = new wxTextCtrl(pConstellation, -1, wxT("Constellation"), sConstellation->GetPosition(), sConstellation->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sConstellation->Add(tConstellation, 1, wxEXPAND | wxALL, 4);
	tConstellation->SetFont(wxFont(12, wxTELETYPE, wxNORMAL, wxNORMAL));

	tEKF = new wxTextCtrl(pEKF, -1, wxT("EKF"), sAcquisition->GetPosition(), sAcquisition->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sEKF->Add(tEKF, 1, wxEXPAND | wxALL, 4);
	tEKF->SetFont(wxFont(12, wxTELETYPE, wxNORMAL, wxNORMAL));

	tThreads = new wxTextCtrl(pThreads, -1, wxT("Threads"), sThreads->GetPosition(), sThreads->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sThreads->Add(tThreads, 1, wxEXPAND | wxALL, 4);
	tThreads->SetFont(wxFont(12, wxTELETYPE, wxNORMAL, wxNORMAL));

	tCommands = new wxTextCtrl(pCommands, -1, wxT("Commands"), sCommands->GetPosition(), sCommands->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sCommands->Add(tCommands, 1, wxEXPAND | wxALL, 4);
	tCommands->SetFont(wxFont(12, wxTELETYPE, wxNORMAL, wxNORMAL));

	k = 0;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI::~GUI()
{

	if(gpipe_open)
		close(gpipe);




}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
bool GUI::openPipe()
{

	gpipe = open("/tmp/GUIPIPE",O_RDONLY | O_NONBLOCK);

	if(gpipe != -1)
		return(true);
	else
		return(false);

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
    //Close(TRUE);
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
void kill_program_gui(int _sig)
{
	//grun = false;
	//printf("Lost GPS-GUI!\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::readPipe()
{

	char *p;
	int32 nbytes, bread;

	bread = read(gpipe, &tGUI, sizeof(Telem_2_GUI_S));

	if(bread == sizeof(Telem_2_GUI_S))
		k++;

	/* Get data from pipe (1 ms) */
//	nbytes = 0; p = (char *)&tGUI;
//	while(nbytes < sizeof(Telem_2_GUI_S))
//	{
//		signal(SIGPIPE, kill_program_gui);
//		bread = read(gpipe, &p[nbytes], PIPE_BUF);
//		if(bread >= 0)
//			nbytes += bread;
//	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::render(wxDC& dc)
{
	bool flag;
	int page;
    wxString str;

	/* Open the pipe */
	if(!gpipe_open)
		gpipe_open = openPipe();

	/* Get data from the pipe */
	if(gpipe_open)
		readPipe();

	/* Render proper page */
	page = Main->GetSelection();
	switch(page)
	{
		case 0: renderNavigation(); 	break;
		case 1: renderAcquisition(); 	break;
		case 2: renderEphemeris(); 		break;
		case 3: renderConstellation(); 	break;
		case 4: renderEKF(); 			break;
		case 5: renderThreads(); 		break;
		case 6: renderCommands(); 		break;
		default: renderNavigation();	break;
	}

	/* Status at the bottom */
	str.Printf(wxT("Pipe Reads %d, Selected Page %d"),k,page);
	SetStatusText(str);

	flag = wxGetApp().getRenderLoop();

	usleep(10000);
}
/*----------------------------------------------------------------------------------------------*/

//FIFO_2_Telem_S 	tFIFO;
//PVT_2_Telem_S 	tNav;
//Chan_Packet_S 	tChan[MAX_CHANNELS];
//Acq_Result_S		tAcq;
//Ephem_2_Telem_S 	tEphem;
//SV_Select_2_Telem_S tSelect;

/*----------------------------------------------------------------------------------------------*/
void GUI::renderNavigation()
{
	Chan_Packet_S *p;
	Nav_Solution_S		*pNav		= &tGUI.tNav.master_nav;				/* Navigation Solution */
	Clock_S				*pClock		= &tGUI.tNav.master_clock;			/* Clock solution */

    wxString str;
    wxString str2;
	int lcv, lcv2;
	char buff[1024];
	float cn0;

	tNavigation->Clear();

	str.Printf(wxT("FIFO:\t%d\t%d\t%d\t%d\n\n"),(FIFO_DEPTH-(tGUI.tFIFO.head-tGUI.tFIFO.tail)) % FIFO_DEPTH,tGUI.tFIFO.count,
			tGUI.tFIFO.agc_scale,tGUI.tFIFO.overflw);
	tNavigation->AppendText(str);

	str.Printf(wxT("Ch#  SV   CL       Faccel          Doppler     CN0   BE       Locks        Power   Active\n"));
	tNavigation->AppendText(str);

	str.Printf(wxT("-----------------------------------------------------------------------------------------\n"));
	tNavigation->AppendText(str);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		p = &tGUI.tChan[lcv];
		if(p->count > 3000)
		{

			//strcpy(buff, "---------");

			str2.Clear();

			/*Flag buffer*/
			((int32)p->fll_lock_ticks > 200) ?	str2 += 'p' : str2 += 'f';
			((int32)p->bit_lock) ? 				str2 += 'B' : str2 += '-';
			((int32)p->frame_lock) ? 			str2 += 'F' : str2 += '-';
			(pNav->nsvs >> lcv) & 0x1 ? 		str2 += 'N' : str2 += '-';

			for(lcv2 = 1; lcv2 < 6; lcv2++)
			{
				if((int32)p->subframe == lcv2)
					str2 += (char)((int32)p->subframe + 48);
				else
					str2 += '-';
			}

//			if(((int32)p->subframe > 0) &&  ((int32)p->subframe < 6))
//				str2 += (char)((int32)p->subframe + 48);

			cn0 = p->CN0 > p->CN0_old ? p->CN0 : p->CN0_old;

			str.Printf(wxT("%2d   %2d   %2d   %10.3f   %14.3f   %5.2f   %2d   %s   %10.0f   %6d\n"),
				lcv,
				(int32)p->sv+1,
				(int32)p->len,
				p->w,
				p->carrier_nco - IF_FREQUENCY,
				cn0,
				(int32)p->best_epoch,
				str2.c_str(),
				p->P_avg,
				(int32)p->count/1000);

			tNavigation->AppendText(str);
		}
		else
		{
			str.Printf(wxT("%2d   --   --   ----------   --------------   -----   --   ---------   ----------   ------\n"),lcv);
			tNavigation->AppendText(str);
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::renderAcquisition()
{
	//wxTextCtrl *textLog;
//	textLog = new wxTextCtrl(Acquisition, wxID_ANY, _T("Log\n"), wxPoint(0, 250), wxSize(100, 50), wxTE_MULTILINE, wxDefaultValidator, _T("Crap"));
//	mysizer->Add(textLog, 1, wxEXPAND | wxALL, 5);


	//sAcquisition->




}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::renderEphemeris()
{









}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::renderConstellation()
{









}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::renderEKF()
{









}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::renderThreads()
{









}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::renderCommands()
{









}
/*----------------------------------------------------------------------------------------------*/
