/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/ResourceManager.h"
#include "vapor/render/Program.h"

FWD_DECL_NS_TYPEDEF_INT(resources, Text)

namespace vapor { namespace render {

//-----------------------------------//

/**
 * Manages a set of programs. The rest of the engine, when needing a 
 * program will ask the ProgramManager for the program with a name.
 * This way the engine will be able to provide, for example, fallback 
 * programs for when it is not possible to load the requested program.
 * This will be pretty useful when resource auto-loading is added, if a 
 * given file is corrupted we can safely render the scene, falling back
 * to default programs.
 */

class VAPOR_API ProgramManager : public boost::noncopyable
{
public:

	ProgramManager();
	~ProgramManager();

	// Gets a program given a name identifier.
	ProgramPtr getProgram( const std::string& program );

private:

	// Registers a new program in the manager.
	bool registerProgram( const std::string& name, const ProgramPtr& program );

	// Creates the shaders given their source text.
	void createShaders( const resources::TextPtr& );

	// Populates a shader when the text file is loaded.
	void onLoad( const resources::ResourceEvent& evt );

	// Reloads a shader when the text file changes.
	void onReload( const resources::ResourceEvent& evt );

	// Maps the identifiers to the programs.
	std::map< std::string, ProgramPtr > programs;
	typedef std::pair< const std::string&, ProgramPtr > programPair;

	resources::ResourceManagerPtr rm;
};

TYPEDEF_PTR(ProgramManager)

//-----------------------------------//

} } // end namespaces