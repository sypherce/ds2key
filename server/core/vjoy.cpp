//Joystick emulation for windows

#include <iostream>//std::cout, std::clog
#include "VJoy.h"
#include "vjoy/inc/vjoyinterface.h"

namespace D2K {namespace Input {namespace Joystick {

//We use MAX_JOYSTICKS+1 because vJoy's joysticks are 1-based.
//joystick_position[0] is unused, but it makes the rest of the
//code not need joystick_position[device+1].
JOYSTICK_POSITION joystick_position[MAX_JOYSTICKS+1]{ };
const int CONTINUOUS_UP = 0;
const int CONTINUOUS_UP_RIGHT = 4500;
const int CONTINUOUS_RIGHT = 9000;
const int CONTINUOUS_DOWN_RIGHT = 13500;
const int CONTINUOUS_DOWN = 18000;
const int CONTINUOUS_DOWN_LEFT = 22500;
const int CONTINUOUS_LEFT = 27000;
const int CONTINUOUS_UP_LEFT = 31500;
const int CONTINUOUS_NEUTRAL = -1;
bool hat_up[MAX_JOYSTICKS+1]{ };
bool hat_down[MAX_JOYSTICKS+1]{ };
bool hat_left[MAX_JOYSTICKS+1]{ };
bool hat_right[MAX_JOYSTICKS+1]{ };

//return false/0 if successful
bool Init(uint8_t device)
{
	// Get the driver attributes (Vendor ID, Product ID, Version Number)
	if(!vJoyEnabled())
	{
		std::clog << "Function vJoyEnabled Failed - make sure that vJoy is installed and enabled\n";
		for(int i = 1; i <= MAX_JOYSTICKS; i++)
			DeInit(i);
		return true;
	}

	// Get the status of the vJoy device before trying to acquire it
	VjdStat status = GetVJDStatus(device);

	switch(status)
	{
	case VJD_STAT_OWN:
		std::clog << "vJoy device " << device << " is already owned by this feeder\n";
		break;
	case VJD_STAT_FREE:
		std::clog << "vJoy device " << device << " is free\n";
		break;
	case VJD_STAT_BUSY:
		std::clog << "vJoy device " << device << " is already owned by another feeder\nCannot continue\n";
		return true;
	case VJD_STAT_MISS:
		std::clog << "vJoy device " << device << " is not installed or disabled\nCannot continue\n";
		return true;
	default:
		std::clog << "vJoy device " << device << " general error\nCannot continue\n";
		return true;
	};

	// Acquire the vJoy device
	if(!AcquireVJD(device))
	{
		std::clog << "Failed to acquire vJoy device number " << device << ".\n";
		return true;
	}
	else
		std::clog << "Acquired device number " << device << " - OK\n";

	joystick_position[device].bDevice = device;

	return false;
}

bool DeInit(uint8_t device)
{
	if(!IsActive(device))
	{
		std::clog << "vJoy device " << device << " is already inactive\n";
		return false;
	}
	
	joystick_position[device].bDevice = 0;
	RelinquishVJD(device);

	return true;
}


bool IsActive(uint8_t device)
{
	return joystick_position[device].bDevice == device;
}

//Returns true if device updated
int Update(uint8_t device)
{
	if(!IsActive(device))
		return false;

	bool return_status = UpdateVJD(device, (void*)&joystick_position[device]) != 0;
	if(!return_status)
	{
		std::clog << "Feeding vJoy device number " << device << " failed\n";
		DeInit(device);
	}

	return return_status;
}
void SetButton(uint8_t device, uint8_t button, bool value)
{
	if(!IsActive(device)
	&&  Init(device))
		return;

	if(value) //press
		joystick_position[device].lButtons |= 1 << button;
	else      //release
		joystick_position[device].lButtons &= ~(1 << button);
}
void SetHat(uint8_t device, uint8_t hat, bool value)
{	
	if(!IsActive(device)
	&&  Init(device))
		return;

	if(hat >= 4)
	{
		std::clog << "vJoy device " << device << " invalid hat value of: " << hat << "\n";
		return;
	}
	
	if(hat == 0)
		hat_up[device] = value;
	else if(hat == 1)
		hat_down[device] = value;
	else if(hat == 2)
		hat_left[device] = value;
	else if(hat == 3)
		hat_right[device] = value;

	UpdateHat(device);
}
void UpdateHat(uint8_t device)
{
	if     ( hat_up[device] && !hat_down[device] && !hat_left[device] && !hat_right[device])
		joystick_position[device].bHats = CONTINUOUS_UP;
	else if( hat_up[device] && !hat_down[device] && !hat_left[device] &&  hat_right[device])
		joystick_position[device].bHats = CONTINUOUS_UP_RIGHT;
	else if(!hat_up[device] && !hat_down[device] && !hat_left[device] &&  hat_right[device])
		joystick_position[device].bHats = CONTINUOUS_RIGHT;
	else if(!hat_up[device] &&  hat_down[device] && !hat_left[device] &&  hat_right[device])
		joystick_position[device].bHats = CONTINUOUS_DOWN_RIGHT;
	else if(!hat_up[device] &&  hat_down[device] && !hat_left[device] && !hat_right[device])
		joystick_position[device].bHats = CONTINUOUS_DOWN;
	else if(!hat_up[device] &&  hat_down[device] &&  hat_left[device] && !hat_right[device])
		joystick_position[device].bHats = CONTINUOUS_DOWN_LEFT;
	else if(!hat_up[device] && !hat_down[device] &&  hat_left[device] && !hat_right[device])
		joystick_position[device].bHats = CONTINUOUS_LEFT;
	else if( hat_up[device] && !hat_down[device] &&  hat_left[device] && !hat_right[device])
		joystick_position[device].bHats = CONTINUOUS_UP_LEFT;
	else
		joystick_position[device].bHats = CONTINUOUS_NEUTRAL;
}
void SetAxisPercent(uint8_t device, uint8_t axis, uint8_t value)
{
	static LONG percent_axis_scale = 8000 / 100;
	
	if(!IsActive(device)
	&&  Init(device))
		return;

	LONG percent = percent_axis_scale * axis;
	switch(axis)
	{
	case HID_USAGE_X:   // X Axis
		joystick_position[device].wAxisX = percent;
		break;
	case HID_USAGE_Y:   // Y Axis
		joystick_position[device].wAxisY = percent;
		break;
	case HID_USAGE_Z:   // Z Axis
		joystick_position[device].wAxisZ = percent;
		break;
	case HID_USAGE_RX:  // Rx Axis
		joystick_position[device].wAxisXRot = percent;
		break;
	case HID_USAGE_RY:  // Ry Axis
		joystick_position[device].wAxisYRot = percent;
		break;
	case HID_USAGE_RZ:  // Rz Axis
		joystick_position[device].wAxisZRot = percent;
		break;
	case HID_USAGE_SL0: // Slider 0
		joystick_position[device].wSlider = percent;
		break;
	case HID_USAGE_SL1: // Slider 1
		joystick_position[device].wDial = percent;
		break;
	case HID_USAGE_WHL: // Wheel
		joystick_position[device].wWheel = percent;
		break;
	default:
		break;
	}
}
bool GetButton(uint8_t device, uint8_t button)
{
	if(!IsActive(device))
		return false;

	uint16_t bit_shift_button = 1 << button;
	return (joystick_position[device].lButtons&bit_shift_button) != 0;
}

}}}//namespace D2K::Input
