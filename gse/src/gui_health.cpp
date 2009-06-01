/*
 * gui_Channel.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

/*----------------------------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(GUI_Health, wxFrame)
    EVT_CLOSE(GUI_Health::onClose)
END_EVENT_TABLE()
/*----------------------------------------------------------------------------------------------*/

GUI_Health::GUI_Health():iGUI_Health(NULL, wxID_ANY, wxT("Board Health"), wxDefaultPosition, wxSize(650,250), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{


}

GUI_Health::~GUI_Health()
{


}

void GUI_Health::onClose(wxCloseEvent& evt)
{
	wxCommandEvent cevt;
	evt.Veto();
	pToplevel->onHealth(cevt);
}

void GUI_Health::paintNow()
{

    wxClientDC dc(this);
    render(dc);

}


void GUI_Health::render(wxDC& dc)
{

	wxString str;
	Board_Health_M *pHealth = &p->board_health;


	str.Printf(wxT("%02d.%02d.%02d"),(pHealth->trk_version >> 24) & 0xff,
									(pHealth->trk_version >> 16) & 0xff,
									(pHealth->trk_version >> 8) & 0xff);
	trkv->SetLabel(str);

	str.Printf(wxT("%02d.%02d.%02d"),(pHealth->acq_version >> 24) & 0xff,
									(pHealth->acq_version >> 16) & 0xff,
									(pHealth->acq_version >> 8) & 0xff);
	acqv->SetLabel(str);

	str.Printf(wxT("%02d.%02d.%02d"),(pHealth->fft_version >> 24) & 0xff,
									(pHealth->fft_version >> 16) & 0xff,
									(pHealth->fft_version >> 8) & 0xff);
	fftv->SetLabel(str);

	str.Printf(wxT("%02d.%02d.%02x"),(pHealth->sft_version >> 16) & 0xff,
									(pHealth->sft_version >> 8) & 0xff,
									(pHealth->sft_version >> 0) & 0xff);
	sftv->SetLabel(str);

	str.Printf(wxT("%15d"),pHealth->dsa0);			dsa0->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->dsa1);			dsa1->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->dsa2);			dsa2->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->dsa3);			dsa3->SetLabel(str);

	str.Printf(wxT("%15d"),pHealth->ovrflw0);		ovr0->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->ovrflw1);		ovr1->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->ovrflw2);		ovr2->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->ovrflw3);		ovr3->SetLabel(str);

	str.Printf(wxT("%15d"),pHealth->adc_values[8]);		nsptr->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->adc_values[10]);	pcctr->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->adc_values[9]);		rftr->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->adc_values[1]);		vref->SetLabel(str);

	str.Printf(wxT("%15d"),pHealth->adc_values[0]);		gnd1->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->adc_values[2]);		gnd2->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->adc_values[3]);		p5->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->adc_values[4]);		p15->SetLabel(str);

	str.Printf(wxT("%15d"),pHealth->adc_values[5]);		n15->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->adc_values[6]);		p25->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->adc_values[7]);		p33->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->adc_values[11]);	p1p5->SetLabel(str);

	str.Printf(wxT("%15d"),pHealth->adc_values[12]);	rf0->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->adc_values[13]);	rf1->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->adc_values[14]);	rf2->SetLabel(str);
	str.Printf(wxT("%15d"),pHealth->adc_values[15]);	rf3->SetLabel(str);



}
