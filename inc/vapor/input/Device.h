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
 * Different types of input devices.
 */

namespace DeviceType
{
	enum Enum
	{
		Keyboard,
		Mouse,
		Joystick
	};

	std::string getString( const DeviceType::Enum dt );
}

//-----------------------------------//

/**
 * Base struct for input events.
 */

struct VAPOR_API Event : private boost::noncopyable
{
	Event( DeviceType::Enum );

	DeviceType::Enum deviceType;
};

//-----------------------------------//

/**
 * Input device.
 */

class VAPOR_API Device : private boost::noncopyable
{
public:

	Device();
	virtual ~Device();

	// Processes an event.
	virtual void processEvent( const Event& event ) = 0;

	// Gets the type of this device.
	virtual const DeviceType::Enum getType() const = 0;
};

TYPEDEF_PTR(Device)

//-----------------------------------//

} // end namespace