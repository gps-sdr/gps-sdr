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
    EVT_LEFT_DOWN(GUI_Almanac::onMouse)
    EVT_BUTTON(ID_ALMANAC_SAVE, GUI_Almanac::onSave)
    EVT_BUTTON(ID_ALMANAC_LOAD, GUI_Almanac::onLoad)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Almanac::GUI_Almanac():iGUI_Almanac(NULL, wxID_ANY, wxT("Almanac"), wxDefaultPosition, wxSize(400,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{
	sv = 0;
	loaded = 0;
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
	int32 val;

	renderDecoded();
	renderSV();

	if((tic++ % 10) == 0)
	{
		val = NUM_CODES;
		pSerial->formCommand(GET_EPHEMERIS_VALID_C_ID, &val);
	}
}

void GUI_Almanac::onMouse(wxMouseEvent& event)
{

	wxString str;
	wxPoint br, tl, sp, mp;
	wxCoord w, h;

	int32 row, col;
	int32 dX,dY;

	pDecoded->GetClientSize(&w, &h);
	sp = this->GetScreenPosition();
	tl = pDecoded->GetScreenPosition() - sp;
	mp = wxGetMousePosition() - sp;

	dY = (h/4);
	dX = (w/(NUM_CODES>>2));

	br.x = tl.x + w;
	br.y = tl.y + h;

	/* Within the pDecoded area */
	if((mp.x >= tl.x) && (mp.x <= br.x) && (mp.y >= tl.y) && (mp.y <= br.y))
	{
		mp -= tl;

		row = mp.y/dY;
		col = mp.x/dX;
		sv = row*(NUM_CODES>>2);
		sv += col;

		if(sv >= 0 && sv <= NUM_CODES)
		{
			pSerial->formCommand(GET_ALMANAC_C_ID, &sv);
		}
		else
			sv = 0;
	}
}

void GUI_Almanac::renderDecoded()
{

	int32 dX, dY, lcv;
	wxPoint box[4];
	wxCoord w, h;
	wxString str;

	wxBufferedPaintDC dc(pDecoded, wxBUFFER_VIRTUAL_AREA);
	dc.Clear();

	pDecoded->GetClientSize(&w, &h);

	dY = (h/4);
	dX = (w/(NUM_CODES>>2));

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
		dc.DrawPolygon(4, box, (lcv%(NUM_CODES>>2))*dX, (lcv/(NUM_CODES>>2))*dY);

		str.Printf(wxT("%02d"),(int)lcv+1);
		dc.DrawText(str, (lcv%(NUM_CODES>>2))*dX + dX/3, (lcv/(NUM_CODES>>2))*dY + dY/5);
	}

	if(loaded == 0)
	{
		loaded = NUM_CODES;
		pSerial->formCommand(GET_ALMANAC_C_ID, &loaded);
	}

}

void GUI_Almanac::renderSV()
{

	wxString str;
	tDisplay->Clear();

	Almanac_M *a;
	a = &p->almanacs[sv];
	str.Printf(wxT("*********** Week %d almanac for PRN-%02d ***********\n"),a->week,sv+1);
	tDisplay->AppendText(str);
	str.Printf(wxT("ID:                        %02d\n"),sv+1);
	tDisplay->AppendText(str);
	str.Printf(wxT("Health:                    %03d\n"),a->health);
	tDisplay->AppendText(str);
	str.Printf(wxT("Eccentricity:              %.10g\n"),a->ecc);
	tDisplay->AppendText(str);
	str.Printf(wxT("Time of Applicability(s):  %.10g\n"),a->toa);
	tDisplay->AppendText(str);
	str.Printf(wxT("Orbital Inclination(rad):  %.10g\n"),a->in0);
	tDisplay->AppendText(str);
	str.Printf(wxT("Rate of Right Ascen(r/s):  %.10g\n"),a->omd);
	tDisplay->AppendText(str);
	str.Printf(wxT("SQRT(A)  (m 1/2):          %.10g\n"),a->sqrta);
	tDisplay->AppendText(str);
	str.Printf(wxT("Right Ascen at Week(rad):  %.10g\n"),a->om0);
	tDisplay->AppendText(str);
	str.Printf(wxT("Argument of Perigee(rad):  %.10g\n"),a->argp);
	tDisplay->AppendText(str);
	str.Printf(wxT("Mean Anom(rad):            %.10g\n"),a->m0);
	tDisplay->AppendText(str);
	str.Printf(wxT("Af0(s):                    %.10g\n"),a->af0);
	tDisplay->AppendText(str);
	str.Printf(wxT("Af1(s/s):                  %.10g\n"),a->af1);
	tDisplay->AppendText(str);

}

void GUI_Almanac::onSave(wxCommandEvent& event)
{

	int32 prn, week, lcv;
	wxString fileName, str;
	Almanac_M *a;
	FILE *fp;

	wxFileDialog * openFileDialog = new wxFileDialog(this, wxT("Save Almanac"), wxT(""), wxT(""), wxT("*.*"), wxSAVE);

	if(openFileDialog->ShowModal() == wxID_OK)
	{
		fileName = openFileDialog->GetPath();
		fp = fopen(fileName.mb_str(),"wt");

		if(fp != NULL)
		{
			lcv = NUM_CODES;
			pSerial->formCommand(GET_ALMANAC_C_ID, &lcv);
			sleep(1);

			for(lcv = 0; lcv < NUM_CODES; lcv++)
			{
				a = &p->almanacs[lcv];
				if(a->decoded)
				{
					fprintf(fp,"******** Week %d almanac for PRN-%02d ********\n",a->week,lcv+1);
					fprintf(fp,"ID:                        % 02d\n",lcv+1);
					fprintf(fp,"Health:                    % 03d\n",a->health);
					fprintf(fp,"Eccentricity:              % .10E\n",a->ecc);
					fprintf(fp,"Time of Applicability(s):  % .4f\n",a->toa);
					fprintf(fp,"Orbital Inclination(rad):  % .10E\n",a->in0);
					fprintf(fp,"Rate of Right Ascen(r/s):  % .10E\n",a->omd);
					fprintf(fp,"SQRT(A)  (m 1/2):          % .6f\n",a->sqrta);
					fprintf(fp,"Right Ascen at Week(rad):  % 0.10E\n",a->om0);
					fprintf(fp,"Argument of Perigee(rad):  % .9f\n",a->argp);
					fprintf(fp,"Mean Anom(rad):            % 0.10e\n",a->m0);
					fprintf(fp,"Af0(s):                    % 0.10G\n",a->af0);
					fprintf(fp,"Af1(s/s):                  % 0.10g\n",a->af1);
					fprintf(fp,"week:                      % 4d\n",a->week);
					fprintf(fp,"\n");
				}
			}

			fclose(fp);
		}
	}

}

void GUI_Almanac::onLoad(wxCommandEvent& event)
{

	int32 prn, week;
	wxString fileName;
	Set_Almanac_C c;
	Almanac_M *a;
	FILE *fp;

	wxFileDialog * openFileDialog = new wxFileDialog(this, wxT("Load Almanac"), wxT(""), wxT(""), wxT("*.*"), wxOPEN);

	if(openFileDialog->ShowModal() == wxID_OK)
	{
		fileName = openFileDialog->GetPath();

		fp = fopen(fileName.mb_str(),"rt");

		if(fp != NULL)
		{
			while(!feof(fp))
			{
				/* Get the PRN number */
				fscanf(fp,"******** Week %d almanac for PRN-%02d ********\n",&week,&prn);
				c.sv = prn-1;
				a = &c.almanac;
				a->decoded = true;
				a->sv = prn-1;
				fscanf(fp,"ID:                         %02d\n",&prn);
				fscanf(fp,"Health:                     %03d\n",&a->health);
				fscanf(fp,"Eccentricity:               %lE\n",&a->ecc);
				fscanf(fp,"Time of Applicability(s):   %lf\n",&a->toa);
				fscanf(fp,"Orbital Inclination(rad):   %lE\n",&a->in0);
				fscanf(fp,"Rate of Right Ascen(r/s):   %lE\n",&a->omd);
				fscanf(fp,"SQRT(A)  (m 1/2):           %lf\n",&a->sqrta);
				fscanf(fp,"Right Ascen at Week(rad):   %lE\n",&a->om0);
				fscanf(fp,"Argument of Perigee(rad):   %lf\n",&a->argp);
				fscanf(fp,"Mean Anom(rad):             %lE\n",&a->m0);
				fscanf(fp,"Af0(s):                     %lE\n",&a->af0);
				fscanf(fp,"Af1(s/s):                   %lE\n",&a->af1);
				fscanf(fp,"week:                       %4d\n",&a->week);
				fscanf(fp,"\n");

				pSerial->formCommand(SET_ALMANAC_C_ID, &c);
			}

			fclose(fp);
		}
	}
}


