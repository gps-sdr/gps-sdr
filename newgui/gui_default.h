/*
 * gui_default.h
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#ifndef GUI_DEFAULT_H_
#define GUI_DEFAULT_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Default: public iGUI_Default
{

	private:

		wxGauge 	*gCN0[MAX_CHANNELS];
		wxStaticText *tCN0[MAX_CHANNELS];
		Message_Struct *p;

	public:

		GUI_Default();
		~GUI_Default();

		void render(wxDC& dc);
		void paintNow();
		void renderCN0();
		void renderSkyPlot();
		void renderPVT();
		void setPointer(Message_Struct *_p){p = _p;};
		void onClose(wxCloseEvent& evt){evt.Veto();};

		DECLARE_EVENT_TABLE()
};
/*----------------------------------------------------------------------------------------------*/

#endif /* GUI_DEFAULT_H_ */
