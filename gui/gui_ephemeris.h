#ifndef GUI_EPHEMERIS_H_
#define GUI_EPHEMERIS_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Ephemeris: public iGUI_Ephemeris, public GUI_Object
{

	private:

		/* Add all the "subwindows" here */
		int32 sv;
		int32 loaded;
		int32 tic;

	public:

		GUI_Ephemeris();
		~GUI_Ephemeris();

		void onClose(wxCloseEvent& evt);
		void render(wxDC& dc);
		void paintNow();

		void onMouse(wxMouseEvent& event);
		void onSave(wxCommandEvent& event);
		void onLoad(wxCommandEvent& event);
		void renderDecoded();
		void renderSV();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
