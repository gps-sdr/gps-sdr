#ifndef GUI_ALMANAC_H_
#define GUI_ALMANAC_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Almanac: public iGUI_Almanac
{

	private:

		Message_Struct *p;
		class GUI_Serial *pSerial;
		int32 sv;
		int32 loaded;

	public:

		GUI_Almanac();
		~GUI_Almanac();

		void render(wxDC& dc);
		void paintNow();
		void setSerial(GUI_Serial *_p){pSerial = _p;};
		void setPointer(Message_Struct *_p){p = _p;};
		void onClose(wxCloseEvent& evt){evt.Veto();};
		void onMouse(wxMouseEvent& event);
		void onSave(wxCommandEvent& event);
		void onLoad(wxCommandEvent& event);
		void renderDecoded();
		void renderSV();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
