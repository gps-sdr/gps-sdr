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
    EVT_MENU(ID_QUIT,				GUI_Toplevel::onQuit)
    EVT_MENU(ID_ABOUT,				GUI_Toplevel::onAbout)
    EVT_MENU(ID_GPS_START,			GUI_Toplevel::onGPSStart)
    EVT_MENU(ID_GPS_STOP,			GUI_Toplevel::onGPSStop)
    EVT_MENU(ID_USRP_START,			GUI_Toplevel::onUSRPStart)
    EVT_MENU(ID_USRP_STOP,			GUI_Toplevel::onUSRPStop)
    EVT_MENU(ID_LOG_CONFIG,			GUI_Toplevel::onLogConfig)
    EVT_MENU(ID_LOG_START,			GUI_Toplevel::onLogStart)
	EVT_MENU(ID_LOG_STOP,			GUI_Toplevel::onLogStop)
	EVT_MENU(ID_LOG_CLEAR,			GUI_Toplevel::onLogClear)
	EVT_MENU(ID_NAMED_PIPE,			GUI_Toplevel::onNamedPipe)
	EVT_MENU(ID_SERIAL_PORT,		GUI_Toplevel::onSerialPort)
    EVT_TIMER(ID_TIMER,				GUI_Toplevel::onTimer)
    EVT_TOGGLEBUTTON(ID_MAIN_B,		GUI_Toplevel::onMain)
    EVT_TOGGLEBUTTON(ID_CHANNEL_B,	GUI_Toplevel::onChannel)
    EVT_TOGGLEBUTTON(ID_PSEUDO_B,	GUI_Toplevel::onPseudo)
    EVT_TOGGLEBUTTON(ID_SPEED_B,	GUI_Toplevel::onSpeed)
    EVT_TOGGLEBUTTON(ID_COMMANDS_B,	GUI_Toplevel::onCommands)
    EVT_TOGGLEBUTTON(ID_ALMANAC_B,	GUI_Toplevel::onAlmanac)
    EVT_TOGGLEBUTTON(ID_EPHEMERIS_B,GUI_Toplevel::onEphemeris)
    EVT_TOGGLEBUTTON(ID_SELECT_B,	GUI_Toplevel::onSelect)
    EVT_TOGGLEBUTTON(ID_ACQUISITION_B,	GUI_Toplevel::onAcquisition)
    EVT_PAINT(GUI_Toplevel::paintEvent)
    EVT_CLOSE(GUI_Toplevel::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
GUI_Toplevel::GUI_Toplevel():iGUI_Toplevel(NULL, wxID_ANY, wxT("GPS-SDR"), wxDefaultPosition, wxSize( 600,600 ), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, wxT("GPS-SDR") )
{

    timer = new wxTimer(this, ID_TIMER);
    timer->Start(100, wxTIMER_CONTINUOUS); //Shoot for 20 fps

    wMain = NULL;

    pSerial = new GUI_Serial;
    pSerial->setIO(0);
    pSerial->Start();

    kB_sec = 0;
    last_tic = 0;
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI_Toplevel::~GUI_Toplevel()
{
	pSerial->Stop();
	delete pSerial;

	if(wMain)
		delete wMain;

	if(wChannel)
		delete wChannel;

	if(wPseudo)
		delete wPseudo;

	if(wAlmanac)
		delete wAlmanac;

	if(wEphemeris)
		delete wEphemeris;

	if(wCommands)
		delete wCommands;

	if(wSelect)
		delete wSelect;

	if(wAcquisition)
		delete wAcquisition;

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

	wxCommandEvent event;

	if(gps_active == 1)
		onGPSStop(event);

	if(usrp_active == 1)
		onUSRPStop(event);

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
			mReceiver->Enable(ID_GPS_START, false);
			mReceiver->Enable(ID_GPS_STOP, true);
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
		mReceiver->Enable(ID_GPS_START, true);
		mReceiver->Enable(ID_GPS_STOP, false);
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
			mUSRP->Enable(ID_USRP_START, false);
			mUSRP->Enable(ID_USRP_STOP, true);
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
		mUSRP->Enable(ID_USRP_START, true);
		mUSRP->Enable(ID_USRP_STOP, false);
	}
	else
	{
		status_str = wxT("GPS-USRP Not Running!");
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onLogConfig(wxCommandEvent& WXUNUSED(event))
{
	int32 val;
	wxString str;
	iGUI_Log log(this);

	log.SetAffirmativeId(wxID_OK);
	log.SetEscapeId(wxID_ANY);

	log.cChan->SetValue(pSerial->getLog(CHANNEL_M_ID));
	log.cClock->SetValue(pSerial->getLog(CLOCK_M_ID));
	log.cPseudo->SetValue(pSerial->getLog(PSEUDORANGE_M_ID));
	log.cTask->SetValue(pSerial->getLog(TASK_HEALTH_M_ID));
	log.cEKF->SetValue(pSerial->getLog(EKF_M_ID));
	log.cPVT->SetValue(pSerial->getLog(SPS_M_ID));

	//str.sprintf(wxT("%s"),pSerial->getLogFile());

	//log.mFile->SetPath(str);
	//status_str = str;

	if(log.ShowModal() == wxID_OK)
	{
		pSerial->Lock();

		/* Get options */
		if(log.cChan->IsChecked())
			pSerial->logOn(CHANNEL_M_ID, true);
		else
			pSerial->logOn(CHANNEL_M_ID, false);

		if(log.cClock->IsChecked())
			pSerial->logOn(CLOCK_M_ID, true);
		else
			pSerial->logOn(CLOCK_M_ID, false);

		if(log.cPseudo->IsChecked())
			pSerial->logOn(PSEUDORANGE_M_ID,true);
		else
			pSerial->logOn(PSEUDORANGE_M_ID,false);

		if(log.cTask->IsChecked())
			pSerial->logOn(TASK_HEALTH_M_ID, true);
		else
			pSerial->logOn(TASK_HEALTH_M_ID, false);

		if(log.cEKF->IsChecked())
			pSerial->logOn(EKF_M_ID, true);
		else
			pSerial->logOn(EKF_M_ID, false);

		if(log.cPVT->IsChecked())
			pSerial->logOn(SPS_M_ID, true);
		else
			pSerial->logOn(SPS_M_ID, false);

		/* Get Filename */
		str = log.mFile->GetPath();
		pSerial->setLogFile(str.mb_str());

		pSerial->Unlock();

		if(pSerial->getLog(PSEUDORANGE_M_ID))
		{
			val = 1;
			pSerial->formCommand(GET_PSEUDORANGE_C_ID, &val);
		}

		if(pSerial->getLog(CHANNEL_M_ID))
		{
			val = 1;
			pSerial->formCommand(GET_CHANNEL_C_ID, &val);
		}

		mLogging->Enable(ID_LOG_START, true);
		mLogging->Enable(ID_LOG_CLEAR, true);
		mLogging->Enable(ID_LOG_STOP, false);
		mLogging->Enable(ID_LOG_CONFIG, true);
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onLogStart(wxCommandEvent& WXUNUSED(event))
{

	pSerial->Lock();
	pSerial->logStart();
	pSerial->Unlock();


	mLogging->Enable(ID_LOG_START, false);
	mLogging->Enable(ID_LOG_CLEAR, false);
	mLogging->Enable(ID_LOG_STOP, true);
	mLogging->Enable(ID_LOG_CONFIG, false);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onLogStop(wxCommandEvent& WXUNUSED(event))
{
	int32 val;

	pSerial->Lock();
	pSerial->logStop();
	pSerial->Unlock();

	if(wChannel == NULL && (pSerial->getLog(CHANNEL_M_ID) == 0))
	{
		val = 0;
		pSerial->formCommand(GET_CHANNEL_C_ID, &val);
	}

	mLogging->Enable(ID_LOG_START, true);
	mLogging->Enable(ID_LOG_CLEAR, true);
	mLogging->Enable(ID_LOG_STOP, false);
	mLogging->Enable(ID_LOG_CONFIG, true);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onLogClear(wxCommandEvent& WXUNUSED(event))
{
	pSerial->Lock();
	pSerial->logClear();
	pSerial->Unlock();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onNamedPipe(wxCommandEvent& WXUNUSED(event))
{
//	mIO->Enable(ID_SERIAL_PORT, true);
//	mIO->Enable(ID_NAMED_PIPE, false);
	pSerial->Lock();
	pSerial->setIO(0);
	pSerial->Unlock();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onSerialPort(wxCommandEvent& WXUNUSED(event))
{
//	mIO->Enable(ID_SERIAL_PORT, false);
//	mIO->Enable(ID_NAMED_PIPE, true);
	pSerial->Lock();
	pSerial->setIO(1);
	pSerial->Unlock();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onClose(wxCloseEvent& evt)
{

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

	int page, this_tic;
    wxString str;
    wxString str2;

    pSerial->Lock();
    memcpy(&messages,pSerial->GetMessages(),sizeof(Message_Struct));
    pSerial->Unlock();

    this_tic = pSerial->decoded_packet.tic;
    if(this_tic != last_tic)
    {

    	last_tic = this_tic;
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

		/* Render main window */
		if(wMain != NULL)
			wMain->paintNow();

		/* Render channel window */
		if(wChannel != NULL)
			wChannel->paintNow();

		/* Render channel window */
		if(wPseudo != NULL)
			wPseudo->paintNow();

		/* Render channel window */
		if(wAlmanac != NULL)
			wAlmanac->paintNow();

		/* Render channel window */
		if(wEphemeris != NULL)
			wEphemeris->paintNow();

		/* Render SV Select window */
		if(wSelect != NULL)
			wSelect->paintNow();

		/* Display acquisition */
		if(wAcquisition != NULL)
			wAcquisition->paintNow();

    }

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

	wxString str;

	pSerial->Lock();

	tRS422->Clear();

	str = wxT("Pipe is ");

	if(pSerial->GetPipeOpen())
		str += wxT("open\n");
	else
		str += wxT("closed\n");
	tRS422->AppendText(str);

	str.Printf(wxT("Commands Free:\t\t%d\n"),pSerial->GetCommandFree());
	tRS422->AppendText(str);
	str.Printf(wxT("Synchronized Count:\t%d\n"),pSerial->message_sync);
	tRS422->AppendText(str);
	str.Printf(wxT("Last Message Tic:\t%d\n"),pSerial->decoded_packet.tic);
	tRS422->AppendText(str);
	str.Printf(wxT("Failed Messages:\t%d\n"),pSerial->packet_count[LAST_M_ID]);
	tRS422->AppendText(str);
	kB_sec += .25 * (((float)bytes_sec/1024.0) - kB_sec);
	str.Printf(wxT("Serial Bandwidth:\t%.2f "),kB_sec);
	str += wxT("kB/sec\n");
	tRS422->AppendText(str);

	if(count % 10 == 0)
	{
		bytes_pres = pSerial->GetByteCount();
		bytes_sec = bytes_pres - bytes_prev;
		bytes_prev = bytes_pres;
	}

	pSerial->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::renderTask()
{
	int32 lcv;
	Task_Health_M *pTask;

	wxString str;
	wxString names[MAX_TASKS];

	names[FIFO_TASK_ID] 			= wxT("FIFO        ");
	names[COMMANDO_TASK_ID]		 	= wxT("COMMANDO    ");
	names[SERIAL_TELEMETRY_TASK_ID] = wxT("TELEMETRY   ");
	names[KEYBOARD_TASK_ID] 		= wxT("KEYBOARD    ");
	names[EPHEMERIS_TASK_ID] 		= wxT("EPHEMERIS   ");
	names[SV_SELECT_TASK_ID] 		= wxT("SV_SELECT   ");
	names[ACQUISITION_TASK_ID] 		= wxT("ACQUISITION ");
	names[PVT_TASK_ID] 				= wxT("PVT         ");

	tTask->Clear();

	pTask = &messages.task_health;

		 str = wxT("Task        Execution Tic   Delta     Start Tic    Stop Tic\n");
	tTask->AppendText(str);
	     str = wxT("-----------------------------------------------------------\n");
	tTask->AppendText(str);

	for(lcv = 0; lcv < MAX_TASKS; lcv++)
	{
		if(names[lcv].Len())
		{
			str.Printf(wxT("%s   %10d  %6d    %9d    %9d\n"),
				names[lcv].c_str(),
				pTask->execution_tic[lcv],
				pTask->stop_tic[lcv]-pTask->start_tic[lcv],
				pTask->start_tic[lcv],
				pTask->stop_tic[lcv]);
			tTask->AppendText(str);
		}
	}

}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onMain(wxCommandEvent& WXUNUSED(event))
{

	int32 val;

	if(wMain)
	{
		delete wMain;
		wMain = NULL;
		bMain->SetValue(false);

		val = 0;
		pSerial->formCommand(GET_SV_POSITION_C_ID, &val);

		if(wChannel == NULL && (pSerial->getLog(CHANNEL_M_ID) == 0))
		{
			val = 0;
			pSerial->formCommand(GET_CHANNEL_C_ID, &val);
		}
	}
	else
	{
		wMain = new GUI_Main();
		wMain->setPointer(&messages);
		wMain->setSerial(pSerial);
		wMain->setToplevel(this);
		wMain->Show(TRUE);
		bMain->SetValue(true);

		val = 1;
		pSerial->formCommand(GET_SV_POSITION_C_ID, &val);

		val = 1;
		pSerial->formCommand(GET_CHANNEL_C_ID, &val);
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onChannel(wxCommandEvent& WXUNUSED(event))
{
	int32 val;

	if(wChannel)
	{
		delete wChannel;
		wChannel = NULL;
		bChannel->SetValue(false);

		if(wMain == NULL && (pSerial->getLog(CHANNEL_M_ID) == 0))
		{
			val = 0;
			pSerial->formCommand(GET_CHANNEL_C_ID, &val);
		}
	}
	else
	{
		wChannel = new GUI_Channel();
		wChannel->setPointer(&messages);
		wChannel->setSerial(pSerial);
		wChannel->setToplevel(this);
		wChannel->Show(TRUE);
		bChannel->SetValue(true);

		val = 1;
		pSerial->formCommand(GET_CHANNEL_C_ID, &val);
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onPseudo(wxCommandEvent& WXUNUSED(event))
{
	int32 val;

	if(wPseudo)
	{
		delete wPseudo;
		wPseudo = NULL;
		bPseudo->SetValue(false);

		if(wPseudo == NULL && (pSerial->getLog(PSEUDORANGE_M_ID) == 0))
		{
			val = 0;
			pSerial->formCommand(GET_PSEUDORANGE_C_ID, &val);
		}
	}
	else
	{
		wPseudo = new GUI_Pseudo();
		wPseudo->setPointer(&messages);
		wPseudo->setSerial(pSerial);
		wPseudo->setToplevel(this);
		wPseudo->Show(TRUE);
		bPseudo->SetValue(true);

		val = 1;
		pSerial->formCommand(GET_PSEUDORANGE_C_ID, &val);
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onCommands(wxCommandEvent& WXUNUSED(event))
{

	if(wCommands)
	{
		delete wCommands;
		wCommands = NULL;
		bCommands->SetValue(false);
	}
	else
	{
		wCommands = new GUI_Commands();
		wCommands->setPointer(&messages);
		wCommands->setSerial(pSerial);
		wCommands->setToplevel(this);
		wCommands->Show(TRUE);
		bCommands->SetValue(true);
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onAlmanac(wxCommandEvent& WXUNUSED(event))
{

	if(wAlmanac)
	{
		delete wAlmanac;
		wAlmanac = NULL;
		bAlmanac->SetValue(false);
	}
	else
	{
		wAlmanac = new GUI_Almanac();
		wAlmanac->setPointer(&messages);
		wAlmanac->setSerial(pSerial);
		wAlmanac->setToplevel(this);
		wAlmanac->Show(TRUE);
		bAlmanac->SetValue(true);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onEphemeris(wxCommandEvent& WXUNUSED(event))
{

	if(wEphemeris)
	{
		delete wEphemeris;
		wEphemeris = NULL;
		bEphemeris->SetValue(false);
	}
	else
	{
		wEphemeris = new GUI_Ephemeris();
		wEphemeris->setPointer(&messages);
		wEphemeris->setSerial(pSerial);
		wEphemeris->setToplevel(this);
		wEphemeris->Show(TRUE);
		bEphemeris->SetValue(true);
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onSelect(wxCommandEvent& WXUNUSED(event))
{

	if(wSelect)
	{
		delete wSelect;
		wSelect = NULL;
		bSelect->SetValue(false);
	}
	else
	{
		wSelect = new GUI_Select();
		wSelect->setPointer(&messages);
		wSelect->setSerial(pSerial);
		wSelect->setToplevel(this);
		wSelect->Show(TRUE);
		bSelect->SetValue(true);
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onAcquisition(wxCommandEvent& WXUNUSED(event))
{

	if(wAcquisition)
	{
		delete wAcquisition;
		wAcquisition = NULL;
		bAcquisition->SetValue(false);
	}
	else
	{
		wAcquisition = new GUI_Acquisition();
		wAcquisition->setPointer(&messages);
		wAcquisition->setSerial(pSerial);
		wAcquisition->setToplevel(this);
		wAcquisition->Show(TRUE);
		bAcquisition->SetValue(true);
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onSpeed(wxCommandEvent& WXUNUSED(event))
{



}
/*----------------------------------------------------------------------------------------------*/
