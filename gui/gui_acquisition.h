#ifndef GUI_ACQUISITION_H_
#define GUI_ACQUISITION_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Acquisition: public iGUI_Acquisition, public GUI_Object
{

	private:

		Acq_Command_M acq_command[3][NUM_CODES];
		int32 sv;
		float scale[3];
		int32 tic;

	public:

		GUI_Acquisition();
		~GUI_Acquisition();

		void render(wxDC& dc);
		void paintNow();
		void onClose(wxCloseEvent& evt);
		void renderBars(int32 _type);
		void copyBuff();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
