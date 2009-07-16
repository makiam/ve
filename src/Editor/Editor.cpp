/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "Editor.h"

using namespace vapor;
using namespace vapor::math;
using namespace vapor::render;

//-----------------------------------//

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
END_EVENT_TABLE()

//-----------------------------------//

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

//-----------------------------------//

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // call the base class initialization method, currently it only parses a
    // few common command-line options but it could be do more in the future
    if ( !wxApp::OnInit() )
        return false;

	// let's add support for PNG images
	wxImage::AddHandler(new wxPNGHandler);

    // create the main application window
    MyFrame *frame = new MyFrame("vaporEditor - level editor for vapor");

	frame->SetSize(750, 500);

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(true);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

//-----------------------------------//

// frame constructor
MyFrame::MyFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title)
{
    // set the frame icon
    SetIcon(wxICON(sample));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu* fileMenu = new wxMenu;

    // the "About" item should be in the help menu
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About...\tF1", "Show about dialog");

    fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
	SetStatusText("vaporEngine (FPS: OVER 9000!)");
#endif // wxUSE_STATUSBAR

	wxToolBar* toolBar = new wxToolBar();
	SetToolBar(toolBar);

	// initialize vaporEngine
	initEngine();

	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

	treeCtrl = new SceneTreeCtrl(engine, this, ID_SceneTree,
		wxDefaultPosition, wxSize(200, -1), wxTR_DEFAULT_STYLE);
	
	sizer->Add(treeCtrl, 0, wxALL|wxEXPAND, 0 );

	// add a new vaporControl in the frame
	control = new vaporControl(engine, this);
	sizer->Add(control, 1, wxALL|wxEXPAND, 0 );

	SetSizerAndFit(sizer);
}

//-----------------------------------//

MyFrame::~MyFrame()
{
	if(engine) delete engine;
}

//-----------------------------------//

void MyFrame::initEngine()
{
	engine = new vapor::Engine(false);

	engine->setupResourceManager();
	engine->setupLogger("vaporEditor", "vaporEditor.html");
	engine->setupDevices();
}

//-----------------------------------//
// event handlers
//-----------------------------------//

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

//-----------------------------------//

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxString::Format
                 (
                    "Welcome to %s!\n"
                    "\n"
                    "This is the minimal wxWidgets sample\n"
                    "running under %s.",
                    wxVERSION_STRING,
                    wxGetOsDescription()
                 ),
                 "About wxWidgets minimal sample",
                 wxOK | wxICON_INFORMATION,
                 this);
}

//-----------------------------------//