/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "Example.h"

//-----------------------------------//

Example::Example(const char** argv)
	: Framework("Example", argv), c( 0.0f, 0.10f, 0.25f ),
	fpsUpdateTime( 0.0f )
{

}

//-----------------------------------//

void Example::onInit()
{
	// Mount VFS points
	const std::string& media = "media";

	if ( !getVFS()->mount( media ) )
	{
		Log::MessageDialog( "Missing archive/directory '" + media + "'." );
		exit(1);
	}

	//physicsManager->createWorld();
}

//-----------------------------------//

void Example::onSetupResources() 
{
	ResourceManager* rm = getResourceManager();
	
	ImagePtr img = rm->loadResource< Image >( "triton.png" );
	snd = rm->loadResource< resources::Sound >( "stereo.ogg" );
}

//-----------------------------------//

std::string getFPS( float lastFrameTime )
{
	std::string str = "FPS: ";
	if( lastFrameTime == 0 ) return str;
	std::string fps( num_to_str(int( 1.0f / lastFrameTime ) ) );
	return str + fps;
}

//-----------------------------------//

void Example::onSetupScene() 
{
	ScenePtr scene = getSceneManager();
	ResourceManager* rm = getResourceManager();

	// TODO: make shaders automatically loaded?
	ProgramPtr tex( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "tex.vs" ),
			rm->loadResource< GLSL_Shader >( "tex.fs" ) ) );

	ProgramPtr diffuse( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "diffuse.vs" ),
			rm->loadResource< GLSL_Shader >( "diffuse.fs" ) ) );

	ProgramPtr toon( new GLSL_Program( 
			rm->loadResource< GLSL_Shader >( "toon.vs" ),
			rm->loadResource< GLSL_Shader >( "toon.fs" ) ) );

	ProgramManager::getInstance().registerProgram( "diffuse", diffuse );
	ProgramManager::getInstance().registerProgram( "tex", tex );
	ProgramManager::getInstance().registerProgram( "toon", toon );

	// Create a new Camera
	NodePtr camera( new Node( "MainCamera" ) );
	cam.reset( new FirstPersonCamera( getInputManager(), getRenderDevice() ) );
	camera->addComponent( TransformPtr( new Transform( 0.0f, 20.0f, -65.0f ) ) );
	camera->addComponent( cam );
	scene->add( camera );

	MeshPtr mesh = rm->loadResource<Mesh>( "ct.ms3d" );
	foreach( const RenderablePtr& rend, mesh->getGeometry()->getRenderables() )
		rend->getMaterial()->setProgram( tex );

	NodePtr ct( new Node( "ct" ) );
	ct->addComponent( TransformPtr( new Transform() ) );
	ct->addComponent( mesh->getGeometry() );
	scene->add(ct);
	
	// Materials too?
	MaterialPtr mat2( new Material( "FontMaterial", tex ) );
	FontPtr font = rm->loadResource< Font >( "Verdana.font" );
	label.reset( new Label( getFPS( lastFrameTime ), font, mat2 ) );
	NodePtr fps( new Node( "FPSNode" ) );
	fps->addComponent( TransformPtr( new Transform() ) );
	fps->addComponent( label );
	fps->getTransform()->translate( -300.0f, 220.0f, 0.0f );
	scene->add( fps );

	MaterialPtr mat( new Material( "GridMaterial", diffuse ) );
	NodePtr grid( new Node( "Grid" ) );
	grid->addComponent( TransformPtr( new Transform() ) );
	grid->addComponent( ComponentPtr( new Grid( mat ) ) );
	scene->add( grid );

	foreach( const RenderablePtr& rend, grid->getComponent<Geometry>("Grid")->getRenderables() )
	{
		rend->getMaterial()->setProgram( diffuse );
	}

	NodePtr lnode( new Node( "Light" ) );
	LightPtr light( new Light( LightType::Point ) );
	light->diffuseColor = Colors::Red;
	light->ambientColor = Colors::Yellow;
	lnode->addComponent( light );
	scene->add( lnode );

	TerrainSettings settings;
	settings.CellSize = 1024;
	settings.TileDimensions = 32;
	settings.MaxHeight = 150;

	TerrainPtr terrain( new Terrain( settings ) );

	NodePtr terreno( new Node( "Terreno" ) );
	terreno->addComponent( TransformPtr( new Transform() ) );
	terreno->addComponent( terrain );
	scene->add( terreno );

	ImagePtr heightmap = rm->loadResource< Image >( "height2.png" );
	CellPtr cell = terrain->createCell( heightmap, 0, 0 );
}

//-----------------------------------//
 
void Example::onUpdate( double delta ) 
{
	if( fpsUpdateTime <= 1.0f )
	{
		fpsUpdateTime += delta;
	}
	else
	{
		label->setText( getFPS(lastFrameTime) );
		fpsUpdateTime = 0.0f;
	}
}

//-----------------------------------//

void Example::onRender()
{
	render::Device* device = getRenderDevice();

	device->setClearColor( c );
	device->clearTarget();

	cam->render();
}

//-----------------------------------//

void Example::onKeyPressed( const KeyEvent& keyEvent )
{
	if( keyEvent.keyCode == Keys::Space )
		debug( "time: %f", frameTimer.getElapsedTime() );

	if( keyEvent.keyCode == Keys::Pause )
		Log::showDebug = !Log::showDebug;

	if( keyEvent.keyCode == Keys::F )
		debug( "fps: %d", int( 1.0f / lastFrameTime ) );

	if( keyEvent.keyCode == Keys::G )
	{
		debug( "min/avg/max: %f / %f / %f", 
					minFrameTime, avgFrameTime, maxFrameTime );
	}
}

//-----------------------------------//
