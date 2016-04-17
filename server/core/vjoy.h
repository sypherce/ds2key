#pragma once

#include <windows.h>
#include <stdint.h>
#include "vjoy/inc/public.h"

namespace D2K {namespace Input { namespace Joystick {
const uint8_t MAX_JOYSTICKS = 4;
extern JOYSTICK_POSITION joystick_position[MAX_JOYSTICKS+1];

extern bool Init(uint8_t device);
extern bool DeInit(uint8_t device);
extern bool IsActive(uint8_t device);
extern int Update(uint8_t device);
extern void SetButton(uint8_t device, uint8_t button, bool value);
extern void SetAxisPercent(uint8_t device, uint8_t axis, uint8_t value);
extern bool GetButton(uint8_t device, uint8_t button);


}}}//namespace D2K::Input::Joystick
