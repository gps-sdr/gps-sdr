///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
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
#include <wx/textctrl.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/filepicker.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_ABOUT 1000
#define ID_QUIT 1001
#define ID_LOG_CONFIG 1002
#define ID_LOG_START 1003
#define ID_LOG_STOP 1004
#define ID_LOG_CLEAR 1005
#define ID_SERIAL 1006
#define ID_NPIPE 1007
#define ID_PVT_B 1008
#define ID_EKF_B 1009
#define ID_CHANNEL_B 1010
#define ID_PSEUDO_B 1011
#define ID_EPHEMERIS_B 1012
#define ID_ALMANAC_B 1013
#define ID_SELECT_B 1014
#define ID_HEALTH_B 1015
#define ID_COMMANDS_B 1016
#define ID_MESSAGES_B 1017
#define ID_SPEEDO_B 1018
#define ID_EPHEMERIS_REFRESH 1019
#define ID_EPHEMERIS_SAVE 1020
#define ID_EPHEMERIS_LOAD 1021
#define ID_ALMANAC_REFRESH 1022
#define ID_ALMANAC_SAVE 1023
#define ID_ALMANAC_LOAD 1024
#define ID_NULL 1025
#define ID_RESET_WATCHDOG 1026
#define ID_RESET_ALL 1027
#define ID_RESET_PPS 1028
#define ID_RESET_PVT 1029
#define ID_RESET_EKF 1030
#define ID_RESET_CHANNEL 1031
#define ID_RESET_EPHEMERIS 1032
#define ID_RESET_ALMANAC 1033
#define ID_EEPROM_STOP 1034
#define ID_EEPROM_START 1035
#define ID_SELECT_REFRESH 1036

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Toplevel
///////////////////////////////////////////////////////////////////////////////
class iGUI_Toplevel : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* mMenuBar;
		wxMenu* mFile;
		wxMenu* mLogging;
		wxMenu* mInterface;
		wxToggleButton* bPVT;
		wxToggleButton* bEKF;
		wxToggleButton* bChannel;
		wxToggleButton* bPseudo;
		wxToggleButton* bEphemeris;
		wxToggleButton* bAlmanac;
		wxToggleButton* bSelect;
		wxToggleButton* bHealth;
		wxToggleButton* bCommands;
		wxToggleButton* bMessages;
		wxToggleButton* bSpeedo;
		wxTextCtrl* tRS422;
		wxTextCtrl* tTask;
		wxStatusBar* mStatus;
	
	public:
		iGUI_Toplevel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Navigator GSE"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("GPS-SDR") );
		~iGUI_Toplevel();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_PVT
///////////////////////////////////////////////////////////////////////////////
class iGUI_PVT : public wxFrame 
{
	private:
	
	protected:
		wxPanel* pCN0;
		wxStaticText* m_staticText5311;
		wxStaticText* m_staticText5343;
		wxStaticText* rticks;
		wxStaticText* m_staticText536111;
		wxStaticText* sticks;
		wxStaticText* m_staticText53411;
		wxStaticText* cticks;
		wxStaticText* m_staticText5321;
		
		wxStaticText* m_staticText53511;
		wxStaticText* iter;
		wxStaticText* m_staticText53431;
		wxStaticText* nsv;
		wxStaticText* m_staticText531;
		wxStaticText* m_staticText534;
		wxStaticText* lat;
		wxStaticText* m_staticText5341;
		wxStaticText* lon;
		wxStaticText* m_staticText53611;
		wxStaticText* alt;
		wxStaticText* m_staticText53614;
		wxStaticText* gdop;
		wxStaticText* m_staticText532;
		wxStaticText* m_staticText535;
		wxStaticText* px;
		wxStaticText* m_staticText5351;
		wxStaticText* py;
		wxStaticText* m_staticText53612;
		wxStaticText* pz;
		
