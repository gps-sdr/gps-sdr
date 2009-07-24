///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
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
	wxMenuItem* mAbout;
	mAbout = new wxMenuItem( mFile, ID_ABOUT, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	mFile->Append( mAbout );
	
	wxMenuItem* mExit;
	mExit = new wxMenuItem( mFile, ID_QUIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	mFile->Append( mExit );
	
	mMenuBar->Append( mFile, wxT("File") );
	
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
	
	mInterface = new wxMenu();
	wxMenuItem* mSerial;
	mSerial = new wxMenuItem( mInterface, ID_SERIAL, wxString( wxT("RS232") ) , wxEmptyString, wxITEM_NORMAL );
	mInterface->Append( mSerial );
	
	wxMenuItem* mNpipe;
	mNpipe = new wxMenuItem( mInterface, ID_NPIPE, wxString( wxT("Named Pipe") ) , wxEmptyString, wxITEM_NORMAL );
	mInterface->Append( mNpipe );
	
	mMenuBar->Append( mInterface, wxT("Interface") );
	
	this->SetMenuBar( mMenuBar );
	
	wxBoxSizer* sMain;
	sMain = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* SubWindows;
	SubWindows = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Windows") ), wxVERTICAL );
	
	bPVT = new wxToggleButton( this, ID_PVT_B, wxT("PVT"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bPVT, 0, wxALL|wxEXPAND, 5 );
	
	bEKF = new wxToggleButton( this, ID_EKF_B, wxT("EKF"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bEKF, 0, wxALL|wxEXPAND, 5 );
	
	bChannel = new wxToggleButton( this, ID_CHANNEL_B, wxT("Channel"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bChannel, 0, wxALL|wxEXPAND, 5 );
	
	bPseudo = new wxToggleButton( this, ID_PSEUDO_B, wxT("Pseudo"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bPseudo, 0, wxALL|wxEXPAND, 5 );
	
	bEphemeris = new wxToggleButton( this, ID_EPHEMERIS_B, wxT("Ephemeris"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bEphemeris, 0, wxALL|wxEXPAND, 5 );
	
	bAlmanac = new wxToggleButton( this, ID_ALMANAC_B, wxT("Almanac"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bAlmanac, 0, wxALL|wxEXPAND, 5 );
	
	bSelect = new wxToggleButton( this, ID_SELECT_B, wxT("SV Select"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bSelect, 0, wxALL|wxEXPAND, 5 );
	
	bHealth = new wxToggleButton( this, ID_HEALTH_B, wxT("Board Health"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bHealth, 0, wxALL|wxEXPAND, 5 );
	
	bCommands = new wxToggleButton( this, ID_COMMANDS_B, wxT("Commands"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bCommands, 0, wxALL|wxEXPAND, 5 );
	
	bMessages = new wxToggleButton( this, ID_MESSAGES_B, wxT("Messages"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bMessages, 0, wxALL|wxEXPAND, 5 );
	
	bSpeedo = new wxToggleButton( this, ID_SPEEDO_B, wxT("Speedometer"), wxDefaultPosition, wxDefaultSize, 0 );
	SubWindows->Add( bSpeedo, 0, wxALL|wxEXPAND, 5 );
	
	sMain->Add( SubWindows, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* sStatus;
	sStatus = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sRS422;
	sRS422 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Connection") ), wxVERTICAL );
	
	tRS422 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_LEFT|wxTE_MULTILINE|wxTE_NO_VSCROLL );
	tRS422->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	sRS422->Add( tRS422, 1, wxALL|wxEXPAND, 5 );
	
	sStatus->Add( sRS422, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sTask;
	sTask = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Task Health") ), wxVERTICAL );
	
	tTask = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_LEFT|wxTE_MULTILINE|wxTE_NO_VSCROLL );
	tTask->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	sTask->Add( tTask, 1, wxALL|wxEXPAND, 5 );
	
	sStatus->Add( sTask, 2, wxALL|wxEXPAND, 5 );
	
	sMain->Add( sStatus, 3, wxEXPAND, 5 );
	
	this->SetSizer( sMain );
	this->Layout();
	mStatus = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
}

iGUI_Toplevel::~iGUI_Toplevel()
{
}

iGUI_PVT::iGUI_PVT( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetExtraStyle( wxFRAME_EX_METAL );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* bSizer29;
	bSizer29 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("CN0") ), wxVERTICAL );
	
	pCN0 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	pCN0->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );
	
	bSizer29->Add( pCN0, 1, wxEXPAND, 5 );
	
	bSizer9->Add( bSizer29, 4, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer40;
	sbSizer40 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("PVT Status") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer199;
	bSizer199 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText5311 = new wxStaticText( this, wxID_ANY, wxT("Ticks"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5311->Wrap( -1 );
	m_staticText5311->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer199->Add( m_staticText5311, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* bSizer633;
	bSizer633 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5343 = new wxStaticText( this, wxID_ANY, wxT("Receiver"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5343->Wrap( -1 );
	m_staticText5343->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer633->Add( m_staticText5343, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	rticks = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer633->Add( rticks, 2, wxALL|wxEXPAND, 5 );
	
	bSizer199->Add( bSizer633, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer65111;
	bSizer65111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText536111 = new wxStaticText( this, wxID_ANY, wxT("Stale"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText536111->Wrap( -1 );
	m_staticText536111->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer65111->Add( m_staticText536111, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	sticks = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer65111->Add( sticks, 2, wxALL|wxEXPAND, 5 );
	
	bSizer199->Add( bSizer65111, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6311;
	bSizer6311 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53411 = new wxStaticText( this, wxID_ANY, wxT("Converged"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53411->Wrap( -1 );
	m_staticText53411->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6311->Add( m_staticText53411, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	cticks = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer6311->Add( cticks, 2, wxALL|wxEXPAND, 5 );
	
	bSizer199->Add( bSizer6311, 1, wxEXPAND, 5 );
	
	sbSizer40->Add( bSizer199, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer200;
	bSizer200 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText5321 = new wxStaticText( this, wxID_ANY, wxT("Other"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5321->Wrap( -1 );
	m_staticText5321->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer200->Add( m_staticText5321, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	
	bSizer200->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6411;
	bSizer6411 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53511 = new wxStaticText( this, wxID_ANY, wxT("Iterations"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53511->Wrap( -1 );
	m_staticText53511->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6411->Add( m_staticText53511, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	iter = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer6411->Add( iter, 2, wxALL|wxEXPAND, 5 );
	
	bSizer200->Add( bSizer6411, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer642;
	bSizer642 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53431 = new wxStaticText( this, wxID_ANY, wxT("Nav SV"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText53431->Wrap( -1 );
	m_staticText53431->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer642->Add( m_staticText53431, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	nsv = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer642->Add( nsv, 2, wxALL|wxEXPAND, 5 );
	
	bSizer200->Add( bSizer642, 1, wxEXPAND, 5 );
	
	sbSizer40->Add( bSizer200, 1, wxEXPAND, 5 );
	
	bSizer18->Add( sbSizer40, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer13;
	sbSizer13 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("PVT") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer196;
	bSizer196 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText531 = new wxStaticText( this, wxID_ANY, wxT("Position"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText531->Wrap( -1 );
	m_staticText531->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer196->Add( m_staticText531, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* bSizer63;
	bSizer63 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText534 = new wxStaticText( this, wxID_ANY, wxT("Lat"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText534->Wrap( -1 );
	m_staticText534->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer63->Add( m_staticText534, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	lat = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer63->Add( lat, 3, wxALL, 5 );
	
	bSizer196->Add( bSizer63, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer631;
	bSizer631 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5341 = new wxStaticText( this, wxID_ANY, wxT("Long"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5341->Wrap( -1 );
	m_staticText5341->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer631->Add( m_staticText5341, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	lon = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer631->Add( lon, 3, wxALL, 5 );
	
	bSizer196->Add( bSizer631, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6511;
	bSizer6511 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53611 = new wxStaticText( this, wxID_ANY, wxT("Alt"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53611->Wrap( -1 );
	m_staticText53611->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6511->Add( m_staticText53611, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	alt = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer6511->Add( alt, 3, wxALL, 5 );
	
	bSizer196->Add( bSizer6511, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6514;
	bSizer6514 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53614 = new wxStaticText( this, wxID_ANY, wxT("GDOP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53614->Wrap( -1 );
	m_staticText53614->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6514->Add( m_staticText53614, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	gdop = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer6514->Add( gdop, 3, wxALL, 5 );
	
	bSizer196->Add( bSizer6514, 1, wxEXPAND, 5 );
	
	sbSizer13->Add( bSizer196, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer197;
	bSizer197 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText532 = new wxStaticText( this, wxID_ANY, wxT("Position (ECEF)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText532->Wrap( -1 );
	m_staticText532->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer197->Add( m_staticText532, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* bSizer64;
	bSizer64 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText535 = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText535->Wrap( -1 );
	m_staticText535->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer64->Add( m_staticText535, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	px = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer64->Add( px, 4, wxALL, 5 );
	
	bSizer197->Add( bSizer64, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer641;
	bSizer641 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5351 = new wxStaticText( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5351->Wrap( -1 );
	m_staticText5351->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer641->Add( m_staticText5351, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	py = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer641->Add( py, 4, wxALL, 5 );
	
	bSizer197->Add( bSizer641, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6512;
	bSizer6512 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53612 = new wxStaticText( this, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53612->Wrap( -1 );
	m_staticText53612->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6512->Add( m_staticText53612, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	pz = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer6512->Add( pz, 4, wxALL, 5 );
	
	bSizer197->Add( bSizer6512, 1, wxEXPAND, 5 );
	
	
	bSizer197->Add( 0, 0, 1, wxEXPAND, 5 );
	
	sbSizer13->Add( bSizer197, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer198;
	bSizer198 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText533 = new wxStaticText( this, wxID_ANY, wxT("Velocity (ECEF)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText533->Wrap( -1 );
	m_staticText533->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer198->Add( m_staticText533, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* bSizer65;
	bSizer65 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText536 = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText536->Wrap( -1 );
	m_staticText536->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer65->Add( m_staticText536, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	vx = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer65->Add( vx, 4, wxALL, 5 );
	
	bSizer198->Add( bSizer65, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer651;
	bSizer651 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5361 = new wxStaticText( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5361->Wrap( -1 );
	m_staticText5361->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer651->Add( m_staticText5361, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	vy = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer651->Add( vy, 4, wxALL, 5 );
	
	bSizer198->Add( bSizer651, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6513;
	bSizer6513 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53613 = new wxStaticText( this, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53613->Wrap( -1 );
	m_staticText53613->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6513->Add( m_staticText53613, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	vz = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer6513->Add( vz, 4, wxALL, 5 );
	
	bSizer198->Add( bSizer6513, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer632;
	bSizer632 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5342 = new wxStaticText( this, wxID_ANY, wxT("Vel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5342->Wrap( -1 );
	m_staticText5342->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer632->Add( m_staticText5342, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	speed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer632->Add( speed, 4, wxALL, 5 );
	
	bSizer198->Add( bSizer632, 1, wxEXPAND, 5 );
	
	sbSizer13->Add( bSizer198, 1, wxEXPAND, 5 );
	
	bSizer18->Add( sbSizer13, 0, wxALL|wxEXPAND|wxFIXED_MINSIZE, 5 );
	
	wxBoxSizer* bSizer110;
	bSizer110 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer37;
	sbSizer37 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Date/Time") ), wxVERTICAL );
	
	wxBoxSizer* bSizer9221;
	bSizer9221 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText57721 = new wxStaticText( this, wxID_ANY, wxT("UTC Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57721->Wrap( -1 );
	m_staticText57721->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer9221->Add( m_staticText57721, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	utct = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer9221->Add( utct, 2, wxALL, 5 );
	
	sbSizer37->Add( bSizer9221, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer92;
	bSizer92 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText577 = new wxStaticText( this, wxID_ANY, wxT("GPS Week"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText577->Wrap( -1 );
	m_staticText577->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer92->Add( m_staticText577, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	gpsw = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer92->Add( gpsw, 2, wxALL, 5 );
	
	sbSizer37->Add( bSizer92, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer922;
	bSizer922 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5772 = new wxStaticText( this, wxID_ANY, wxT("GPS Second"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5772->Wrap( -1 );
	m_staticText5772->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer922->Add( m_staticText5772, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	gpss = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer922->Add( gpss, 2, wxALL, 5 );
	
	sbSizer37->Add( bSizer922, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer921;
	bSizer921 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5771 = new wxStaticText( this, wxID_ANY, wxT("Clock Bias"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5771->Wrap( -1 );
	m_staticText5771->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer921->Add( m_staticText5771, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	cb = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer921->Add( cb, 2, wxALL, 5 );
	
	sbSizer37->Add( bSizer921, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9211;
	bSizer9211 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText57711 = new wxStaticText( this, wxID_ANY, wxT("Clock Rate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57711->Wrap( -1 );
	m_staticText57711->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer9211->Add( m_staticText57711, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	cr = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer9211->Add( cr, 2, wxALL, 5 );
	
	sbSizer37->Add( bSizer9211, 1, wxEXPAND, 5 );
	
	bSizer110->Add( sbSizer37, 3, wxALL|wxEXPAND|wxFIXED_MINSIZE, 5 );
	
	wxStaticBoxSizer* sbSizer371;
	sbSizer371 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("PPS ") ), wxVERTICAL );
	
	wxBoxSizer* bSizer923;
	bSizer923 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5773 = new wxStaticText( this, wxID_ANY, wxT("State"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5773->Wrap( -1 );
	m_staticText5773->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer923->Add( m_staticText5773, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	ppsstate = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer923->Add( ppsstate, 3, wxALL, 5 );
	
	sbSizer371->Add( bSizer923, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer92211;
	bSizer92211 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText577211 = new wxStaticText( this, wxID_ANY, wxT("Feedback"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText577211->Wrap( -1 );
	m_staticText577211->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer92211->Add( m_staticText577211, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	ppscmd = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer92211->Add( ppscmd, 3, wxALL, 5 );
	
	sbSizer371->Add( bSizer92211, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9212;
	bSizer9212 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText57712 = new wxStaticText( this, wxID_ANY, wxT("Clock Rate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57712->Wrap( -1 );
	m_staticText57712->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer9212->Add( m_staticText57712, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	ppscr = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer9212->Add( ppscr, 3, wxALL, 5 );
	
	sbSizer371->Add( bSizer9212, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer92111;
	bSizer92111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText577111 = new wxStaticText( this, wxID_ANY, wxT("Filtered Error"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText577111->Wrap( -1 );
	m_staticText577111->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer92111->Add( m_staticText577111, 2, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	ppserr = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	bSizer92111->Add( ppserr, 3, wxALL, 5 );
	
	sbSizer371->Add( bSizer92111, 1, wxEXPAND, 5 );
	
	
	sbSizer371->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer110->Add( sbSizer371, 2, wxALL|wxEXPAND, 5 );
	
	bSizer18->Add( bSizer110, 1, wxEXPAND, 5 );
	
	bSizer9->Add( bSizer18, 5, wxEXPAND, 5 );
	
	this->SetSizer( bSizer9 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( iGUI_PVT::OnClose ) );
}

iGUI_PVT::~iGUI_PVT()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( iGUI_PVT::OnClose ) );
}

iGUI_EKF::iGUI_EKF( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* bSizer29;
	bSizer29 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Residuals") ), wxVERTICAL );
	
	tResidual = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE|wxTE_NO_VSCROLL );
	tResidual->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer29->Add( tResidual, 1, wxALL|wxEXPAND, 5 );
	
	bSizer9->Add( bSizer29, 4, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer40;
	sbSizer40 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("EKF Status") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer199;
	bSizer199 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText5311 = new wxStaticText( this, wxID_ANY, wxT("Ticks"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5311->Wrap( -1 );
	m_staticText5311->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer199->Add( m_staticText5311, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* bSizer633;
	bSizer633 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5343 = new wxStaticText( this, wxID_ANY, wxT("Receiver"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5343->Wrap( -1 );
	m_staticText5343->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer633->Add( m_staticText5343, 0, wxALL, 5 );
	
	rticks = new wxStaticText( this, wxID_ANY, wxT("rticks"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	rticks->Wrap( -1 );
	bSizer633->Add( rticks, 1, wxALL|wxEXPAND, 5 );
	
	bSizer199->Add( bSizer633, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6311;
	bSizer6311 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53411 = new wxStaticText( this, wxID_ANY, wxT("State Updates"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53411->Wrap( -1 );
	m_staticText53411->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6311->Add( m_staticText53411, 0, wxALL, 5 );
	
	sticks = new wxStaticText( this, wxID_ANY, wxT("sticks"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	sticks->Wrap( -1 );
	bSizer6311->Add( sticks, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	bSizer199->Add( bSizer6311, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6411;
	bSizer6411 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53511 = new wxStaticText( this, wxID_ANY, wxT("Update Period"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53511->Wrap( -1 );
	m_staticText53511->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6411->Add( m_staticText53511, 0, wxALL, 5 );
	
	period = new wxStaticText( this, wxID_ANY, wxT("period"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	period->Wrap( -1 );
	bSizer6411->Add( period, 1, wxALL|wxEXPAND, 5 );
	
	bSizer199->Add( bSizer6411, 1, wxEXPAND, 5 );
	
	sbSizer40->Add( bSizer199, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer200;
	bSizer200 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText5321 = new wxStaticText( this, wxID_ANY, wxT("Other"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5321->Wrap( -1 );
	m_staticText5321->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer200->Add( m_staticText5321, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* bSizer642;
	bSizer642 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53431 = new wxStaticText( this, wxID_ANY, wxT("Status"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53431->Wrap( -1 );
	m_staticText53431->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer642->Add( m_staticText53431, 0, wxALL, 5 );
	
	status = new wxStaticText( this, wxID_ANY, wxT("status"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	status->Wrap( -1 );
	bSizer642->Add( status, 1, wxALL|wxEXPAND, 5 );
	
	bSizer200->Add( bSizer642, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6421;
	bSizer6421 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText534311 = new wxStaticText( this, wxID_ANY, wxT("Errors"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText534311->Wrap( -1 );
	m_staticText534311->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6421->Add( m_staticText534311, 0, wxALL, 5 );
	
	err = new wxStaticText( this, wxID_ANY, wxT("err"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	err->Wrap( -1 );
	bSizer6421->Add( err, 1, wxALL|wxEXPAND, 5 );
	
	bSizer200->Add( bSizer6421, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6422;
	bSizer6422 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText534312 = new wxStaticText( this, wxID_ANY, wxT("Nav Satellites"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText534312->Wrap( -1 );
	m_staticText534312->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6422->Add( m_staticText534312, 0, wxALL, 5 );
	
	nsv = new wxStaticText( this, wxID_ANY, wxT("nsv"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	nsv->Wrap( -1 );
	bSizer6422->Add( nsv, 1, wxALL|wxEXPAND, 5 );
	
	bSizer200->Add( bSizer6422, 1, wxEXPAND, 5 );
	
	sbSizer40->Add( bSizer200, 1, wxEXPAND, 5 );
	
	bSizer18->Add( sbSizer40, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer13;
	sbSizer13 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("EKF State") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer196;
	bSizer196 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText531 = new wxStaticText( this, wxID_ANY, wxT("Other"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText531->Wrap( -1 );
	m_staticText531->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer196->Add( m_staticText531, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* bSizer65111;
	bSizer65111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText536111 = new wxStaticText( this, wxID_ANY, wxT("SRP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText536111->Wrap( -1 );
	m_staticText536111->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer65111->Add( m_staticText536111, 0, wxALL, 5 );
	
	srp = new wxStaticText( this, wxID_ANY, wxT("srp"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	srp->Wrap( -1 );
	bSizer65111->Add( srp, 1, wxALL|wxEXPAND, 5 );
	
	bSizer196->Add( bSizer65111, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6511;
	bSizer6511 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53611 = new wxStaticText( this, wxID_ANY, wxT("Drag"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53611->Wrap( -1 );
	m_staticText53611->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6511->Add( m_staticText53611, 0, wxALL, 5 );
	
	drag = new wxStaticText( this, wxID_ANY, wxT("drag"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	drag->Wrap( -1 );
	bSizer6511->Add( drag, 1, wxALL|wxEXPAND, 5 );
	
	bSizer196->Add( bSizer6511, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9212;
	bSizer9212 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText57712 = new wxStaticText( this, wxID_ANY, wxT("CB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57712->Wrap( -1 );
	m_staticText57712->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer9212->Add( m_staticText57712, 0, wxALL, 5 );
	
	cb = new wxStaticText( this, wxID_ANY, wxT("cb"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	cb->Wrap( -1 );
	bSizer9212->Add( cb, 1, wxALL|wxEXPAND, 5 );
	
	bSizer196->Add( bSizer9212, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9213;
	bSizer9213 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText57713 = new wxStaticText( this, wxID_ANY, wxT("CR"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57713->Wrap( -1 );
	m_staticText57713->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer9213->Add( m_staticText57713, 0, wxALL, 5 );
	
	cr = new wxStaticText( this, wxID_ANY, wxT("cr"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	cr->Wrap( -1 );
	bSizer9213->Add( cr, 1, wxALL|wxEXPAND, 5 );
	
	bSizer196->Add( bSizer9213, 1, wxEXPAND, 5 );
	
	sbSizer13->Add( bSizer196, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer197;
	bSizer197 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText532 = new wxStaticText( this, wxID_ANY, wxT("Position (ECEF)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText532->Wrap( -1 );
	m_staticText532->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer197->Add( m_staticText532, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* bSizer64;
	bSizer64 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText535 = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText535->Wrap( -1 );
	m_staticText535->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer64->Add( m_staticText535, 0, wxALL, 5 );
	
	px = new wxStaticText( this, wxID_ANY, wxT("px"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	px->Wrap( -1 );
	bSizer64->Add( px, 1, wxALL|wxEXPAND, 5 );
	
	bSizer197->Add( bSizer64, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer641;
	bSizer641 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5351 = new wxStaticText( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5351->Wrap( -1 );
	m_staticText5351->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer641->Add( m_staticText5351, 0, wxALL, 5 );
	
	py = new wxStaticText( this, wxID_ANY, wxT("py"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	py->Wrap( -1 );
	bSizer641->Add( py, 1, wxALL|wxEXPAND, 5 );
	
	bSizer197->Add( bSizer641, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6512;
	bSizer6512 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53612 = new wxStaticText( this, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53612->Wrap( -1 );
	m_staticText53612->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6512->Add( m_staticText53612, 0, wxALL, 5 );
	
	pz = new wxStaticText( this, wxID_ANY, wxT("pz"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	pz->Wrap( -1 );
	bSizer6512->Add( pz, 1, wxALL|wxEXPAND, 5 );
	
	bSizer197->Add( bSizer6512, 1, wxEXPAND, 5 );
	
	
	bSizer197->Add( 0, 0, 1, wxEXPAND, 5 );
	
	sbSizer13->Add( bSizer197, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer198;
	bSizer198 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText533 = new wxStaticText( this, wxID_ANY, wxT("Velocity (ECEF)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText533->Wrap( -1 );
	m_staticText533->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer198->Add( m_staticText533, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* bSizer65;
	bSizer65 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText536 = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText536->Wrap( -1 );
	m_staticText536->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer65->Add( m_staticText536, 0, wxALL, 5 );
	
	vx = new wxStaticText( this, wxID_ANY, wxT("vx"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	vx->Wrap( -1 );
	bSizer65->Add( vx, 1, wxEXPAND|wxALL, 5 );
	
	bSizer198->Add( bSizer65, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer651;
	bSizer651 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5361 = new wxStaticText( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5361->Wrap( -1 );
	m_staticText5361->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer651->Add( m_staticText5361, 0, wxALL, 5 );
	
	vy = new wxStaticText( this, wxID_ANY, wxT("vy"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT|wxSIMPLE_BORDER );
	vy->Wrap( -1 );
	bSizer651->Add( vy, 1, wxALL|wxEXPAND, 5 );
	
	bSizer198->Add( bSizer651, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6513;
	bSizer6513 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53613 = new wxStaticText( this, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53613->Wrap( -1 );
	m_staticText53613->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6513->Add( m_staticText53613, 0, wxALL, 5 );
	
	vz = new wxStaticText( this, wxID_ANY, wxT("vz"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	vz->Wrap( -1 );
	bSizer6513->Add( vz, 1, wxALL|wxEXPAND, 5 );
	
	bSizer198->Add( bSizer6513, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer632;
	bSizer632 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5342 = new wxStaticText( this, wxID_ANY, wxT("Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5342->Wrap( -1 );
	m_staticText5342->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer632->Add( m_staticText5342, 0, wxALL, 5 );
	
	speed = new wxStaticText( this, wxID_ANY, wxT("speed"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	speed->Wrap( -1 );
	bSizer632->Add( speed, 1, wxALL|wxEXPAND, 5 );
	
	bSizer198->Add( bSizer632, 1, wxEXPAND, 5 );
	
	sbSizer13->Add( bSizer198, 1, wxEXPAND, 5 );
	
	bSizer18->Add( sbSizer13, 0, wxALL|wxEXPAND|wxFIXED_MINSIZE, 5 );
	
	wxStaticBoxSizer* sbSizer131;
	sbSizer131 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("EKF Covariance") ), wxHORIZONTAL );
	
	wxBoxSizer* bSizer1961;
	bSizer1961 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText5312 = new wxStaticText( this, wxID_ANY, wxT("Other"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5312->Wrap( -1 );
	m_staticText5312->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer1961->Add( m_staticText5312, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* bSizer651111;
	bSizer651111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5361111 = new wxStaticText( this, wxID_ANY, wxT("SRP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5361111->Wrap( -1 );
	m_staticText5361111->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer651111->Add( m_staticText5361111, 0, wxALL, 5 );
	
	csrp = new wxStaticText( this, wxID_ANY, wxT("csrp"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	csrp->Wrap( -1 );
	bSizer651111->Add( csrp, 1, wxALL|wxEXPAND, 5 );
	
	bSizer1961->Add( bSizer651111, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer65112;
	bSizer65112 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText536112 = new wxStaticText( this, wxID_ANY, wxT("Drag"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText536112->Wrap( -1 );
	m_staticText536112->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer65112->Add( m_staticText536112, 0, wxALL, 5 );
	
	cdrag = new wxStaticText( this, wxID_ANY, wxT("cdrag"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	cdrag->Wrap( -1 );
	bSizer65112->Add( cdrag, 1, wxALL|wxEXPAND, 5 );
	
	bSizer1961->Add( bSizer65112, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer651121;
	bSizer651121 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5361121 = new wxStaticText( this, wxID_ANY, wxT("CB"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5361121->Wrap( -1 );
	m_staticText5361121->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer651121->Add( m_staticText5361121, 0, wxALL, 5 );
	
	ccb = new wxStaticText( this, wxID_ANY, wxT("ccb"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	ccb->Wrap( -1 );
	bSizer651121->Add( ccb, 1, wxALL|wxEXPAND, 5 );
	
	bSizer1961->Add( bSizer651121, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer651122;
	bSizer651122 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5361122 = new wxStaticText( this, wxID_ANY, wxT("CR"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5361122->Wrap( -1 );
	m_staticText5361122->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer651122->Add( m_staticText5361122, 0, wxALL, 5 );
	
	ccr = new wxStaticText( this, wxID_ANY, wxT("ccr"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	ccr->Wrap( -1 );
	bSizer651122->Add( ccr, 1, wxALL|wxEXPAND, 5 );
	
	bSizer1961->Add( bSizer651122, 1, wxEXPAND, 5 );
	
	sbSizer131->Add( bSizer1961, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1971;
	bSizer1971 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText5322 = new wxStaticText( this, wxID_ANY, wxT("Position (ECEF)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5322->Wrap( -1 );
	m_staticText5322->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer1971->Add( m_staticText5322, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* bSizer643;
	bSizer643 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5352 = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5352->Wrap( -1 );
	m_staticText5352->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer643->Add( m_staticText5352, 0, wxALL, 5 );
	
	cpx = new wxStaticText( this, wxID_ANY, wxT("cpx"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	cpx->Wrap( -1 );
	bSizer643->Add( cpx, 1, wxALL|wxEXPAND, 5 );
	
	bSizer1971->Add( bSizer643, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6412;
	bSizer6412 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53512 = new wxStaticText( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53512->Wrap( -1 );
	m_staticText53512->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6412->Add( m_staticText53512, 0, wxALL, 5 );
	
	cpy = new wxStaticText( this, wxID_ANY, wxT("cpy"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	cpy->Wrap( -1 );
	bSizer6412->Add( cpy, 1, wxALL|wxEXPAND, 5 );
	
	bSizer1971->Add( bSizer6412, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer65121;
	bSizer65121 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText536121 = new wxStaticText( this, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText536121->Wrap( -1 );
	m_staticText536121->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer65121->Add( m_staticText536121, 0, wxALL, 5 );
	
	cpz = new wxStaticText( this, wxID_ANY, wxT("cpz"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	cpz->Wrap( -1 );
	bSizer65121->Add( cpz, 1, wxALL|wxEXPAND, 5 );
	
	bSizer1971->Add( bSizer65121, 1, wxEXPAND, 5 );
	
	
	bSizer1971->Add( 0, 0, 1, wxEXPAND, 5 );
	
	sbSizer131->Add( bSizer1971, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1981;
	bSizer1981 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText5331 = new wxStaticText( this, wxID_ANY, wxT("Velocity (ECEF)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5331->Wrap( -1 );
	m_staticText5331->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 94, 92, true, wxEmptyString ) );
	
	bSizer1981->Add( m_staticText5331, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* bSizer652;
	bSizer652 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5362 = new wxStaticText( this, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5362->Wrap( -1 );
	m_staticText5362->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer652->Add( m_staticText5362, 0, wxALL, 5 );
	
	cvx = new wxStaticText( this, wxID_ANY, wxT("cvx"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	cvx->Wrap( -1 );
	bSizer652->Add( cvx, 1, wxEXPAND|wxALL, 5 );
	
	bSizer1981->Add( bSizer652, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6514;
	bSizer6514 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText53614 = new wxStaticText( this, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53614->Wrap( -1 );
	m_staticText53614->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer6514->Add( m_staticText53614, 0, wxALL, 5 );
	
	cvy = new wxStaticText( this, wxID_ANY, wxT("cvy"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE|wxSIMPLE_BORDER );
	cvy->Wrap( -1 );
	bSizer6514->Add( cvy, 1, wxALL|wxEXPAND, 5 );
	
	bSizer1981->Add( bSizer6514, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer65131;
	bSizer65131 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText536131 = new wxStaticText( this, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText536131->Wrap( -1 );
	m_staticText536131->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer65131->Add( m_staticText536131, 0, wxALL, 5 );
	
	cvz = new wxStaticText( this, wxID_ANY, wxT("cvz"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	cvz->Wrap( -1 );
	bSizer65131->Add( cvz, 1, wxALL|wxEXPAND, 5 );
	
	bSizer1981->Add( bSizer65131, 1, wxEXPAND, 5 );
	
	
	bSizer1981->Add( 0, 0, 1, wxEXPAND, 5 );
	
	sbSizer131->Add( bSizer1981, 1, wxEXPAND, 5 );
	
	bSizer18->Add( sbSizer131, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer37;
	sbSizer37 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("EKF Date/Time") ), wxVERTICAL );
	
	wxBoxSizer* bSizer9221;
	bSizer9221 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText57721 = new wxStaticText( this, wxID_ANY, wxT("UTC Time"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57721->Wrap( -1 );
	m_staticText57721->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer9221->Add( m_staticText57721, 0, wxALL, 5 );
	
	utct = new wxStaticText( this, wxID_ANY, wxT("utct"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	utct->Wrap( -1 );
	bSizer9221->Add( utct, 1, wxALL|wxEXPAND, 5 );
	
	sbSizer37->Add( bSizer9221, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer92;
	bSizer92 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText577 = new wxStaticText( this, wxID_ANY, wxT("GPS Week"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText577->Wrap( -1 );
	m_staticText577->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer92->Add( m_staticText577, 0, wxALL, 5 );
	
	gpsw = new wxStaticText( this, wxID_ANY, wxT("gpsw"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	gpsw->Wrap( -1 );
	bSizer92->Add( gpsw, 1, wxALL|wxEXPAND, 5 );
	
	sbSizer37->Add( bSizer92, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer922;
	bSizer922 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5772 = new wxStaticText( this, wxID_ANY, wxT("GPS Second"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5772->Wrap( -1 );
	m_staticText5772->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	bSizer922->Add( m_staticText5772, 0, wxALL, 5 );
	
	gpss = new wxStaticText( this, wxID_ANY, wxT("gpss"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	gpss->Wrap( -1 );
	bSizer922->Add( gpss, 1, wxALL|wxEXPAND, 5 );
	
	sbSizer37->Add( bSizer922, 1, wxEXPAND, 5 );
	
	bSizer18->Add( sbSizer37, 0, wxALL|wxEXPAND|wxFIXED_MINSIZE, 5 );
	
	bSizer9->Add( bSizer18, 5, wxEXPAND, 5 );
	
	this->SetSizer( bSizer9 );
	this->Layout();
}

iGUI_EKF::~iGUI_EKF()
{
}

iGUI_Channel::iGUI_Channel( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	tChannel = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_LEFT|wxTE_MULTILINE );
	tChannel->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer9->Add( tChannel, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer9 );
	this->Layout();
}

iGUI_Channel::~iGUI_Channel()
{
}

iGUI_Pseudo::iGUI_Pseudo( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	tPseudo = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_LEFT|wxTE_MULTILINE );
	tPseudo->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer9->Add( tPseudo, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer9 );
	this->Layout();
}

iGUI_Pseudo::~iGUI_Pseudo()
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
	pDecoded->SetMinSize( wxSize( -1,120 ) );
	
	bSizer22->Add( pDecoded, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer22, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	tDisplay = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
	tDisplay->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer23->Add( tDisplay, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer23, 10, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer21->SetMinSize( wxSize( -1,30 ) ); 
	
	bSizer21->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bRefresh = new wxButton( this, ID_EPHEMERIS_REFRESH, wxT("Refresh"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bRefresh, 0, wxALL, 5 );
	
	bSave = new wxButton( this, ID_EPHEMERIS_SAVE, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bSave, 0, wxALL, 5 );
	
	bLoad = new wxButton( this, ID_EPHEMERIS_LOAD, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bLoad, 0, wxALL, 5 );
	
	
	bSizer21->Add( 0, 0, 1, wxEXPAND, 5 );
	
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
	pDecoded->SetMinSize( wxSize( -1,120 ) );
	
	bSizer22->Add( pDecoded, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer22, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	tDisplay = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
	tDisplay->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer23->Add( tDisplay, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer23, 10, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer21->SetMinSize( wxSize( -1,30 ) ); 
	
	bSizer21->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bRefresh = new wxButton( this, ID_ALMANAC_REFRESH, wxT("Refresh"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bRefresh, 0, wxALL, 5 );
	
	bSave = new wxButton( this, ID_ALMANAC_SAVE, wxT("Save"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bSave, 0, wxALL, 5 );
	
	bLoad = new wxButton( this, ID_ALMANAC_LOAD, wxT("Load"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bLoad, 0, wxALL, 5 );
	
	
	bSizer21->Add( 0, 0, 1, wxEXPAND, 5 );
	
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

iGUI_Health::iGUI_Health( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_APPWORKSPACE ) );
	
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer32;
	sbSizer32 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("FPGA Versions") ), wxVERTICAL );
	
	wxGridSizer* gSizer14;
	gSizer14 = new wxGridSizer( 2, 5, 0, 0 );
	
	
	gSizer14->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText533 = new wxStaticText( this, wxID_ANY, wxT("TRK"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText533->Wrap( -1 );
	m_staticText533->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer14->Add( m_staticText533, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText534 = new wxStaticText( this, wxID_ANY, wxT("ACQ"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText534->Wrap( -1 );
	m_staticText534->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer14->Add( m_staticText534, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText535 = new wxStaticText( this, wxID_ANY, wxT("FFT"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText535->Wrap( -1 );
	m_staticText535->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer14->Add( m_staticText535, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText536 = new wxStaticText( this, wxID_ANY, wxT("Software"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText536->Wrap( -1 );
	m_staticText536->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer14->Add( m_staticText536, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText537 = new wxStaticText( this, wxID_ANY, wxT("Version"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText537->Wrap( -1 );
	m_staticText537->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer14->Add( m_staticText537, 0, wxALL, 5 );
	
	trkv = new wxStaticText( this, wxID_ANY, wxT("trkv"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	trkv->Wrap( -1 );
	trkv->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer14->Add( trkv, 0, wxALL|wxEXPAND, 5 );
	
	acqv = new wxStaticText( this, wxID_ANY, wxT("acqv"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	acqv->Wrap( -1 );
	acqv->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer14->Add( acqv, 0, wxALL|wxEXPAND, 5 );
	
	fftv = new wxStaticText( this, wxID_ANY, wxT("fftv"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	fftv->Wrap( -1 );
	fftv->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer14->Add( fftv, 0, wxALL|wxEXPAND, 5 );
	
	sftv = new wxStaticText( this, wxID_ANY, wxT("sftv"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	sftv->Wrap( -1 );
	sftv->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer14->Add( sftv, 0, wxALL|wxEXPAND, 5 );
	
	sbSizer32->Add( gSizer14, 1, wxEXPAND, 5 );
	
	bSizer61->Add( sbSizer32, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer35;
	sbSizer35 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Gain Control") ), wxVERTICAL );
	
	wxGridSizer* gSizer15;
	gSizer15 = new wxGridSizer( 3, 5, 0, 0 );
	
	
	gSizer15->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText53 = new wxStaticText( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText53->Wrap( -1 );
	m_staticText53->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer15->Add( m_staticText53, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText56 = new wxStaticText( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText56->Wrap( -1 );
	m_staticText56->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer15->Add( m_staticText56, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText54 = new wxStaticText( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText54->Wrap( -1 );
	m_staticText54->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer15->Add( m_staticText54, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText55 = new wxStaticText( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText55->Wrap( -1 );
	m_staticText55->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer15->Add( m_staticText55, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("DSA Values"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE );
	m_staticText5->Wrap( -1 );
	m_staticText5->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer15->Add( m_staticText5, 0, wxALL|wxEXPAND, 5 );
	
	dsa0 = new wxStaticText( this, wxID_ANY, wxT("dsa0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	dsa0->Wrap( -1 );
	dsa0->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer15->Add( dsa0, 0, wxALL|wxEXPAND, 5 );
	
	dsa1 = new wxStaticText( this, wxID_ANY, wxT("dsa1"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	dsa1->Wrap( -1 );
	dsa1->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer15->Add( dsa1, 0, wxALL|wxEXPAND, 5 );
	
	dsa2 = new wxStaticText( this, wxID_ANY, wxT("dsa2"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	dsa2->Wrap( -1 );
	dsa2->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer15->Add( dsa2, 0, wxALL|wxEXPAND, 5 );
	
	dsa3 = new wxStaticText( this, wxID_ANY, wxT("dsa3"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	dsa3->Wrap( -1 );
	dsa3->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer15->Add( dsa3, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText52 = new wxStaticText( this, wxID_ANY, wxT("Overflow Count"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE );
	m_staticText52->Wrap( -1 );
	m_staticText52->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer15->Add( m_staticText52, 0, wxALL|wxEXPAND, 5 );
	
	ovr0 = new wxStaticText( this, wxID_ANY, wxT("ovr0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	ovr0->Wrap( -1 );
	ovr0->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer15->Add( ovr0, 0, wxALL|wxEXPAND, 5 );
	
	ovr1 = new wxStaticText( this, wxID_ANY, wxT("ovr1"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	ovr1->Wrap( -1 );
	ovr1->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer15->Add( ovr1, 0, wxALL|wxEXPAND, 5 );
	
	ovr2 = new wxStaticText( this, wxID_ANY, wxT("ovr2"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	ovr2->Wrap( -1 );
	ovr2->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer15->Add( ovr2, 0, wxALL|wxEXPAND, 5 );
	
	ovr3 = new wxStaticText( this, wxID_ANY, wxT("ovr3"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	ovr3->Wrap( -1 );
	ovr3->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer15->Add( ovr3, 0, wxALL|wxEXPAND, 5 );
	
	sbSizer35->Add( gSizer15, 0, wxEXPAND, 5 );
	
	bSizer61->Add( sbSizer35, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer36;
	sbSizer36 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Status and Health") ), wxVERTICAL );
	
	wxGridSizer* gSizer151;
	gSizer151 = new wxGridSizer( 8, 5, 0, 0 );
	
	
	gSizer151->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText53141 = new wxStaticText( this, wxID_ANY, wxT("NSP TR"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText53141->Wrap( -1 );
	m_staticText53141->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText53141, 0, wxALL|wxEXPAND, 5 );
	
	nsptr = new wxStaticText( this, wxID_ANY, wxT("nsptr"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	nsptr->Wrap( -1 );
	nsptr->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( nsptr, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText531421 = new wxStaticText( this, wxID_ANY, wxT("PCC TR"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText531421->Wrap( -1 );
	m_staticText531421->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText531421, 0, wxALL|wxEXPAND, 5 );
	
	pcctr = new wxStaticText( this, wxID_ANY, wxT("pcctr"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	pcctr->Wrap( -1 );
	pcctr->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( pcctr, 0, wxALL|wxEXPAND, 5 );
	
	
	gSizer151->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText53142 = new wxStaticText( this, wxID_ANY, wxT("RF TR"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText53142->Wrap( -1 );
	m_staticText53142->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText53142, 0, wxALL|wxEXPAND, 5 );
	
	rftr = new wxStaticText( this, wxID_ANY, wxT("rftr"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	rftr->Wrap( -1 );
	rftr->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( rftr, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText561 = new wxStaticText( this, wxID_ANY, wxT("VREF"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText561->Wrap( -1 );
	m_staticText561->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText561, 0, wxALL|wxEXPAND, 5 );
	
	vref = new wxStaticText( this, wxID_ANY, wxT("vref"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	vref->Wrap( -1 );
	vref->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( vref, 0, wxALL|wxEXPAND, 5 );
	
	
	gSizer151->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText531 = new wxStaticText( this, wxID_ANY, wxT("GND1"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText531->Wrap( -1 );
	m_staticText531->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText531, 0, wxALL|wxEXPAND, 5 );
	
	gnd1 = new wxStaticText( this, wxID_ANY, wxT("gnd1"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	gnd1->Wrap( -1 );
	gnd1->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( gnd1, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText541 = new wxStaticText( this, wxID_ANY, wxT("GND2"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText541->Wrap( -1 );
	m_staticText541->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText541, 0, wxALL|wxEXPAND, 5 );
	
	gnd2 = new wxStaticText( this, wxID_ANY, wxT("gnd2"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	gnd2->Wrap( -1 );
	gnd2->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( gnd2, 0, wxALL|wxEXPAND, 5 );
	
	
	gSizer151->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText551 = new wxStaticText( this, wxID_ANY, wxT("+5V"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText551->Wrap( -1 );
	m_staticText551->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText551, 0, wxALL|wxEXPAND, 5 );
	
	p5 = new wxStaticText( this, wxID_ANY, wxT("p5"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	p5->Wrap( -1 );
	p5->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( p5, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5311 = new wxStaticText( this, wxID_ANY, wxT("+15V"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText5311->Wrap( -1 );
	m_staticText5311->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText5311, 0, wxALL|wxEXPAND, 5 );
	
	p15 = new wxStaticText( this, wxID_ANY, wxT("p15"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	p15->Wrap( -1 );
	p15->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( p15, 0, wxALL|wxEXPAND, 5 );
	
	
	gSizer151->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText5312 = new wxStaticText( this, wxID_ANY, wxT("-15V"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText5312->Wrap( -1 );
	m_staticText5312->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText5312, 0, wxALL|wxEXPAND, 5 );
	
	n15 = new wxStaticText( this, wxID_ANY, wxT("n15"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	n15->Wrap( -1 );
	n15->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( n15, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5313 = new wxStaticText( this, wxID_ANY, wxT("+2.5V"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText5313->Wrap( -1 );
	m_staticText5313->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText5313, 0, wxALL|wxEXPAND, 5 );
	
	p25 = new wxStaticText( this, wxID_ANY, wxT("p25"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	p25->Wrap( -1 );
	p25->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( p25, 0, wxALL|wxEXPAND, 5 );
	
	
	gSizer151->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText5314 = new wxStaticText( this, wxID_ANY, wxT("+3.3V"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText5314->Wrap( -1 );
	m_staticText5314->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText5314, 0, wxALL|wxEXPAND, 5 );
	
	p33 = new wxStaticText( this, wxID_ANY, wxT("p33"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	p33->Wrap( -1 );
	p33->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( p33, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText531422 = new wxStaticText( this, wxID_ANY, wxT("+1.5V"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText531422->Wrap( -1 );
	m_staticText531422->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText531422, 0, wxALL|wxEXPAND, 5 );
	
	p1p5 = new wxStaticText( this, wxID_ANY, wxT("p1p5"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	p1p5->Wrap( -1 );
	p1p5->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( p1p5, 0, wxALL|wxEXPAND, 5 );
	
	
	gSizer151->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText5314221 = new wxStaticText( this, wxID_ANY, wxT("RF0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText5314221->Wrap( -1 );
	m_staticText5314221->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText5314221, 0, wxALL|wxEXPAND, 5 );
	
	rf0 = new wxStaticText( this, wxID_ANY, wxT("rf0"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	rf0->Wrap( -1 );
	rf0->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( rf0, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5314222 = new wxStaticText( this, wxID_ANY, wxT("RF1"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText5314222->Wrap( -1 );
	m_staticText5314222->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText5314222, 0, wxALL|wxEXPAND, 5 );
	
	rf1 = new wxStaticText( this, wxID_ANY, wxT("rf1"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	rf1->Wrap( -1 );
	rf1->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( rf1, 0, wxALL|wxEXPAND, 5 );
	
	
	gSizer151->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText53142221 = new wxStaticText( this, wxID_ANY, wxT("RF2"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText53142221->Wrap( -1 );
	m_staticText53142221->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText53142221, 0, wxALL|wxEXPAND, 5 );
	
	rf2 = new wxStaticText( this, wxID_ANY, wxT("rf2"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	rf2->Wrap( -1 );
	rf2->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( rf2, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText53142222 = new wxStaticText( this, wxID_ANY, wxT("RF3"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_staticText53142222->Wrap( -1 );
	m_staticText53142222->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	gSizer151->Add( m_staticText53142222, 0, wxALL|wxEXPAND, 5 );
	
	rf3 = new wxStaticText( this, wxID_ANY, wxT("rf3"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	rf3->Wrap( -1 );
	rf3->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	
	gSizer151->Add( rf3, 0, wxALL|wxEXPAND, 5 );
	
	sbSizer36->Add( gSizer151, 0, wxEXPAND, 5 );
	
	bSizer61->Add( sbSizer36, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer61 );
	this->Layout();
	bSizer61->Fit( this );
}

iGUI_Health::~iGUI_Health()
{
}

iGUI_Commands::iGUI_Commands( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer291;
	bSizer291 = new wxBoxSizer( wxHORIZONTAL );
	
	bNull = new wxButton( this, ID_NULL, wxT("Null"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	bSizer291->Add( bNull, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer291, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer293;
	bSizer293 = new wxBoxSizer( wxHORIZONTAL );
	
	bResetWatchdog = new wxButton( this, ID_RESET_WATCHDOG, wxT("Reset Watchdog"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	bSizer293->Add( bResetWatchdog, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer293, 0, 0, 5 );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxHORIZONTAL );
	
	bResetAll = new wxButton( this, ID_RESET_ALL, wxT("Reset All"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	bSizer29->Add( bResetAll, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer29, 0, 0, 5 );
	
	wxBoxSizer* bSizer292;
	bSizer292 = new wxBoxSizer( wxHORIZONTAL );
	
	bResetPPS = new wxButton( this, ID_RESET_PPS, wxT("Reset PPS"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	bSizer292->Add( bResetPPS, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer292, 0, 0, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	bReset_PVT = new wxButton( this, ID_RESET_PVT, wxT("Reset PVT"), wxDefaultPosition, wxDefaultSize, 0 );
	bReset_PVT->SetMinSize( wxSize( 150,-1 ) );
	
	bSizer17->Add( bReset_PVT, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer17, 0, 0, 5 );
	
	wxBoxSizer* bSizer2921;
	bSizer2921 = new wxBoxSizer( wxHORIZONTAL );
	
	bResetEKF = new wxButton( this, ID_RESET_EKF, wxT("Reset EKF"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	bSizer2921->Add( bResetEKF, 0, wxALL, 5 );
	
	bSizer11->Add( bSizer2921, 0, 0, 5 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	bReset_Channel = new wxButton( this, ID_RESET_CHANNEL, wxT("Reset Channel"), wxDefaultPosition, wxDefaultSize, 0 );
	bReset_Channel->SetMinSize( wxSize( 150,-1 ) );
	
	bSizer18->Add( bReset_Channel, 0, wxALL, 5 );
	
	wxString mReset_ChannelChoices[] = { wxT("All"), wxT("00"), wxT("01"), wxT("02"), wxT("03"), wxT("04"), wxT("05"), wxT("06"), wxT("07"), wxT("08"), wxT("09"), wxT("10"), wxT("11") };
	int mReset_ChannelNChoices = sizeof( mReset_ChannelChoices ) / sizeof( wxString );
	mReset_Channel = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mReset_ChannelNChoices, mReset_ChannelChoices, 0 );
	mReset_Channel->SetSelection( 0 );
	bSizer18->Add( mReset_Channel, 1, wxALL|wxEXPAND, 5 );
	
	bSizer11->Add( bSizer18, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	bReset_Ephemeris = new wxButton( this, ID_RESET_EPHEMERIS, wxT("Reset Ephemeris"), wxDefaultPosition, wxDefaultSize, 0 );
	bReset_Ephemeris->SetMinSize( wxSize( 150,-1 ) );
	
	bSizer19->Add( bReset_Ephemeris, 0, wxALL, 5 );
	
	wxString mReset_EphemerisChoices[] = { wxT("All"), wxT("01"), wxT("02"), wxT("03"), wxT("04"), wxT("05"), wxT("06"), wxT("07"), wxT("08"), wxT("09"), wxT("10"), wxT("11"), wxT("12"), wxT("13"), wxT("14"), wxT("15"), wxT("16"), wxT("17"), wxT("18"), wxT("19"), wxT("20"), wxT("21"), wxT("22"), wxT("23"), wxT("24"), wxT("25"), wxT("26"), wxT("27"), wxT("28"), wxT("29"), wxT("30"), wxT("31"), wxT("32") };
	int mReset_EphemerisNChoices = sizeof( mReset_EphemerisChoices ) / sizeof( wxString );
	mReset_Ephemeris = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mReset_EphemerisNChoices, mReset_EphemerisChoices, 0 );
	mReset_Ephemeris->SetSelection( 0 );
	bSizer19->Add( mReset_Ephemeris, 1, wxALL|wxEXPAND, 5 );
	
	bSizer11->Add( bSizer19, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	bReset_Almanac = new wxButton( this, ID_RESET_ALMANAC, wxT("Reset Almanac"), wxDefaultPosition, wxDefaultSize, 0 );
	bReset_Almanac->SetMinSize( wxSize( 150,-1 ) );
	
	bSizer20->Add( bReset_Almanac, 0, wxALL, 5 );
	
	wxString mReset_AlmanacChoices[] = { wxT("All"), wxT("01"), wxT("02"), wxT("03"), wxT("04"), wxT("05"), wxT("06"), wxT("07"), wxT("08"), wxT("09"), wxT("10"), wxT("11"), wxT("12"), wxT("13"), wxT("14"), wxT("15"), wxT("16"), wxT("17"), wxT("18"), wxT("19"), wxT("20"), wxT("21"), wxT("22"), wxT("23"), wxT("24"), wxT("25"), wxT("26"), wxT("27"), wxT("28"), wxT("29"), wxT("30"), wxT("31"), wxT("32") };
	int mReset_AlmanacNChoices = sizeof( mReset_AlmanacChoices ) / sizeof( wxString );
	mReset_Almanac = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mReset_AlmanacNChoices, mReset_AlmanacChoices, 0 );
	mReset_Almanac->SetSelection( 0 );
	bSizer20->Add( mReset_Almanac, 1, wxALL|wxEXPAND, 5 );
	
	bSizer11->Add( bSizer20, 0, wxEXPAND, 5 );
	
	tCommand_Ack = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE );
	tCommand_Ack->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer11->Add( tCommand_Ack, 1, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer11 );
	this->Layout();
}

iGUI_Commands::~iGUI_Commands()
{
}

iGUI_EEPROM::iGUI_EEPROM( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxHORIZONTAL );
	
	wxString mBurnChoices[] = { wxT("Burn"), wxT("Dump"), wxT("Verify") };
	int mBurnNChoices = sizeof( mBurnChoices ) / sizeof( wxString );
	mBurn = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mBurnNChoices, mBurnChoices, 0 );
	mBurn->SetSelection( 0 );
	bSizer34->Add( mBurn, 0, wxALL, 5 );
	
	wxString mBankChoices[] = { wxT("Bank A"), wxT("Bank B") };
	int mBankNChoices = sizeof( mBankChoices ) / sizeof( wxString );
	mBank = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mBankNChoices, mBankChoices, 0 );
	mBank->SetSelection( 0 );
	bSizer34->Add( mBank, 1, wxALL, 5 );
	
	wxString mTypeChoices[] = { wxT("Application Code"), wxT("Checksum Block"), wxT("Configuration Table") };
	int mTypeNChoices = sizeof( mTypeChoices ) / sizeof( wxString );
	mType = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, mTypeNChoices, mTypeChoices, 0 );
	mType->SetSelection( 0 );
	bSizer34->Add( mType, 0, wxALL, 5 );
	
	mEEPROMFile = new wxFilePickerCtrl( this, wxID_ANY, wxT("/home/gheckler/navigator.bin"), wxT("Choose a file to burn to EEPROM"), wxT("*.bin"), wxDefaultPosition, wxSize( 600,-1 ), wxFLP_OPEN|wxFLP_SAVE );
	bSizer34->Add( mEEPROMFile, 3, wxALL, 5 );
	
	bSizer33->Add( bSizer34, 0, wxEXPAND, 5 );
	
	tFeedback = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_LEFT|wxTE_MULTILINE );
	tFeedback->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer33->Add( tFeedback, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer( wxHORIZONTAL );
	
	mStop = new wxButton( this, ID_EEPROM_STOP, wxT("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer35->Add( mStop, 0, wxALL, 5 );
	
	mStart = new wxButton( this, ID_EEPROM_START, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer35->Add( mStart, 0, wxALL, 5 );
	
	bSizer33->Add( bSizer35, 0, wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer33 );
	this->Layout();
}

iGUI_EEPROM::~iGUI_EEPROM()
{
}

iGUI_Log::iGUI_Log( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer27;
	sbSizer27 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Log Path") ), wxVERTICAL );
	
	mFile = new wxDirPickerCtrl( this, wxID_ANY, wxT("/home/gpssim/workspace/gps-sdr/gse"), wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE|wxDIRP_DIR_MUST_EXIST|wxDIRP_USE_TEXTCTRL );
	sbSizer27->Add( mFile, 1, wxALL|wxEXPAND, 5 );
	
	bSizer30->Add( sbSizer27, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer14;
	sbSizer14 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Logged Items") ), wxVERTICAL );
	
	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 2, 2, 0, 0 );
	
	cPVT = new wxCheckBox( this, wxID_ANY, wxT("PVT State"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cPVT, 0, wxALL, 5 );
	
	cClock = new wxCheckBox( this, wxID_ANY, wxT("Clock State"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cClock, 0, wxALL, 5 );
	
	cPPS = new wxCheckBox( this, wxID_ANY, wxT("PPS State"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cPPS, 0, wxALL, 5 );
	
	cEKFState = new wxCheckBox( this, wxID_ANY, wxT("EKF State"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cEKFState, 0, wxALL, 5 );
	
	cEKFCov = new wxCheckBox( this, wxID_ANY, wxT("EKF Covariance"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cEKFCov, 0, wxALL, 5 );
	
	cEKFRes = new wxCheckBox( this, wxID_ANY, wxT("EKF Residual"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cEKFRes, 0, wxALL, 5 );
	
	cPseudo = new wxCheckBox( this, wxID_ANY, wxT("Pseudoranges"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cPseudo, 0, wxALL, 5 );
	
	cChan = new wxCheckBox( this, wxID_ANY, wxT("Channel Health"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cChan, 0, wxALL, 5 );
	
	cTask = new wxCheckBox( this, wxID_ANY, wxT("Task Health"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cTask, 0, wxALL, 5 );
	
	cBoard = new wxCheckBox( this, wxID_ANY, wxT("Board Health"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cBoard, 0, wxALL, 5 );
	
	cSVPred = new wxCheckBox( this, wxID_ANY, wxT("SV Prediction"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cSVPred, 0, wxALL, 5 );
	
	cGearth = new wxCheckBox( this, wxID_ANY, wxT("Google Earth"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cGearth, 0, wxALL, 5 );
	
	cRobs = new wxCheckBox( this, wxID_ANY, wxT("Rinex Observation"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cRobs, 0, wxALL, 5 );
	
	cRephem = new wxCheckBox( this, wxID_ANY, wxT("Rinex Ephemeris"), wxDefaultPosition, wxDefaultSize, 0 );
	
	gSizer3->Add( cRephem, 0, wxALL, 5 );
	
	sbSizer14->Add( gSizer3, 0, wxEXPAND, 5 );
	
	bSizer30->Add( sbSizer14, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer31->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bCancel = new wxButton( this, wxID_CANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( bCancel, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	bOK = new wxButton( this, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer31->Add( bOK, 0, wxLEFT, 5 );
	
	bSizer30->Add( bSizer31, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	this->SetSizer( bSizer30 );
	this->Layout();
}

iGUI_Log::~iGUI_Log()
{
}

iGUI_Messages::iGUI_Messages( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	tMess = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_LEFT|wxTE_MULTILINE|wxTE_NO_VSCROLL );
	tMess->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer9->Add( tMess, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer9 );
	this->Layout();
}

iGUI_Messages::~iGUI_Messages()
{
}

iGUI_Select::iGUI_Select( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	bSizer22->SetMinSize( wxSize( -1,120 ) ); 
	pVisible = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer22->Add( pVisible, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer22, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer94;
	bSizer94 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer94->SetMinSize( wxSize( -1,50 ) ); 
	pMode = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer94->Add( pMode, 1, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	bSizer17->Add( bSizer94, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	tDisplay = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_DONTWRAP|wxTE_MULTILINE );
	tDisplay->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxT("Monospace") ) );
	
	bSizer23->Add( tDisplay, 1, wxALL|wxEXPAND, 5 );
	
	bSizer17->Add( bSizer23, 10, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer21->SetMinSize( wxSize( -1,30 ) ); 
	
	bSizer21->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bRefresh = new wxButton( this, ID_SELECT_REFRESH, wxT("Refresh"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer21->Add( bRefresh, 0, wxALL, 5 );
	
	
	bSizer21->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer17->Add( bSizer21, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer17 );
	this->Layout();
	
	// Connect Events
	pVisible->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( iGUI_Select::onMouse ), NULL, this );
}

iGUI_Select::~iGUI_Select()
{
	// Disconnect Events
	pVisible->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( iGUI_Select::onMouse ), NULL, this );
}

iGUI_Speedo::iGUI_Speedo( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer107;
	bSizer107 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer116;
	bSizer116 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer18;
	sbSizer18 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Speed") ), wxHORIZONTAL );
	
	pspeed = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer18->Add( pspeed, 1, wxEXPAND, 5 );
	
	bSizer116->Add( sbSizer18, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer43;
	sbSizer43 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("???") ), wxVERTICAL );
	
	pdist = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer43->Add( pdist, 1, wxEXPAND, 5 );
	
	bSizer116->Add( sbSizer43, 1, wxALL|wxEXPAND, 5 );
	
	bSizer107->Add( bSizer116, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer109;
	bSizer109 = new wxBoxSizer( wxHORIZONTAL );
	
	wxGridSizer* gSizer5;
	gSizer5 = new wxGridSizer( 2, 3, 0, 0 );
	
	wxStaticBoxSizer* sbSizer235;
	sbSizer235 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Overall Avg") ), wxVERTICAL );
	
	pstop1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer235->Add( pstop1, 1, wxEXPAND, 5 );
	
	gSizer5->Add( sbSizer235, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer23;
	sbSizer23 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Moving Avg") ), wxVERTICAL );
	
	pmavg = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer23->Add( pmavg, 1, wxEXPAND, 5 );
	
	gSizer5->Add( sbSizer23, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer231;
	sbSizer231 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Max Speed") ), wxVERTICAL );
	
	pmax = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer231->Add( pmax, 1, wxEXPAND, 5 );
	
	gSizer5->Add( sbSizer231, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer232;
	sbSizer232 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Total Time") ), wxVERTICAL );
	
	pttime = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer232->Add( pttime, 1, wxEXPAND, 5 );
	
	gSizer5->Add( sbSizer232, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer233;
	sbSizer233 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Moving Time") ), wxVERTICAL );
	
	pmtime = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer233->Add( pmtime, 1, wxEXPAND, 5 );
	
	gSizer5->Add( sbSizer233, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer234;
	sbSizer234 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Stopped Time") ), wxVERTICAL );
	
	pstop = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer234->Add( pstop, 1, wxEXPAND, 5 );
	
	gSizer5->Add( sbSizer234, 1, wxALL|wxEXPAND, 5 );
	
	bSizer109->Add( gSizer5, 1, wxEXPAND, 5 );
	
	bSizer107->Add( bSizer109, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer107 );
	this->Layout();
}

iGUI_Speedo::~iGUI_Speedo()
{
}
