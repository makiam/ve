/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_AUDIO_OPENAL

#ifdef PLATFORM_MACOSX
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#else
	#include <al.h>
	#include <alc.h>
#endif

#endif