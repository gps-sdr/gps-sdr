#ifndef GUI_ALMANAC_H_
#define GUI_ALMANAC_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Almanac : public iGUI_Almanac, public GUI_Object
{

	private:

		int32 sv;
		int32 loaded;
		int32 tic;

	public:

		GUI_Almanac();
		~GUI_Almanac();

		void render(wxDC& dc);
		void paintNow();
		void onClose(wxCloseEvent& event);
		void onMouse(wxMouseEvent& event);
		void onSave(wxCommandEvent& event);
		void onLoad(wxCommandEvent& event);
		void renderDecoded();
		void renderSV();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
