#ifndef GUI_COMMAND_H_
#define GUI_COMMAND_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Commands: public iGUI_Commands
{

	private:

		/* Add all the "subwindows" here */

	public:

		GUI_Commands();
		~GUI_Commands();

		void render(wxDC& dc);
		void paintNow();


};
/*----------------------------------------------------------------------------------------------*/

#endif
