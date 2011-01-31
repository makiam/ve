/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include <vapor/PCH.h>
#include <vapor/Framework.h>
#include <vapor/render/Device.h>
#include <vapor/Core/FileSystem.h>
#include <vapor/input/InputManager.h>
#include <vapor/resources/ResourceManager.h>
#include <vapor/Utilities.h>

namespace vapor {

//-----------------------------------//

Framework::Framework(const std::string& app, const char** argv)
{
	Log::info( "Engine framework getting into action" );
	Engine::create(app, argv);
}

//-----------------------------------//

void Framework::run()
{
	Framework::init();
	mainLoop();
}

//-----------------------------------//

void Framework::init()
{
	// Init the engine.
	Engine::init(true);

	// Register default media locations.
	FileSystem* fs = getFileSystem();
	fs->mountDefaultLocations();

	// Register input callbacks.
	registerCallbacks();

	// User init callback.
	onInit();

	// User resources setup callback.
	onSetupResources();

	// User scene setup callback.
	onSetupScene();

	// Wait until all resources are loaded.
	ResourceManager* rm = getResourceManager();
	rm->loadQueuedResources();
}

//-----------------------------------//

/**
 * Check out this article for a detailed analysis of some possible 
 * game loop implementations: http://dewitters.koonsolo.com/gameloop.html
 */

void Framework::mainLoop()
{
	RenderDevice* renderDevice = getRenderDevice();
	Window* window = renderDevice->getWindow();

	const ushort numUpdatesSecond = 25;
	const double maxUpdateTime = 1.0f / numUpdatesSecond;

	Timer updateTimer;
	double nextTick = updateTimer.reset();

	while( true )
	{
		frameTimer.reset();
		
		if( !window->pumpEvents() )
			break;

		while( updateTimer.getCurrentTime() > nextTick )
		{
			update( maxUpdateTime );

			// User update callback.
			onUpdate( maxUpdateTime );

			nextTick += maxUpdateTime;
		}

		// User rendering callback.
		onRender();

		// Update the active target (swaps buffers).
		window->update();

		// Calculates the new frame times.
		updateFrameTimes();
	}

	System::sleep(0);
}

//-----------------------------------//

void Framework::updateFrameTimes()
{
	frameStats.lastFrameTime = frameTimer.getElapsedTime();

	if(frameStats.lastFrameTime > 0.1)
		Log::debug("HOTSPOT! %lf", frameStats.lastFrameTime);

	frameStats.frameStep();
}

//-----------------------------------//

void Framework::registerCallbacks()
{
	InputManager* input = getInputManager();

	if(!input)
		return;
	
	Keyboard* keyboard = input->getKeyboard();
	keyboard->onKeyPress.Connect( this, &Framework::onKeyPressed );
	
	Mouse* mouse = input->getMouse();
	mouse->onMouseButtonPress.Connect( this, &Framework::onButtonPressed );
	mouse->onMouseButtonRelease.Connect( this, &Framework::onButtonReleased );
}

//-----------------------------------//

} // end namespace