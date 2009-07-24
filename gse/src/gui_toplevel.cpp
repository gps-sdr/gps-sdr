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
    EVT_MENU(ID_LOG_CONFIG,			GUI_Toplevel::onLogConfig)
    EVT_MENU(ID_LOG_START,			GUI_Toplevel::onLogStart)
	EVT_MENU(ID_LOG_STOP,			GUI_Toplevel::onLogStop)
	EVT_MENU(ID_LOG_CLEAR,			GUI_Toplevel::onLogClear)
	EVT_MENU(ID_SERIAL		,		GUI_Toplevel::onSerial)
	EVT_MENU(ID_NPIPE,				GUI_Toplevel::onNpipe)
    EVT_TIMER(ID_TIMER,				GUI_Toplevel::onTimer)
    EVT_TOGGLEBUTTON(ID_PVT_B,		GUI_Toplevel::onPVT)
    EVT_TOGGLEBUTTON(ID_EKF_B,		GUI_Toplevel::onEKF)
    EVT_TOGGLEBUTTON(ID_CHANNEL_B,	GUI_Toplevel::onChannel)
    EVT_TOGGLEBUTTON(ID_PSEUDO_B,	GUI_Toplevel::onPseudo)
    EVT_TOGGLEBUTTON(ID_COMMANDS_B,	GUI_Toplevel::onCommands)
    EVT_TOGGLEBUTTON(ID_ALMANAC_B,	GUI_Toplevel::onAlmanac)
    EVT_TOGGLEBUTTON(ID_SELECT_B,	GUI_Toplevel::onSelect)
    EVT_TOGGLEBUTTON(ID_EPHEMERIS_B,GUI_Toplevel::onEphemeris)
