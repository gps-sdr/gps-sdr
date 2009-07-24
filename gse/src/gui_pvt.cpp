/*
 * gui_default.cpp
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
BEGIN_EVENT_TABLE(GUI_PVT, wxFrame)
    EVT_CLOSE(GUI_PVT::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_PVT::GUI_PVT():iGUI_PVT(NULL, wxID_ANY, wxT("PVT"), wxDefaultPosition, wxSize(1000,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{

	int32 lcv;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
		CN0[lcv] = 0;

}

GUI_PVT::~GUI_PVT()
{


}

void GUI_PVT::onClose(wxCloseEvent& evt)
{
	wxCommandEvent cevt;
	evt.Veto();
	pToplevel->onPVT(cevt);
}

void GUI_PVT::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_PVT::render(wxDC& dc)
{

	renderCN0();
	renderPVT();

}

void GUI_PVT::renderCN0()
{

	Channel_M *pchan;
	SPS_M *pNav = &p->sps;
	int mX, mY, lcv, gval, rval;
	double maxX, maxY, svX, svY, dX, dY;
	double scaleX, scaleY, cn0;
	wxPoint bar[4];

	wxString str;
	wxBufferedPaintDC dc(pCN0, wxBUFFER_CLIENT_AREA);
	//dc.SetBackground(wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND)));
	dc.Clear();

	maxX = maxY = 1000;

	wxCoord w, h;
	pCN0->GetClientSize(&w, &h);

	scaleX = w/maxX; scaleY = h/maxY;
	mX = w/2; mY = h/2;

	dc.SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxNORMAL, wxNORMAL));

	dc.SetPen(wxPen(black, 1, wxLONG_DASH ));
	for(lcv = 200; lcv < 1000; lcv+=40)
		dc.DrawLine(mX-500*scaleX, h-lcv*scaleY, mX+500*scaleX, h-lcv*scaleY);

    dc.SetPen(wxPen(black, 1));
	for(lcv = 200; lcv < 1000; lcv+=200)
		dc.DrawLine(mX-500*scaleX, h-lcv*scaleY, mX+500*scaleX, h-lcv*scaleY);

    dc.DrawText(wxT("60 dB-Hz"), mX - 500*scaleX, h - 1000*scaleY);
    dc.DrawText(wxT("50 dB-Hz"), mX - 500*scaleX, h - 800*scaleY);
    dc.DrawText(wxT("40 dB-Hz"), mX - 500*scaleX, h - 600*scaleY);
    dc.DrawText(wxT("30 dB-Hz"), mX - 500*scaleX, h - 400*scaleY);
    dc.DrawText(wxT("20 dB-Hz"), mX - 500*scaleX, h - 200*scaleY);

    dX = (1000-150)/MAX_CHANNELS;
    dX *= scaleX;

    dc.DrawText(wxT("CH#"),mX-500*scaleX, h - 30*scaleY);
    dc.DrawText(wxT("SV#"),mX-500*scaleX, h - 60*scaleY);
    dc.DrawText(wxT("Locks"),mX-500*scaleX, h - 90*scaleY);

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		pchan = &p->channel[lcv];
		cn0 = icn0_2_fcn0(pchan->cn0);

		dc.SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxNORMAL));

		if((pchan->count > 2000.0) && (cn0 > 20.0))
		{

			if((pNav->nsvs >> lcv) & 0x1)
				dc.SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD));

			str.Printf(wxT("%02d"),(int)pchan->sv+1);
			dc.DrawText(str, lcv*dX + 150*scaleX, h - 60*scaleY);

			cn0 = icn0_2_fcn0(pchan->cn0);
			if((cn0 < 60.0) && (cn0 > 20.0))
					CN0[lcv] += (cn0 - CN0[lcv])*0.15;

			cn0 = CN0[lcv];

			dY = cn0 - 20.0;
			dY /= 40.0;
			dY *= 800;
			dY *= scaleY;
			bar[0].x = 0;		bar[0].y = 0;
			bar[1].x = dX/2;	bar[1].y = 0;
			bar[2].x = dX/2;	bar[2].y = -dY;
			bar[3].x = 0;		bar[3].y = -dY;

			gval = 255.0*(cn0 - 20.0)/40.0;
			rval = 122.0 - 122.0*(cn0 - 20.0)/40.0;
			dc.SetBrush(wxBrush(wxColor(rval,gval,0)));
			dc.SetPen(wxPen(black, 1));
			dc.DrawPolygon(4, bar, lcv*dX + 150*scaleX, h-200*scaleY);

			str.Clear();
			((int32)pchan->bit_lock) ? 		str += 'B' : str += '_';
			((int32)pchan->frame_lock) ? 	str += 'F' : str += '_';
			dc.DrawText(str, lcv*dX + 150*scaleX, h - 90*scaleY);

		}
		else
		{
			CN0[lcv] = 25.0;
			dc.DrawText(wxT("__"), lcv*dX + 150*scaleX, h - 60*scaleY);
			dc.DrawText(wxT("__"), lcv*dX + 150*scaleX, h - 90*scaleY);
		}

		str.Printf(wxT("%02d"),lcv);
		dc.DrawText(str, lcv*dX + 150*scaleX, h - 30*scaleY);

	}

}


void GUI_PVT::renderPVT()
{
	int32 lcv, nchan;
	wxDateTime theTime;
	wxString str, str2;
	time_t utcsec;
	SPS_M *pNav = &p->sps;
	Clock_M *pClock = &p->clock;
	PPS_M *pPPS = &p->pps;
	TOT_M *pTOT = &p->tot;

	double vel;

	nchan = 0;
	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(pNav->nsvs >> lcv & 0x1)
			nchan++;
	}

	vel = sqrt(pNav->vz*pNav->vz + pNav->vy*pNav->vy + pNav->vx*pNav->vx);

	str.Printf(wxT("%15d"),pNav->tic);						rticks->Clear(); rticks->WriteText(str);
	str.Printf(wxT("%15d"),pNav->converged_ticks);			cticks->Clear(); cticks->WriteText(str);
	str.Printf(wxT("%15d"),pNav->stale_ticks);				sticks->Clear(); sticks->WriteText(str);
	str.Printf(wxT("%15d"),nchan);							nsv->Clear(); nsv->WriteText(str);
	str.Printf(wxT("%15.2f m"),pNav->x);					px->Clear(); px->WriteText(str);
	str.Printf(wxT("%15.2f m"),pNav->y);					py->Clear(); py->WriteText(str);
	str.Printf(wxT("%15.2f m"),pNav->z);					pz->Clear(); pz->WriteText(str);
	str.Printf(wxT("%.2f cm/s"),100.0*pNav->vx);			vx->Clear(); vx->WriteText(str);
	str.Printf(wxT("%.2f cm/s"),100.0*pNav->vy);			vy->Clear(); vy->WriteText(str);
	str.Printf(wxT("%.2f cm/s"),100.0*pNav->vz);			vz->Clear(); vz->WriteText(str);
	str.Printf(wxT("%.2f m/s"),vel);						speed->Clear(); speed->WriteText(str);

	str.Printf(wxT("%15.9f"),pNav->latitude*RAD_2_DEG);		lat->Clear(); lat->WriteText(str);
	str.Printf(wxT("%15.9f"),pNav->longitude*RAD_2_DEG);	lon->Clear(); lon->WriteText(str);
	str.Printf(wxT("%15.2f"),pNav->altitude);				alt->Clear(); alt->WriteText(str);

	str.Printf(wxT("%15.9f"),pClock->bias);					cb->Clear(); cb->WriteText(str);
	str.Printf(wxT("%15.6f"),pClock->rate);					cr->Clear(); cr->WriteText(str);
	str.Printf(wxT("%15.6f"),pClock->time);					gpss->Clear(); gpss->WriteText(str);
	str.Printf(wxT("%8u"),pClock->week);					gpsw->Clear(); gpsw->WriteText(str);

	str.Printf(wxT("%8u"),pNav->iterations);				iter->Clear(); iter->WriteText(str);
	str.Printf(wxT("%15.2f"),pNav->gdop);					gdop->Clear(); gdop->WriteText(str);
	str.Printf(wxT("%.5e"),pPPS->err_lp);					ppserr->Clear(); ppserr->WriteText(str);
	str.Printf(wxT("%.5e"),pPPS->feedback*SPEED_OF_LIGHT);	ppscmd->Clear(); ppscmd->WriteText(str);
	str.Printf(wxT("%.5e"),pPPS->clock_rate*SPEED_OF_LIGHT);ppscr->Clear(); ppscr->WriteText(str);

	if(pPPS->state)
	{
		ppsstate->Clear(); ppsstate->WriteText(wxT("Slow Slew"));
	}
	else
	{
		ppsstate->Clear(); ppsstate->WriteText(wxT("Fast Slew"));
	}

	/* Get into unix time */
	if(pTOT->valid)
	{
		utcsec = floor(pTOT->second);
		utcsec += (pTOT->week)*SECONDS_IN_WEEK;
		utcsec += (pTOT->day)*SECONDS_IN_DAY;
		utcsec -= 10; 		 //!< Difference of TAI and UTC
		utcsec += 315964819; //!< 0 Unix time --> GPS time offset

		theTime.Set(utcsec);
		str = theTime.FormatISODate();
		str += wxT(' ');
		str += theTime.FormatISOTime();
		str2.Printf(wxT("%.6f"),fmod(pTOT->second,1.0));
		str += str2.Mid(1,wxSTRING_MAXLEN);
		utct->Clear(); utct->WriteText(str);
	}
	else
	{
		utcsec = 0;
		utct->Clear(); utct->WriteText(wxT("No UTC Information"));
	}

}
