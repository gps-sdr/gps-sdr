#ifndef GUI_TOPLEVEL_H
#define GUI_TOPLEVEL_H

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Toplevel: public iGUI_Toplevel
{

	private:

		/* Add all the "subwindows" here */
		int count;
		GUI_Default		*wDefault;
		class GUI_Serial*pSerial;

		wxTimer 		*timer;
		wxString		status_str;

		/* GPS-SDR exec variables */
		wxInputStream* 	gps_in;
		wxOutputStream* gps_out;
		wxProcess*		gps_proc;
		int				gps_pid;
		int				gps_active;

		/* GPS-USRP exec variables */
		wxInputStream* 	usrp_in;
		wxOutputStream* usrp_out;
		wxProcess*		usrp_proc;
		int				usrp_pid;
		int				usrp_active;

		int				bytes_pres;
		int				bytes_prev;
		int				bytes_sec;

	public:

		GUI_Toplevel();
		~GUI_Toplevel();


		void onTimer(wxTimerEvent& evt);
		void onClose(wxCloseEvent& evt);
		void onQuit(wxCommandEvent& event);
		void onAbout(wxCommandEvent& event);
		void onGPSStart(wxCommandEvent& event);
		void onGPSStop(wxCommandEvent& event);
		void onUSRPStart(wxCommandEvent& event);
		void onUSRPStop(wxCommandEvent& event);

		void onMain(wxCommandEvent& event);


	    void paintEvent(wxPaintEvent& evt);
	    void paintNow();

	    void render(wxDC& dc);
	    void renderFIFO();
	    void renderRS422();
	    void renderTask();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
