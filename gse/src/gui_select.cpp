/*
 * gui_toplevel.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

wxColor grey(127,127,127);
wxColor blue(0,0,255);
wxColor white(255,255,255);
wxColor yellow(255,255,0);
wxColor green(0,255,0);
wxColor red(255,0,0);
wxColor black(0,0,0);

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_Select, wxFrame)
    EVT_CLOSE(GUI_Select::onClose)
    EVT_LEFT_DOWN(GUI_Select::onMouse)
    EVT_BUTTON(ID_SELECT_REFRESH, GUI_Select::onRefresh)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Select::GUI_Select():iGUI_Select(NULL, wxID_ANY, wxT("SV Select"), wxDefaultPosition, wxSize(400,800), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{
	sv = 0;
	loaded = 0;
}

GUI_Select::~GUI_Select()
{


}

void GUI_Select::onClose(wxCloseEvent& evt)
{
	wxCommandEvent cevt;
	evt.Veto();
	pToplevel->onSelect(cevt);
}

void GUI_Select::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_Select::render(wxDC& dc)
{
	int32 val;

	renderVisible();
	renderMode();
	renderSV();

	if(loaded == 0)
	{
		pSerial->formCommand(GET_SV_SELECT_STATUS_C_ID, &loaded);
		loaded = 1;
	}
}


void GUI_Select::onMouse(wxMouseEvent& event)
{

	wxString str;
	wxPoint br, tl, sp, mp;
	wxCoord w, h;

	int32 row, col;
	int32 dX,dY;

	pVisible->GetClientSize(&w, &h);
	sp = this->GetScreenPosition();
	tl = pVisible->GetScreenPosition() - sp;
	mp = wxGetMousePosition() - sp;

	dY = (h/4);
	dX = (w/(MAX_SV>>2));

	br.x = tl.x + w;
	br.y = tl.y + h;

	/* Within the pVisible area */
	if((mp.x >= tl.x) && (mp.x <= br.x) && (mp.y >= tl.y) && (mp.y <= br.y))
	{
		mp -= tl;

		row = mp.y/dY;
		col = mp.x/dX;
		sv = row*(MAX_SV>>2);
		sv += col;

		if(sv >= 0 && sv <= MAX_SV)
			pSerial->formCommand(GET_SV_PREDICTION_C_ID, &sv);
		else
			sv = 0;
	}
}

void GUI_Select::renderMode()
{

	int32 dX, dY, lcv;

	wxPoint box[4];
	wxString str[3];
	wxCoord w, h;

	wxBufferedPaintDC dc(pMode, wxBUFFER_CLIENT_AREA);
	dc.Clear();

	str[0] = wxT("Cold");
	str[1] = wxT("Warm");
	str[2] = wxT("Hot");

	pMode->GetClientSize(&w, &h);

	dY = h;
	dX = w/3;

	box[0].x = 0;	box[0].y = 0;
	box[1].x = dX;	box[1].y = 0;
	box[2].x = dX;	box[2].y = dY;
	box[3].x = 0;	box[3].y = dY;

	dc.SetFont(wxFont(12, wxDEFAULT, wxNORMAL, wxNORMAL));

	/* Render the decoded ephemerides */
	for(lcv = 0; lcv < 3; lcv++)
	{
		if(p->sv_select_status.mode == lcv)
		{
			dc.SetBrush(wxBrush(green));
			dc.SetPen(wxPen(black, 1));
		}
		else
		{
			dc.SetBrush(wxBrush(grey));
			dc.SetPen(wxPen(black, 1));
		}

		dc.DrawPolygon(4, box, lcv*dX, 0);
		dc.DrawText(str[lcv], lcv*dX + dX/3, 0 + dY/3);
	}

}

