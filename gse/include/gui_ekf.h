/*
 * gui_pseudo.h
 *
 *  Created on: Nov 4, 2008
 *      Author: gheckler
 */

#ifndef GUI_EKF_H_
#define GUI_EKF_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_EKF: public iGUI_EKF, public GUI_Object
{

	private:

	public:

		GUI_EKF();
		~GUI_EKF();

		void onClose(wxCloseEvent& evt);
		void paintNow();
		void render(wxDC& dc);
		void renderEKF();
		void renderResidual();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif /* GUI_EKF_H_ */
