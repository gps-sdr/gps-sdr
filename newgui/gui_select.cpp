/*
 * gui_almnanac.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_Select, wxFrame)
    EVT_CLOSE(GUI_Select::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Select::GUI_Select():iGUI_Select(NULL, wxID_ANY, wxT("SV Select"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{
	sv = 0;
}

GUI_Select::~GUI_Select()
{


}

void GUI_Select::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_Select::render(wxDC& dc)
{
	int32 val;

	renderDoppler();
	renderSV();
	renderRec();

	if((tic++ % 10) == 0)
	{
		val = NUM_CODES;
		pSerial->formCommand(GET_SV_PREDICTION_C_ID, &val);
	}

}


void GUI_Select::renderDoppler()
{

	int mX, mY, lcv, gval, rval;
	double maxX, maxY, dX, pY;
	double scaleX, scaleY;
	wxPoint bar[4];
	wxCoord w, h;
	SV_Prediction_M *psv;
	Acq_Command_M *pacq;

	wxString str;
	wxBufferedPaintDC dc(pDoppler, wxBUFFER_CLIENT_AREA);
	dc.Clear();

	maxX = maxY = 2000;

	pDoppler->GetClientSize(&w, &h);

	scaleX = w/maxX; scaleY = h/maxY;
	mX = w/2; mY = h/2;

    /* Draw top,middle,bottom lines */
    dc.SetPen(wxPen(wxColor(0,0,0), 1));
    dc.DrawLine(mX-1000*scaleX, 0000*scaleY, mX+1000*scaleX, 0000*scaleY);
    dc.DrawLine(mX-1000*scaleX, 1000*scaleY, mX+1000*scaleX, 1000*scaleY);
    dc.DrawLine(mX-1000*scaleX, 2000*scaleY, mX+1000*scaleX, 2000*scaleY);

    /* Draw intermediate lines */
    dc.SetBrush(wxBrush(wxColor(0,0,0)));
    dc.SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD));
    dc.SetPen(wxPen(wxColor(0,0,0), 1, wxLONG_DASH ));
    for(lcv = 0; lcv < 11; lcv++)
    {
    	dc.DrawLine(mX-1000*scaleX, lcv*200*scaleY, mX+1000*scaleX, lcv*200*scaleY);
        str.Printf(wxT("%d"),MAX_DOPPLER-lcv*(2*MAX_DOPPLER/10));
        dc.DrawText(str,0, lcv*200*scaleY);
    }

    dX = (2000-200)/NUM_CODES;
    dX *= scaleX;

    /* Now place the Doppler bars */
	bar[0].x = 0;		bar[0].y = 0;
	bar[1].x = dX;		bar[1].y = 0;
	bar[2].x = dX;		bar[2].y = 0;
	bar[3].x = 0;		bar[3].y = 0;

    for(lcv = 0; lcv < NUM_CODES; lcv++)
    {
    	psv = &p->sv_predictions[lcv];
    	pacq = &p->acq_command[lcv];

//    	pY = -1000*(pacq->maxdopp-pacq->mindopp)/MAX_DOPPLER;
//    	pY *= scaleY;
//
//    	bar[2].y = pY;
//    	bar[3].y = pY;
//
//    	pY = -1000*pacq->mindopp/MAX_DOPPLER;
//    	pY += 1000;
//    	pY *= scaleY;

    	//dc.SetPen(wxPen(wxColor(255,0,0), 1));
    	//dc.SetBrush(wxBrush(wxColor(255,255,255)));
    	//dc.DrawPolygon(4, bar, lcv*dX+100*scaleX, pY);
    	//dc.DrawLine(dX*lcv+100*scaleX, pY, dX*(lcv+1)+100*scaleX, pY);
    	//dc.DrawLine(dX*lcv+100*scaleX, pY+bar[2].y, dX*(lcv+1)+100*scaleX, pY+bar[2].y);

    	if(psv->visible)
    		dc.SetPen(wxPen(wxColor(0,0,0), 4));
    	else
    		dc.SetPen(wxPen(wxColor(200,200,200), 4));

		if(psv->tracked)
			dc.SetPen(wxPen(wxColor(0,255,0), 4));

    	pY = -1000*psv->doppler/MAX_DOPPLER;
    	pY += 1000;
    	pY *= scaleY;
    	dc.DrawLine(dX*lcv+100*scaleX, pY, dX*(lcv+1)+100*scaleX, pY);

    	//dc.DrawLine(dX*lcv+100*scaleX, pY, dX*(lcv+1)+100*scaleX, pY);
    }

}

