/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/math/Ray.h"

namespace vapor { namespace math {

//-----------------------------------//

/**
 * Axis-aligned Bounding Box.
 */

class VAPOR_API AABB : private boost::noncopyable
{
public:

	AABB();

	// Gets the chosen corner vertice.
	Vector3 getCorner( int index ) const;

	// Resets the bounding box.
	void reset();

	// Gets the center point of the bounding box.
	Vector3 getCenter() const;

	// Gets the minimum corner of the bounding box.
	const Vector3& getMinimum() const;

	// Gets the maximum corner of the bounding box.
	const Vector3& getMaximum() const;
    
	// Gets the size of the bounding box.
	//Vector3 getSize() const;

	// Expands the bounding box to contain the vertex.
	void add( const Vector3& vertex );

	// Expands the bounding box to contain the bounding box.
	void add( const AABB& aabb );

	// Intersection with a ray
	 bool intersects( const Ray& ray, float& distance ) const;

public:

	Vector3 min;
	Vector3 max;
};

//-----------------------------------//

} } // end namespaces
