/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/input/Device.h"

namespace vapor {
	namespace input {

//-----------------------------------//

/**
 * Different types of joystick axis.
 */

namespace JoystickAxis
{
	enum Enum
	{
		AxisX,
		AxisY,
		AxisZ,
		AxisR,
		AxisU,
		AxisV,
		AxisPOV,
	};
}

//-----------------------------------//

/**
 * Different types of joystick events.
 */

namespace JoystickEventType
{
	enum Enum
	{
		JoystickPress,
		JoystickRelease,
		JoystickMove
	};
}

//-----------------------------------//

/**
 * Generic event for joystick actions.
 */

struct JoystickEvent : public input::Event
{
	friend class Joystick;

	JoystickEvent( JoystickEventType::Enum eventType );

private:

	JoystickEventType::Enum eventType;
};

//-----------------------------------//

/**
 * Event that happens when a joystick is moved.
 */

struct JoyMoveEvent: public JoystickEvent
{
	JoyMoveEvent(uint JoystickId, JoystickAxis::Enum Axis, 
		float Position);
	
	uint JoystickId;
	JoystickAxis::Enum Axis;
	float Position;
};

//-----------------------------------//

/**
 * Event that happens when a joystick button is pressed or released.
 */

struct JoyButtonEvent: public JoystickEvent
{	
	JoyButtonEvent(uint JoystickId, uint Button, 
		JoystickEventType::Enum eventType);

	uint JoystickId;
	uint Button;
};

//-----------------------------------//

class Joystick : public Device
{
	Joystick();
	virtual ~Joystick();
	
	// Processes an event (only if it's a joystick event).
	virtual void processEvent( const input::Event& event );

	// Return this device as a joystick.
	virtual const input::DeviceType::Enum getType();

	//-----------------------------------//
	// Events
	//-----------------------------------//

	// Occurs when the joystick is moved.
	fd::delegate< void( const JoyMoveEvent& ) > onJoystickMove;
	
	// Occurs when a joystick button is pressed.
	fd::delegate< void( const JoyButtonEvent& ) > onJoystickButtonPress;
	
	// Occurs when a joystick button is released.
	fd::delegate< void( const JoyButtonEvent& ) > onJoystickButtonRelease;
	

private:

	// Private event handlers.
	void joyButtonPressed(const JoyButtonEvent& jevt);
	void joyButtonReleased(const JoyButtonEvent& jevt);
	void joyMoved(const JoyMoveEvent& jevt);
};

//-----------------------------------//

} } // end namespaces