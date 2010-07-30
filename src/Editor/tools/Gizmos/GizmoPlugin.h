/************************************************************************
*
* vapor3D Editor � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Plugin.h"
#include "Gizmo.h"

namespace vapor { namespace editor {

//-----------------------------------//

class GizmoOperation;

//-----------------------------------//

namespace GizmoTool
{
	enum Enum
	{
		Camera = wxID_HIGHEST,
		Select,
		Translate,
		Rotate,
		Scale
	};
}

//-----------------------------------//

class GizmoPlugin : public Plugin
{
public:

	GizmoPlugin( EditorFrame* );

	// Gets metadata about this plugin.
	virtual PluginMetadata getMetadata();
	
	// Plugin enable callback.
	virtual void onPluginEnable();

	// Plugin disable callback.
	virtual void onPluginDisable();

	// Mouse events.
	virtual void onMouseMove( const MouseMoveEvent& );
	virtual void onMouseDrag( const MouseDragEvent& );
	virtual void onMouseButtonPress( const MouseButtonEvent& );
	virtual void onMouseButtonRelease( const MouseButtonEvent& );

	// Node events.
	virtual void onNodeSelect( const NodePtr& );
	virtual void onNodeUnselect( const NodePtr& );

protected:

	// Handles gizmo tools buttons click.
	void onGizmoButtonClick(wxCommandEvent& event);

	// Checks if the passed tool the current one.
	bool isTool(GizmoTool::Enum mode);

	void createGizmo( const NodePtr& );
	void removeGizmo( const NodePtr& );
	void createOperation();

	void setBoundingBoxVisible( const NodePtr& node, bool state );

	bool pickBoundingTest( const MouseMoveEvent& );
	bool pickImageTest( const MouseMoveEvent&, GizmoAxis::Enum& );
	
	void disableSelectedNodes();

	// Current Gizmo tool.
	GizmoTool::Enum tool;

	// Current Gizmo geometry.
	GizmoPtr gizmo;

	// Current Gizmo axis selection.
	GizmoAxis::Enum axis;

	// Current gizmo operation.
	GizmoOperation* op;
	
	ScenePtr editorScene;

	typedef std::map<NodePtr, NodePtr> GizmoNodeMap;
	GizmoNodeMap gizmos;
	std::vector<NodePtr> selected;

	// Toolbar buttons.
	wxToolBarToolBase* buttonCamera;
	wxToolBarToolBase* buttonSelect;
	wxToolBarToolBase* buttonTranslate;
	wxToolBarToolBase* buttonRotate;
	wxToolBarToolBase* buttonScale;
};

//-----------------------------------//

} } // end namespaces