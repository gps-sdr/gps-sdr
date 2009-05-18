/*
 * gui_messages.h
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#ifndef GUI_MESSAGES_H_
#define GUI_MESSAGES_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Messages: public iGUI_Messages, public GUI_Object
{

	private:

	public:

		GUI_Messages();
		~GUI_Messages();

		void onClose(wxCloseEvent& evt);
		void paintNow();
		void render(wxDC& dc);
		void renderMessages();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif /* GUI_MESSAGES_H_ */
