#ifndef GUI_SPEEDO_H_
#define GUI_SPEEDO_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Speedo : public iGUI_Speedo, public GUI_Object
{

	private:

		int32 tick;
		double speed;

	public:

		GUI_Speedo();
		~GUI_Speedo();

		void onClose(wxCloseEvent& event);
		void render(wxDC& dc);
		void paintNow();
		void renderSpeedo();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
