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
	
	mLogging = new wxMenu();
	wxMenuItem* mLogconfig;
	mLogconfig = new wxMenuItem( mLogging, ID_LOG_CONFIG, wxString( wxT("Config") ) , wxEmptyString, wxITEM_NORMAL );
	mLogging->Append( mLogconfig );
	
	wxMenuItem* mLogstart;
	mLogstart = new wxMenuItem( mLogging, ID_LOG_START, wxString( wxT("Start") ) , wxEmptyString, wxITEM_NORMAL );
	mLogging->Append( mLogstart );
	mLogstart->Enable( false );
	
	wxMenuItem* mLogstop;
	mLogstop = new wxMenuItem( mLogging, ID_LOG_STOP, wxString( wxT("Stop") ) , wxEmptyString, wxITEM_NORMAL );
	mLogging->Append( mLogstop );
	mLogstop->Enable( false );
	
	wxMenuItem* mLogclear;
	mLogclear = new wxMenuItem( mLogging, ID_LOG_CLEAR, wxString( wxT("Clear") ) , wxEmptyString, wxITEM_NORMAL );
	mLogging->Append( mLogclear );
	mLogclear->Enable( false );
	
	mMenuBar->Append( mLogging, wxT("Logging") );
	
	this->SetMenuBar( mMenuBar );
	
	wxBoxSizer* sMain;
	sMain = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* SubWindows;
	SubWindows = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Windows") ), wxVERTICAL );
	
	bMain = new wxToggleButton( this, ID_MAIN_B, wxT("Main"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bMain, 0, wxALL|wxEXPAND, 5 );
	
	bChannel = new wxToggleButton( this, ID_CHANNEL_B, wxT("Channel"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bChannel, 0, wxALL|wxEXPAND, 5 );
	
	bPseudo = new wxToggleButton( this, ID_PSEUDO_B, wxT("Pseudo"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bPseudo, 0, wxALL|wxEXPAND, 5 );
	
	bAcquisition = new wxToggleButton( this, ID_ACQUISITION_B, wxT("Acquisition"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bAcquisition, 0, wxALL|wxEXPAND, 5 );
	
	bSV_Select = new wxToggleButton( this, ID_SELECT_B, wxT("SV_Select"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bSV_Select, 0, wxALL|wxEXPAND, 5 );
	
	bEphemeris = new wxToggleButton( this, ID_EPHEMERIS_B, wxT("Ephemeris"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bEphemeris, 0, wxALL|wxEXPAND, 5 );
	
	bAlmanac = new wxToggleButton( this, ID_ALMANAC_B, wxT("Almanac"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bAlmanac, 0, wxALL|wxEXPAND, 5 );
	
	bCommands = new wxToggleButton( this, ID_COMMANDS_B, wxT("Commands"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bCommands, 0, wxALL|wxEXPAND, 5 );
	
	bConfig = new wxToggleButton( this, ID_CONFIG_B, wxT("Config"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bConfig, 0, wxALL|wxEXPAND, 5 );
	
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
	
	sbSizer13->SetMinSize( wxSize( -1,180 ) ); 
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 8, 4, 0, 0 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText2->Wrap( -1 );
	m_staticText2->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer1->Add( m_staticText2, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer1->Add( m_staticText3, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText4 = new wxStaticText( this, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText4->Wrap( -1 );
	m_staticText4->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer1->Add( m_staticText4, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Position (m)"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE );
	m_staticText5->Wrap( -1 );
	m_staticText5->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer1->Add( m_staticText5, 0, wxALL|wxEXPAND, 5 );
	
	px = new wxStaticText( this, wxID_ANY, wxT("posx"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	px->Wrap( -1 );
	gSizer1->Add( px, 0, wxALL|wxEXPAND, 5 );
	
	py = new wxStaticText( this, wxID_ANY, wxT("posy"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	py->Wrap( -1 );
	gSizer1->Add( py, 0, wxALL|wxEXPAND, 5 );
	
	pz = new wxStaticText( this, wxID_ANY, wxT("posz"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	pz->Wrap( -1 );
	gSizer1->Add( pz, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Vel (cm/s)"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE );
	m_staticText9->Wrap( -1 );
	m_staticText9->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer1->Add( m_staticText9, 0, wxALL|wxEXPAND, 5 );
	
	vx = new wxStaticText( this, wxID_ANY, wxT("velx"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	vx->Wrap( -1 );
	gSizer1->Add( vx, 0, wxALL|wxEXPAND, 5 );
	
	vy = new wxStaticText( this, wxID_ANY, wxT("vely"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	vy->Wrap( -1 );
	gSizer1->Add( vy, 0, wxALL|wxEXPAND, 5 );
	
	vz = new wxStaticText( this, wxID_ANY, wxT("velz"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	vz->Wrap( -1 );
	gSizer1->Add( vz, 0, wxALL|wxEXPAND, 5 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText18 = new wxStaticText( this, wxID_ANY, wxT("Lat"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText18->Wrap( -1 );
	m_staticText18->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer1->Add( m_staticText18, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText19 = new wxStaticText( this, wxID_ANY, wxT("Long"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText19->Wrap( -1 );
	m_staticText19->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer1->Add( m_staticText19, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText21 = new wxStaticText( this, wxID_ANY, wxT("Height"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText21->Wrap( -1 );
	m_staticText21->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer1->Add( m_staticText21, 0, wxALL|wxEXPAND, 5 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	lat = new wxStaticText( this, wxID_ANY, wxT("lat"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	lat->Wrap( -1 );
	gSizer1->Add( lat, 0, wxALL|wxEXPAND, 5 );
	
	lon = new wxStaticText( this, wxID_ANY, wxT("lon"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	lon->Wrap( -1 );
	gSizer1->Add( lon, 0, wxALL|wxEXPAND, 5 );
	
	alt = new wxStaticText( this, wxID_ANY, wxT("alt"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	alt->Wrap( -1 );
	gSizer1->Add( alt, 0, wxALL|wxEXPAND, 5 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText26 = new wxStaticText( this, wxID_ANY, wxT("Clock Bias"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText26->Wrap( -1 );
	m_staticText26->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer1->Add( m_staticText26, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText27 = new wxStaticText( this, wxID_ANY, wxT("Clock Rate"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText27->Wrap( -1 );
	m_staticText27->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer1->Add( m_staticText27, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText28 = new wxStaticText( this, wxID_ANY, wxT("GPS Second"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText28->Wrap( -1 );
	m_staticText28->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer1->Add( m_staticText28, 0, wxALL|wxEXPAND, 5 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cb = new wxStaticText( this, wxID_ANY, wxT("cb"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	cb->Wrap( -1 );
	gSizer1->Add( cb, 0, wxALL|wxEXPAND, 5 );
	
	cr = new wxStaticText( this, wxID_ANY, wxT("cr"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	cr->Wrap( -1 );
	gSizer1->Add( cr, 0, wxALL|wxEXPAND, 5 );
	
	gpst = new wxStaticText( this, wxID_ANY, wxT("gpst"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	gpst->Wrap( -1 );
	gSizer1->Add( gpst, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText33 = new wxStaticText( this, wxID_ANY, wxT("Time"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE );
	m_staticText33->Wrap( -1 );
	m_staticText33->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer1->Add( m_staticText33, 0, wxALL|wxEXPAND, 5 );
	
	day = new wxStaticText( this, wxID_ANY, wxT("day"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	day->Wrap( -1 );
	gSizer1->Add( day, 0, wxALL|wxEXPAND, 5 );
	
	month = new wxStaticText( this, wxID_ANY, wxT("month"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	month->Wrap( -1 );
	gSizer1->Add( month, 0, wxALL|wxEXPAND, 5 );
	
	time = new wxStaticText( this, wxID_ANY, wxT("time"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	time->Wrap( -1 );
	gSizer1->Add( time, 0, wxALL|wxEXPAND, 5 );
	
	sbSizer13->Add( gSizer1, 1, wxEXPAND, 5 );
	
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
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxHORIZONTAL );
	
	bResetAll = new wxButton( this, ID_RESET_ALL, wxT("Reset All"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	bSizer29->Add( bResetAll, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer29, 0, 0, 5 );
	
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
	
	wxString mReset_ChannelChoices[] = { wxT("All"), wxT("00"), wxT("01"), wxT("02"), wxT("03"), wxT("04"), wxT("05"), wxT("06"), wxT("07"), wxT("08"), wxT("09"), wxT("10"), wxT("11") };
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
	tCommand_Ack->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer11->Add( tCommand_Ack, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer11 );
	this->Layout();
	bSizer11->Fit( this );
}

iGUI_Commands::~iGUI_Commands()
{
}

iGUI_Ephemeris::iGUI_Ephemeris( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	bSizer22->SetMinSize( wxSize( -1,120 ) ); 
	pDecoded = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer22->Add( pDecoded, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer22, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	tDisplay = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	tDisplay->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer23->Add( tDisplay, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer23, 10, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	bSizer21->Add( bSizer24, 1, wxEXPAND, 5 );
	
	bSave = new wxButton( this, ID_EPHEMERIS_SAVE, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bSave, 0, wxALL, 5 );
	
	bLoad = new wxButton( this, ID_EPHEMERIS_LOAD, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bLoad, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	bSizer21->Add( bSizer25, 1, wxEXPAND, 5 );
	
	bSizer17->Add( bSizer21, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer17 );
	this->Layout();
	
	// Connect Events
	pDecoded->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( iGUI_Ephemeris::onMouse ), NULL, this );
}

iGUI_Ephemeris::~iGUI_Ephemeris()
{
	// Disconnect Events
	pDecoded->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( iGUI_Ephemeris::onMouse ), NULL, this );
}

iGUI_Almanac::iGUI_Almanac( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	bSizer22->SetMinSize( wxSize( -1,120 ) ); 
	pDecoded = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer22->Add( pDecoded, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer22, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	tDisplay = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	tDisplay->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer23->Add( tDisplay, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer23, 10, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	bSizer21->Add( bSizer24, 1, wxEXPAND, 5 );
	
	bSave = new wxButton( this, ID_ALMANAC_SAVE, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bSave, 0, wxALL, 5 );
	
	bLoad = new wxButton( this, ID_ALMANAC_LOAD, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bLoad, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	bSizer21->Add( bSizer25, 1, wxEXPAND, 5 );
	
	bSizer17->Add( bSizer21, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer17 );
	this->Layout();
	
	// Connect Events
	pDecoded->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( iGUI_Almanac::onMouse ), NULL, this );
}

iGUI_Almanac::~iGUI_Almanac()
{
	// Disconnect Events
	pDecoded->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( iGUI_Almanac::onMouse ), NULL, this );
}

iGUI_Acquisition::iGUI_Acquisition( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer8;
	sbSizer8 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Strong") ), wxVERTICAL );
	
	pStrong = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer8->Add( pStrong, 1, wxEXPAND, 5 );
	
	bSizer25->Add( sbSizer8, 4, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Medium") ), wxVERTICAL );
	
	pMedium = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer9->Add( pMedium, 1, wxEXPAND, 5 );
	
	bSizer25->Add( sbSizer9, 4, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Weak") ), wxVERTICAL );
	
	pWeak = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer10->Add( pWeak, 1, wxEXPAND, 5 );
	
	bSizer25->Add( sbSizer10, 4, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxVERTICAL );
	
	pSV = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer29->Add( pSV, 1, wxALL|wxEXPAND, 5 );
	
	bSizer25->Add( bSizer29, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer25 );
	this->Layout();
}

iGUI_Acquisition::~iGUI_Acquisition()
{
}

iGUI_Select::iGUI_Select( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer13;
	sbSizer13 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Doppler") ), wxVERTICAL );
	
	pDoppler = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer13->Add( pDoppler, 1, wxEXPAND, 5 );
	
	bSizer26->Add( sbSizer13, 2, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer11;
	sbSizer11 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("SV---Receiver Sky Plot") ), wxVERTICAL );
	
	pSV = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer11->Add( pSV, 1, wxEXPAND, 5 );
	
	bSizer28->Add( sbSizer11, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer12;
	sbSizer12 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Receiver---SV Sky Plot") ), wxVERTICAL );
	
	pRec = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer12->Add( pRec, 1, wxEXPAND, 5 );
	
	bSizer28->Add( sbSizer12, 1, wxALL|wxEXPAND, 5 );
	
	bSizer26->Add( bSizer28, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer26 );
	this->Layout();
}

iGUI_Select::~iGUI_Select()
{
}

iGUI_Config::iGUI_Config( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
}

iGUI_Config::~iGUI_Config()
{
}

iGUI_Pseudo::iGUI_Pseudo( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	tPseudos = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE );
	tPseudos->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer9->Add( tPseudos, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer9 );
	this->Layout();
}

iGUI_Pseudo::~iGUI_Pseudo()
{
}

iGUI_Log::iGUI_Log( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer14;
	sbSizer14 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Logged Items") ), wxVERTICAL );
	
	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 2, 2, 0, 0 );
	
	cChan = new wxCheckBox( this, wxID_ANY, wxT("Channel Status"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cChan, 0, wxALL, 5 );
	
	cClock = new wxCheckBox( this, wxID_ANY, wxT("Clock State"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cClock, 0, wxALL, 5 );
	
	cPseudo = new wxCheckBox( this, wxID_ANY, wxT("Pseudoranges"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cPseudo, 0, wxALL, 5 );
	
	cTask = new wxCheckBox( this, wxID_ANY, wxT("Task Health"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cTask, 0, wxALL, 5 );
	
	cEKF = new wxCheckBox( this, wxID_ANY, wxT("EKF State"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cEKF, 0, wxALL, 5 );
	
	cPVT = new wxCheckBox( this, wxID_ANY, wxT("PVT State"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cPVT, 0, wxALL, 5 );
	
	sbSizer14->Add( gSizer3, 0, wxEXPAND, 5 );
	
	bSizer30->Add( sbSizer14, 1, wxALL|wxEXPAND, 5 );
	
	mFile = new wxFilePickerCtrl( this, wxID_ANY, wxEmptyString, wxT("Create/Open File for Logging"), wxT("*.log"), wxDefaultPosition, wxSize( 600,-1 ), wxFLP_OVERWRITE_PROMPT|wxFLP_SAVE|wxFLP_USE_TEXTCTRL );
	bSizer30->Add( mFile, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer31->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( bCancel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );
	
	bOK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( bOK, 0, wxLEFT|wxRIGHT, 5 );
	
	bSizer30->Add( bSizer31, 0, wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer30 );
	this->Layout();
	bSizer30->Fit( this );
}

iGUI_Log::~iGUI_Log()
{
}
