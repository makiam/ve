/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/ResourceManager.h"
#include "Resources/ResourceLoader.h"

#include "Core/Log.h"
#include "Core/Memory.h"
#include "Core/Stream.h"
#include "Core/Archive.h"
#include "Core/Utilities.h"

NAMESPACE_BEGIN

//-----------------------------------//

void ResourceTaskRun(Task* task)
{
	ResourceLoadOptions* options = (ResourceLoadOptions*) task->Userdata;
	Resource* resource = options->resource;
	const Path& path = resource->getPath();

	Stream* stream = options->stream;
	
	ResourceManager* res = GetResourceManager();
	ResourceLoader* loader = res->findLoader( PathGetFileExtension(path) );

	bool decoded = loader->decode(*stream, resource);
		
	if( !decoded )
	{
		resource->setStatus( ResourceStatus::Error );
		LogWarn("Error decoding resource '%s'", path.c_str());
		return;
	}

	resource->setStatus( ResourceStatus::Loaded );

	if( options->sendLoadEvent )
	{
		ResourceEvent event;
		event.resource = resource;

		res->resourceTaskEvents.push(event);
	}

	AtomicDecrement(&res->numResourcesQueuedLoad);
	ConditionWakeOne(res->resourceFinishLoad);

	LogInfo("Loaded resource '%s'", path.c_str());

	Deallocate(GetResourcesAllocator(), options);
	StreamDestroy(stream, GetResourcesAllocator());
}

//-----------------------------------//

NAMESPACE_END