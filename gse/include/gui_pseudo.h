/*
 * gui_pseudo.h
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#ifndef GUI_PSEUDO_H_
#define GUI_PSEUDO_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Pseudo: public iGUI_Pseudo, public GUI_Object
{

	private:

	public:

		GUI_Pseudo();
		~GUI_Pseudo();

		void onClose(wxCloseEvent& evt);
		void paintNow();
		void render(wxDC& dc);

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif /* GUI_Pseudo_H_ */
