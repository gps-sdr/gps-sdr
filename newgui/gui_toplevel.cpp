/*
 * gui_toplevel.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"


DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_Toplevel, wxFrame)
    EVT_MENU(ID_Quit,				GUI_Toplevel::onQuit)
    EVT_MENU(ID_About,				GUI_Toplevel::onAbout)
    EVT_MENU(ID_GPS_Start,			GUI_Toplevel::onGPSStart)
    EVT_MENU(ID_GPS_Stop,			GUI_Toplevel::onGPSStop)
    EVT_MENU(ID_USRP_Start,			GUI_Toplevel::onUSRPStart)
    EVT_MENU(ID_USRP_Stop,			GUI_Toplevel::onUSRPStop)
    EVT_TIMER(ID_Timer,				GUI_Toplevel::onTimer)
    EVT_TOGGLEBUTTON(ID_MAIN_B,		GUI_Toplevel::onMain)
    EVT_PAINT(GUI_Toplevel::paintEvent)
    EVT_CLOSE(GUI_Toplevel::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI_Toplevel::GUI_Toplevel():iGUI_Toplevel(NULL, wxID_ANY, wxT("GPS-SDR"), wxDefaultPosition, wxSize( 800,600 ), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, wxT("GPS-SDR") )
{

    timer = new wxTimer(this, ID_Timer);
    timer->Start(66, wxTIMER_CONTINUOUS); //Shoot for 15 fps

    wdefault = NULL;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI_Toplevel::~GUI_Toplevel()
{


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onTimer(wxTimerEvent& evt)
{
	paintNow();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
bool GUI_Toplevel::openPipe()
{

	gpipe[READ] = open("/tmp/GPS2GUI",O_RDONLY | O_NONBLOCK);
	gpipe[WRITE] = open("/tmp/GUI2GPS",O_WRONLY | O_NONBLOCK);

	if((gpipe[READ] != -1) || (gpipe[WRITE] != -1))
		return(true);
	else
		return(false);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onQuit(wxCommandEvent& WXUNUSED(event))
{



    Close(TRUE);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onAbout(wxCommandEvent& WXUNUSED(event))
{
	wxString message;

	message = wxT("GPS-SDR\nCopyright 2008 Gregory W. Heckler\nPath: ");
	message += wxGetCwd();

	wxMessageBox(message,wxT("About GPS-SDR"), wxOK | wxICON_INFORMATION, this);
}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onGPSStart(wxCommandEvent& WXUNUSED(event))
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
void GUI_Toplevel::onGPSStop(wxCommandEvent& WXUNUSED(event))
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
void GUI_Toplevel::onUSRPStart(wxCommandEvent& WXUNUSED(event))
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
void GUI_Toplevel::onUSRPStop(wxCommandEvent& WXUNUSED(event))
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
void GUI_Toplevel::onClose(wxCloseEvent& evt)
{

	if(wdefault)
	{
		delete wdefault;
		wdefault = NULL;
	}

    wxGetApp().activateRenderLoop(false);
    evt.Skip(); // don't stop event, we still want window to close
    //Close(TRUE);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::paintEvent(wxPaintEvent& evt)
{
	int val;

	val = gUSRP->GetValue();
	val = (val + 1) % gUSRP->GetRange();
	gUSRP->SetValue(val);


    wxPaintDC dc(this);
    render(dc);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::DecodeCCSDSPacketHeader(CCSDS_Header *_h, CCSDS_PH *_p)
{

	_h->id 		= _p->pid - CCSDS_APID_BASE;
	_h->type 	= _p->psc & 0x3;
	_h->tic		= (_p->psc >> 2) & 0x3FFF;
	_h->length 	= _p->pdl & 0xFFFF;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::readPipe()
{
	int nbytes, bread, k, chan;
	char q[9] = "01234567";
	char p[9] = "01234567";
	char v;

	uint32 buff[1024];

	if(message_sync)
	{

		/* Check to make sure the first 8 bytes are "AAAAAAAA" */
		bread = read(gpipe[READ], &p[0], 0x8);
		if(strcmp(p, "AAAAAAAA"))
		{
			message_sync = false;
			return;
		}

		/* If so get the packet header */
		bread = read(gpipe[READ], &pheader, sizeof(CCSDS_PH));
		DecodeCCSDSPacketHeader(&ccsds_header, &pheader);

		/* Check to make sure the type is supported */
