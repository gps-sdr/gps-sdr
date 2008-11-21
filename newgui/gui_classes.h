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
#include <wx/button.h>
#include <wx/choice.h>

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
#define ID_SELECT_B 1009
#define ID_EPHEMERIS_B 1010
#define ID_ALMANAC_B 1011
#define ID_LOG_B 1012
#define ID_COMMANDS_B 1013
#define ID_SPEED_B 1014
#define ID_RESET_PVT 1015
#define ID_RESET_CHANNEL 1016
#define ID_RESET_EPHEMERIS 1017
#define ID_RESET_ALMANAC 1018
#define ID_EPHEMERIS_SAVE 1019
#define ID_EPHEMERIS_LOAD 1020
#define ID_ALMANAC_SAVE 1021
#define ID_ALMANAC_LOAD 1022

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
		wxToggleButton* bCommands;
		wxToggleButton* bSpeed;
		wxGauge* gUSRP;
		wxTextCtrl* tUSRP;
		wxTextCtrl* tRS422;
		wxTextCtrl* tTask;
		wxStatusBar* mStatus;
	
	public:
		iGUI_Toplevel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("GPS-SDR"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("GPS-SDR") );
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
/// Class iGUI_Commands
///////////////////////////////////////////////////////////////////////////////
class iGUI_Commands : public wxFrame 
{
	private:
	
	protected:
		wxButton* bReset_PVT;
		wxChoice* mReset_PVT;
		wxButton* bReset_Channel;
		wxChoice* mReset_Channel;
		wxButton* bReset_Ephemeris;
		wxChoice* mReset_Ephemeris;
		wxButton* bReset_Almanac;
		wxChoice* mReset_Almanac;
		wxTextCtrl* tCommand_Ack;
	
	public:
		iGUI_Commands( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Commands"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Commands();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Ephemeris
///////////////////////////////////////////////////////////////////////////////
class iGUI_Ephemeris : public wxFrame 
{
	private:
	
	protected:
		wxPanel* pDecoded;
		wxTextCtrl* tDisplay;
		wxButton* bSave;
		wxButton* bLoad;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onMouse( wxMouseEvent& event ){ event.Skip(); }
		
	
	public:
		iGUI_Ephemeris( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Ephemeris"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Ephemeris();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Almanac
///////////////////////////////////////////////////////////////////////////////
class iGUI_Almanac : public wxFrame 
{
	private:
	
	protected:
		wxPanel* pDecoded;
		wxTextCtrl* tDisplay;
		wxButton* bSave;
		wxButton* bLoad;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onMouse( wxMouseEvent& event ){ event.Skip(); }
		
	
	public:
		iGUI_Almanac( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Almanac"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Almanac();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Acquisition
///////////////////////////////////////////////////////////////////////////////
class iGUI_Acquisition : public wxFrame 
{
	private:
	
	protected:
		wxPanel* pStrong;
		wxPanel* pMedium;
		wxPanel* pWeak;
		wxPanel* pSV;
	
	public:
		iGUI_Acquisition( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Acquisition();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Select
///////////////////////////////////////////////////////////////////////////////
class iGUI_Select : public wxFrame 
{
	private:
	
	protected:
		wxPanel* pDoppler;
		wxPanel* pSV;
		wxPanel* pRec;
	
	public:
		iGUI_Select( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("SV Select"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Select();
	
};

#endif //__gui_classes__
