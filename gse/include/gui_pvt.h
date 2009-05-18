/*
 * gui_main.h
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#ifndef GUI_PVT_H_
#define GUI_PVT_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_PVT: public iGUI_PVT, public GUI_Object
{

	private:

		wxGauge 	*gCN0[MAX_CHANNELS];
		wxStaticText *tCN0[MAX_CHANNELS];
		double CN0[MAX_CHANNELS];

	public:

		GUI_PVT();
		~GUI_PVT();

		void onClose(wxCloseEvent& evt);
		void render(wxDC& dc);
		void paintNow();
		void renderCN0();
		void renderPVT();

		DECLARE_EVENT_TABLE()
};
/*----------------------------------------------------------------------------------------------*/

#endif /* GUI_PVT_H */
