/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Geometry.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Generates a procedural cube mesh.
 */

REFLECT_DECLARE_CLASS(Cube)

class API_SCENE Cube : public Geometry
{
	REFLECT_DECLARE_OBJECT(Cube)

public:

	Cube();
	Cube( float width, float height );

	// Creates the cube.
	void create();

private:

	float width;
	float height;
};

void BuildCube( GeometryBuffer* vb, float width, float height );

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Cube );

//-----------------------------------//

NAMESPACE_GRAPHICS_END