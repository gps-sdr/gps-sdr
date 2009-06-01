/*
 * gui_pseudo.h
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#ifndef GUI_HEALTH_H_
#define GUI_HEALTH_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Health: public iGUI_Health, public GUI_Object
{

	private:

	public:

		GUI_Health();
		~GUI_Health();

		void onClose(wxCloseEvent& evt);
		void paintNow();
		void render(wxDC& dc);

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif /* GUI_Health_H_ */
