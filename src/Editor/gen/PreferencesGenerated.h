///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __PreferencesGenerated__
#define __PreferencesGenerated__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/filepicker.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/srchctrl.h>
#include <wx/slider.h>
#include <wx/splitter.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace vapor
{
	namespace gui
	{
		///////////////////////////////////////////////////////////////////////////////
		/// Class Bindings
		///////////////////////////////////////////////////////////////////////////////
		class Bindings : public wxPanel 
		{
			private:
			
			protected:
				wxPanel* m_panelBindings;
				wxStaticText* m_staticText2;
				wxTreeCtrl* m_treeCtrl1;
				wxStaticText* m_staticText3;
				wxListBox* m_listBox1;
				wxButton* m_button1;
				wxButton* m_button2;
				wxStaticText* m_staticText4;
				wxTextCtrl* m_textCtrl2;
				wxStaticText* m_staticText5;
				wxTextCtrl* m_textCtrl3;
				wxButton* m_button3;
				wxStaticText* m_staticText1;
				wxTextCtrl* m_textCtrl1;
			
			public:
				
				Bindings( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,450 ), long style = wxTAB_TRAVERSAL );
				~Bindings();
			
		};
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class Plugins
		///////////////////////////////////////////////////////////////////////////////
		class Plugins : public wxPanel 
		{
			private:
			
			protected:
				wxListCtrl* m_listPlugins;
				wxButton* m_buttonPluginEnable;
				wxButton* m_buttonPluginUninstall;
				wxStaticLine* m_staticline1;
				wxButton* m_buttonPluginCheckUpdates;
				wxButton* m_buttonPluginInstall;
				wxStaticText* m_staticText11;
				wxTextCtrl* m_textPluginDescription;
				
				// Virtual event handlers, overide them in your derived class
				void OnPluginInit( wxInitDialogEvent& event );
				void OnPluginSelected( wxListEvent& event );
				void OnPluginEnable( wxCommandEvent& event );
				void OnPluginCheckUpdates( wxCommandEvent& event );
				void OnPluginInstall( wxCommandEvent& event );
				
			
			public:
				
				Plugins( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,450 ), long style = wxTAB_TRAVERSAL );
				~Plugins();
			
		};
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class Resources
		///////////////////////////////////////////////////////////////////////////////
		class Resources : public wxPanel 
		{
			private:
			
			protected:
				wxStaticText* m_staticText8;
				wxDirPickerCtrl* m_dirPicker1;
			
			public:
				
				Resources( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,450 ), long style = wxTAB_TRAVERSAL );
				~Resources();
			
		};
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class Renderers
		///////////////////////////////////////////////////////////////////////////////
		class Renderers : public wxPanel 
		{
			private:
			
			protected:
				wxStaticText* m_staticText8;
				wxChoice* m_choice1;
				wxStaticText* m_staticText10;
				wxListCtrl* m_listCtrl2;
				wxStaticText* m_staticText9;
				wxListBox* m_listBox3;
			
			public:
				
				Renderers( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 400,450 ), long style = wxTAB_TRAVERSAL );
				~Renderers();
			
		};
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class ResourcesBrowser
		///////////////////////////////////////////////////////////////////////////////
		class ResourcesBrowser : public wxDialog 
		{
			private:
			
			protected:
				wxSplitterWindow* m_splitter2;
				wxPanel* m_panel2;
				wxTreeCtrl* m_resourceGroups;
				wxPanel* m_panel3;
				wxSearchCtrl* m_searchCtrl;
				wxSlider* m_detailSlider;
				wxListCtrl* m_resourceList;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
				virtual void onResourceGroupChanged( wxTreeEvent& event ) { event.Skip(); }
				virtual void onResourceSliderScroll( wxScrollEvent& event ) { event.Skip(); }
				virtual void OnListBeginDrag( wxListEvent& event ) { event.Skip(); }
				virtual void onResourceListActivated( wxListEvent& event ) { event.Skip(); }
				virtual void onResourceListSelection( wxListEvent& event ) { event.Skip(); }
				
			
			public:
				
				ResourcesBrowser( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 580,370 ), long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );
				~ResourcesBrowser();
				
				void m_splitter2OnIdle( wxIdleEvent& )
				{
					m_splitter2->SetSashPosition( 121 );
					m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( ResourcesBrowser::m_splitter2OnIdle ), NULL, this );
				}
			
		};
		
	} // namespace gui
} // namespace vapor

#endif //__PreferencesGenerated__