void GUI_Select::renderVisible()
{

	int32 dX, dY, lcv;
	wxPoint box[4];
	wxString str;

	wxBufferedPaintDC dc(pVisible, wxBUFFER_CLIENT_AREA);
	dc.Clear();

	wxCoord w, h;
	pVisible->GetClientSize(&w, &h);

	dY = (h/4);
	dX = (w/(MAX_SV>>2));

	box[0].x = 0;	box[0].y = 0;
	box[1].x = dX;	box[1].y = 0;
	box[2].x = dX;	box[2].y = dY;
	box[3].x = 0;	box[3].y = dY;

	dc.SetFont(wxFont(12, wxDEFAULT, wxNORMAL, wxNORMAL));

	/* Render the decoded ephemerides */
	for(lcv = 0; lcv < MAX_SV; lcv++)
	{

		switch(p->sv_select_status.state[lcv])
		{
			case 3:
				dc.SetBrush(wxBrush(green));
				break;
			case 2:
				dc.SetBrush(wxBrush(yellow));
				break;
			case 1:
				dc.SetBrush(wxBrush(blue));
				break;
			default:
				dc.SetBrush(wxBrush(grey));
				break;
		}

		dc.SetPen(wxPen(black, 1));
		dc.DrawPolygon(4, box, (lcv%(MAX_SV>>2))*dX, (lcv/(MAX_SV>>2))*dY);

		str.Printf(wxT("%02d"),(int)lcv+1);
		dc.DrawText(str, (lcv%(MAX_SV>>2))*dX + dX/3, (lcv/(MAX_SV>>2))*dY + dY/5);
	}

	/* Draw highlighted box */
	switch(p->sv_select_status.state[sv])
	{
		case 3:
			dc.SetBrush(wxBrush(green));
			break;
		case 2:
			dc.SetBrush(wxBrush(yellow));
			break;
		case 1:
			dc.SetBrush(wxBrush(blue));
			break;
		default:
			dc.SetBrush(wxBrush(grey));
			break;
	}

	dc.SetPen(wxPen(red, 2));
	dc.DrawPolygon(4, box, (sv%(MAX_SV>>2))*dX, (sv/(MAX_SV>>2))*dY);

	str.Printf(wxT("%02d"),(int)sv+1);
	dc.DrawText(str, (sv%(MAX_SV>>2))*dX + dX/3, (sv/(MAX_SV>>2))*dY + dY/5);

}

void GUI_Select::renderSV()
{

	int32 lcv, lcv2, start, stop, lines;
	wxTextAttr text;
	wxString str;
	tDisplay->Clear();

	SV_Prediction_M *psv;

	text.SetBackgroundColour(white);
	tDisplay->SetDefaultStyle(text);

	str.Printf(wxT("Ch# Vis   Elev     Azim     Doppler        Time\n\n"));
	tDisplay->AppendText(str);

	for(lcv = 0; lcv < MAX_SV; lcv++)
	{
		psv = &p->sv_predictions[lcv];

		if(lcv == p->sv_predictions[MAX_SV].sv)
		{
			text.SetBackgroundColour(grey);
			tDisplay->SetDefaultStyle(text);
		}
		else
		{
			text.SetBackgroundColour(white);
			tDisplay->SetDefaultStyle(text);
		}

		if(psv->visible)
			str.Printf(wxT("%02d   V %7.2f  %7.2f  %10.2f  %10.2f\n"),lcv+1,psv->elev*RAD_2_DEG,psv->azim*RAD_2_DEG,psv->doppler,psv->time);
		else
			str.Printf(wxT("%02d   _ %7.2f  %7.2f  %10.2f  %10.2f\n"),lcv+1,psv->elev*RAD_2_DEG,psv->azim*RAD_2_DEG,psv->doppler,psv->time);
		tDisplay->AppendText(str);
	}

}

void GUI_Select::onRefresh(wxCommandEvent& event)
{
	int32 val = 0;
	pSerial->formCommand(GET_SV_SELECT_STATUS_C_ID, &val);
}
