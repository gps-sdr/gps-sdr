/*
 * gui_main.h
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#ifndef GUI_MAIN_H_
#define GUI_MAIN_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Main: public iGUI_Main, public GUI_Object
{

	private:

		wxGauge 	*gCN0[MAX_CHANNELS];
		wxStaticText *tCN0[MAX_CHANNELS];

	public:

		GUI_Main();
		~GUI_Main();

		void onClose(wxCloseEvent& evt);
		void render(wxDC& dc);
		void paintNow();
		void renderCN0();
		void renderSkyPlot();
		void renderPVT();

		DECLARE_EVENT_TABLE()
};
/*----------------------------------------------------------------------------------------------*/

#endif /* GUI_MAIN_H */
