#ifndef GUI_SELECT_H_
#define GUI_SELECT_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Select: public iGUI_Select, public GUI_Object
{

	private:

		int32 sv;
		int32 tic;

	public:

		GUI_Select();
		~GUI_Select();

		void onClose(wxCloseEvent& evt);
		void render(wxDC& dc);
		void paintNow();

		void renderDoppler();
		void renderSV();
		void renderRec();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
