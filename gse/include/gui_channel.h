/*
 * gui_Channel.h
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#ifndef GUI_CHANNEL_H_
#define GUI_CHANNEL_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Channel: public iGUI_Channel, public GUI_Object
{

	private:


	public:

		GUI_Channel();
		~GUI_Channel();

		void paintNow();
		void render(wxDC& dc);
		void onClose(wxCloseEvent& evt);

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif /* GUI_Channel_H_ */
