/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Engine.h"

#ifdef VAPOR_IMAGE_PICOPNG
	#include "vapor/resources/PNG_Loader.h"
#endif

#ifdef VAPOR_IMAGE_STB
	#include "vapor/resources/STB_Image_Loader.h"
#endif

#ifdef VAPOR_MESH_MILKSHAPE3D
	#include "vapor/resources/MS3D_Loader.h"
#endif

#ifdef VAPOR_AUDIO_OGG
	#include "vapor/resources/OGG_Loader.h"
#endif

#ifdef VAPOR_SHADER_GLSL
	#include "vapor/resources/GLSL_Loader.h"
#endif

#ifdef VAPOR_SCRIPTING_LUA
	#include "vapor/resources/Lua_Loader.h"
#endif

#ifdef VAPOR_AUDIO_OPENAL
	#include "vapor/audio/Device.h"
#endif

#if defined(VAPOR_FONT_BITMAP) || defined(VAPOR_FONT_FREETYPE2)
	#include "vapor/resources/Font_Loader.h"
#endif

using namespace vapor::audio;
using namespace vapor::scene;
using namespace vapor::resources;
using namespace vapor::physics;
using namespace vapor::render;
using namespace vapor::log;
using namespace vapor::vfs;
using namespace vapor::input;

namespace vapor {

//-----------------------------------//

Engine::Engine(const std::string& app, const char** argv, bool autoInit)
	: app(app), argv(argv), 
	renderDevice( nullptr ), resourceManager( nullptr ), audioDevice( nullptr ),
	physicsManager( nullptr ), vfs( nullptr ), log( nullptr ), scriptState( nullptr )
{
	if( autoInit )
	{
		init();
	}
}

//-----------------------------------//

Engine::~Engine()
{
	sceneNode.reset();

#ifdef VAPOR_AUDIO_OPENAL
	delete audioDevice;
#endif

	delete renderDevice;
	delete resourceManager;
	delete physicsManager;
	delete scriptState;
	delete vfs;
	delete log;
}

//-----------------------------------//

void Engine::init( bool createWindow )
{
	// setup the global logger
	setupLogger();

	info( "engine", "Starting vaporEngine version '%s'", VAPOR_ENGINE_VERSION );

	// create the virtual filesystem
	vfs = new VFS(app, argv ? argv[0] : nullptr);

	// create the resource manager
	resourceManager = ResourceManager::getInstancePtr();

	// create the physics manager
	physicsManager = PhysicsManager::getInstancePtr();

	// register default codecs
	setupResourceLoaders();

	// create a rendering and audio device
	setupDevices( createWindow );

	// create the root scene node
	sceneNode.reset( new scene::Scene() );

#ifdef VAPOR_SCRIPTING_LUA
	// Initialize the scripting
	scriptState = new script::State();
	scriptState->bind( this );
#endif
}

//-----------------------------------//

void Engine::setupLogger()
{
	// get a suitabfle log filename
	std::string file = app + ".html";
	
	// create and set a new logger
	log = new Log(app, file);
}

//-----------------------------------//

void Engine::setupDevices( bool createWindow )
{
	// create render device
	renderDevice = new render::Device();

	if( createWindow )
	{
		// create a window and set the title
		renderDevice->createWindow();

		// init the render device now that it has a context
		renderDevice->init();
		
		// creates the default input devices
		setupInput();
	}

#ifdef VAPOR_AUDIO_OPENAL

	// create the audio device
	audioDevice = new audio::Device();

#endif
}

//-----------------------------------//

void Engine::setupInput()
{
	Window& window = *renderDevice->getWindow();

	// obtains the input manager instance
	InputManager& im = window.getInputManager();

	// let's register some input devices
	im.addDevice( new input::Keyboard() );
	im.addDevice( new input::Mouse() );
}

//-----------------------------------//

void Engine::setupResourceLoaders()
{
	if(!resourceManager) return;

	std::vector<ResourceLoader*> loaders;

	// register default compiled codecs
	#ifdef VAPOR_IMAGE_PICOPNG
		loaders.push_back( new PNG_Pico_Loader() );
	#endif

	#ifdef VAPOR_MESH_MILKSHAPE3D
		loaders.push_back( new MS3D_Loader() );
	#endif

	#ifdef VAPOR_AUDIO_OGG
		loaders.push_back( new OGG_Loader() );
	#endif

	#ifdef VAPOR_SHADER_GLSL
		loaders.push_back( new GLSL_Loader() );
	#endif

	#ifdef VAPOR_SCRIPTING_LUA
		loaders.push_back( new Lua_Loader() );
	#endif

	#ifdef VAPOR_IMAGE_STB
		loaders.push_back( new STB_Image_Loader() );
	#endif
	
	#ifdef VAPOR_FONT_BITMAP
		loaders.push_back( new Font_Loader() );
	#endif

	foreach( ResourceLoader* loader, loaders )
	{
		resourceManager->registerLoader( loader );
	}
}

//-----------------------------------//

void Engine::update( double delta )
{
	this->getSceneManager()->update( delta );

#ifdef VAPOR_SCRIPTING_LUA
	this->getScriptState()->update( delta );
#endif

	//this->getPhysicsManager()->update( delta );
}

//-----------------------------------//

} // end namespace