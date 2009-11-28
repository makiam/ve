/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/resources/Resource.h"
#include "vapor/scene/Geometry.h"

namespace vapor { namespace resources {

//-----------------------------------//

struct ms3d_vertex_t;
struct ms3d_triangle_t;
struct ms3d_group_t;
struct ms3d_material_t;

//-----------------------------------//

class MS3D : public Resource, public scene::Geometry
{
public:

	MS3D(const vfs::File& file);
	virtual ~MS3D();

	ResourceGroup::Enum getResourceGroup() const { return ResourceGroup::Meshes; }

private:

	bool load();
	void clear();

	// Builds the vertex and index buffers representing the mesh.
	void build();

	bool read_header();
	void read_vertices();
	void read_triangles();
	void read_groups();
	void read_materials();
	//void read_animatin();
	//void read_joints();
	//void read_comments();

	//int get_num_groups() const;
	//ms3d_group_t *get_group(int index);

	//int get_num_triangles() const;
	//ms3d_triangle_t *get_triangle(int index);

	//int get_num_vertices() const;
	//ms3d_vertex_t *get_vertex(int index);

	//int get_num_materials() const;
	//ms3d_material_t *get_material(int index);

private:

	const vfs::File& file;

	std::vector<ms3d_vertex_t> m_vertices;
	std::vector<ms3d_triangle_t> m_triangles;
	std::vector<ms3d_group_t> m_groups;
	std::vector<ms3d_material_t> m_materials;
	std::vector<char> m_comment;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( MS3D );

//-----------------------------------//

} } // end namespaces