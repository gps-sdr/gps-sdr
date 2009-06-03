#ifndef GUI_TOPLEVEL_H
#define GUI_TOPLEVEL_H

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Toplevel: public iGUI_Toplevel
{

	private:

		/* Add all the "subwindows" here */
		int count;
		GUI_PVT			*wPVT;
		GUI_EKF			*wEKF;
		GUI_Channel		*wChannel;
		GUI_Pseudo		*wPseudo;
		GUI_Ephemeris	*wEphemeris;
		GUI_Almanac		*wAlmanac;
		GUI_Select		*wSelect;
		GUI_Health		*wHealth;
		GUI_Commands	*wCommands;
		//GUI_EEPROM		*wEEPROM;
		GUI_Messages	*wMessages;
		GUI_Speedo		*wSpeedo;
		class GUI_Serial*pSerial;

		wxTimer 		*timer;
		wxString		status_str;
		wxString		log_filename;

		int				logging_on;
		int				bytes_pres;
		int				bytes_prev;
		int				bytes_sec;

		Message_Struct	messages;						//!< Hold all the messages

		float 			kB_sec;
		int				last_tic;						//!< Only update when new info is available

	public:

		GUI_Toplevel();
		~GUI_Toplevel();


		void onTimer(wxTimerEvent& evt);
		void onClose(wxCloseEvent& evt);
		void onQuit(wxCommandEvent& event);
		void onAbout(wxCommandEvent& event);
		void onLogConfig(wxCommandEvent& event);
		void onLogStart(wxCommandEvent& event);
		void onLogStop(wxCommandEvent& event);
		void onLogClear(wxCommandEvent& event);
		void onSerial(wxCommandEvent& event);
		void onNpipe(wxCommandEvent& event);
		void onUSB(wxCommandEvent& event);

		void onPVT(wxCommandEvent& event);
		void onEKF(wxCommandEvent& event);
		void onChannel(wxCommandEvent& event);
		void onPseudo(wxCommandEvent& event);
		void onEphemeris(wxCommandEvent& event);
		void onAlmanac(wxCommandEvent& event);
		void onSelect(wxCommandEvent& event);
		void onHealth(wxCommandEvent& event);
		void onCommands(wxCommandEvent& event);
		void onEEPROM(wxCommandEvent& event);
		void onMessages(wxCommandEvent& event);
		void onSpeedo(wxCommandEvent& event);

	    void paintEvent(wxPaintEvent& evt);
	    void paintNow();

	    void render(wxDC& dc);
	    void renderRS422();
	    void renderTask();

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