//    EVT_TOGGLEBUTTON(ID_EEPROM_B,	GUI_Toplevel::onEEPROM)
    EVT_TOGGLEBUTTON(ID_HEALTH_B,	GUI_Toplevel::onHealth)
    EVT_TOGGLEBUTTON(ID_MESSAGES_B, GUI_Toplevel::onMessages)
    EVT_TOGGLEBUTTON(ID_SPEEDO_B, 	GUI_Toplevel::onSpeedo)
    EVT_PAINT(GUI_Toplevel::paintEvent)
    EVT_CLOSE(GUI_Toplevel::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI_Toplevel::GUI_Toplevel():iGUI_Toplevel(NULL, wxID_ANY, wxT("GPS GUI"), wxDefaultPosition, wxSize(600,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, wxT("GPS GUI") )
{

	wPVT 			= NULL;
	wEKF 			= NULL;
	wChannel 		= NULL;
	wPseudo 		= NULL;
	wEphemeris 		= NULL;
	wAlmanac 		= NULL;
	wHealth 		= NULL;
	wCommands 		= NULL;
	//wEEPROM 		= NULL;
	wSpeedo 		= NULL;

    pSerial = new GUI_Serial;
    pSerial->setIO(0);
    pSerial->Start();

    kB_sec = 0;
    last_tic = 0;

    timer = new wxTimer(this, ID_TIMER);
    timer->Start(100, wxTIMER_CONTINUOUS); //Shoot for 20 fps

    log_filename.Clear();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
GUI_Toplevel::~GUI_Toplevel()
{
	pSerial->Stop();
	delete pSerial;

	if(wPVT)
		delete wPVT;

	if(wEKF)
		delete wEKF;

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

	if(wSpeedo)
		delete wSpeedo;

//	if(wEEPROM)
//		delete wEEPROM;

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

    Close(TRUE);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onAbout(wxCommandEvent& WXUNUSED(event))
{
	wxString message;

	message = wxT("GPS-SDR\nCopyright 2009 Gregory W. Heckler\nPath: ");
	message += wxGetCwd();

	wxMessageBox(message,wxT("About GPS-SDR"), wxOK | wxICON_INFORMATION, this);
}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onLogConfig(wxCommandEvent& WXUNUSED(event))
{
	int32 val;
	iGUI_Log log(this);

	log.SetAffirmativeId(wxID_OK);
	log.SetEscapeId(wxID_ANY);

	pSerial->Lock();

	log.mFile->SetPath(log_filename);
	log.cPVT->SetValue(pSerial->getLog(SPS_M_ID));
	log.cClock->SetValue(pSerial->getLog(CLOCK_M_ID));
	log.cEKFState->SetValue(pSerial->getLog(EKF_STATE_M_ID));
	log.cEKFCov->SetValue(pSerial->getLog(EKF_COVARIANCE_M_ID));
	log.cEKFRes->SetValue(pSerial->getLog(EKF_RESIDUAL_M_ID));
	log.cPPS->SetValue(pSerial->getLog(PPS_M_ID));
	log.cPseudo->SetValue(pSerial->getLog(PSEUDORANGE_M_ID));
	//log.cMeas->SetValue(pSerial->getLog(MEASUREMENT_M_ID));
	log.cChan->SetValue(pSerial->getLog(CHANNEL_M_ID));
	log.cTask->SetValue(pSerial->getLog(TASK_HEALTH_M_ID));
	log.cBoard->SetValue(pSerial->getLog(BOARD_HEALTH_M_ID));
	log.cSVPred->SetValue(pSerial->getLog(SV_PREDICTION_M_ID));
	log.cGearth->SetValue(pSerial->getLog(LAST_M_ID));
	log.cRobs->SetValue(pSerial->getLog(LAST_M_ID + 1));
	log.cRephem->SetValue(pSerial->getLog(LAST_M_ID + 2));

	if(log.ShowModal() == wxID_OK)
	{
		/* Get options */
		pSerial->logOn(TASK_HEALTH_M_ID, 	log.cTask->IsChecked());
		pSerial->logOn(SPS_M_ID, 			log.cPVT->IsChecked());
		pSerial->logOn(CLOCK_M_ID, 			log.cClock->IsChecked());
		pSerial->logOn(EKF_STATE_M_ID, 		log.cEKFState->IsChecked());
		pSerial->logOn(EKF_COVARIANCE_M_ID, log.cEKFCov->IsChecked());
		pSerial->logOn(EKF_RESIDUAL_M_ID, 	log.cEKFRes->IsChecked());
		pSerial->logOn(PPS_M_ID, 			log.cPPS->IsChecked());
		pSerial->logOn(PSEUDORANGE_M_ID, 	log.cPseudo->IsChecked());
		//pSerial->logOn(MEASUREMENT_M_ID, 	log.cMeas->IsChecked());
		pSerial->logOn(CHANNEL_M_ID, 		log.cChan->IsChecked());
		pSerial->logOn(TASK_HEALTH_M_ID, 	log.cTask->IsChecked());
		pSerial->logOn(BOARD_HEALTH_M_ID, 	log.cBoard->IsChecked());
		pSerial->logOn(SV_PREDICTION_M_ID, 	log.cSVPred->IsChecked());
		pSerial->logOn(LAST_M_ID, 			log.cGearth->IsChecked());
		pSerial->logOn(LAST_M_ID + 1, 		log.cRobs->IsChecked());
		pSerial->logOn(LAST_M_ID + 2, 		log.cRephem->IsChecked());

		/* Get Filename */
		log_filename = log.mFile->GetPath();
		pSerial->setLogFile(log_filename.mb_str());

		mLogging->Enable(ID_LOG_START, true);
		mLogging->Enable(ID_LOG_CLEAR, true);
		mLogging->Enable(ID_LOG_STOP, false);
		mLogging->Enable(ID_LOG_CONFIG, true);
	}

	pSerial->Unlock();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onLogStart(wxCommandEvent& WXUNUSED(event))
{

	pSerial->Lock();
	pSerial->logStart();
	pSerial->Unlock();

	logging_on = true;

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

	logging_on = false;

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
void GUI_Toplevel::onSerial(wxCommandEvent& WXUNUSED(event))
{
	pSerial->Lock();
	pSerial->setIO(1);
	pSerial->Unlock();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onNpipe(wxCommandEvent& WXUNUSED(event))
{
	pSerial->Lock();
	pSerial->setIO(0);
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

	int page, this_tic, update;
    wxString str;
    wxString str2;

	if(pSerial->TryLock() == 0)
	{
		if(pSerial->messages.sps.tic != last_tic)
		{
			memcpy(&messages,pSerial->GetMessages(),sizeof(Message_Struct));
			last_tic = pSerial->messages.sps.tic;
			update = true;
		}
		else
		{
			update = false;
		}
		pSerial->Unlock();
	}

	/* Render RS422 Panel */
	renderRS422();

//	if(update)
	{

		/* Render Task Panel */
		renderTask();

		str.Printf(wxT("Count: %d"),count++);
		str += status_str;

		SetStatusText(str);

		/* Render main window */
		if(wPVT != NULL)
			wPVT->paintNow();

		/* Render main window */
		if(wEKF != NULL)
			wEKF->paintNow();

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
		if(wSelect != NULL)
			wSelect->paintNow();

		/* Render channel window */
		if(wEphemeris != NULL)
			wEphemeris->paintNow();

		/* Render SV Select window */
		if(wHealth != NULL)
			wHealth->paintNow();

		/* Display EEPROM */
//		if(wEEPROM != NULL)
//			wEEPROM->paintNow();

		/* Display Messages */
		if(wMessages != NULL)
			wMessages->paintNow();

		/* Display Speedo */
		if(wSpeedo != NULL)
			wSpeedo->paintNow();
    }

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::renderRS422()
{

	wxString str;

	tRS422->Clear();

	str = wxT("Interface: ");
	if(pSerial->serial)
		str += wxT("\t\tSerial");
	else
		str += wxT("\t\tNamed Pipe");

	if(pSerial->npipe_open)
		str += wxT(" (OPEN)\n");
	else
		str += wxT(" (CLOSED)\n");

	tRS422->AppendText(str);

	str.Printf(wxT("Synchronized Count:\t%d\n"),pSerial->message_sync);
	tRS422->AppendText(str);
	str.Printf(wxT("Last Message Tic:\t%d\n"),pSerial->decoded_packet.tic);
	tRS422->AppendText(str);
	str.Printf(wxT("Last Command Tic:\t%d\n"),messages.command_ack.command_tic);
	tRS422->AppendText(str);
	str.Printf(wxT("Failed Messages:\t%d\n"),pSerial->packet_count[LAST_M_ID]);
	tRS422->AppendText(str);
	kB_sec += .25 * (((float)bytes_sec/1024.0) - kB_sec);
	str.Printf(wxT("Serial Bandwidth:\t%.2f "),kB_sec);
	str += wxT("kB/sec\n");
	tRS422->AppendText(str);

	if(logging_on)
	{
		str = wxT("Logging:\t\tON\n");
		tRS422->AppendText(str);
		str = wxT("Filename:\t\t");
		str += log_filename;
		str += wxT("\n");
		tRS422->AppendText(str);
	}
	else
	{
		str = wxT("Logging:\t\tOFF\n");
		tRS422->AppendText(str);
	}

	if(count % 10 == 0)
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
	int32 lcv;
	Task_Health_M *pTask;

	wxString str;
	wxString names[MAX_TASKS];

	names[COMMANDO_TASK_ID]		= wxT("COMMANDO    ");
	names[ACQUISITION_TASK_ID]	= wxT("ACQUISITION ");
	names[SV_SELECT_TASK_ID]	= wxT("SV_SELECT   ");
	names[EPHEMERIS_TASK_ID]	= wxT("EPHEMERIS   ");
	names[TRACKING_ISR_TASK_ID] = wxT("TRACKING    ");
	names[TELEMETRY_TASK_ID] 	= wxT("TELEMETRY   ");
	names[PATIENCE_TASK_ID] 	= wxT("PATIENCE    ");
	names[IDLE_TASK_ID] 		= wxT("IDLE        ");
	names[EKF_TASK_ID]	 		= wxT("EKF         ");
	names[PVT_TASK_ID]	 		= wxT("PVT         ");
	names[PPS_TASK_ID]	 		= wxT("PPS         ");

	tTask->Clear();

	pTask = &messages.task_health;

	str = wxT("Task        Execution Tic   Delta     Start Tic    Stop Tic\n");
	tTask->AppendText(str);
	str = wxT("-----------------------------------------------------------\n");
	tTask->AppendText(str);

	for(lcv = 0; lcv < MAX_TASKS-1; lcv++)
	{
		if(names[lcv].Len())
		{
			str.Printf(wxT("%s   %10u %7d    %10u  %10u\n"),
				names[lcv].c_str(),
				pTask->execution_tic[lcv],
				pTask->stop_tic[lcv]-pTask->start_tic[lcv],
				pTask->start_tic[lcv],
				pTask->stop_tic[lcv]);
			tTask->AppendText(str);
		}
	}

	if(names[lcv].Len())
	{
		str.Printf(wxT("%s   %10u %7d    %10u  %10u\n"),
			names[lcv].c_str(),
			pTask->execution_tic[lcv],
			pTask->stop_tic[lcv]-pTask->start_tic[lcv],
			pTask->start_tic[lcv],
			pTask->stop_tic[lcv]);
		tTask->AppendText(str);
	}

//	str.Printf(wxT("\nMissed Interrupts: %10d"),pTask->missed_interrupts),
//	tTask->AppendText(str);
//
//	str.Printf(wxT("\nFPU Multiplies:    %10d"),pTask->tic_fpu_mul),
//	tTask->AppendText(str);
//
//	str.Printf(wxT("\nFPU Divides:       %10d"),pTask->tic_fpu_div),
//	tTask->AppendText(str);


}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onPVT(wxCommandEvent& WXUNUSED(event))
{

	int32 val;

	if(wPVT)
	{
		delete wPVT;
		wPVT = NULL;
		bPVT->SetValue(false);
	}
	else
	{
		wPVT = new GUI_PVT();
		wPVT->setPointer(&messages);
		wPVT->setSerial(pSerial);
		wPVT->setToplevel(this);
		wPVT->Show(TRUE);
		bPVT->SetValue(true);
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
	}
	else
	{
		wChannel = new GUI_Channel();
		wChannel->setPointer(&messages);
		wChannel->setSerial(pSerial);
		wChannel->setToplevel(this);
		wChannel->Show(TRUE);
		bChannel->SetValue(true);
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

	}
	else
	{
		wPseudo = new GUI_Pseudo();
		wPseudo->setPointer(&messages);
		wPseudo->setSerial(pSerial);
		wPseudo->setToplevel(this);
		wPseudo->Show(TRUE);
		bPseudo->SetValue(true);
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
void GUI_Toplevel::onEEPROM(wxCommandEvent& WXUNUSED(event))
{

//	if(wEEPROM)
//	{
//		delete wEEPROM;
//		wEEPROM = NULL;
//		bEEPROM->SetValue(false);
//	}
//	else
//	{
//		wEEPROM = new GUI_EEPROM();
//		wEEPROM->setPointer(&messages);
//		wEEPROM->setSerial(pSerial);
//		wEEPROM->setToplevel(this);
//		wEEPROM->Show(TRUE);
//		bEEPROM->SetValue(true);
//	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onEKF(wxCommandEvent& WXUNUSED(event))
{

	if(wEKF)
	{
		delete wEKF;
		wEKF = NULL;
		bEKF->SetValue(false);
	}
	else
	{
		wEKF = new GUI_EKF();
		wEKF->setPointer(&messages);
		wEKF->setSerial(pSerial);
		wEKF->setToplevel(this);
		wEKF->Show(TRUE);
		bEKF->SetValue(true);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onHealth(wxCommandEvent& WXUNUSED(event))
{

	if(wHealth)
	{
		delete wHealth;
		wHealth = NULL;
		bHealth->SetValue(false);
	}
	else
	{
		wHealth = new GUI_Health();
		wHealth->setPointer(&messages);
		wHealth->setSerial(pSerial);
		wHealth->setToplevel(this);
		wHealth->Show(TRUE);
		bHealth->SetValue(true);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onMessages(wxCommandEvent& WXUNUSED(event))
{

	if(wMessages)
	{
		delete wMessages;
		wMessages = NULL;
		bMessages->SetValue(false);
	}
	else
	{
		wMessages = new GUI_Messages();
		wMessages->setPointer(&messages);
		wMessages->setSerial(pSerial);
		wMessages->setToplevel(this);
		wMessages->Show(TRUE);
		bMessages->SetValue(true);
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GUI_Toplevel::onSpeedo(wxCommandEvent& WXUNUSED(event))
{

	if(wSpeedo)
	{
		delete wSpeedo;
		wSpeedo = NULL;
		bSpeedo->SetValue(false);
	}
	else
	{
		wSpeedo = new GUI_Speedo();
		wSpeedo->setPointer(&messages);
		wSpeedo->setSerial(pSerial);
		wSpeedo->setToplevel(this);
		wSpeedo->Show(TRUE);
		bSpeedo->SetValue(true);
	}

}
/*----------------------------------------------------------------------------------------------*/
