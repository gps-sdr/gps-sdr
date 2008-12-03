#ifndef GUI_EPHEMERIS_H_
#define GUI_EPHEMERIS_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Ephemeris: public iGUI_Ephemeris
{

	private:

		/* Add all the "subwindows" here */
		Message_Struct *p;
		class GUI_Serial *pSerial;
		int32 sv;
		int32 loaded;
		int32 tic;

	public:

		GUI_Ephemeris();
		~GUI_Ephemeris();

		void render(wxDC& dc);
		void paintNow();
		void setSerial(GUI_Serial *_p){pSerial = _p;};
		void setPointer(Message_Struct *_p){p = _p;};
		void onMouse(wxMouseEvent& event);
		void onClose(wxCloseEvent& evt){evt.Veto();};
		void onSave(wxCommandEvent& event);
		void onLoad(wxCommandEvent& event);
		void renderDecoded();
		void renderSV();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
