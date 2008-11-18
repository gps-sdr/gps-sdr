#ifndef GUI_COMMAND_H_
#define GUI_COMMAND_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Commands: public iGUI_Commands
{

	private:

		/* Add all the "subwindows" here */
		Message_Struct *p;
		class GUI_Serial *pSerial;

	public:

		GUI_Commands();
		~GUI_Commands();

		void render(wxDC& dc);
		void paintNow();

		void setPointer(Message_Struct *_p){p = _p;};
		void setSerial(GUI_Serial *_p){pSerial = _p;};

		void onPVT(wxCommandEvent& event);
		void onChannel(wxCommandEvent& event);
		void onEphemeris(wxCommandEvent& event);
		void onAlmanac(wxCommandEvent& event);
		void onClose(wxCloseEvent& evt){evt.Veto();};

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
