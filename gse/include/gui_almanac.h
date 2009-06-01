#ifndef GUI_ALMANAC_H_
#define GUI_ALMANAC_H_

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Almanac : public iGUI_Almanac, public GUI_Object
{

	private:

		int32 sv;		//!< Selected SV
		int32 loaded;	//!< Has the almanac status packet been emitted
		int32 tic;		//!< Count updates

		int32 state; 	//!< 0: inactive, 1: save, 2: load cycle
		int32 sv_index;	//!< Used to cycle through the SVs
		FILE *fp;		//!< File for reading/writings

	public:

		GUI_Almanac();
		~GUI_Almanac();

		void render(wxDC& dc);
		void paintNow();
		void onClose(wxCloseEvent& event);
		void onMouse(wxMouseEvent& event);
		void onSave(wxCommandEvent& event);
		void onLoad(wxCommandEvent& event);
		void onRefresh(wxCommandEvent& event);
		void renderDecoded();
		void renderSV();
		void svSave();
		void svLoad();
		void setState(int32 _state);

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