void GUI_Select::renderSV()
{

	int mX, mY, lcv;
	double maxX, maxY, svX, svY;
	double scaleX, scaleY;
	wxString str;

	SV_Prediction_M *psv;

	maxX = maxY = 1100;

	wxBufferedPaintDC dc(pSV, wxBUFFER_CLIENT_AREA);
	dc.Clear();

	wxCoord w, h;
	pSV->GetClientSize(&w, &h);

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
    dc.SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD));

    /* Draw the "NESW" */
    str = wxT("N"); dc.DrawText(str,mX-5, mY-900*scaleY-20);
    str = wxT("S"); dc.DrawText(str,mX-5, mY+900*scaleY);
    str = wxT("E"); dc.DrawText(str,mX+900*scaleY+5, mY-10);
    str = wxT("W"); dc.DrawText(str,mX-900*scaleY-25, mY-10);

    /* Now place the SVs */
    for(lcv = 0; lcv < NUM_CODES; lcv++)
    {
    	psv = &p->sv_predictions[lcv];
		if(psv->elev < 0.0)
		{
			str.Printf(wxT("%02d"),lcv+1);
		    dc.SetPen(wxPen(wxColor(200,200,200), 1 ));
			dc.SetBrush(wxBrush(wxColor(200,200,200)));
			dc.SetTextForeground(wxColor(200,200,200));
			svX = scaleY*(900 + 10.0*RAD_2_DEG*psv->elev) * cos(psv->azim - PI_OVER_2);
			svY = scaleY*(900 + 10.0*RAD_2_DEG*psv->elev) * sin(psv->azim - PI_OVER_2);
			dc.DrawCircle(mX + svX, mY + svY, 3);
			dc.DrawText(str, mX + svX, mY + svY);
		}
    }

    /* Now place the SVs */
    for(lcv = 0; lcv < NUM_CODES; lcv++)
    {
    	psv = &p->sv_predictions[lcv];
		if(psv->elev > 0.0)
		{
			str.Printf(wxT("%02d"),lcv+1);


			dc.SetPen(wxPen(wxColor(0,0,0), 1 ));
			dc.SetBrush(wxBrush(wxColor(0,0,0)));
			dc.SetTextForeground(wxColor(0,0,0));
			svX = scaleY*(900 - 10.0*RAD_2_DEG*psv->elev) * cos(psv->azim - PI_OVER_2);
			svY = scaleY*(900 - 10.0*RAD_2_DEG*psv->elev) * sin(psv->azim - PI_OVER_2);
			dc.DrawText(str, mX + svX, mY + svY);

			if(psv->tracked)
				dc.SetBrush(wxBrush(wxColor(0,255,0)));
			else
				dc.SetBrush(wxBrush(wxColor(0,0,0)));

			dc.DrawCircle(mX + svX, mY + svY, 3);

		}
    }

}


void GUI_Select::renderRec()
{

	int mX, mY, lcv;
	double maxX, maxY, svX, svY;
	double scaleX, scaleY;
	wxString str;

	SV_Prediction_M *psv;

	maxX = maxY = 1100;

	wxBufferedPaintDC dc(pRec, wxBUFFER_CLIENT_AREA);
	dc.Clear();

	wxCoord w, h;
	pRec->GetClientSize(&w, &h);

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
    dc.SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxBOLD));

    /* Draw the "NESW" */
    str = wxT("N"); dc.DrawText(str,mX-5, mY-900*scaleY-20);
    str = wxT("S"); dc.DrawText(str,mX-5, mY+900*scaleY);
    str = wxT("E"); dc.DrawText(str,mX+900*scaleY+5, mY-10);
    str = wxT("W"); dc.DrawText(str,mX-900*scaleY-25, mY-10);

    /* Now place the SVs */
    for(lcv = 0; lcv < NUM_CODES; lcv++)
    {
    	psv = &p->sv_predictions[lcv];
		if(psv->elev < 0.0)
		{
			str.Printf(wxT("%02d"),lcv+1);
		    dc.SetPen(wxPen(wxColor(200,200,200), 1 ));
			dc.SetBrush(wxBrush(wxColor(200,200,200)));
			dc.SetTextForeground(wxColor(200,200,200));
			svX = scaleY*(900 - 10.0*RAD_2_DEG*psv->v_elev) * cos(psv->v_azim);
			svY = scaleY*(900 - 10.0*RAD_2_DEG*psv->v_elev) * sin(psv->v_azim);
			dc.DrawCircle(mX + svX, mY + svY, 3);
			dc.DrawText(str, mX + svX, mY + svY);
		}
    }

    /* Now place the SVs */
    for(lcv = 0; lcv < NUM_CODES; lcv++)
    {
    	psv = &p->sv_predictions[lcv];
		if(psv->elev > 0.0)
		{
			str.Printf(wxT("%02d"),lcv+1);
		    dc.SetPen(wxPen(wxColor(0,0,0), 1 ));
			dc.SetBrush(wxBrush(wxColor(0,0,0)));
			dc.SetTextForeground(wxColor(0,0,0));
			svX = scaleY*(900 - 10.0*RAD_2_DEG*psv->v_elev) * cos(psv->v_azim);
			svY = scaleY*(900 - 10.0*RAD_2_DEG*psv->v_elev) * sin(psv->v_azim);
			dc.DrawText(str, mX + svX, mY + svY);

			if(psv->tracked)
				dc.SetBrush(wxBrush(wxColor(0,255,0)));
			else
				dc.SetBrush(wxBrush(wxColor(0,0,0)));

			dc.DrawCircle(mX + svX, mY + svY, 3);

		}
    }





}
