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
    EVT_TIMER(ID_TIMER,				GUI_Toplevel::onTimer)
    EVT_TOGGLEBUTTON(ID_MAIN_B,		GUI_Toplevel::onMain)
    EVT_TOGGLEBUTTON(ID_CHANNEL_B,	GUI_Toplevel::onChannel)
    EVT_TOGGLEBUTTON(ID_SPEED_B,	GUI_Toplevel::onSpeed)
    EVT_PAINT(GUI_Toplevel::paintEvent)
    EVT_CLOSE(GUI_Toplevel::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
GUI_Toplevel::GUI_Toplevel():iGUI_Toplevel(NULL, wxID_ANY, wxT("GPS-SDR"), wxDefaultPosition, wxSize( 800,600 ), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, wxT("GPS-SDR") )
{

    timer = new wxTimer(this, ID_TIMER);
    timer->Start(50, wxTIMER_CONTINUOUS); //Shoot for 20 fps

    wDefault = NULL;

    pSerial = new GUI_Serial;
    pSerial->Start();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI_Toplevel::~GUI_Toplevel()
{
	pSerial->Stop();
	delete pSerial;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onTimer(wxTimerEvent& evt)
{
	paintNow();
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
		cmd += wxT(" -p gps.dbb -gui");
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

	if(wDefault)
	{
		delete wDefault;
		wDefault = NULL;
	}

    wxGetApp().activateRenderLoop(false);
    evt.Skip(); // don't stop event, we still want window to close
    //Close(TRUE);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::paintEvent(wxPaintEvent& evt)
{
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
void GUI_Toplevel::render(wxDC& dc)
{

	int page;
    wxString str;
    wxString str2;


    pSerial->Lock();
    memcpy(&messages,pSerial->GetMessages(),sizeof(Message_Struct));
    pSerial->Unlock();

    /* Render FIFO Panel */
	renderFIFO();

	/* Render RS422 Panel */
	renderRS422();

	/* Render Task Panel */
	renderTask();

	str = status_str;
	str2.Printf(wxT("Count: %d"),count++);
	str += str2;

	SetStatusText(str);

	/* Render default window */
	if(wDefault != NULL)
		wDefault->paintNow();

	/* Render channel window */
	if(wChannel != NULL)
		wChannel->paintNow();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::renderFIFO()
{
	wxString str;
	float fifo_p;
	int fifo_i;
	FIFO_M *p = &messages.fifo;

	fifo_p = FIFO_DEPTH - (p->head - p->tail) % FIFO_DEPTH;
	fifo_p = fifo_p / FIFO_DEPTH;
	fifo_i = 100 - (int)fifo_p;

	tUSRP->Clear();

	gUSRP->SetValue(fifo_i);

	str.Printf(wxT("AGC Scale:\t%d\n"),p->agc_scale);
	tUSRP->AppendText(str);
	str.Printf(wxT("AGC Overflws:\t%d\n"),p->overflw);
	tUSRP->AppendText(str);
	str.Printf(wxT("FIFO Count:\t%d\n"),p->count);
	tUSRP->AppendText(str);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::renderRS422()
{

	float kB_sec;
	wxString str;

	tRS422->Clear();

	str = wxT("Pipe is ");

	if(pSerial->GetPipeOpen())
		str += wxT("open\n");
	else
		str += wxT("closed\n");
	tRS422->AppendText(str);

	str.Printf(wxT("Synchronized Count:\t%d\n"),pSerial->message_sync);
	tRS422->AppendText(str);
	str.Printf(wxT("Last Message Tic:\t%d\n"),pSerial->decoded_header.tic);
	tRS422->AppendText(str);
	str.Printf(wxT("Failed Messages:\t%d\n"),pSerial->packet_count[LAST_M_ID]);
	tRS422->AppendText(str);

	kB_sec = (float)bytes_sec / 1024.0;
	str.Printf(wxT("Serial Bandwidth:\t%.2f "),kB_sec);
	str += wxT("kB/sec\n");
	tRS422->AppendText(str);

	if(count % 20 == 0)
	{
		bytes_pres = pSerial->GetByteCount();
		bytes_sec = bytes_pres - bytes_prev;
		bytes_prev = bytes_pres;
	}
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

	if(wDefault)
	{
		delete wDefault;
		wDefault = NULL;
	}
	else
	{
		wDefault = new GUI_Default();
		wDefault->setPointer(&messages);
		wDefault->Show(TRUE);
	}
}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onChannel(wxCommandEvent& WXUNUSED(event))
{

	if(wChannel)
	{
		delete wChannel;
		wChannel = NULL;
	}
	else
	{
		wChannel = new GUI_Channel();
		wChannel->setPointer(&messages);
		wChannel->Show(TRUE);
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onSpeed(wxCommandEvent& WXUNUSED(event))
{

	pSerial->Lock();
	pSerial->SendCommand();
	pSerial->Unlock();
}
/*----------------------------------------------------------------------------------------------*/
