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
	
	bCommands = new wxToggleButton( this, ID_COMMANDS_B, wxT("Commands"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bCommands, 0, wxALL|wxEXPAND, 5 );
	
	bSpeed = new wxToggleButton( this, ID_SPEED_B, wxT("Speedometer"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bSpeed, 0, wxALL|wxEXPAND, 5 );
	
	sMain->Add( SubWindows, 1, wxALIGN_LEFT|wxALIGN_TOP|wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* sStatus;
	sStatus = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sUSRP;
	sUSRP = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("USRP") ), wxHORIZONTAL );
	
	gUSRP = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_SMOOTH|wxGA_VERTICAL );
	sUSRP->Add( gUSRP, 0, wxALL|wxEXPAND, 5 );
	
	tUSRP = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE );
	tUSRP->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	sUSRP->Add( tUSRP, 1, wxALL|wxEXPAND, 5 );
	
	sStatus->Add( sUSRP, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sRS422;
	sRS422 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("RS422") ), wxVERTICAL );
	
	tRS422 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE );
	tRS422->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	sRS422->Add( tRS422, 1, wxALL|wxEXPAND, 5 );
	
	sStatus->Add( sRS422, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sTask;
	sTask = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Task") ), wxVERTICAL );
	
	tTask = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE );
	tTask->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
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
	
	wxStaticBoxSizer* bSizer29;
	bSizer29 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("CN0") ), wxVERTICAL );
	
	pCN0 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer29->Add( pCN0, 1, wxEXPAND, 5 );
	
	bSizer9->Add( bSizer29, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	bSizer18->SetMinSize( wxSize( 500,-1 ) ); 
	wxStaticBoxSizer* sbSizer11;
	sbSizer11 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Sky Plot") ), wxVERTICAL );
	
	pSkyPlot = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer11->Add( pSkyPlot, 1, wxEXPAND, 5 );
	
	bSizer18->Add( sbSizer11, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer13;
	sbSizer13 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("PVT") ), wxVERTICAL );
	
	sbSizer13->SetMinSize( wxSize( -1,220 ) ); 
	tPVT = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE );
	tPVT->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	sbSizer13->Add( tPVT, 1, wxEXPAND, 5 );
	
	bSizer18->Add( sbSizer13, 0, wxALL|wxEXPAND|wxFIXED_MINSIZE, 5 );
	
	bSizer9->Add( bSizer18, 0, wxEXPAND|wxFIXED_MINSIZE, 5 );
	
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

iGUI_Channel::iGUI_Channel( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	tChannel = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE );
	tChannel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer9->Add( tChannel, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer9 );
	this->Layout();
}

iGUI_Channel::~iGUI_Channel()
{
}

iGUI_Commands::iGUI_Commands( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	bReset_PVT = new wxButton( this, ID_RESET_PVT, wxT("Reset PVT"), wxDefaultPosition, wxDefaultSize, 0 );
	bReset_PVT->SetMinSize( wxSize( 150,-1 ) );
	
	bSizer17->Add( bReset_PVT, 0, wxALL, 5 );
	
	wxString mReset_PVTChoices[] = { wxT("All") };
	int mReset_PVTNChoices = sizeof( mReset_PVTChoices ) / sizeof( wxString );
	mReset_PVT = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mReset_PVTNChoices, mReset_PVTChoices, 0 );
	mReset_PVT->SetSelection( 0 );
	bSizer17->Add( mReset_PVT, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer17, 0, 0, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	bReset_Channel = new wxButton( this, ID_RESET_CHANNEL, wxT("Reset Channel"), wxDefaultPosition, wxDefaultSize, 0 );
	bReset_Channel->SetMinSize( wxSize( 150,-1 ) );
	
	bSizer18->Add( bReset_Channel, 0, wxALL, 5 );
	
	wxString mReset_ChannelChoices[] = { wxT("All"), wxT("01"), wxT("02"), wxT("03"), wxT("04"), wxT("05"), wxT("06"), wxT("07"), wxT("08"), wxT("09"), wxT("10"), wxT("11") };
	int mReset_ChannelNChoices = sizeof( mReset_ChannelChoices ) / sizeof( wxString );
	mReset_Channel = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mReset_ChannelNChoices, mReset_ChannelChoices, 0 );
	mReset_Channel->SetSelection( 0 );
	bSizer18->Add( mReset_Channel, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer18, 0, 0, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	bReset_Ephemeris = new wxButton( this, ID_RESET_EPHEMERIS, wxT("Reset Ephemeris"), wxDefaultPosition, wxDefaultSize, 0 );
	bReset_Ephemeris->SetMinSize( wxSize( 150,-1 ) );
	
	bSizer19->Add( bReset_Ephemeris, 0, wxALL, 5 );
	
	wxString mReset_EphemerisChoices[] = { wxT("All"), wxT("01"), wxT("02"), wxT("03"), wxT("04"), wxT("05"), wxT("06"), wxT("07"), wxT("08"), wxT("09"), wxT("10"), wxT("11"), wxT("12"), wxT("13"), wxT("14"), wxT("15"), wxT("16"), wxT("17"), wxT("18"), wxT("19"), wxT("20"), wxT("21"), wxT("22"), wxT("23"), wxT("24"), wxT("25"), wxT("26"), wxT("27"), wxT("28"), wxT("29"), wxT("30"), wxT("31"), wxT("32") };
	int mReset_EphemerisNChoices = sizeof( mReset_EphemerisChoices ) / sizeof( wxString );
	mReset_Ephemeris = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mReset_EphemerisNChoices, mReset_EphemerisChoices, 0 );
	mReset_Ephemeris->SetSelection( 0 );
	bSizer19->Add( mReset_Ephemeris, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer19, 0, 0, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	bReset_Almanac = new wxButton( this, ID_RESET_ALMANAC, wxT("Reset Almanac"), wxDefaultPosition, wxDefaultSize, 0 );
	bReset_Almanac->SetMinSize( wxSize( 150,-1 ) );
	
	bSizer20->Add( bReset_Almanac, 0, wxALL, 5 );
	
	wxString mReset_AlmanacChoices[] = { wxT("All"), wxT("01"), wxT("02"), wxT("03"), wxT("04"), wxT("05"), wxT("06"), wxT("07"), wxT("08"), wxT("09"), wxT("10"), wxT("11"), wxT("12"), wxT("13"), wxT("14"), wxT("15"), wxT("16"), wxT("17"), wxT("18"), wxT("09"), wxT("19"), wxT("20"), wxT("21"), wxT("22"), wxT("23"), wxT("24"), wxT("25"), wxT("26"), wxT("27"), wxT("28"), wxT("29"), wxT("30"), wxT("31"), wxT("32") };
	int mReset_AlmanacNChoices = sizeof( mReset_AlmanacChoices ) / sizeof( wxString );
	mReset_Almanac = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mReset_AlmanacNChoices, mReset_AlmanacChoices, 0 );
	mReset_Almanac->SetSelection( 0 );
	bSizer20->Add( mReset_Almanac, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer20, 0, 0, 5 );
	
	tCommand_Ack = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE );
	bSizer11->Add( tCommand_Ack, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer11 );
	this->Layout();
}

