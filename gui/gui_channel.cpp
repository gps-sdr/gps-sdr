/*
 * gui_Channel.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_Channel, wxFrame)
    EVT_CLOSE(GUI_Channel::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Channel::GUI_Channel():iGUI_Channel(NULL, wxID_ANY, wxT("Channel"), wxDefaultPosition, wxSize(650,250), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{


}

GUI_Channel::~GUI_Channel()
{


}

void GUI_Channel::onClose(wxCloseEvent& evt)
{
	wxCommandEvent cevt;
	evt.Veto();
	pToplevel->onChannel(cevt);
}

void GUI_Channel::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_Channel::render(wxDC& dc)
{

	SPS_M *pNav = &p->sps;
	Channel_M *pchan;
	wxTextAttr text;
    wxString str, str2;
	int32 lcv, lcv2, start, stop, lines;
	float cn0;

	tChannel->Clear();

	str.Printf(wxT("Ch#  SV   CL       Faccel          Doppler     CN0   BE       Locks        Power   Active\n"));
	tChannel->AppendText(str);

	//str.Printf(wxT("-----------------------------------------------------------------------------------------\n"));
	//tChannel->AppendText(str);
	str.Printf(wxT("\n"));
	tChannel->AppendText(str);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		pchan = &p->channel[lcv];

		if(pchan->count > 3000)
		{

			str2.Clear();

			/*Flag buffer*/
			str2 += ' ';
			((int32)pchan->bit_lock) ? 				str2 += 'B' : str2 += '-';
			((int32)pchan->frame_lock) ? 			str2 += 'F' : str2 += '-';
			(pNav->nsvs >> lcv) & 0x1 ? 		str2 += 'N' : str2 += '-';

			for(lcv2 = 1; lcv2 < 6; lcv2++)
			{
				if((int32)pchan->subframe == lcv2)
					str2 += (char)((int32)pchan->subframe + 48);
				else
					str2 += '-';
			}

			cn0 = pchan->CN0;

			str.Printf(wxT("%2d   %2d   %2d   %10.3f   %14.3f   %5.2f   %2d   %s   %10.0f   %6d\n"),
				lcv,
				(int32)pchan->sv+1,
				(int32)pchan->len,
				pchan->w,
				pchan->carrier_nco - IF_FREQUENCY,
				cn0,
				(int32)pchan->best_epoch,
				str2.c_str(),
				pchan->p_avg,
				(int32)pchan->count/1000);

			tChannel->AppendText(str);
		}
		else
		{
			//str.Printf(wxT("%2d   --   --   ----------   --------------   -----   --   ---------   ----------   ------\n"),lcv);
			str.Printf(wxT("%2d                                                                                       \n"),lcv);
			tChannel->AppendText(str);
		}
	}

	text = tChannel->GetDefaultStyle();
	lines = tChannel->GetNumberOfLines();
	start = 0; stop = 0;
	for(lcv = 0; lcv < lines; lcv++)
	{
		stop += tChannel->GetLineLength(lcv)+1;

		if((lcv+1) & 0x1)
		{
			text.SetBackgroundColour(wxColor(248,248,255));
			tChannel->SetStyle(start, stop, text);
		}
		else
		{
			text.SetBackgroundColour(wxColor(255,255,255));
			tChannel->SetStyle(start, stop, text);
		}

		start = stop+1;
	}

}
