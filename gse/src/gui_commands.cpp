/*
 * gui_toplevel.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_Commands, wxFrame)
	EVT_BUTTON(ID_NULL,				GUI_Commands::onNull)
	EVT_BUTTON(ID_RESET_ALL,		GUI_Commands::onAll)
    EVT_BUTTON(ID_RESET_PVT,		GUI_Commands::onPVT)
    EVT_BUTTON(ID_RESET_CHANNEL,	GUI_Commands::onChannel)
	EVT_BUTTON(ID_RESET_EPHEMERIS,	GUI_Commands::onEphemeris)
	EVT_BUTTON(ID_RESET_ALMANAC,	GUI_Commands::onAlmanac)
	EVT_BUTTON(ID_RESET_PPS,		GUI_Commands::onPPS)
	EVT_BUTTON(ID_RESET_EKF,		GUI_Commands::onEKF)
	EVT_BUTTON(ID_RESET_WATCHDOG,	GUI_Commands::onWatchdog)
    EVT_CLOSE(GUI_Commands::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Commands::GUI_Commands():iGUI_Commands(NULL, wxID_ANY, wxT("Commands"), wxDefaultPosition, wxSize(400,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{


}

GUI_Commands::~GUI_Commands()
{


}

void GUI_Commands::onClose(wxCloseEvent& evt)
{
	wxCommandEvent cevt;
	evt.Veto();
	pToplevel->onCommands(cevt);
}

void GUI_Commands::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_Commands::render(wxDC& dc)
{


}

void GUI_Commands::onNull(wxCommandEvent& event)
{
	wxString str;
	int32 val;

	val = 1;

	pSerial->pendCommand();
	pSerial->formCommand(NULL_C_ID, &val);

	str.Printf(wxT("Null Cmd:\t\t%d\n"),val);
	tCommand_Ack->AppendText(str);
}

void GUI_Commands::onAll(wxCommandEvent& event)
{
	wxString str;
	int32 val;

	val = 1;

	pSerial->pendCommand();
	pSerial->formCommand(RESET_ALL_C_ID, &val);

	str.Printf(wxT("Reset All:\t\t%d\n"),val);
	tCommand_Ack->AppendText(str);
}

void GUI_Commands::onPVT(wxCommandEvent& event)
{

	wxString str;
	int32 val;

	val = 0;

	pSerial->pendCommand();
	pSerial->formCommand(RESET_PVT_C_ID, &val);

	str.Printf(wxT("Reset PVT:\t\t%d\n"),val);
	tCommand_Ack->AppendText(str);

}

void GUI_Commands::onEKF(wxCommandEvent& event)
{

	wxString str;
	int32 val;

	val = 0;

	pSerial->pendCommand();
	pSerial->formCommand(RESET_EKF_C_ID, &val);

	str.Printf(wxT("Reset EKF:\t\t%d\n"),val);
	tCommand_Ack->AppendText(str);

}


void GUI_Commands::onWatchdog(wxCommandEvent& event)
{

	wxString str;
	int32 val;

	val = 0;

	pSerial->pendCommand();
	pSerial->formCommand(RESET_WATCHDOG_C_ID, &val);

	str.Printf(wxT("Reset Watchdog:\t\t%d\n"),val);
	tCommand_Ack->AppendText(str);

}

void GUI_Commands::onPPS(wxCommandEvent& event)
{

	wxString str;
	int32 val;

	val = 0;

	pSerial->pendCommand();
	pSerial->formCommand(RESET_PPS_C_ID, &val);

	str.Printf(wxT("Reset PPS:\t\t%d\n"),val);
	tCommand_Ack->AppendText(str);

}

void GUI_Commands::onChannel(wxCommandEvent& event)
{

	wxString str;
	int32 val;

	val = mReset_Channel->GetCurrentSelection();

	if(val == 0)
		val = MAX_CHANNELS;
	else
		val = val-1;

	pSerial->formCommand(RESET_CHANNEL_C_ID, &val);
	str.Printf(wxT("Reset Channel:\t\t%d\n"),val);
	tCommand_Ack->AppendText(str);
}

void GUI_Commands::onEphemeris(wxCommandEvent& event)
{

	wxString str;
	int32 val;

	val = mReset_Ephemeris->GetCurrentSelection();

	if(val == 0)
		val = MAX_SV;
	else
		val = val-1;

	pSerial->pendCommand();
	pSerial->formCommand(RESET_EPHEMERIS_C_ID, &val);

	str.Printf(wxT("Reset Ephemeris:\t%d\n"),val+1);
	tCommand_Ack->AppendText(str);

	pSerial->pendCommand();
	pSerial->formCommand(GET_EPHEMERIS_STATUS_C_ID, &val);
}

void GUI_Commands::onAlmanac(wxCommandEvent& event)
{

	wxString str;
	int32 val;

	val = mReset_Almanac->GetCurrentSelection();

	if(val == 0)
		val = MAX_SV;
	else
		val = val-1;

	pSerial->pendCommand();
	pSerial->formCommand(RESET_ALMANAC_C_ID, &val);

	str.Printf(wxT("Reset Almanac:\t\t%d\n"),val+1);
	tCommand_Ack->AppendText(str);

	pSerial->pendCommand();
	pSerial->formCommand(GET_EPHEMERIS_STATUS_C_ID, &val);
}