//		if(ccsds_header.id >= LAST_M_ID || ccsds_header.id <= FIRST_M_ID)
//		{
//			packet_count[LAST_M_ID]++;
//			message_sync = false;
//			return;
//		}
//		else
//		{
//			packet_count[ccsds_header.id]++;
//		}

		bread = read(gpipe[READ], &buff[0], ccsds_header.length);
		return;

		/* Now copy in the body */
		switch(ccsds_header.type)
		{
			case BOARD_HEALTH_M_ID:
				bread = read(gpipe[READ], &board_health, sizeof(Board_Health_M));
				if(bread != sizeof(Board_Health_M))
				{
					message_sync = false;
					packet_count[LAST_M_ID]++;
				}
				break;
			case TASK_HEALTH_M_ID:
				bread = read(gpipe[READ], &task_health, sizeof(Task_Health_M));
				if(bread != sizeof(Task_Health_M))
				{
					message_sync = false;
					packet_count[LAST_M_ID]++;
				}
				break;
			case CHANNEL_HEALTH_M_ID:
				bread = read(gpipe[READ], &channel_health[MAX_CHANNELS], sizeof(Channel_Health_M));
				if(bread != sizeof(Channel_Health_M))
				{
					message_sync = false;
					packet_count[LAST_M_ID]++;
				}
				else
				{
					chan = channel_health[MAX_CHANNELS].chan;
					memcpy(&channel_health[chan], &channel_health[MAX_CHANNELS], sizeof(Channel_Health_M));
				}
				break;
			case SPS_M_ID:
				bread = read(gpipe[READ], &sps, sizeof(SPS_M));
				if(bread != sizeof(SPS_M))
				{
					message_sync = false;
					packet_count[LAST_M_ID]++;
				}
				break;
			case CLOCK_M_ID:
				bread = read(gpipe[READ], &clock, sizeof(Clock_M));
				if(bread != sizeof(Clock_M))
				{
					message_sync = false;
					packet_count[LAST_M_ID]++;
				}
				break;
			case SV_POSITION_M_ID:
				bread = read(gpipe[READ], &sv_positions[MAX_CHANNELS], sizeof(SV_Position_M));
				if(bread != sizeof(SV_Position_M))
				{
					message_sync = false;
					packet_count[LAST_M_ID]++;
				}
				else
				{
					chan = sv_positions[MAX_CHANNELS].chan;
					memcpy(&sv_positions[chan], &sv_positions[MAX_CHANNELS], sizeof(SV_Position_M));
				}
				break;
			case EKF_M_ID:
				bread = read(gpipe[READ], &task_health, sizeof(Task_Health_M));
				if(bread != sizeof(Task_Health_M))
				{
					message_sync = false;
					packet_count[LAST_M_ID]++;
				}
				break;
			case MEASUREMENT_M_ID:
				bread = read(gpipe[READ], &measurements[MAX_CHANNELS], sizeof(Measurement_M));
				if(bread != sizeof(Measurement_M))
				{
					message_sync = false;
					packet_count[LAST_M_ID]++;
				}
				else
				{
					chan = measurements[MAX_CHANNELS].chan;
					memcpy(&measurements[chan], &measurements[MAX_CHANNELS], sizeof(Measurement_M));
				}
				break;
			case PSEUDORANGE_M_ID:
				bread = read(gpipe[READ], &pseudoranges[MAX_CHANNELS], sizeof(Pseudorange_M));
				if(bread != sizeof(Pseudorange_M))
				{
					message_sync = false;
					packet_count[LAST_M_ID]++;
				}
				else
				{
					chan = pseudoranges[MAX_CHANNELS].chan;
					memcpy(&pseudoranges[chan], &pseudoranges[MAX_CHANNELS], sizeof(Pseudorange_M));
				}
				break;
			case EPHEMERIS_M_ID:
				bread = read(gpipe[READ], &ephemeris[NUM_CODES], sizeof(Ephemeris_M));
				if(bread != sizeof(Ephemeris_M))
				{
					message_sync = false;
					packet_count[LAST_M_ID]++;
				}
				else
				{
					chan = ephemeris[NUM_CODES].sv;
					memcpy(&ephemeris[chan], &ephemeris[NUM_CODES], sizeof(Ephemeris_M));
				}
				break;
			case ALMANAC_M_ID:
				bread = read(gpipe[READ], &almanac[NUM_CODES], sizeof(Almanac_M));
				if(bread != sizeof(Almanac_M))
				{
					message_sync = false;
					packet_count[LAST_M_ID]++;
				}
				else
				{
					chan = almanac[NUM_CODES].sv;
					memcpy(&almanac[chan], &almanac[NUM_CODES], sizeof(Almanac_M));
				}
				break;
			case EPHEMERIS_VALID_M_ID:
				bread = read(gpipe[READ], &ephemeris_status, sizeof(Ephemeris_Status_M));
				if(bread != sizeof(Ephemeris_Status_M))
				{
					message_sync = false;
					packet_count[LAST_M_ID]++;
				}
				break;
//			case FIFO_M_ID:
//				bread = read(gpipe[READ], &task_health, sizeof(Task_Health_M));
//				if(bread != sizeof(Task_Health_M))
//				{
//					message_sync = false;
//					packet_count[LAST_M_ID]++;
//				}
//				break;
//			case COMMAND_ACQ_M_ID:
//				bread = read(gpipe[READ], &task_health, sizeof(Task_Health_M));
//				if(bread != sizeof(Task_Health_M))
//				{
//					message_sync = false;
//					packet_count[LAST_M_ID]++;
//				}
//				break;
			case LAST_M_ID:
				message_sync = false;
				packet_count[LAST_M_ID]++;
				break;
			default:
				break;
		}


	}
	else
	{
		k = 0;
		bread = 1;
		while(bread == 1)
		{
			v = '0';
			bread = read(gpipe[READ], &v, 0x1);
			if(bread == 1)
			{
				memcpy(&q[0], &p[0], 7*sizeof(char));
				p[0] = v;
				memcpy(&p[1], &q[0], 7*sizeof(char));
				if(strcmp(p, "AAAAAAAA") == 0)
				{
					message_sync = true;

					/* If so get the packet header */
					bread = read(gpipe[READ], &pheader, sizeof(CCSDS_PH));
					DecodeCCSDSPacketHeader(&ccsds_header, &pheader);

					bread = read(gpipe[READ], &buff[0], ccsds_header.length);

					return;
				}
			}
			k++;
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::render(wxDC& dc)
{

	int page;
    wxString str;
    wxString str2;

	/* Open the pipe */
	if(!gpipe_open)
		gpipe_open = openPipe();

	/* Get data from the pipe */
	if(gpipe_open)
		readPipe();


	/* Render FIFO Panel */
	renderFIFO();

	/* Render RS422 Panel */
	renderRS422();

	/* Render Task Panel */
	renderTask();

	str += status_str;
	str += '\t';
	str2.Printf(wxT("Count: %d"),ccsds_header.tic);
	str += str2;

	SetStatusText(str);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::renderFIFO()
{
	wxString str;

	tUSRP->Clear();


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::renderRS422()
{
	wxString str;

	tRS422->Clear();

	str.Printf(wxT("Synchronized: %d\n"),message_sync);
	tRS422->AppendText(str);
	str.Printf(wxT("Message Count: %d\n"),ccsds_header.tic);
	tRS422->AppendText(str);
	str.Printf(wxT("Failed Message Count: %d\n"),packet_count[LAST_M_ID]);
	tRS422->AppendText(str);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::renderTask()
{


}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onMain(wxCommandEvent& WXUNUSED(event))
{

	if(wdefault)
	{
		delete wdefault;
		wdefault = NULL;
	}
	else
	{
		wdefault = new GUI_Default();
		wdefault->Show(TRUE);
	}
}
/*----------------------------------------------------------------------------------------------*/
