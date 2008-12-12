/*
 * gui_Channel.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_Pseudo, wxFrame)
    EVT_CLOSE(GUI_Pseudo::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Pseudo::GUI_Pseudo():iGUI_Pseudo(NULL, wxID_ANY, wxT("Pseudoranges"), wxDefaultPosition, wxSize(650,250), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{


}

GUI_Pseudo::~GUI_Pseudo()
{


}

void GUI_Pseudo::onClose(wxCloseEvent& evt)
{
	wxCommandEvent cevt;
	evt.Veto();
	pToplevel->onPseudo(cevt);
}

void GUI_Pseudo::paintNow()
{

    wxClientDC dc(this);
    render(dc);

}

void GUI_Pseudo::render(wxDC& dc)
{

	SPS_M *pNav = &p->sps;
	Channel_M *pchan;
	Pseudorange_M *ps;

	wxTextAttr text;
    wxString str, str2;
	int32 lcv, lcv2, start, stop, lines;

	tPseudo->Clear();

	str.Printf(wxT("Ch#  SV     Pseudorange        Residual         PR Rate        Residual\n"));
	tPseudo->AppendText(str);
	str.Printf(wxT("                    (m)             (m)           (m/s)           (m/s)\n"));
	tPseudo->AppendText(str);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		pchan = &p->channel[lcv];
		ps = &p->pseudoranges[lcv];

		if((pNav->nsvs >> lcv) & 0x1)
		{
			str.Printf(wxT("%2d   %2d  % 14.3f  % 14.3f  % 14.3f  % 14.3f\n"),
				lcv,
				pNav->chanmap[lcv]+1,
				ps->meters,
				ps->residual,
				ps->meters_rate - pNav->clock_rate,
				ps->rate_residual);
				tPseudo->AppendText(str);
		}
		else
		{
			str.Printf(wxT("%2d                                                                     \n"),lcv);
			tPseudo->AppendText(str);
		}
	}

	text = tPseudo->GetDefaultStyle();
	lines = tPseudo->GetNumberOfLines();
	start = 0; stop = 0;
	for(lcv = 0; lcv < lines; lcv++)
	{
		stop += tPseudo->GetLineLength(lcv)+1;

		if((lcv+1) & 0x1)
		{
			text.SetBackgroundColour(wxColor(248,248,255));
			tPseudo->SetStyle(start, stop, text);
		}
		else
		{
			text.SetBackgroundColour(wxColor(255,255,255));
			tPseudo->SetStyle(start, stop, text);
		}

		start = stop+1;
	}


}
