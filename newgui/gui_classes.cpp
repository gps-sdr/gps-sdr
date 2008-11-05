///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 21 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui_classes.h"

///////////////////////////////////////////////////////////////////////////

iGUI_Toplevel::iGUI_Toplevel( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxFrame( parent, id, title, pos, size, style, name )
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
	
	sMain->Add( SubWindows, 1, wxALIGN_LEFT|wxALIGN_TOP|wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* sStatus;
	sStatus = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sUSRP;
	sUSRP = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("USRP") ), wxHORIZONTAL );
	
	gUSRP = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	sUSRP->Add( gUSRP, 0, wxALL|wxEXPAND, 5 );
	
	tUSRP = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE );
	sUSRP->Add( tUSRP, 1, wxALL|wxEXPAND, 5 );
	
	sStatus->Add( sUSRP, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sRS422;
	sRS422 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("RS422") ), wxVERTICAL );
	
	tRS422 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE );
	sRS422->Add( tRS422, 1, wxALL|wxEXPAND, 5 );
	
	sStatus->Add( sRS422, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sTask;
	sTask = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Task") ), wxVERTICAL );
	
	tTask = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE );
	sTask->Add( tTask, 1, wxALL|wxEXPAND, 5 );
	
	sStatus->Add( sTask, 2, wxEXPAND, 5 );
	
	sMain->Add( sStatus, 3, wxEXPAND, 5 );
	
	this->SetSizer( sMain );
	this->Layout();
	mStatus = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
}

iGUI_Toplevel::~iGUI_Toplevel()
{
}

iGUI_Default::iGUI_Default( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("SNR") ), wxVERTICAL );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	sbSizer10->Add( bSizer22, 2, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	g1 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	bSizer19->Add( g1, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	g2 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	bSizer19->Add( g2, 1, wxALL|wxEXPAND, 5 );
	
	g3 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	bSizer19->Add( g3, 1, wxALL|wxEXPAND, 5 );
	
	g4 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	bSizer19->Add( g4, 1, wxALL|wxEXPAND, 5 );
	
	g5 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	bSizer19->Add( g5, 1, wxALL|wxEXPAND, 5 );
	
	g6 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	bSizer19->Add( g6, 1, wxALL|wxEXPAND, 5 );
	
	g7 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	bSizer19->Add( g7, 1, wxALL|wxEXPAND, 5 );
	
	g8 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	bSizer19->Add( g8, 1, wxALL|wxEXPAND, 5 );
	
	g9 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	bSizer19->Add( g9, 1, wxALL|wxEXPAND, 5 );
	
	g10 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	bSizer19->Add( g10, 1, wxALL|wxEXPAND, 5 );
	
	g11 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	bSizer19->Add( g11, 1, wxALL|wxEXPAND, 5 );
	
	g12 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	bSizer19->Add( g12, 1, wxALL|wxEXPAND, 5 );
	
	sbSizer10->Add( bSizer19, 16, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	t1 = new wxStaticText( this, wxID_ANY, wxT("00"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	t1->Wrap( -1 );
	t1->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	bSizer21->Add( t1, 1, wxEXPAND, 5 );
	
	t2 = new wxStaticText( this, wxID_ANY, wxT("01"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	t2->Wrap( -1 );
	bSizer21->Add( t2, 1, wxEXPAND, 5 );
	
	t3 = new wxStaticText( this, wxID_ANY, wxT("02"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	t3->Wrap( -1 );
	bSizer21->Add( t3, 1, wxEXPAND, 5 );
	
	t4 = new wxStaticText( this, wxID_ANY, wxT("03"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	t4->Wrap( -1 );
	bSizer21->Add( t4, 1, wxEXPAND, 5 );
	
	t5 = new wxStaticText( this, wxID_ANY, wxT("04"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	t5->Wrap( -1 );
	bSizer21->Add( t5, 1, wxEXPAND, 5 );
	
	t6 = new wxStaticText( this, wxID_ANY, wxT("05"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	t6->Wrap( -1 );
	bSizer21->Add( t6, 1, wxEXPAND, 5 );
	
	t7 = new wxStaticText( this, wxID_ANY, wxT("06"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	t7->Wrap( -1 );
	bSizer21->Add( t7, 1, wxEXPAND, 5 );
	
	t8 = new wxStaticText( this, wxID_ANY, wxT("07"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	t8->Wrap( -1 );
	bSizer21->Add( t8, 1, wxEXPAND, 5 );
	
	t9 = new wxStaticText( this, wxID_ANY, wxT("11"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	t9->Wrap( -1 );
	bSizer21->Add( t9, 1, wxEXPAND, 5 );
	
	t10 = new wxStaticText( this, wxID_ANY, wxT("08"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	t10->Wrap( -1 );
	bSizer21->Add( t10, 1, wxEXPAND, 5 );
	
	t11 = new wxStaticText( this, wxID_ANY, wxT("09"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	t11->Wrap( -1 );
	bSizer21->Add( t11, 1, wxEXPAND, 5 );
	
	t12 = new wxStaticText( this, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	t12->Wrap( -1 );
	bSizer21->Add( t12, 1, wxEXPAND, 5 );
	
	sbSizer10->Add( bSizer21, 1, wxEXPAND, 5 );
	
	bSizer9->Add( sbSizer10, 3, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer11;
	sbSizer11 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Sky Plot") ), wxVERTICAL );
	
	bSizer18->Add( sbSizer11, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer13;
	sbSizer13 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("PVT") ), wxVERTICAL );
	
	m_textCtrl8 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT );
	sbSizer13->Add( m_textCtrl8, 1, wxALL|wxEXPAND, 5 );
	
	bSizer18->Add( sbSizer13, 1, wxEXPAND, 5 );
	
	bSizer9->Add( bSizer18, 2, wxEXPAND, 5 );
	
	bSizer8->Add( bSizer9, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer8 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( iGUI_Default::OnClose ) );
}

iGUI_Default::~iGUI_Default()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( iGUI_Default::OnClose ) );
}
