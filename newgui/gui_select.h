#ifndef GUI_SELECT_H_
#define GUI_SELECT_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Select: public iGUI_Select
{

	private:

		Message_Struct *p;
		class GUI_Serial *pSerial;
		int32 sv;

	public:

		GUI_Select();
		~GUI_Select();

		void render(wxDC& dc);
		void paintNow();
		void setSerial(GUI_Serial *_p){pSerial = _p;};
		void setPointer(Message_Struct *_p){p = _p;};
		void onClose(wxCloseEvent& evt){evt.Veto();};
		void renderDoppler();
		void renderSV();
		void renderRec();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
