///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 21 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "noname.h"

///////////////////////////////////////////////////////////////////////////

GUI_Main::GUI_Main( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxFrame( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	mMenuBar = new wxMenuBar( 0 );
	mFile = new wxMenu();
	wxMenuItem* miAbout;
	miAbout = new wxMenuItem( mFile, ID_About, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	mFile->Append( miAbout );
	
	wxMenuItem* miExit;
	miExit = new wxMenuItem( mFile, ID_Quit, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	mFile->Append( miExit );
	
	mMenuBar->Append( mFile, wxT("File") );
	
	mReceiver = new wxMenu();
	wxMenuItem* miReceiverStart;
	miReceiverStart = new wxMenuItem( mReceiver, ID_GPS_Start, wxString( wxT("Start") ) , wxEmptyString, wxITEM_NORMAL );
	mReceiver->Append( miReceiverStart );
	
	wxMenuItem* miReceiverStop;
	miReceiverStop = new wxMenuItem( mReceiver, ID_GPS_Stop, wxString( wxT("Stop") ) , wxEmptyString, wxITEM_NORMAL );
	mReceiver->Append( miReceiverStop );
	
	mMenuBar->Append( mReceiver, wxT("Receiver") );
	
	mUSRP = new wxMenu();
	wxMenuItem* miUSRPStart;
	miUSRPStart = new wxMenuItem( mUSRP, ID_USRP_Start, wxString( wxT("Start") ) , wxEmptyString, wxITEM_NORMAL );
	mUSRP->Append( miUSRPStart );
	
	wxMenuItem* miUSRPStop;
	miUSRPStop = new wxMenuItem( mUSRP, ID_USRP_Stop, wxString( wxT("Stop") ) , wxEmptyString, wxITEM_NORMAL );
	mUSRP->Append( miUSRPStop );
	
	mMenuBar->Append( mUSRP, wxT("USRP") );
	
	this->SetMenuBar( mMenuBar );
	
	wxBoxSizer* sMain;
	sMain = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* SubWindows;
	SubWindows = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Windows") ), wxVERTICAL );
	
	bMain = new wxToggleButton( this, ID_MAIN_B, wxT("Main"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bMain, 0, wxALL|wxEXPAND, 5 );
	
	bChannel = new wxToggleButton( this, ID_CHANNEL_B, wxT("Channel"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bChannel, 0, wxALL|wxEXPAND, 5 );
	
	bAcquisition = new wxToggleButton( this, ID_ACQUISITION_B, wxT("Acquisition"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bAcquisition, 0, wxALL|wxEXPAND, 5 );
	
	bSV_Select = new wxToggleButton( this, ID_SV_SELECT_B, wxT("SV_Select"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bSV_Select, 0, wxALL|wxEXPAND, 5 );
	
	bEphemeris = new wxToggleButton( this, ID_EPHEMERIS_B, wxT("Ephemeris"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bEphemeris, 0, wxALL|wxEXPAND, 5 );
	
	bAlmanac = new wxToggleButton( this, ID_ALMANAC_B, wxT("Almanac"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bAlmanac, 0, wxALL|wxEXPAND, 5 );
	
	bLog = new wxToggleButton( this, ID_LOG_B, wxT("Log"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bLog, 0, wxALL|wxEXPAND, 5 );
	
	bSpeed = new wxToggleButton( this, ID_SPEED_B, wxT("Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bSpeed, 0, wxALL|wxEXPAND, 5 );
	
	sMain->Add( SubWindows, 1, wxALIGN_LEFT|wxALIGN_TOP|wxALL, 5 );
	
	wxBoxSizer* sStatus;
	sStatus = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sUSRP;
	sUSRP = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("USRP Status") ), wxVERTICAL );
	
	pUSRP = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	sUSRP->Add( pUSRP, 1, wxEXPAND | wxALL, 5 );
	
	sStatus->Add( sUSRP, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sRS422;
	sRS422 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("RS422 Status") ), wxVERTICAL );
	
	pUSRP1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	sRS422->Add( pUSRP1, 1, wxEXPAND | wxALL, 5 );
	
	sStatus->Add( sRS422, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sTask;
	sTask = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Task Status") ), wxVERTICAL );
	
	pUSRP2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxTAB_TRAVERSAL );
	sTask->Add( pUSRP2, 1, wxEXPAND | wxALL, 5 );
	
	sStatus->Add( sTask, 3, wxEXPAND, 5 );
	
	sMain->Add( sStatus, 3, wxEXPAND, 5 );
	
	this->SetSizer( sMain );
	this->Layout();
}

GUI_Main::~GUI_Main()
{
}
