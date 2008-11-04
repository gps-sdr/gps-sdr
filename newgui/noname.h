///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 21 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __noname__
#define __noname__

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/tglbtn.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_About 1000
#define ID_Quit 1001
#define ID_GPS_Start 1002
#define ID_GPS_Stop 1003
#define ID_USRP_Start 1004
#define ID_USRP_Stop 1005
#define ID_MAIN_B 1006
#define ID_CHANNEL_B 1007
#define ID_ACQUISITION_B 1008
#define ID_SV_SELECT_B 1009
#define ID_EPHEMERIS_B 1010
#define ID_ALMANAC_B 1011
#define ID_LOG_B 1012
#define ID_SPEED_B 1013

///////////////////////////////////////////////////////////////////////////////
/// Class GUI_Main
///////////////////////////////////////////////////////////////////////////////
class GUI_Main : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* mMenuBar;
		wxMenu* mFile;
		wxMenu* mReceiver;
		wxMenu* mUSRP;
		wxToggleButton* bMain;
		wxToggleButton* bChannel;
		wxToggleButton* bAcquisition;
		wxToggleButton* bSV_Select;
		wxToggleButton* bEphemeris;
		wxToggleButton* bAlmanac;
		wxToggleButton* bLog;
		wxToggleButton* bSpeed;
		wxPanel* pUSRP;
		wxPanel* pUSRP1;
		wxPanel* pUSRP2;
	
	public:
		GUI_Main( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("GPS-SDR"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 729,635 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("GPS-SDR") );
		~GUI_Main();
	
};

#endif //__noname__
