#ifndef GUI_TOPLEVEL_H
#define GUI_TOPLEVEL_H

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_Toplevel: public iGUI_Toplevel
{

	private:

		/* Add all the "subwindows" here */
		int count;
		GUI_Default		*wdefault;
		wxTimer 		*timer;
		wxString		status_str;

		/* Pipe for communication to GUI */
		int				gpipe[2];		//!< Handle to read and write pipes
		bool			gpipe_open;		//!< Flag to see if the pipe is open
		int				message_sync;	//!< Are we synched to the packets

		/* Serial port for communication to GUI */


		/* Headers for CCSDC packets */
		CCSDS_PH 			pheader;						//!< CCSDS Packet header
		CCSDS_CH 			cheader;						//!< CCSDS Command header
		CCSDS_Header		ccsds_header;					//!< Decoded header

		int packet_count[LAST_M_ID+1];						//!< Count the packets


		/* Data gets stored here! */
		Board_Health_M 		board_health;					//!< Board health message
		Task_Health_M		task_health;					//!< Task health message
		Channel_Health_M 	channel_health[MAX_CHANNELS+1]; //!< Channel health message, last element is used as a buffer

		SPS_M				sps;							//!< SPS message
		Clock_M				clock;							//!< Clock message
		SV_Position_M		sv_positions[MAX_CHANNELS+1];	//!< SV Positions, last element is used as a buffer
		Pseudorange_M 		pseudoranges[MAX_CHANNELS+1];	//!< Pseudoranges, last element is used as a buffer
		Measurement_M 		measurements[MAX_CHANNELS+1];	//!< Measurements, last element is used as a buffer

		Ephemeris_M			ephemeris[NUM_CODES+1];			//!< Ephemeris message, last element is used as a buffer
		Almanac_M			almanac[NUM_CODES+1];			//!< Almanac message, last element is used as a buffer
		Ephemeris_Status_M	ephemeris_status;				//!< Status of ephemeris
		FIFO_M				fifo_status;					//!< FIFO status


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

	public:

		GUI_Toplevel();
		~GUI_Toplevel();

		bool openPipe();
		void readPipe();

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

	    void DecodeCCSDSPacketHeader(CCSDS_Header *_h, CCSDS_PH *_p);

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
