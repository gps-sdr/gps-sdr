///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 21 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __gui_classes__
#define __gui_classes__

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
#include <wx/gauge.h>
#include <wx/textctrl.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/stattext.h>

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
/// Class iGUI_Toplevel
///////////////////////////////////////////////////////////////////////////////
class iGUI_Toplevel : public wxFrame 
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
		wxGauge* gUSRP;
		wxTextCtrl* tUSRP;
		wxTextCtrl* tRS422;
		wxTextCtrl* tTask;
		wxStatusBar* mStatus;
	
	public:
		iGUI_Toplevel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("GPS-SDR"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 771,635 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("GPS-SDR") );
		~iGUI_Toplevel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Default
///////////////////////////////////////////////////////////////////////////////
class iGUI_Default : public wxFrame 
{
	private:
	
	protected:
		wxPanel* pCN0;
		wxPanel* pSkyPlot;
		wxTextCtrl* tPVT;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		
	
	public:
		iGUI_Default( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Main"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 935,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Default();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Channel
///////////////////////////////////////////////////////////////////////////////
class iGUI_Channel : public wxFrame 
{
	private:
	
	protected:
		wxTextCtrl* tChannel;
	
	public:
		iGUI_Channel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Channel Status"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Channel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame4
///////////////////////////////////////////////////////////////////////////////
class MyFrame4 : public wxFrame 
{
	private:
	
	protected:
		wxGauge* g1;
		wxGauge* g2;
		wxGauge* g3;
		wxGauge* g4;
		wxGauge* g5;
		wxGauge* g6;
		wxGauge* g7;
		wxGauge* g8;
		wxGauge* g9;
		wxGauge* g10;
		wxGauge* g11;
		wxGauge* g12;
		wxStaticText* t1;
		wxStaticText* t2;
		wxStaticText* t3;
		wxStaticText* t4;
		wxStaticText* t5;
		wxStaticText* t6;
		wxStaticText* t7;
		wxStaticText* t8;
		wxStaticText* t9;
		wxStaticText* t10;
		wxStaticText* t11;
		wxStaticText* t12;
	
	public:
		MyFrame4( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MyFrame4();
	
};

#endif //__gui_classes__
