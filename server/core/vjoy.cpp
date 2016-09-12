//Joystick emulation for windows

#include "common/easylogging++Wrapper.h"

#ifdef _WIN32
#include "VJoy.h"
#include "vjoy/inc/vjoyinterface.h"
#endif

namespace D2K {namespace Input {namespace Joystick {

//We use MAX_JOYSTICKS+1 because vJoy's joysticks are 1-based.
//joystick_position[0] is unused, but it makes the rest of the
//code not need joystick_position[device+1].
#ifdef _WIN32
JOYSTICK_POSITION joystick_position[MAX_JOYSTICKS+1]{};
#endif
bool hat_up[MAX_JOYSTICKS+1]{};
bool hat_down[MAX_JOYSTICKS+1]{};
bool hat_left[MAX_JOYSTICKS+1]{};
bool hat_right[MAX_JOYSTICKS+1]{};
//TODO: function vjoyinterface.h::GetVJDAxisMax
const LONG MAX_AXIS_VALUE = 32767;
const DWORD CONTINUOUS_UP = 0;
const DWORD CONTINUOUS_UP_RIGHT = 4500;
const DWORD CONTINUOUS_RIGHT = 9000;
const DWORD CONTINUOUS_DOWN_RIGHT = 13500;
const DWORD CONTINUOUS_DOWN = 18000;
const DWORD CONTINUOUS_DOWN_LEFT = 22500;
const DWORD CONTINUOUS_LEFT = 27000;
const DWORD CONTINUOUS_UP_LEFT = 31500;
const DWORD CONTINUOUS_NEUTRAL = 0xFFFFFFFF;

bool IsIDValid(uint8_t device)
{
#ifdef _WIN32
	if(device == 0 || device > MAX_JOYSTICKS)
		return false;
	return true;
#else
	return false;
#endif
}

//return false if successful
bool Init(uint8_t device)
{
	// If Device ID is Invalid, return failure
	if(!IsIDValid(device))
	{
		if(device > 0)
			LOG(ERROR) << "vJoy device " << (int)device << " out of valid range. (1-" << (int)MAX_JOYSTICKS << ")";

		return true;
	}

	// If Device is Active, return success
	if(IsActive(device))
		return false;

#ifdef _WIN32
	// Get the driver attributes (Vendor ID, Product ID, Version Number)
	if(!vJoyEnabled())
	{
		LOG(ERROR) << "Function vJoyEnabled Failed - make sure that vJoy is installed and enabled";
		for(uint8_t i = 1; i <= MAX_JOYSTICKS; i++)
			DeInit(i);
		return true;
	}

	// Get the status of the vJoy device before trying to acquire it
	VjdStat status = GetVJDStatus(device);

	switch(status)
	{
	case VJD_STAT_OWN:
		LOG(INFO) << "vJoy device " << (int)device << " is already owned by this feeder";
		break;
	case VJD_STAT_FREE:
		LOG(INFO) << "vJoy device " << (int)device << " is free";
		break;
	case VJD_STAT_BUSY:
		LOG(ERROR) << "vJoy device " << (int)device << " is already owned by another feeder\nCannot continue";
		return true;
	case VJD_STAT_MISS:
		LOG(ERROR) << "vJoy device " << (int)device << " is not installed or disabled\nCannot continue";
		return true;
	default:
		LOG(ERROR) << "vJoy device " << (int)device << " general error\nCannot continue";
		return true;
	};

	// Acquire the vJoy device
	if(!AcquireVJD(device))
	{
		LOG(ERROR) << "Failed to acquire vJoy device number " << (int)device << ".";
		return true;
	}
	else
		LOG(INFO) << "Acquired device number " << (int)device << " - OK";

	joystick_position[device].bDevice = device;

//TODO: function vjoyinterface.h::ResetAll();
	// Reset all buttons
	joystick_position[device].lButtons = 0;
	// Reset dpad
	for(uint8_t hat = 0; hat <= 3; hat++)
		SetHat(device, hat, false);

	// Reset all axis
	for(uint8_t axis = HID_USAGE_X; axis <= HID_USAGE_WHL; axis++)
		SetAxisPercent(device, axis, 50);

	// Actually perform all releases
	Update(device);
#endif

	return false;
}

bool DeInit(uint8_t device)
{
	if(!IsIDValid(device))
		return false;

#ifdef _WIN32
	if(!IsActive(device))
	{
		std::clog << "vJoy device " << (int)device << " is already inactive\n";
		return false;
	}

	// Reset all buttons
	joystick_position[device].lButtons = 0;
	// Reset dpad
	for(uint8_t hat = 0; hat <= 3; hat++)
		SetHat(device, hat, false);

	// Reset all axis
	for(uint8_t axis = HID_USAGE_X; axis <= HID_USAGE_WHL; axis++)
		SetAxisPercent(device, axis, 50);

	// Actually perform all releases
	Update(device);

	// Then we actually destroy the joystick
	joystick_position[device].bDevice = 0;
	RelinquishVJD(device);
#endif

	return true;
}


//Returns true if device is active
bool IsActive(uint8_t device)
{
	if(!IsIDValid(device))
		return false;

#ifdef _WIN32
	return joystick_position[device].bDevice == device;
#else
	return false;
#endif
}

//Returns true if device updated
int Update(uint8_t device)
{
#ifdef _WIN32
	if(!IsActive(device))
		return false;

	bool return_status = UpdateVJD(device, (void*)&joystick_position[device]) != 0;
	if(!return_status)
	{
		LOG(ERROR) << "Feeding vJoy device number " << (int)device << " failed";
		DeInit(device);
	}

	return return_status;
#else
	return false;
#endif
}
void SetButton(uint8_t device, uint8_t button, bool value)
{
#ifdef _WIN32
	if(Init(device) != 0)
		return;

	if(value) //press
		joystick_position[device].lButtons |= 1 << button;
	else      //release
		joystick_position[device].lButtons &= ~(1 << button);
#endif
}
void UpdateHat(uint8_t device)
{
#ifdef _WIN32
	if(Init(device) != 0)
		return;

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
#endif
}
void SetHat(uint8_t device, uint8_t hat, bool value)
{
#ifdef _WIN32
	if(Init(device) != 0)
		return;

	if(hat == 0)
		hat_up[device] = value;
	else if(hat == 1)
		hat_down[device] = value;
	else if(hat == 2)
		hat_left[device] = value;
	else if(hat == 3)
		hat_right[device] = value;
	else
	{
		LOG(ERROR) << "vJoy device " << (int)device << " invalid hat value of: " << (int)hat;
		return;
	}

	UpdateHat(device);
#endif
}
void SetAxisPercent(uint8_t device, uint8_t axis, uint8_t value)
{
#ifdef _WIN32
	static LONG percent_axis_scale = MAX_AXIS_VALUE / 100;

	if(Init(device) != 0)
		return;

	value = value > 100 ? 100 : value;

	SetAxisRaw(device, axis, percent_axis_scale * value);
#endif
}
void SetAxisRaw(uint8_t device, uint8_t axis, LONG value)
{
#ifdef _WIN32
	if(Init(device) != 0)
		return;

	switch(axis)
	{
	case HID_USAGE_X:   // X Axis
		joystick_position[device].wAxisX = value;
		break;
	case HID_USAGE_Y:   // Y Axis
		joystick_position[device].wAxisY = value;
		break;
	case HID_USAGE_Z:   // Z Axis
		joystick_position[device].wAxisZ = value;
		break;
	case HID_USAGE_RX:  // Rx Axis
		joystick_position[device].wAxisXRot = value;
		break;
	case HID_USAGE_RY:  // Ry Axis
		joystick_position[device].wAxisYRot = value;
		break;
	case HID_USAGE_RZ:  // Rz Axis
		joystick_position[device].wAxisZRot = value;
		break;
	case HID_USAGE_SL0: // Slider 0
		joystick_position[device].wSlider = value;
		break;
	case HID_USAGE_SL1: // Slider 1
		joystick_position[device].wDial = value;
		break;
	case HID_USAGE_WHL: // Wheel
		joystick_position[device].wWheel = value;
		break;
	default:
		break;
	}
#endif
}

void SetAxisSignedMax(uint8_t device, uint8_t axis, LONG value, LONG max)
{
#ifdef _WIN32
	if(Init(device) != 0)
		return;

	double axis_scale = MAX_AXIS_VALUE / (max * 2);

	value = value < -max ? -max :
	       (value >  max ?  max :
	        value);
	value += max;
	value = (LONG)(value * axis_scale);

	SetAxisRaw(device, axis, value);
#endif
}
bool GetButton(uint8_t device, uint8_t button)
{
	if(!IsIDValid(device))
		return false;

#ifdef _WIN32
	if(!IsActive(device))
		return false;

	uint16_t bit_shift_button = 1 << button;
	return (joystick_position[device].lButtons&bit_shift_button) != 0;
#else
	return false;
#endif
}

}}}//namespace D2K::Input
