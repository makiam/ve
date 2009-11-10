/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/resources/Shader.h"
#include "vapor/render/VertexBuffer.h"
#include "vapor/math/Matrix4x3.h"
#include "vapor/math/Matrix4x4.h"

namespace vapor {
	namespace render {

//-----------------------------------//

/**
 * A program is a collection of shaders (at least a vertex shader 
 * and a pixel shader). When it is linked it is ready to be used
 * by the renderer. A program will have some parameters automatically
 * bound, like world matrices and such. Refer to the documentation
 * to have more information about these.
 */

class VAPOR_API Program
{
public:

	Program( resources::ShaderPtr vs, resources::ShaderPtr ps );
	virtual ~Program();

	// Adds a named parameter to the program.
	virtual void setAttribute( const std::string& slot, VertexAttribute::Enum attr ) = 0;

	// Adds a named int uniform to the program.
	virtual void setUniform( const std::string& slot, int ) = 0;

	// Adds a named matrix uniform to the program.
	virtual void setUniform( const std::string& slot, const math::Matrix4x3& ) = 0;

	// Adds a named 4x4 matrix uniform to the program.
	virtual void setUniform( const std::string& slot, const math::Matrix4x4& ) = 0;

	// Links the program and returns if it was successful.
	virtual bool link() = 0;

	// Binds the program to be used for rendering operations.
	virtual void bind() = 0;

	// Unbinds the program.
	virtual void unbind() = 0;

	// Returns if this program has been successfully linked.
	virtual bool isLinked() const;

	// Gets the linking log.
	const std::string& getLog() const;

	// Serialization

	//virtual void load( const std::string& name ) = 0;
	//virtual void save( const std::string& name ) = 0;

protected:

	std::vector< resources::ShaderPtr > shaders;
	std::string log;
	bool linked;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Program );

//-----------------------------------//

} } // end namespaces