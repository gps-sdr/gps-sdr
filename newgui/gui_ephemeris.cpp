/*
 * gui_toplevel.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

GUI_Ephemeris::GUI_Ephemeris():iGUI_Ephemeris(NULL, wxID_ANY, wxT("Ephemeris"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{


}

GUI_Ephemeris::~GUI_Ephemeris()
{


}

void GUI_Ephemeris::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_Ephemeris::render(wxDC& dc)
{


}
