#ifndef GUI_COMMAND_H_
#define GUI_COMMAND_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Commands: public iGUI_Commands, public GUI_Object
{

	private:


	public:

		GUI_Commands();
		~GUI_Commands();

		void onClose(wxCloseEvent& evt);
		void render(wxDC& dc);
		void paintNow();

		void onAll(wxCommandEvent& event);
		void onPVT(wxCommandEvent& event);
		void onChannel(wxCommandEvent& event);
		void onEphemeris(wxCommandEvent& event);
		void onAlmanac(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
