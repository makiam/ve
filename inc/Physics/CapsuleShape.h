/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_PHYSICS_BULLET

#include "Physics/Shape.h"

class btCapsuleShape;

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(CapsuleShape)

class API_ENGINE CapsuleShape : public Shape
{
	REFLECT_DECLARE_OBJECT(CapsuleShape)

public:

	CapsuleShape();
	virtual ~CapsuleShape();

	// Updates the shape.
	void update( float delta );

	// Gets the internal Bullet shape.
	btCollisionShape* const getBulletShape() const;

protected:

	// Field change callback.
	void onFieldChanged(const Field& field);

	// Height of the capsule.
	float height;

	// Radius of the capsule.
	float radius;

	// Bullet box shape.
	btCapsuleShape* capsuleShape;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( CapsuleShape );

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif