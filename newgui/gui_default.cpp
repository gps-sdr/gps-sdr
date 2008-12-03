/*
 * gui_default.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_Default, wxFrame)
    EVT_CLOSE(GUI_Default::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Default::GUI_Default():iGUI_Default(NULL, wxID_ANY, wxT("Main"), wxDefaultPosition, wxSize(980,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{

}

GUI_Default::~GUI_Default()
{


}

void GUI_Default::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_Default::render(wxDC& dc)
{

	renderCN0();
	renderSkyPlot();
	renderPVT();

}

void GUI_Default::renderCN0()
{

	Channel_M *pchan;
	int mX, mY, lcv, gval, rval;
	double maxX, maxY, svX, svY, dX, dY;
	double scaleX, scaleY;
	wxPoint bar[4];

	wxString str;
//	wxPaintDC dc(pCN0);
	wxBufferedPaintDC dc(pCN0, wxBUFFER_CLIENT_AREA);
	dc.Clear();

	maxX = maxY = 1000;

	wxCoord w, h;
	pCN0->GetClientSize(&w, &h);

	scaleX = w/maxX; scaleY = h/maxY;
	mX = w/2; mY = h/2;

	dc.SetFont(wxFont(12, wxDEFAULT, wxNORMAL, wxNORMAL));

	dc.SetPen(wxPen(wxColor(0,0,0), 1, wxLONG_DASH ));
    dc.DrawLine(mX-500*scaleX, h-800*scaleY, mX+500*scaleX, h-800*scaleY);
    dc.DrawLine(mX-500*scaleX, h-400*scaleY, mX+500*scaleX, h-400*scaleY);
    dc.DrawLine(mX-500*scaleX, h-200*scaleY, mX+500*scaleX, h-200*scaleY);

    /* Draw a circle */
    dc.SetPen(wxPen(wxColor(0,0,0), 1));
    dc.DrawLine(mX-500*scaleX, h-600*scaleY, mX+500*scaleX, h-600*scaleY);

    dc.DrawText(wxT("60 dB-Hz"), mX - 500*scaleX, h - 1000*scaleY);
    dc.DrawText(wxT("50 dB-Hz"), mX - 500*scaleX, h - 800*scaleY);
    dc.DrawText(wxT("40 dB-Hz"), mX - 500*scaleX, h - 600*scaleY);
    dc.DrawText(wxT("30 dB-Hz"), mX - 500*scaleX, h - 400*scaleY);
    dc.DrawText(wxT("20 dB-Hz"), mX - 500*scaleX, h - 200*scaleY);

    dX = (1000-150)/MAX_CHANNELS;
    dX *= scaleX;

    dc.DrawText(wxT("CH#"),mX-500*scaleX, h - 30*scaleY);
    dc.DrawText(wxT("SV#"),mX-500*scaleX, h - 60*scaleY);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		pchan = &p->channel_health[lcv];
		if(pchan->count > 2000.0)
		{
			str.Printf(wxT("%02d"),(int)pchan->sv+1);
			dc.DrawText(str, lcv*dX + 150*scaleX, h - 60*scaleY);

			dY = pchan->CN0 - 40.0;
			dY /= 40.0;
			dY *= 800;
			dY *= scaleY;
			bar[0].x = 0;		bar[0].y = 0;
			bar[1].x = dX/2;	bar[1].y = 0;
			bar[2].x = dX/2;	bar[2].y = -dY;
			bar[3].x = 0;		bar[3].y = -dY;

			gval = 255.0*(pchan->CN0 - 20.0)/40.0;
			rval = 122.0 - 122.0*(pchan->CN0 - 20.0)/40.0;
			dc.SetBrush(wxBrush(wxColor(rval,gval,0)));
			dc.SetPen(wxPen(wxColor(0,0,0), 1));
			dc.DrawPolygon(4, bar, lcv*dX + 150*scaleX, h-600*scaleY);
		}
		else
		{
			dc.DrawText(wxT("__"), lcv*dX + 150*scaleX, h - 60*scaleY);
		}

		str.Printf(wxT("%02d"),lcv);
		dc.DrawText(str, lcv*dX + 150*scaleX, h - 30*scaleY);

	}

}

