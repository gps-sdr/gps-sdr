/*
 * gui_almnanac.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_Almanac, wxFrame)
    EVT_CLOSE(GUI_Almanac::onClose)
    EVT_CHOICE(ID_ALMANAC_SV, GUI_Almanac::onSV)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Almanac::GUI_Almanac():iGUI_Almanac(NULL, wxID_ANY, wxT("Almanac"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{

	sv = 0;

}

GUI_Almanac::~GUI_Almanac()
{


}

void GUI_Almanac::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_Almanac::render(wxDC& dc)
{
	renderDecoded();
	renderSV();
}

void GUI_Almanac::onSV(wxCommandEvent& event)
{
	int32 val;

	val = mSV->GetCurrentSelection();

	if(val == 0)
		val = NUM_CODES;
	else
		val = val-1;

	if(val >= NUM_CODES)
		sv = 0;
	else
		sv = val;

	pSerial->formCommand(GET_ALMANAC_C_ID, &val);
}


void GUI_Almanac::renderDecoded()
{

	int32 dX, dY, lcv;

	wxPoint box[4];
	wxString str;
	wxPaintDC dc(pDecoded);
	dc.Clear();

	wxCoord w, h;
	dc.GetSize(&w, &h);

	dY = (h/2);
	dX = (2*w/NUM_CODES);

	box[0].x = 0;	box[0].y = 0;
	box[1].x = dX;	box[1].y = 0;
	box[2].x = dX;	box[2].y = dY;
	box[3].x = 0;	box[3].y = dY;

	/* Render the decoded ephemerides */
	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		if(p->ephemeris_status.avalid[lcv])
		{
			dc.SetBrush(wxBrush(wxColor(0,255,0)));
			dc.SetPen(wxPen(wxColor(0,0,0), 1));
		}
		else
		{
			dc.SetBrush(wxBrush(wxColor(127,127,127)));
			dc.SetPen(wxPen(wxColor(0,0,0), 1));
		}
		dc.DrawPolygon(4, box, (lcv%(NUM_CODES>>1))*dX, (lcv/(NUM_CODES>>1))*dY);

		str.Printf(wxT("%02d"),(int)lcv+1);
		dc.DrawText(str, (lcv%(NUM_CODES>>1))*dX + dX/3, (lcv/(NUM_CODES>>1))*dY + dY/5);
	}

}

void GUI_Almanac::renderSV()
{

	wxString str;
	tDisplay->Clear();

	Almanac_M *a;
	a = &p->almanac[sv];
	str.Printf(wxT("******** Week %d almanac for PRN-%02d ********\n"),a->week,sv+1);
	tDisplay->AppendText(str);
	str.Printf(wxT("ID:                        % 02d\n"),sv+1);
	tDisplay->AppendText(str);
	str.Printf(wxT("Health:                    % 03d\n"),a->health);
	tDisplay->AppendText(str);
	str.Printf(wxT("Eccentricity:              % .10E\n"),a->ecc);
	tDisplay->AppendText(str);
	str.Printf(wxT("Time of Applicability(s):  % .4f\n"),a->toa);
	tDisplay->AppendText(str);
	str.Printf(wxT("Orbital Inclination(rad):  % .10E\n"),a->in0);
	tDisplay->AppendText(str);
	str.Printf(wxT("Rate of Right Ascen(r/s):  % .10E\n"),a->omd);
	tDisplay->AppendText(str);
	str.Printf(wxT("SQRT(A)  (m 1/2):          % .6f\n"),a->sqrta);
	tDisplay->AppendText(str);
	str.Printf(wxT("Right Ascen at Week(rad):  % 0.10E\n"),a->om0);
	tDisplay->AppendText(str);
	str.Printf(wxT("Argument of Perigee(rad):  % .9f\n"),a->argp);
	tDisplay->AppendText(str);
	str.Printf(wxT("Mean Anom(rad):            % 0.10e\n"),a->m0);
	tDisplay->AppendText(str);
	str.Printf(wxT("Af0(s):                    % 0.10G\n"),a->af0);
	tDisplay->AppendText(str);
	str.Printf(wxT("Af1(s/s):                  % 0.10g\n"),a->af1);
	tDisplay->AppendText(str);
	str.Printf(wxT("Week:                      % -4d"),a->week);
	tDisplay->AppendText(str);

}
