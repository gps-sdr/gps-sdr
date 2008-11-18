#ifndef GUI_ALMANAC_H_
#define GUI_ALMANAC_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Almanac: public iGUI_Almanac
{

	private:

		/* Add all the "subwindows" here */

	public:

		GUI_Almanac();
		~GUI_Almanac();

		void render(wxDC& dc);
		void paintNow();


};
/*----------------------------------------------------------------------------------------------*/

#endif
