/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//// 
//// fuckEngine by triton
////
//// License: fuckGNU License
////
//// Changelog:
////	(18/08/08) Initial implementantion
////
//// Thanks to:
////	Zero (love ya bitch)
////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////
// System headers
/////////////////////////////////////////////////////////////////////////////

#include <string>
#include <cstdio>
#include <cassert>
#include <vector>

#include <vapor/Types.h>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// Engine information
/////////////////////////////////////////////////////////////////////////////

static const string ENGINE_COMPILE_DATE = __DATE__;
static const string ENGINE_NAME = "vaporEngine";
static const string ENGINE_AUTHOR = "triton";
static const string ENGINE_VERSION = "0.123";

/////////////////////////////////////////////////////////////////////////////
// Engine headers
/////////////////////////////////////////////////////////////////////////////

#include "Resource.h"
#include "Color.h"
#include "Font.h"
#include "Singleton.h"
#include "Log.h"
#include "Window.h"
#include "SDL_Window.h"
#include "Math.h"
#include "Core.h"
#include "PNG.h"
#include "Milkshape3D.h"
#include "RenderQueue.h"