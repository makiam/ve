/************************************************************************
*
* vaporEngine by triton (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Renderable.h"
#include "vapor/scene/Transform.h"
#include "vapor/scene/Light.h"

namespace vapor { namespace render {

//-----------------------------------//

/**
 * Use these different kinds of render groups to signal to the renderer
 * how you want your renderables to be sorted by the render device.
 * Lower numbers render before higher numbers.
 */

namespace RenderGroup
{
	enum Enum
	{
		Normal = 0,
		Transparency = 5,
		Overlays = 10
	};
}

//-----------------------------------//

/**
 * This contains all the rendering state information that is needed by
 * the rendering device to properly render the associated renderable.
 * This should be set by the camera and then appended to a render queue
 * where it will be sorted to minimize the state changes and finally
 * rendered to the render target that is currently active on the device.
 */

struct RenderState /*: private boost::noncopyable*/
{
	RenderState()
		: group( RenderGroup::Normal ), priority(0) 
	{}
	
	RenderState( const RenderState& rhs )
		: renderable( rhs.renderable ), group( rhs.group ),
		modelMatrix( rhs.modelMatrix ), priority( rhs.priority )
	{}

	RenderablePtr renderable;
	Matrix4x3 modelMatrix;
	RenderGroup::Enum group;
	int priority;
};

//-----------------------------------//

/**
 * Light properties that will be passed down to the renderer.
 */

struct LightState
{
	scene::LightPtr light;
	scene::TransformPtr transform;
	render::TexturePtr depth;
	Matrix4x4 projection;
};

//-----------------------------------//

/**
 * This is a queue of objects that are usually returned by performing
 * a culling operation on the camera. Each renderable can be assigned
 * a different priority to be rendered on different times. For example
 * you might want to render the GUI after everything else is rendered.
 */

typedef std::vector<RenderState> RenderQueue;
typedef std::vector<LightState> LightQueue;

//-----------------------------------//

struct RenderBlock
{
	RenderQueue renderables;
	LightQueue lights;
};

//-----------------------------------//

} } // end namespaces