#ifndef GUI_EEPROM_H
#define GUI_EEPROM_H

#include "gui.h"

/*----------------------------------------------------------------------------------------------*/
class GUI_EEPROM: public iGUI_EEPROM, public GUI_Object
{

	private:

		/* State machine for EEPROM writes */
		uint32 execution_tic;				//!< Execution tic
		int32 active;						//!< Is the process active (START button)
		FILE *fp;							//!< File pointer
		uint32 bwrote;						//!< Bytes written
		uint32 b2write;						//!< Bytes to be written
		uint32 fsize;						//!< Size of file
		uint32 type;						//!< Application code/ checksum block/ configuration table
		uint32 bank;						//!< Burn EEPROM A or B
		uint32 offset;						//!< Offset (address) to burn to
		uint32 mode;						//!< Mode: burn, dump, or verify
		uint32 count;						//!< Poll for an ack
		Set_EEPROM_C 			setcmd;		//!< Burn E^2
		Get_EEPROM_C 			getcmd;		//!< Return E^2
		Get_EEPROM_Chksum_C 	chkcmd;		//!< Return E^2 checksum
		EEPROM_Chksum_M 		chkmsg;		//!< E^2 checksum message
		EEPROM_M				eepmsg;		//!< E^2 return message
		Command_Ack_M			ackmsg;		//!< Command ack

		/* Default object variables */
		wxMutex mutex;

	public:

		GUI_EEPROM();
		~GUI_EEPROM();

		void onClose(wxCloseEvent& evt);
		void render(wxDC& dc);
		void paintNow();

		void onStart(wxCommandEvent& event);
		void onStop(wxCommandEvent& event);
		void writeEEPROM();					//!< Burn the EEPROM
		void readEEPROM();					//!< Read back the EEPROM
		void chksumEEPROM();				//!< Checksum the EEPROM against a .bin file
		void resetState();					//!< Reset state to 0s

		DECLARE_EVENT_TABLE()

};
/*----------------------------------------------------------------------------------------------*/

#endif
