/*
 * gui_almnanac.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_Acquisition, wxFrame)
    EVT_CLOSE(GUI_Acquisition::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Acquisition::GUI_Acquisition():iGUI_Acquisition(NULL, wxID_ANY, wxT("Acquisition"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{
	sv = 0;
	scale[0] = scale[1] = scale[2] = THRESH_STRONG;
}

GUI_Acquisition::~GUI_Acquisition()
{
	memset(&acq_command, NUM_CODES*3, sizeof(Acq_Command_M));
}

void GUI_Acquisition::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_Acquisition::render(wxDC& dc)
{
	int32 val;

	copyBuff();
	renderBars(ACQ_STRONG);
	renderBars(ACQ_MEDIUM);
	renderBars(ACQ_WEAK);

	if((tic++ % 10) == 0)
	{
		val = NUM_CODES;
		pSerial->formCommand(GET_ACQ_COMMAND_C_ID, &val);
	}

}

void GUI_Acquisition::copyBuff()
{

	int32 lcv, sv, type;

	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		type = p->acq_command[lcv].type;
		sv = p->acq_command[lcv].sv;
		scale[type] += (p->acq_command[lcv].magnitude - scale[type])*.01;
		memcpy(&acq_command[type][sv], &p->acq_command[lcv], sizeof(Acq_Command_M));
	}
}

void GUI_Acquisition::renderBars(int32 _type)
{

	int mX, mY, lcv, dX, pY;
	float maxX, maxY, svX, svY;
	float scaleX, scaleY;
	float thresh;
	wxPoint bar[4];

	Acq_Command_M *psv;

	maxX = maxY = 2100;

	wxBufferedPaintDC *dc;

	switch(_type)
	{
		case ACQ_STRONG:
			dc = new wxBufferedPaintDC(pStrong, wxBUFFER_CLIENT_AREA);
			thresh = THRESH_STRONG;
			break;
		case ACQ_MEDIUM:
			dc = new wxBufferedPaintDC(pMedium, wxBUFFER_CLIENT_AREA);
			thresh = THRESH_MEDIUM;
			break;
		case ACQ_WEAK:
			dc = new wxBufferedPaintDC(pWeak, wxBUFFER_CLIENT_AREA);
			thresh = THRESH_WEAK;
			break;
		default:
			return;
	}

	dc->Clear();

	wxCoord w, h;
	pStrong->GetClientSize(&w, &h);

	scaleX = w/maxX; scaleY = h/maxY;

	mX = w/2; mY = h/2;

	dX = 2000/NUM_CODES;
	dX *= scaleX;

	/* Draw the threshold */
    dc->SetPen(wxPen(wxColor(0,0,0), 1, wxLONG_DASH ));
	dc->DrawLine(0, (2000-600)*scaleY, maxX*scaleX, (2000-600)*scaleY);

	dc->SetPen(wxPen(wxColor(0,0,0), 1));

	/* Now place the magnitude bars */
	bar[0].x = 0;		bar[0].y = 0;
	bar[1].x = dX/2;	bar[1].y = 0;
	bar[2].x = dX/2;	bar[2].y = 0;
	bar[3].x = 0;		bar[3].y = 0;

	for(lcv = 0; lcv < NUM_CODES; lcv++)
	{
		psv = &acq_command[_type][lcv];

		pY = -600*(psv->magnitude)/thresh;
		if(pY > 2000) pY = 2000;
		pY *= scaleY;

		bar[0].x = 0;		bar[0].y = 0;
		bar[1].x = dX/2;	bar[1].y = 0;
		bar[2].x = dX/2;	bar[2].y = pY;
		bar[3].x = 0;		bar[3].y = pY;

		if(psv->success)
			dc->SetBrush(wxBrush(wxColor(255,0,0)));
		else
			dc->SetBrush(wxBrush(wxColor(0,0,255)));

		dc->DrawPolygon(4, bar, lcv*dX+dX, 2000*scaleY);
	}

	delete dc;
}
