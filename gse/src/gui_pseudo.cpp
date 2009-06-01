/*
 * gui_Channel.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

extern wxColor red;
extern wxColor green;
extern wxColor blue;
extern wxColor yellow;
extern wxColor white;
extern wxColor black;
extern wxColor grey;
extern wxColor htext;

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_Pseudo, wxFrame)
    EVT_CLOSE(GUI_Pseudo::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Pseudo::GUI_Pseudo():iGUI_Pseudo(NULL, wxID_ANY, wxT("Pseudoranges"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{

	int32 strwidth;
	wxString str;

	str.Printf(wxT("Ch#  SV     Pseudorange        Residual         PR Rate        Residual\n"));
	strwidth = str.Length()+2;
	tPseudo->AppendText(str);
	SetSize(tPseudo->GetCharWidth()*strwidth, tPseudo->GetCharHeight()*(MAX_CHANNELS+3));

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
	int32 lcv, strwidth;
	wxTextAttr style;
    wxString str, bstr;
	SPS_M *pNav = &p->sps;
	Channel_M *pchan;
	Pseudorange_M *ps;
	double cr;

	tPseudo->Clear();
	style = tPseudo->GetDefaultStyle();
	style.SetBackgroundColour(white);
	tPseudo->SetDefaultStyle(style);
	cr = pNav->clock_rate;

	str.Printf(wxT("Ch#  SV     Pseudorange        Residual         PR Rate        Residual\n"));
	strwidth = str.Length()+1;
	tPseudo->AppendText(str);
	str.Printf(wxT("                    (m)             (m)           (m/s)           (m/s)\n"));
	tPseudo->AppendText(str);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		pchan = &p->channel[lcv];
		ps = &p->pseudoranges[lcv];

		if((lcv+1) & 0x1)
			style.SetBackgroundColour(htext);
		else
			style.SetBackgroundColour(white);

		tPseudo->SetDefaultStyle(style);

		if((pNav->nsvs >> lcv) & 0x1)
		{
			str.Printf(wxT("%2d   %2d  % 14.3f  % 14.3f  % 14.3f  % 14.3f"),
				lcv,
				pNav->chanmap[lcv]+1,
				ps->meters,
				ps->residual,
				ps->meters_rate - cr,
				ps->residual_rate);

			if(str.Length() <= strwidth)
				tPseudo->AppendText(str);
			else
			{
				bstr.Printf(wxT("%2d                                                                     "),lcv);
				tPseudo->AppendText(bstr);
			}
		}
		else
		{
			bstr.Printf(wxT("%2d                                                                     "),lcv);
			tPseudo->AppendText(bstr);
		}

		if(lcv < (MAX_CHANNELS-1))
			tPseudo->AppendText(wxT("\n"));
	}

}
