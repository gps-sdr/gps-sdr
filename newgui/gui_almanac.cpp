/*
 * gui_almnanac.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

GUI_Almanac::GUI_Almanac():iGUI_Almanac(NULL, wxID_ANY, wxT("Almanac"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{


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


}
