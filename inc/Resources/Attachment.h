/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Math/Vector.h"
#include "Math/Quaternion.h"

FWD_DECL_INTRUSIVE(Bone)

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

/**
 * Attachments allow meshes to be combined onto other meshes.
 * For example, you can attach a sword to the hand of a ninja.
 */

class API_RESOURCE Attachment : public ReferenceCounted
{
public:

	// Attachment bone.
	BonePtr bone;

	// Attachment node.
	EntityPtr node;

	// Position offset.
	Vector3 offsetPosition;

	// Orientation offset.
	Quaternion offsetOrientation;
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Attachment ) ;

//-----------------------------------//

NAMESPACE_RESOURCES_END