/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_IMAGE_STB

#include "Resources/ResourceLoader.h"
#include "Resources/Image.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

/**
 * This codec provides image decoding services using stb_image.
 */

REFLECT_DECLARE_CLASS(STB_Image_Loader)

class STB_Image_Loader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(STB_Image_Loader)

public:

	STB_Image_Loader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Image)

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Image)

	// Decode an image file to a buffer.
	virtual bool decode(ResourceLoadOptions&) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "STB_IMAGE")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Images)
};

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif