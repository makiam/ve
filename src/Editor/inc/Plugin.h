/************************************************************************
*
* vapor3D Editor � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Entity.h"
#include "Input/MouseEvents.h"
#include "Input/KeyboardEvents.h"

FWD_DECL_SHARED(Scene)
FWD_DECL(Engine)

namespace vapor { namespace editor {

//-----------------------------------//

struct PluginMetadata
{
	// Name of this plugin.
	std::string name;

	// Description of this plugin.
	std::string description;

	// Author of this plugin.
	std::string author;

	// Version of this plugin.
	std::string version;
};

//-----------------------------------//

class Viewframe;
class EditorFrame;
class PluginManager;

/**
 * A 3D editor needs to provide different kind of tools to the user,
 * so to be flexible and allow extensibility each tool behaviours and
 * functionality is provided by a custom type derived from this class.
 * As each tool can be loaded/unloaded at runtime, there is a tool
 * lifecycle you must implement. Most of the work is done by asking
 * the editor to execute actions in the different events/callbacks.
 */

class Plugin
{
	friend class PluginManager;

public:

	Plugin( EditorFrame* frame );
	virtual ~Plugin();

	// Gets if plugin is enabled.
	bool isPluginEnabled() const;

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata() = 0;

	// Plugin enable callback.
	virtual void onPluginEnable() = 0;

	// Plugin disable callback.
	virtual void onPluginDisable() = 0;

	// Plugin tool selection callback.
	virtual void onToolSelect( int id ) {}

	// Plugin tool unselection callback.
	virtual void onToolUnselect( int id ) {}

	// Entity selection callback.
	virtual void onEntitySelect( const EntityPtr& ) {}
	
	// Entity unselection callback.
	virtual void onEntityUnselect( const EntityPtr& ) {}

	// Component selection callback.
	virtual void onComponentSelect( const ComponentPtr& ) {}
	
	// Component unselection callback.
	virtual void onComponentUnselect( const ComponentPtr& ) {}

	// Resource selection event.
	virtual void onResourceSelect( const ResourcePtr& ) {}

	// Resource unselection event.
	virtual void onResourceUnselect( const ResourcePtr& ) {}

	// Mouse move callback.
	virtual void onMouseMove( const MouseMoveEvent& ) {}
	
	// Mouse drag callback.
	virtual void onMouseDrag( const MouseDragEvent& ) {}
	
	// Mouse button press callback.
	virtual void onMouseButtonPress( const MouseButtonEvent& ) {}
	
	// Mouse button release callback.
	virtual void onMouseButtonRelease( const MouseButtonEvent& ) {}
	
	// Mouse enter callback.
	virtual void onMouseEnter() {}
	
	// Mouse leave callback.
	virtual void onMouseLeave() {}
	
	// Key press callback.
	virtual void onKeyPress() {}

	// Scene load callback.
	virtual void onSceneLoad( const ScenePtr& scene ) {}

	// Scene unload callback.
	virtual void onSceneUnload( const ScenePtr& scene ) {}

	// Scene update callback.
	virtual void onSceneUpdate() {}

protected:

	// Helper method to disable plugins.
	void doPluginDisable();

	// Registers a new tool in the plugin.
	void addTool( wxAuiToolBarItem* tool, bool addToMenu = false );

	// Removes all the registered tools.
	void removeTools();

	// Returns if the tool is from the plugin.
	bool isPluginTool(int id) const;

	// Removes a page from the main notebook.
	void removePage( wxWindow* page );

	// Access to the engine instance.
	Engine* engine;

	// Access to the editor instance.
	EditorFrame* editor;

	// Access to the main viewframe.
	Viewframe* viewframe;

	// Is plugin enabled?
	bool pluginEnabled;

	// Keeps track of all the registered tools.
	typedef std::vector<wxAuiToolBarItem*> Tools;
	Tools tools;
};

//-----------------------------------//

} } // end namespaces