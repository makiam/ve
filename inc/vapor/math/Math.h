/************************************************************************
*
* vaporEngine (2008-2009)
*
************************************************************************/

#pragma once

#include <cmath>

namespace vapor {
	namespace math {

//-----------------------------------//

using ::sinf;
using ::cosf;
using ::tanf;
using ::sqrt;

//-----------------------------------//

const float PI = 3.141592654f;

//-----------------------------------//

float degreeToRadian( float degree );

//-----------------------------------//

template< typename T >
void limit( T& var, const T min, const T max )
{
	if( var >= max )
		var = max;
	else if( var <= min )
		var = min;
}

//-----------------------------------//

} } // end namespaces