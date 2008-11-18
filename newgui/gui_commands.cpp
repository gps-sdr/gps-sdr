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
    EVT_BUTTON(ID_RESET_PVT,		GUI_Commands::onPVT)
    EVT_BUTTON(ID_RESET_CHANNEL,	GUI_Commands::onChannel)
	EVT_BUTTON(ID_RESET_EPHEMERIS,	GUI_Commands::onEphemeris)
	EVT_BUTTON(ID_RESET_ALMANAC,	GUI_Commands::onAlmanac)
    EVT_CLOSE(GUI_Commands::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Commands::GUI_Commands():iGUI_Commands(NULL, wxID_ANY, wxT("Commands"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{


}

GUI_Commands::~GUI_Commands()
{


}

void GUI_Commands::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_Commands::render(wxDC& dc)
{


}


void GUI_Commands::onPVT(wxCommandEvent& event)
{

	wxString str;
	int32 val;

	val = mReset_PVT->GetCurrentSelection();

	pSerial->formCommand(RESET_PVT_C_ID, &val);

	str.Printf(wxT("Reset PVT:\t\t%d\n"),val);
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
	str.Printf(wxT("Reset Channel:\t%d\n"),val);
	tCommand_Ack->AppendText(str);
}

void GUI_Commands::onEphemeris(wxCommandEvent& event)
{

	wxString str;
	int32 val;

	val = mReset_Ephemeris->GetCurrentSelection();

	if(val == 0)
		val = NUM_CODES;
	else
		val = val-1;

	pSerial->formCommand(RESET_EPHEMERIS_C_ID, &val);
	str.Printf(wxT("Reset Ephemeris:\t%d\n"),val+1);
	tCommand_Ack->AppendText(str);
}

void GUI_Commands::onAlmanac(wxCommandEvent& event)
{

	wxString str;
	int32 val;

	val = mReset_Almanac->GetCurrentSelection();

	if(val == 0)
		val = NUM_CODES;
	else
		val = val-1;

	pSerial->formCommand(RESET_ALMANAC_C_ID, &val);
	str.Printf(wxT("Reset Almanac:\t%d\n"),val+1);
	tCommand_Ack->AppendText(str);
}

