#ifndef GUI_ACQUISITION_H_
#define GUI_ACQUISITION_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Acquisition: public iGUI_Acquisition
{

	private:

		Message_Struct *p;
		class GUI_Serial *pSerial;
		Acq_Command_M acq_command[3][NUM_CODES];
		int32 sv;
		float scale[3];

	public:

		GUI_Acquisition();
		~GUI_Acquisition();

		void render(wxDC& dc);
		void paintNow();
		void setSerial(GUI_Serial *_p){pSerial = _p;};
		void setPointer(Message_Struct *_p){p = _p;};
		void onClose(wxCloseEvent& evt){evt.Veto();};
		void renderBars(int32 _type);
		void copyBuff();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
