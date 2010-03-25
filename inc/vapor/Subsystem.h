/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor {

//-----------------------------------//

/**
 * Subsystems are managed by the engine. They are updated once in a while
 * (the subsystem can control the update rate) and are used mostly to 
 * implement the main services provided by the engine.
 */

class VAPOR_API Subsystem : private boost::noncopyable
{
	friend class Engine;

protected:

	Subsystem() { }
	virtual ~Subsystem() { }
	
	virtual void update( double ) = 0;
};

//-----------------------------------//

} // end namespace