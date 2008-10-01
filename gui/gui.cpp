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
    ID_Start,
    ID_Stop,
    ID_USRP_Start,
    ID_USRP_Stop,
    ID_Timer,
};

IMPLEMENT_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
bool GUI_App::OnInit()
{
	frame = new GUI( wxT("GPS-SDR"), wxPoint(50,50), wxSize(1024,768));
    frame->Show(TRUE);
    SetTopWindow(frame);

	/* Start up the render loop */
//    activateRenderLoop(true);
//    return TRUE;

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


/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI, wxFrame)
    EVT_MENU(ID_Quit, GUI::OnQuit)
    EVT_MENU(ID_About, GUI::OnAbout)
    EVT_MENU(ID_Start, GUI::OnStart)
    EVT_MENU(ID_Stop, GUI::OnStop)
    EVT_MENU(ID_USRP_Start, GUI::OnUSRPStart)
    EVT_MENU(ID_USRP_Stop, GUI::OnUSRPStop)
    EVT_TIMER(ID_Timer, GUI::onTimer)
    EVT_PAINT(GUI::paintEvent)
    EVT_CLOSE(GUI::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI::GUI(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{

	wxBoxSizer* sizer;

	/* Create the Menu */
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_About, wxT("&About...") );
    menuFile->AppendSeparator();
    menuFile->Append(ID_Quit, wxT("E&xit") );

    wxMenu *menuReceiver = new wxMenu;
    menuReceiver->Append(ID_Start, wxT("&Start") );
    menuReceiver->Append(ID_Stop, wxT("S&top") );

    wxMenu *menuUSRP = new wxMenu;
    menuUSRP->Append(ID_USRP_Start, wxT("Sta&rt") );
    menuUSRP->Append(ID_USRP_Stop, wxT("St&op") );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuReceiver, wxT("&Receiver"));
    menuBar->Append(menuUSRP, wxT("&USRP"));
    SetMenuBar(menuBar);

    /* Create a status bar */
    CreateStatusBar();
    SetStatusText(wxT("Welcome to the GPS-SDR!"));

	Main = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	sizer = new wxBoxSizer( wxVERTICAL );
	sizer->Add(Main, 1, wxEXPAND | wxALL, 5);
	this->SetSizer(sizer);
	this->Layout();

	initNavigation();
	initAcquisition();
	initEphemeris();
	initConstellation();
	initEKF();
	initThreads();
	initCommands();
	initConfig();

	Main->SetSelection(0);

	k = 0;

    timer = new wxTimer(this, ID_Timer);
    timer->Start(66, wxTIMER_CONTINUOUS); //Shoot for 15 fps

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::initNavigation()
{

	pNavigation = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage(pNavigation, wxT("Navigation"), true );

	sNavigation = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer *s11;
	wxStaticBoxSizer *s12;

	s11 = new wxStaticBoxSizer(wxHORIZONTAL, pNavigation, wxT("Tracking"));
	s12 = new wxStaticBoxSizer(wxHORIZONTAL, pNavigation, wxT("Navigation"));

	sNavigation->Add(s11, 3, wxEXPAND | wxALL, 4);
	sNavigation->Add(s12, 2, wxEXPAND | wxALL, 4);

	tTracking = new wxTextCtrl(pNavigation, -1, wxT("Tracking"), s11->GetPosition(), s11->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	tTracking->SetFont(wxFont(10, wxTELETYPE, wxNORMAL, wxNORMAL));
	s11->Add(tTracking, 1, wxEXPAND | wxALL, 4);

	tNavigation = new wxTextCtrl(pNavigation, -1, wxT("Navigation"), s12->GetPosition(), s12->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	tNavigation->SetFont(wxFont(10, wxTELETYPE, wxNORMAL, wxNORMAL));
	s12->Add(tNavigation, 1, wxEXPAND | wxALL, 4);

	pNavigation->SetSizer(sNavigation);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::initAcquisition()
{

	pAcquisition = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pAcquisition, wxT("Acquisition"), true);

	sAcquisition = new wxBoxSizer(wxVERTICAL);
	pAcquisition->SetSizer(sAcquisition);

	tAcquisition = new wxTextCtrl(pAcquisition, -1, wxT("Acquisition"), sAcquisition->GetPosition(), sAcquisition->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sAcquisition->Add(tAcquisition, 1, wxEXPAND | wxALL, 4);
	tAcquisition->SetFont(wxFont(10, wxTELETYPE, wxNORMAL, wxNORMAL));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::initEphemeris()
{

	pEphemeris = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pEphemeris, wxT("Ephemeris"), true );

	sEphemeris = new wxBoxSizer(wxVERTICAL);
	pEphemeris->SetSizer(sEphemeris);

	tEphemeris = new wxTextCtrl(pEphemeris, -1, wxT("Ephemeris"), sEphemeris->GetPosition(), sEphemeris->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sEphemeris->Add(tEphemeris, 1, wxEXPAND | wxALL, 4);
	tEphemeris->SetFont(wxFont(10, wxTELETYPE, wxNORMAL, wxNORMAL));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::initConstellation()
{

	pConstellation = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pConstellation, wxT("Constellation"), true );

	sConstellation = new wxBoxSizer(wxVERTICAL);
	pConstellation->SetSizer(sConstellation);

	tConstellation = new wxTextCtrl(pConstellation, -1, wxT("Constellation"), sConstellation->GetPosition(), sConstellation->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sConstellation->Add(tConstellation, 1, wxEXPAND | wxALL, 4);
	tConstellation->SetFont(wxFont(10, wxTELETYPE, wxNORMAL, wxNORMAL));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::initEKF()
{

	pEKF = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pEKF, wxT("EKF"), true );

	sEKF = new wxBoxSizer(wxVERTICAL);
	pEKF->SetSizer(sEKF);

	tEKF = new wxTextCtrl(pEKF, -1, wxT("EKF"), sEKF->GetPosition(), sEKF->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sEKF->Add(tEKF, 1, wxEXPAND | wxALL, 4);
	tEKF->SetFont(wxFont(10, wxTELETYPE, wxNORMAL, wxNORMAL));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::initThreads()
{

	pThreads = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pThreads, wxT("Threads"), true );

	sThreads = new wxBoxSizer(wxVERTICAL);
	pThreads->SetSizer(sThreads);

	tThreads = new wxTextCtrl(pThreads, -1, wxT("Threads"), sThreads->GetPosition(), sThreads->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sThreads->Add(tThreads, 1, wxEXPAND | wxALL, 4);
	tThreads->SetFont(wxFont(10, wxTELETYPE, wxNORMAL, wxNORMAL));

}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void GUI::initCommands()
{

	pCommands = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pCommands, wxT("Commands"), true );

	sCommands = new wxBoxSizer(wxVERTICAL);
	pCommands->SetSizer(sCommands);

	tCommands = new wxTextCtrl(pCommands, -1, wxT("Commands"), sCommands->GetPosition(), sCommands->GetSize(), wxTE_MULTILINE | wxTE_READONLY);
	sCommands->Add(tCommands, 1, wxEXPAND | wxALL, 4);
	tCommands->SetFont(wxFont(10, wxTELETYPE, wxNORMAL, wxNORMAL));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::initConfig()
{

	pConfig = new wxPanel( Main, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	Main->AddPage( pConfig, wxT("Config"), true );

	wxStaticBoxSizer *s1;

	wxStaticBoxSizer *s11;
	wxStaticBoxSizer *s12;

	wxBoxSizer *s121;
	wxBoxSizer *s122;

	sConfig = new wxBoxSizer(wxHORIZONTAL);

	s11 = new wxStaticBoxSizer(wxHORIZONTAL, pConfig, wxT("USRP Config"));
	s12 = new wxStaticBoxSizer(wxHORIZONTAL, pConfig, wxT("File Names"));

	s121 = new wxBoxSizer(wxVERTICAL);
	s122 = new wxBoxSizer(wxVERTICAL);

	wxArrayString str;

	str.Add(wxT("All"),1);
	str.Add(wxT("Navigation"),1);
	str.Add(wxT("Pseudoranges"),1);
	str.Add(wxT("Satellites"),1);
	str.Add(wxT("Tracking"),1);
	str.Add(wxT("Google Earth"),1);

	cConfig = new wxCheckListBox(pConfig, -1, wxDefaultPosition, wxDefaultSize, str, str.GetCount(), wxDefaultValidator);
	s121->Add(new wxStaticText(pConfig, wxID_ANY, wxT("Choices")), 0, wxALIGN_CENTER | wxALL, 4);
	s121->Add(cConfig, 1, wxALIGN_CENTER | wxALL, 4);

	s122->Add(new wxStaticText(pConfig, wxID_ANY, wxT("File Path/Name")), 1, wxALIGN_CENTER | wxALL, 4);
	s122->Add(new wxTextCtrl(pConfig, -1, wxGetCwd(),				wxDefaultPosition, wxDefaultSize), 1, wxEXPAND);
	s122->Add(new wxTextCtrl(pConfig, -1, wxT("navigation.tlm"), 	wxDefaultPosition, wxDefaultSize), 1, wxEXPAND);
	s122->Add(new wxTextCtrl(pConfig, -1, wxT("pseudoranges.tlm"), 	wxDefaultPosition, wxDefaultSize), 1, wxEXPAND);
	s122->Add(new wxTextCtrl(pConfig, -1, wxT("satellites.tlm"), 	wxDefaultPosition, wxDefaultSize), 1, wxEXPAND);
	s122->Add(new wxTextCtrl(pConfig, -1, wxT("tracking.tlm"), 		wxDefaultPosition, wxDefaultSize), 1, wxEXPAND);
	s122->Add(new wxTextCtrl(pConfig, -1, wxT("navigation.klv"), 	wxDefaultPosition, wxDefaultSize), 1, wxEXPAND);

	str.Clear();
	str.Add(wxT("L1 Mode"),1);
	str.Add(wxT("L1-L1 Mode"),1);
	str.Add(wxT("L1-L2C Mode"),1);

	s11->Add(new wxRadioBox(pConfig, -1, wxT(""), wxDefaultPosition, wxDefaultSize, str, 3, wxRA_SPECIFY_ROWS), wxEXPAND);

	s12->Add(s121, 0, wxALL, 4);
	s12->Add(s122, 1, wxALL, 4);

	sConfig->Add(s11, 0, wxALL, 4);
	sConfig->Add(s12, 1, wxALL, 4);

	pConfig->SetSizer(sConfig);

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
void GUI::onTimer(wxTimerEvent& evt)
{
	paintNow();
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

	wxCommandEvent event;

	if(gps_active == 1)
		OnStop(event);

	if(usrp_active == 1)
		OnUSRPStop(event);

    Close(TRUE);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxString message;

	message = wxT("GPS-SDR\nCopyright 2008 Gregory W. Heckler\nPath: ");
	message += wxGetCwd();

	wxMessageBox(message,wxT("About GPS-SDR"), wxOK | wxICON_INFORMATION, this);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::OnStart(wxCommandEvent& WXUNUSED(event))
{
	/* Status at the bottom */
	wxString cmd;

	cmd = wxGetCwd();
	cmd += wxT("/gps-sdr");

	/* First try local directory */
	if(wxFileExists(cmd))
	{
		cmd += wxT(" -gui");
	}
	else if(wxFileExists(wxT("/usr/local/bin/gps-sdr")))
	{
		cmd = wxT("/usr/local/bin/gps-sdr -gui");
	}
	else
	{
		status_str = wxT("Could not find ");
		status_str += cmd;
		return;
	}

	if(gps_active == 0)
	{
		/* Create the process */
		gps_proc = new wxProcess(wxPROCESS_REDIRECT);
		gps_pid = wxExecute(cmd, wxEXEC_ASYNC, gps_proc);
		gps_in = gps_proc->GetInputStream();
		gps_out = gps_proc->GetOutputStream();

		if((gps_pid == 0) || (gps_pid == -1))
		{
			status_str = wxT("Failed to start GPS-SDR");
		}
		else
		{
			status_str.Printf(wxT("Started GPS-SDR %d"),gps_pid);
			gps_active = 1;
		}
	}
	else
	{
		status_str = wxT("GPS-SDR Already Running!");
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::OnStop(wxCommandEvent& WXUNUSED(event))
{

	wxString s;
	s += wxT("Q\n");

	if(gps_active)
	{
		if(gps_out != NULL)
			gps_out->Write(s.c_str(), s.length());

		gps_active = 0;
		gps_in = 0;
		gps_out = 0;

		/* Status at the bottom */
		status_str = wxT("Stopped GPS-SDR");
	}
	else
	{
		status_str = wxT("GPS-SDR Not Running!");
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::OnUSRPStart(wxCommandEvent& WXUNUSED(event))
{
	/* Status at the bottom */
	wxString cmd;

	cmd = wxGetCwd();
	cmd += wxT("/gps-usrp");

	/* First try local directory */
	if(wxFileExists(cmd))
	{
		//Do nothing
	}
	else if(wxFileExists(wxT("/usr/local/bin/gps-usrp")))
	{
		cmd = wxT("/usr/local/bin/gps-usrp");
	}
	else
	{
		status_str = wxT("Could not find ");
		status_str += cmd;
		return;
	}

	if(usrp_active == 0)
	{
		/* Create the process */
		usrp_proc = new wxProcess(wxPROCESS_REDIRECT);
		usrp_pid = wxExecute(cmd, wxEXEC_ASYNC, usrp_proc);
		usrp_in = usrp_proc->GetInputStream();
		usrp_out = usrp_proc->GetOutputStream();

		if((usrp_pid == 0) || (usrp_pid == -1))
		{
			status_str = wxT("Failed to start GPS-USRP");
		}
		else
		{
			status_str.Printf(wxT("Started GPS-USRP %d"),usrp_pid);
			usrp_active = 1;
		}
	}
	else
	{
		status_str = wxT("GPS-SDR Already Running!");
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::OnUSRPStop(wxCommandEvent& WXUNUSED(event))
{

	wxString s;
	s += wxT("Q\n");

	if(usrp_active)
	{
		if(usrp_out != NULL)
			usrp_out->Write(s.c_str(), s.length());

		usrp_active = 0;
		usrp_in = 0;
		usrp_out = 0;

		/* Status at the bottom */
		status_str = wxT("Stopped GPS-USRP");
	}
	else
	{
		status_str = wxT("GPS-USRP Not Running!");
	}

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

	if(k != last_k)
	{
		switch(page)
		{
			case 0: renderNavigation(); 	break;
			case 1: renderAcquisition(); 	break;
			case 2: renderEphemeris(); 		break;
			case 3: renderConstellation(); 	break;
			case 4: renderEKF(); 			break;
			case 5: renderThreads(); 		break;
			case 6: renderCommands(); 		break;
			case 7: renderConfig(); 		break;
			default: renderNavigation();	break;
		}
		last_k = k;
	}
	else
	{
		switch(page)
		{
			case 5: renderThreads(); 		break;
			case 6: renderCommands(); 		break;
			case 7: renderConfig(); 		break;
			default: renderNavigation();	break;
		}
	}

	/* Status at the bottom */
	str.Printf(wxT("Pipe Reads %d\tFIFO:\t%d\t%d\t%d\t%d"),
			page,(FIFO_DEPTH-(tGUI.tFIFO.head-tGUI.tFIFO.tail)) % FIFO_DEPTH,tGUI.tFIFO.count,tGUI.tFIFO.agc_scale,tGUI.tFIFO.overflw);

	str += '\t';
	str += status_str;

	SetStatusText(str);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::renderNavigation()
{
	/* Clear navigation panel */
	tTracking->Clear();
	tNavigation->Clear();

	PrintChan(tTracking);
	PrintNav(tNavigation);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::renderAcquisition()
{

	/* Clear navigation panel */
	tAcquisition->Clear();

	PrintHistory(tAcquisition);

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

	 wxString str;

	/* Clear navigation panel */
	tConstellation->Clear();

	PrintAlmanac(tConstellation);

	str.Printf(wxT("\n"));
	tConstellation->AppendText(str);

	PrintEphem(tConstellation);
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


/*----------------------------------------------------------------------------------------------*/
void GUI::renderConfig()
{

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::PrintChan(wxTextCtrl* _text)
{

	Nav_Solution_S	*pNav = &tGUI.tNav.master_nav;				/* Navigation Solution */
	Chan_Packet_S *p;


    wxString str, str2;
	int32 lcv, lcv2;
	float cn0;

	str.Printf(wxT("Ch#  SV   CL       Faccel          Doppler     CN0   BE       Locks        Power   Active\n"));
	_text->AppendText(str);

	str.Printf(wxT("-----------------------------------------------------------------------------------------\n"));
	_text->AppendText(str);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		p = &tGUI.tChan[lcv];
		if(p->count > 3000)
		{

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

			_text->AppendText(str);
		}
		else
		{
			str.Printf(wxT("%2d   --   --   ----------   --------------   -----   --   ---------   ----------   ------\n"),lcv);
			_text->AppendText(str);
		}
	}

}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void GUI::PrintSV(wxTextCtrl* _text)
{

	Nav_Solution_S	*pNav = &tGUI.tNav.master_nav;				/* Navigation Solution */
	SV_Position_S 	*pPos;
	Pseudorange_S 	*pPseudo;
	Chan_Packet_S 	*pChan;

	int32 lcv;
	wxString str;

	/* Residuals */
	str.Printf(wxT("Ch#  SV         SV Time        VX        VY        VZ    Transit Time        Residual\n"));
	_text->AppendText(str);
	str.Printf(wxT("-------------------------------------------------------------------------------------\n"));
	_text->AppendText(str);

	for(lcv	= 0; lcv < MAX_CHANNELS; lcv++)
	{
		pPos    = (SV_Position_S *)	&tGUI.tNav.sv_positions[lcv];
		pChan   = (Chan_Packet_S *)	&tGUI.tChan[lcv];
		pPseudo = (Pseudorange_S *)	&tGUI.tNav.pseudoranges[lcv];

		if((pNav->nsvs >> lcv) & 0x1)
		{
			str.Printf(wxT("%2d   %2d  %14.7f  %8.2f  %8.2f  %8.2f  %14.8f  %14.8f\n"),
					lcv,
					(int32)pChan->sv+1,
					pPos->time,
					pPos->vx,
					pPos->vy,
					pPos->vz,
					pPseudo->time,
					pPseudo->residual);
			_text->AppendText(str);
		}
		else
		{

			str.Printf(wxT("%2d   --  --------------  --------  --------  --------  --------------  --------------\n"),lcv);
			_text->AppendText(str);
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::PrintNav(wxTextCtrl* _text)
{

	Nav_Solution_S		*pNav		= &tGUI.tNav.master_nav;				/* Navigation Solution */
	Clock_S				*pClock		= &tGUI.tNav.master_clock;			/* Clock solution */

	int32 nsvs, lcv, k;
	wxString str, str2;

	str2.Clear();

	switch(tGUI.tAcq.type)
	{
		case 0: str2 << wxT("STRONG"); break;
		case 1:	str2 << wxT("MEDIUM"); break;
		case 2:	str2 << wxT("  WEAK"); break;
	}

	str.Printf(wxT("Last Acq: %s, %02d, %7.2f, %7.0f, %10.0f\n"),str2.c_str(),tGUI.tAcq.sv+1, tGUI.tAcq.delay, tGUI.tAcq.doppler, tGUI.tAcq.magnitude);
	_text->AppendText(str);

	str.Printf(wxT("\n"));
	_text->AppendText(str);

	/* Nav Solution */
	nsvs = 0;
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if((pNav->nsvs >> lcv) & 0x1)
			nsvs++;
	}

	str.Printf(wxT("Nav SVs:\t%-2d\n"),nsvs);
	_text->AppendText(str);
	str.Printf(wxT("Receiver Time:\t%10.2f\n"),(float)pNav->tic/(float)TICS_PER_SECOND);
	_text->AppendText(str);
	str.Printf(wxT("\t\t\t      X\t\t      Y\t\t      Z\n"));
	_text->AppendText(str);
	str.Printf(wxT("Position (m):\t%15.2f\t%15.2f\t%15.2f\n"),pNav->x,pNav->y,pNav->z);
	_text->AppendText(str);
	str.Printf(wxT("Vel (cm/s):\t%15.2f\t%15.2f\t%15.2f\n"),100.0*pNav->vx,100.0*pNav->vy,100.0*pNav->vz);
	_text->AppendText(str);
	str.Printf(wxT("\n"));
	_text->AppendText(str);
	str.Printf(wxT("\t\t\t    Lat\t\t   Long\t\t    Alt\n"));
	_text->AppendText(str);
	str.Printf(wxT("\t\t%15.9f\t%15.9f\t%15.4f\n"),pNav->latitude*RAD_2_DEG,pNav->longitude*RAD_2_DEG,pNav->altitude);
	_text->AppendText(str);
	str.Printf(wxT("\n"));
	_text->AppendText(str);
	str.Printf(wxT("\t\t     Clock Bias\t     Clock Rate\t       GPS Time\n"));
	_text->AppendText(str);
	str.Printf(wxT("\t\t%15.6f\t%15.7f\t%15.6f\n"),pClock->bias,pClock->rate,pClock->time);
	_text->AppendText(str);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::PrintEphem(wxTextCtrl* _text)
{

	int32 lcv;
	wxString str;

	str.Printf(wxT("EPH: "));
	_text->AppendText(str);

	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		if(tGUI.tEphem.valid[lcv])
		{
			str.Printf(wxT("%2d"),lcv+1);
			_text->AppendText(str);
		}
	}

	str.Printf(wxT("\n"));
	_text->AppendText(str);

	str.Printf(wxT("ALM: "));
	_text->AppendText(str);

	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		if(tGUI.tEphem.avalid[lcv])
		{
			str.Printf(wxT("%2d"),lcv+1);
			_text->AppendText(str);
		}
	}


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::PrintAlmanac(wxTextCtrl* _text)
{

	int32 lcv, nvis, ntrack;
	float elev, azim;
	Acq_Predicted_S *psv;

	wxString str;


	nvis = 0;
	ntrack = 0;

	switch(tGUI.tSelect.mode)
	{
		case 0:	str.Printf(wxT("Acq Mode:\t  COLD\n"));	break;
		case 1: str.Printf(wxT("Acq Mode:\t  WARM\n"));	break;
		case 2:	str.Printf(wxT("Acq Mode:\t   HOT\n"));	break;
		default:str.Printf(wxT("Acq Mode:\t  COLD\n"));	break;
	}
	_text->AppendText(str);

	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{

		psv =  &tGUI.tSelect.sv_predicted[lcv];
		if(psv->visible) nvis++;
		if(psv->tracked) ntrack++;

	}

	str.Printf(wxT("\n"));
	_text->AppendText(str);

	str.Printf(wxT("Mask Angle:\t%6.2f\n"),tGUI.tSelect.mask_angle*(180/PI)-90.0);
	_text->AppendText(str);
	str.Printf(wxT("Visible:\t%6d\n"),nvis);
	_text->AppendText(str);
	str.Printf(wxT("Tracked:\t%6d\n"),ntrack);
	_text->AppendText(str);

	str.Printf(wxT("\n"));
	_text->AppendText(str);

	str.Printf(wxT("SV        Elev        Azim     Doppler           Delay   Visible    Tracked\n"));
	_text->AppendText(str);
	str.Printf(wxT("---------------------------------------------------------------------------\n"));
	_text->AppendText(str);

	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		psv =  &tGUI.tSelect.sv_predicted[lcv];
		elev = psv->elev*180/PI;
		azim = psv->azim*180/PI;

		if(elev != 0.0)
		{
			if(psv->visible && psv->tracked)
				str.Printf(wxT("%02d  %10.2f  %10.2f  %10.2f  %14.8f       YES        YES\n"),lcv+1,elev,azim,psv->doppler,psv->delay);

			if(psv->visible && !psv->tracked)
				str.Printf(wxT("%02d  %10.2f  %10.2f  %10.2f  %14.8f       YES         NO\n"),lcv+1,elev,azim,psv->doppler,psv->delay);

			if(!psv->visible && psv->tracked)
				str.Printf(wxT("%02d  %10.2f  %10.2f  %10.2f  %14.8f        NO        YES\n"),lcv+1,elev,azim,psv->doppler,psv->delay);

			if(!psv->visible && !psv->tracked)
				str.Printf(wxT("%02d  %10.2f  %10.2f  %10.2f  %14.8f        NO         NO\n"),lcv+1,elev,azim,psv->doppler,psv->delay);

			_text->AppendText(str);
		}
		else
		{
			str.Printf(wxT("--  ----------  ----------  ----------  --------------       ---        ---\n"));
			_text->AppendText(str);
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI::PrintHistory(wxTextCtrl* _text)
{

	int32 lcv, nvis, ntrack;
	float elev, azim;
	Acq_Predicted_S *psv;
	Acq_History_S *phist;
	wxString str;

	nvis = 0;
	ntrack = 0;

	switch(tGUI.tSelect.mode)
	{
		case 0:	str.Printf(wxT("Acq Mode:\t  COLD\n"));	break;
		case 1: str.Printf(wxT("Acq Mode:\t  WARM\n"));	break;
		case 2:	str.Printf(wxT("Acq Mode:\t   HOT\n"));	break;
		default:str.Printf(wxT("Acq Mode:\t  COLD\n"));	break;
	}
	_text->AppendText(str);

	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{

		psv =  &tGUI.tSelect.sv_predicted[lcv];
		if(psv->visible) nvis++;
		if(psv->tracked) ntrack++;

	}

	str.Printf(wxT("\n"));
	_text->AppendText(str);

	str.Printf(wxT("Mask Angle:\t%6.2f\n"),tGUI.tSelect.mask_angle*(180/PI)-90.0);
	_text->AppendText(str);
	str.Printf(wxT("Visible:\t%6d\n"),nvis);
	_text->AppendText(str);
	str.Printf(wxT("Tracked:\t%6d\n"),ntrack);
	_text->AppendText(str);

	str.Printf(wxT("\n"));
	_text->AppendText(str);

	str.Printf(wxT("SV  Ant     Type   Attempt   Fail   Success    DoppMin      DoppMax      Doppler      Magnitude\n"));
	_text->AppendText(str);
	str.Printf(wxT("-----------------------------------------------------------------------------------------------\n"));
	_text->AppendText(str);

	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		phist =  &tGUI.tSelect.sv_history[lcv];

		switch(phist->type)
		{
			case 0: str.Printf(wxT("%02d    %01d   STRONG      %4d   %4d      %4d   %8d     %8d     %8d     %10d\n"),
			lcv+1,phist->antenna,phist->attempts[0],phist->failures[0],phist->successes[0],phist->mindopp,phist->maxdopp,phist->doppler,phist->magnitude);
			break;
			case 1: str.Printf(wxT("%02d    %01d   MEDIUM      %4d   %4d      %4d   %8d     %8d     %8d     %10d\n"),
			lcv+1,phist->antenna,phist->attempts[1],phist->failures[1],phist->successes[1],phist->mindopp,phist->maxdopp,phist->doppler,phist->magnitude);
			break;
			case 2: str.Printf(wxT("%02d    %01d     WEAK      %4d   %4d      %4d   %8d     %8d     %8d     %10d\n"),
			lcv+1,phist->antenna,phist->attempts[2],phist->failures[2],phist->successes[2],phist->mindopp,phist->maxdopp,phist->doppler,phist->magnitude);
			break;
		}
		_text->AppendText(str);
	}

}
/*----------------------------------------------------------------------------------------------*/