iGUI_Commands::~iGUI_Commands()
{
}

iGUI_Ephemeris::iGUI_Ephemeris( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	bSizer21->Add( bSizer24, 1, wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("SV#"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer21->Add( m_staticText4, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString mSVChoices[] = { wxT("01"), wxT("02"), wxT("03"), wxT("04"), wxT("05"), wxT("06"), wxT("07"), wxT("08"), wxT("09"), wxT("10"), wxT("11"), wxT("12"), wxT("13"), wxT("14"), wxT("15"), wxT("16"), wxT("17"), wxT("18"), wxT("19"), wxT("20"), wxT("21"), wxT("22"), wxT("23"), wxT("24"), wxT("25"), wxT("26"), wxT("27"), wxT("28"), wxT("29"), wxT("30"), wxT("31") };
	int mSVNChoices = sizeof( mSVChoices ) / sizeof( wxString );
	mSV = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mSVNChoices, mSVChoices, 0 );
	mSV->SetSelection( 0 );
	bSizer21->Add( mSV, 0, wxALL, 5 );
	
	bSave = new wxButton( this, ID_EPHEMERIS_SAVE, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bSave, 0, wxALL, 5 );
	
	bLoad = new wxButton( this, ID_EPHEMERIS_LOAD, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bLoad, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	bSizer21->Add( bSizer25, 1, wxEXPAND, 5 );
	
	bSizer17->Add( bSizer21, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	pDecoded = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer22->Add( pDecoded, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer22, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	tDisplay = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( tDisplay, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer23, 10, wxEXPAND, 5 );
	
	this->SetSizer( bSizer17 );
	this->Layout();
}

iGUI_Ephemeris::~iGUI_Ephemeris()
{
}

iGUI_Almanac::iGUI_Almanac( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	bSizer21->Add( bSizer24, 1, wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("SV#"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	bSizer21->Add( m_staticText4, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString mSVChoices[] = { wxT("01"), wxT("02"), wxT("03"), wxT("04"), wxT("05"), wxT("06"), wxT("07"), wxT("08"), wxT("09"), wxT("10"), wxT("11"), wxT("12"), wxT("13"), wxT("14"), wxT("15"), wxT("16"), wxT("17"), wxT("18"), wxT("19"), wxT("20"), wxT("21"), wxT("22"), wxT("23"), wxT("24"), wxT("25"), wxT("26"), wxT("27"), wxT("28"), wxT("29"), wxT("30"), wxT("31") };
	int mSVNChoices = sizeof( mSVChoices ) / sizeof( wxString );
	mSV = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mSVNChoices, mSVChoices, 0 );
	mSV->SetSelection( 0 );
	bSizer21->Add( mSV, 0, wxALL, 5 );
	
	bSave = new wxButton( this, ID_ALMANAC_SAVE, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bSave, 0, wxALL, 5 );
	
	bLoad = new wxButton( this, ID_ALMANAC_LOAD, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bLoad, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	bSizer21->Add( bSizer25, 1, wxEXPAND, 5 );
	
	bSizer17->Add( bSizer21, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	pDecoded = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer22->Add( pDecoded, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer22, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	tDisplay = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer23->Add( tDisplay, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer23, 10, wxEXPAND, 5 );
	
	this->SetSizer( bSizer17 );
	this->Layout();
}

iGUI_Almanac::~iGUI_Almanac()
{
}
