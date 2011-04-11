/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Projector.h"
#include "Scene/Transform.h"
#include "Scene/Entity.h"
#include "Render/DebugGeometry.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Projector, Component)
	FIELD_CLASS(Frustum, frustum)
END_CLASS()

//-----------------------------------//

Projector::Projector()
{ }

//-----------------------------------//

void Projector::update(float)
{
	const TransformPtr& transform = getEntity()->getTransform();

	frustum.updateProjection(Vector2::Zero);
	frustum.updateCorners(transform->getAbsoluteTransform());

	updateDebugRenderable();
}

//-----------------------------------//

void Projector::updateDebugRenderable() const
{
	if( !debugRenderable )
		return;

	updateDebugFrustum( debugRenderable, frustum );
}

//-----------------------------------//

RenderablePtr Projector::createDebugRenderable() const
{
	assert( !debugRenderable );

	debugInheritsTransform = false;
	return buildFrustum( frustum );
}

//-----------------------------------//

} // end namespace