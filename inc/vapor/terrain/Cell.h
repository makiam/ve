/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Renderable.h"

namespace vapor {

//-----------------------------------//

struct TerrainSettings;

/**
 * Cells are pieces of terrain geometry. They are further subdivided in 
 * cells, that are conceptually similar to tiles in 2D games, and allow
 * you to aligned like in an horizontal grid and are identified by
 * integer coordinates.
 */

class VAPOR_API Cell : public Renderable
{
	DECLARE_UNCOPYABLE(Cell)

public:

	Cell( const TerrainSettings& settings, 
		  const std::vector<float>& heights,
		  int x, int y );

	// Gets the heights of this Cell of terrain.
	const std::vector<float>& getHeights();

	// Sets the heights of this Cell of terrain.
	void setHeights( const std::vector<float>& heights );

	// Rebuilds the terrain geometry.
	void rebuildGeometry();

	// Rebuilds the terrain normals.
	void rebuildNormals();

	// Given an indice, gets the neighbour vertices.
	uint getNeighborFaces( uint index, std::vector<uint>& n );

	// Gets the X coordinate of the cell.
	GETTER(X, int, x)

	// Gets the Y coordinate of the cell.
	GETTER(Y, int, y)

protected:

	// Calculate the vertices of the geometry.
	void rebuildVertices();

	// Calculate the indices of the geometry.
	void rebuildIndices();

	// Calculate the face normals of the geometry.
	void rebuildFaceNormals();

	// Calculate the averaged normals of the geometry.
	void rebuildAveragedNormals();

	// Coordinates of this cell of terrain.
	int x, y;

	// Stores the heights of this cell of terrain.
	std::vector<float> heights;
	
	// Stores the triangle face normals.
	std::vector<Vector3> faceNormals;

	// Cell image.
	ImagePtr image;

	// Terrain settings.
	const TerrainSettings& settings;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Cell );

//-----------------------------------//

} // end namespace