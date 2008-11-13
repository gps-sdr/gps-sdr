/*
 * gui_toplevel.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

GUI_Commands::GUI_Commands():iGUI_Commands(NULL, wxID_ANY, wxT("Commands"), wxDefaultPosition, wxSize(800,600), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{


}

GUI_Commands::~GUI_Commands()
{


}

void GUI_Commands::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void GUI_Commands::render(wxDC& dc)
{


}
