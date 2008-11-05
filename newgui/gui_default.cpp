/*
 * gui_default.cpp
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#include "gui.h"

DECLARE_APP(GUI_App)

GUI_Default::GUI_Default():iGUI_Default(NULL, wxID_ANY, wxT("Default"), wxDefaultPosition, wxSize( 800,600 ), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{


}

GUI_Default::~GUI_Default()
{


}
