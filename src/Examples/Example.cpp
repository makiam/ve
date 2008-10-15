#include <vapor/Engine.h>
#include <vapor/RenderDevice.h>
#include <vapor/OpenGL_Device.h>
#include <vapor/Math.h>

using namespace vapor::resources;
using namespace vapor::render;
using namespace vapor::math;

int main(int argc, char *argv[])
{
	// create the logger
	//Log logger(ENGINE_NAME, "log.html");

	shared_ptr<WindowSettings> wS(new WindowSettings(640, 480));

	// create renderer device
	RenderDevice *device =  new OpenGLDevice(wS);

	// load png image
	//PNG png("media/triton.png");

	// load ms3d mesh
	//Milkshape3D model("media/cubo.ms3d");

	do {

		device->clear();
		device->update();

	} while(device->getWindow().pump());

	delete device;

	return EXIT_SUCCESS;
}