void GUI_Default::renderSkyPlot()
{

	int mX, mY, lcv, rval, gval;
	double maxX, maxY, svX, svY;
	double scaleX, scaleY;
	wxString str;

	SPS_M *pNav = &p->sps;
	SV_Position_M *psv;
	Channel_M *pchan;

	maxX = maxY = 1100;

	//wxPaintDC dc(pSkyPlot);
	//wxBufferedPaintDC dc(pSkyPlot, pSkyPlot->GetSize(), wxBUFFER_CLIENT_AREA);
	wxBufferedPaintDC dc(pSkyPlot, wxBUFFER_CLIENT_AREA);
	dc.Clear();

	wxCoord w, h;
	pSkyPlot->GetClientSize(&w, &h);

	scaleX = .5*w/maxX; scaleY = .5*h/maxY;

	mX = w/2; mY = h/2;

    /* Draw a circle */
    dc.SetPen(wxPen(wxColor(0,0,0), 1 ));
    dc.DrawCircle(mX, mY, 900*scaleY);
    dc.SetPen(wxPen(wxColor(0,0,0), 1, wxLONG_DASH ));
    /* Draw the circles */
    for(lcv = 5; lcv > 0; lcv--)
    {
    	dc.DrawCircle(mX, mY, lcv*150*scaleY);
    }

    /* Draw the 30 deg lines */
    for(lcv = 0; lcv < 6; lcv++)
    {
    	dc.DrawLine(mX-900*scaleY*cos(lcv*30*DEG_2_RAD),
					mY-900*scaleY*sin(lcv*30*DEG_2_RAD),
					mX+900*scaleY*cos(lcv*30*DEG_2_RAD),
					mY+900*scaleY*sin(lcv*30*DEG_2_RAD));
    }

    dc.SetPen(wxPen(wxColor(0,0,0), 1 ));
    dc.DrawLine(mX, mY-900*scaleY, mX, mY+900*scaleY);
    dc.DrawLine(mX-900*scaleY, mY, mX+900*scaleY, mY);

    dc.SetBrush(wxBrush(wxColor(0,0,0)));
    dc.SetFont(wxFont(14, wxDEFAULT, wxNORMAL, wxBOLD));

    /* Draw the "NESW" */
    str = wxT("N"); dc.DrawText(str,mX-5, mY-900*scaleY-20);
    str = wxT("S"); dc.DrawText(str,mX-5, mY+900*scaleY);
    str = wxT("E"); dc.DrawText(str,mX+900*scaleY+5, mY-10);
    str = wxT("W"); dc.DrawText(str,mX-900*scaleY-25, mY-10);

    /* Now place the SVs */
    for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
    {
    	pchan = &p->channel_health[lcv];
    	psv = &p->sv_positions[lcv];
    	if((pNav->nsvs >> lcv) & 0x1)
    	{
			gval = 255.0*(pchan->CN0 - 20.0)/40.0;
			rval = 122.0 - 122.0*(pchan->CN0 - 20.0)/40.0;
			dc.SetBrush(wxBrush(wxColor(rval,gval,0)));

    		str.Printf(wxT("%02d"),(int)pchan->sv+1);

    		svX = scaleY*(900 - 10.0*RAD_2_DEG*psv->elev) * cos(psv->azim);
    		svY = scaleY*(900 - 10.0*RAD_2_DEG*psv->elev) * sin(psv->azim);

    		dc.DrawCircle(mX + svX, mY + svY, 3);
    		dc.DrawText(str, mX + svX, mY + svY);
    	}
    }

}

void GUI_Default::renderPVT()
{

	wxString str;
	SPS_M *pNav = &p->sps;
	Clock_M *pClock = &p->clock;


	tPVT->Clear();
	tPVT->SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Monospace")));


//	str.Printf(wxT("Nav SVs:\t%-2d\n"),pNav->nav_channels);
//	tPVT->AppendText(str);
//	str.Printf(wxT("Receiver Time:\t%10.2f\n"),(float)pNav->tic/(float)TICS_PER_SECOND);
//	tPVT->AppendText(str);
	str.Printf(wxT("\t\t\t      X\t\t      Y\t\t      Z\n"));
	tPVT->AppendText(str);
	str.Printf(wxT("Position (m):\t%15.2f\t%15.2f\t%15.2f\n"),pNav->x,pNav->y,pNav->z);
	tPVT->AppendText(str);
	str.Printf(wxT("Vel (cm/s):\t%15.2f\t%15.2f\t%15.2f\n"),100.0*pNav->vx,100.0*pNav->vy,100.0*pNav->vz);
	tPVT->AppendText(str);
	str.Printf(wxT("\n"));
	tPVT->AppendText(str);
	str.Printf(wxT("\t\t\t    Lat\t\t   Long\t\t    Alt\n"));
	tPVT->AppendText(str);
	str.Printf(wxT("\t\t%15.9f\t%15.9f\t%15.4f\n"),pNav->latitude*RAD_2_DEG,pNav->longitude*RAD_2_DEG,pNav->altitude);
	tPVT->AppendText(str);
	str.Printf(wxT("\n"));
	tPVT->AppendText(str);
	str.Printf(wxT("\t\t     Clock Bias\t     Clock Rate\t       GPS Time\n"));
	tPVT->AppendText(str);
	str.Printf(wxT("\t\t%15.6f\t%15.7f\t%15.6f\n"),pClock->bias,pClock->rate,pClock->time);
	tPVT->AppendText(str);
}
