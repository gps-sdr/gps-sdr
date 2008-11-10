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
class GUI_Channel: public iGUI_Channel
{

	private:

		wxGauge 	*gCN0[MAX_CHANNELS];
		wxStaticText *tCN0[MAX_CHANNELS];
		Message_Struct *p;

	public:

		GUI_Channel();
		~GUI_Channel();

		void render(wxDC& dc);
		void paintNow();
		void renderChannel();
		void setPointer(Message_Struct *_p);

};
/*----------------------------------------------------------------------------------------------*/

#endif /* GUI_Channel_H_ */