		wxStaticText* m_staticText533;
		wxStaticText* m_staticText536;
		wxStaticText* vx;
		wxStaticText* m_staticText5361;
		wxStaticText* vy;
		wxStaticText* m_staticText53613;
		wxStaticText* vz;
		wxStaticText* m_staticText5342;
		wxStaticText* speed;
		wxStaticText* m_staticText57721;
		wxStaticText* utct;
		wxStaticText* m_staticText577;
		wxStaticText* gpsw;
		wxStaticText* m_staticText5772;
		wxStaticText* gpss;
		wxStaticText* m_staticText5771;
		wxStaticText* cb;
		wxStaticText* m_staticText57711;
		wxStaticText* cr;
		wxStaticText* m_staticText5773;
		wxStaticText* ppsstate;
		wxStaticText* m_staticText577211;
		wxStaticText* ppscmd;
		wxStaticText* m_staticText57712;
		wxStaticText* ppscr;
		wxStaticText* m_staticText577111;
		wxStaticText* ppserr;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
		
	
	public:
		iGUI_PVT( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("PVT"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_PVT();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_EKF
///////////////////////////////////////////////////////////////////////////////
class iGUI_EKF : public wxFrame 
{
	private:
	
	protected:
		wxTextCtrl* tResidual;
		wxStaticText* m_staticText5311;
		wxStaticText* m_staticText5343;
		wxStaticText* rticks;
		wxStaticText* m_staticText53411;
		wxStaticText* sticks;
		wxStaticText* m_staticText53511;
		wxStaticText* period;
		wxStaticText* m_staticText5321;
		wxStaticText* m_staticText53431;
		wxStaticText* status;
		wxStaticText* m_staticText534311;
		wxStaticText* err;
		wxStaticText* m_staticText534312;
		wxStaticText* nsv;
		wxStaticText* m_staticText531;
		wxStaticText* m_staticText536111;
		wxStaticText* srp;
		wxStaticText* m_staticText53611;
		wxStaticText* drag;
		wxStaticText* m_staticText57712;
		wxStaticText* cb;
		wxStaticText* m_staticText57713;
		wxStaticText* cr;
		wxStaticText* m_staticText532;
		wxStaticText* m_staticText535;
		wxStaticText* px;
		wxStaticText* m_staticText5351;
		wxStaticText* py;
		wxStaticText* m_staticText53612;
		wxStaticText* pz;
		
		wxStaticText* m_staticText533;
		wxStaticText* m_staticText536;
		wxStaticText* vx;
		wxStaticText* m_staticText5361;
		wxStaticText* vy;
		wxStaticText* m_staticText53613;
		wxStaticText* vz;
		wxStaticText* m_staticText5342;
		wxStaticText* speed;
		wxStaticText* m_staticText5312;
		wxStaticText* m_staticText5361111;
		wxStaticText* csrp;
		wxStaticText* m_staticText536112;
		wxStaticText* cdrag;
		wxStaticText* m_staticText5361121;
		wxStaticText* ccb;
		wxStaticText* m_staticText5361122;
		wxStaticText* ccr;
		wxStaticText* m_staticText5322;
		wxStaticText* m_staticText5352;
		wxStaticText* cpx;
		wxStaticText* m_staticText53512;
		wxStaticText* cpy;
		wxStaticText* m_staticText536121;
		wxStaticText* cpz;
		
		wxStaticText* m_staticText5331;
		wxStaticText* m_staticText5362;
		wxStaticText* cvx;
		wxStaticText* m_staticText53614;
		wxStaticText* cvy;
		wxStaticText* m_staticText536131;
		wxStaticText* cvz;
		
		wxStaticText* m_staticText57721;
		wxStaticText* utct;
		wxStaticText* m_staticText577;
		wxStaticText* gpsw;
		wxStaticText* m_staticText5772;
		wxStaticText* gpss;
	
	public:
		iGUI_EKF( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("EKF"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_EKF();
	
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
/// Class iGUI_Pseudo
///////////////////////////////////////////////////////////////////////////////
class iGUI_Pseudo : public wxFrame 
{
	private:
	
	protected:
		wxTextCtrl* tPseudo;
	
	public:
		iGUI_Pseudo( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Pseudoranges"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Pseudo();
	
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
		
		wxButton* bRefresh;
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
		
		wxButton* bRefresh;
		wxButton* bSave;
		wxButton* bLoad;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void onMouse( wxMouseEvent& event ){ event.Skip(); }
		
	
	public:
		iGUI_Almanac( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Almanac"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Almanac();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Health
///////////////////////////////////////////////////////////////////////////////
class iGUI_Health : public wxFrame 
{
	private:
	
	protected:
		
		wxStaticText* m_staticText533;
		wxStaticText* m_staticText534;
		wxStaticText* m_staticText535;
		wxStaticText* m_staticText536;
		wxStaticText* m_staticText537;
		wxStaticText* trkv;
		wxStaticText* acqv;
		wxStaticText* fftv;
		wxStaticText* sftv;
		
		wxStaticText* m_staticText53;
		wxStaticText* m_staticText56;
		wxStaticText* m_staticText54;
		wxStaticText* m_staticText55;
		wxStaticText* m_staticText5;
		wxStaticText* dsa0;
		wxStaticText* dsa1;
		wxStaticText* dsa2;
		wxStaticText* dsa3;
		wxStaticText* m_staticText52;
		wxStaticText* ovr0;
		wxStaticText* ovr1;
		wxStaticText* ovr2;
		wxStaticText* ovr3;
		
		wxStaticText* m_staticText53141;
		wxStaticText* nsptr;
		wxStaticText* m_staticText531421;
		wxStaticText* pcctr;
		
		wxStaticText* m_staticText53142;
		wxStaticText* rftr;
		wxStaticText* m_staticText561;
		wxStaticText* vref;
		
		wxStaticText* m_staticText531;
		wxStaticText* gnd1;
		wxStaticText* m_staticText541;
		wxStaticText* gnd2;
		
		wxStaticText* m_staticText551;
		wxStaticText* p5;
		wxStaticText* m_staticText5311;
		wxStaticText* p15;
		
		wxStaticText* m_staticText5312;
		wxStaticText* n15;
		wxStaticText* m_staticText5313;
		wxStaticText* p25;
		
		wxStaticText* m_staticText5314;
		wxStaticText* p33;
		wxStaticText* m_staticText531422;
		wxStaticText* p1p5;
		
		wxStaticText* m_staticText5314221;
		wxStaticText* rf0;
		wxStaticText* m_staticText5314222;
		wxStaticText* rf1;
		
		wxStaticText* m_staticText53142221;
		wxStaticText* rf2;
		wxStaticText* m_staticText53142222;
		wxStaticText* rf3;
	
	public:
		iGUI_Health( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Board Health"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Health();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Commands
///////////////////////////////////////////////////////////////////////////////
class iGUI_Commands : public wxFrame 
{
	private:
	
	protected:
		wxButton* bNull;
		wxButton* bResetWatchdog;
		wxButton* bResetAll;
		wxButton* bResetPPS;
		wxButton* bReset_PVT;
		wxButton* bResetEKF;
		wxButton* bReset_Channel;
		wxChoice* mReset_Channel;
		wxButton* bReset_Ephemeris;
		wxChoice* mReset_Ephemeris;
		wxButton* bReset_Almanac;
		wxChoice* mReset_Almanac;
		wxTextCtrl* tCommand_Ack;
	
	public:
		iGUI_Commands( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Commands"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 300,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Commands();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_EEPROM
///////////////////////////////////////////////////////////////////////////////
class iGUI_EEPROM : public wxFrame 
{
	private:
	
	protected:
		wxChoice* mBurn;
		wxChoice* mBank;
		wxChoice* mType;
		wxTextCtrl* tFeedback;
		wxButton* mStop;
		wxButton* mStart;
	
	public:
		wxFilePickerCtrl* mEEPROMFile;
		iGUI_EEPROM( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("EEPROM"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_EEPROM();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Log
///////////////////////////////////////////////////////////////////////////////
class iGUI_Log : public wxDialog 
{
	private:
	
	protected:
		
		wxButton* bCancel;
		wxButton* bOK;
	
	public:
		wxCheckBox* cPVT;
		wxCheckBox* cClock;
		wxCheckBox* cPPS;
		wxCheckBox* cEKFState;
		wxCheckBox* cEKFCov;
		wxCheckBox* cEKFRes;
		wxCheckBox* cPseudo;
		wxCheckBox* cChan;
		wxCheckBox* cTask;
		wxCheckBox* cBoard;
		wxCheckBox* cSVPred;
		wxFilePickerCtrl* mFile;
		iGUI_Log( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Logging Config"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~iGUI_Log();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Messages
///////////////////////////////////////////////////////////////////////////////
class iGUI_Messages : public wxFrame 
{
	private:
	
	protected:
		wxTextCtrl* tMess;
	
	public:
		iGUI_Messages( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Messages"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 250,460 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Messages();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Select
///////////////////////////////////////////////////////////////////////////////
class iGUI_Select : public wxFrame 
{
	private:
	
	protected:
		wxPanel* pVisible;
		wxPanel* pMode;
		wxTextCtrl* tDisplay;
		
		wxButton* bRefresh;
		
		
		// Virtual event handlers, overide them in your derived class
		virtual void onMouse( wxMouseEvent& event ){ event.Skip(); }
		
	
	public:
		iGUI_Select( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("SV Select"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Select();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class iGUI_Speedo
///////////////////////////////////////////////////////////////////////////////
class iGUI_Speedo : public wxFrame 
{
	private:
	
	protected:
		wxPanel* pspeed;
		wxPanel* pdist;
		wxPanel* pstop1;
		wxPanel* pmavg;
		wxPanel* pmax;
		wxPanel* pttime;
		wxPanel* pmtime;
		wxPanel* pstop;
	
	public:
		iGUI_Speedo( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Speedometer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,500 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~iGUI_Speedo();
	
};

#endif //__gui_classes__
