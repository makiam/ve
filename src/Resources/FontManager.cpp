/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef ENABLE_FONT_FREETYPE2

#include "Resources/FontManager.h"

namespace vapor {

//-----------------------------------//

FontManager::FontManager()
{
	if(initFT()) 
	{
		info("ttf", "Initialized FreeType %d.%d.%d", 
			FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH);
	} else {
		Log::error("Failed to initialize FreeType");
		exit(EXIT_FAILURE);
	}
}

//-----------------------------------//

FontManager::~FontManager()
{
	if(FT_Done_FreeType(ft) != 0) 
	{
		Log::warn("Failed to destroy FreeType");
	}
}

//-----------------------------------//

bool FontManager::initFT() 
{
	// initialize FreeType library
	if(FT_Init_FreeType(&ft) == 0)
		return true;
	else
		return false;
}

//-----------------------------------//

} // end namespace

#endif
