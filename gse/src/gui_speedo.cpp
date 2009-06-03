/*
 * gui_speedo.cpp
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
BEGIN_EVENT_TABLE(GUI_Speedo, wxFrame)
    EVT_CLOSE(GUI_Speedo::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Speedo::GUI_Speedo():iGUI_Speedo(NULL, wxID_ANY, wxT("Speedo"), wxDefaultPosition, wxSize(600,400), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{

	speed = 0;

}


GUI_Speedo::~GUI_Speedo()
{


}

void GUI_Speedo::onClose(wxCloseEvent& evt)
{
	wxCommandEvent cevt;
	evt.Veto();
	pToplevel->onSpeedo(cevt);
}

void GUI_Speedo::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_Speedo::render(wxDC& dc)
{

	renderSpeedo();
	//renderStat();

}


void GUI_Speedo::renderSpeedo()
{

	int32 lcv;
	wxPoint box[4];
	wxCoord w, h, rmin, rmax, dx, dy;
	wxCoord dx1, dx2, dy1, dy2;
	wxString str;
	SPS_M *pNav = &p->sps;
	double speed;

	speed += (2.23693629*sqrt(pNav->vz*pNav->vz + pNav->vy*pNav->vy + pNav->vx*pNav->vx) - speed)*.25;

	tick++;

	wxBufferedPaintDC dc(pspeed, wxBUFFER_VIRTUAL_AREA);
	dc.Clear();

	pspeed->GetClientSize(&w, &h);
	dc.SetTextForeground(red);

	rmax = w > h ? w : h;
	rmin = w < h ? w : h;

	box[0].x = 0;	box[0].y = 0;
	box[1].x = w;	box[1].y = 0;
	box[2].x = w;	box[2].y = h;
	box[3].x = 0;	box[3].y = h;

	dc.SetBrush(wxBrush(black));
	dc.SetPen(wxPen(black, 1));
	dc.DrawPolygon(4, box, 0, 0);

	dc.SetBrush(wxBrush(black));
	dc.SetPen(wxPen(red, 2));

	dc.DrawCircle(w/2, h*3/4, rmin * 11/16);

	dc.SetPen(wxPen(red, 2));

	for(lcv = 0; lcv <= 20; lcv++)
	{
		dx1 = (rmin*10/16)*cos((lcv*10 + 170)*DEG_2_RAD);
		dy1 = (rmin*10/16)*sin((lcv*10 + 170)*DEG_2_RAD);
		dx2 = (rmin*11/16)*cos((lcv*10 + 170)*DEG_2_RAD);
		dy2 = (rmin*11/16)*sin((lcv*10 + 170)*DEG_2_RAD);
		dc.DrawLine(w/2 + dx1, h*3/4 + dy1, w/2 + dx2, h*3/4 + dy2);
	}

	dc.SetPen(wxPen(blue, 2));

	dc.DrawCircle(w/2, h*3/4, 4);

	dx = (rmin*9/16)*cos((speed + 170)*DEG_2_RAD);
	dy = (rmin*9/16)*sin((speed + 170)*DEG_2_RAD);
	dc.DrawLine(w/2, h*3/4, w/2 + dx, h*3/4 + dy);

	dc.SetBrush(wxBrush(red));
	dc.SetFont(wxFont(24, wxFONTFAMILY_TELETYPE, wxNORMAL, wxNORMAL));
	str.Printf(wxT("%6.1f"),speed);
	dc.DrawText(str, (w - 7*dc.GetCharWidth())/2, h*4/5);

//	circle at (100, 150) with radius 60 dc.DrawCircle(100, 150, 60) ...

}
