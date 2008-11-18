#ifndef GUI_EPHEMERIS_H_
#define GUI_EPHEMERIS_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Ephemeris: public iGUI_Ephemeris
{

	private:

		/* Add all the "subwindows" here */

	public:

		GUI_Ephemeris();
		~GUI_Ephemeris();

		void render(wxDC& dc);
		void paintNow();


};
/*----------------------------------------------------------------------------------------------*/

#endif
