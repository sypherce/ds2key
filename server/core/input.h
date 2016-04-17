#pragma once

#if defined(__linux__)
#include <X11/extensions/XTest.h>
#endif
#include <cstdint>//uint8_t, etc

//Performs Virtual Keyboard, Mouse, and Gamepad events
namespace D2K {namespace Input {

//Prepares the display for touch input functions (in linux)
void Init();

//Deletes each active VJoy joystick (in windows)
void DeInit();

//Sends a virtual key press, or a virtual gamepad button press
//@param Key Platform specific key or gamepad button value
//@param Joy (vjoy) device number. Value ranges 0-15. Ignored if (key) is not a gamepad button value.
void Press(uint16_t key, unsigned char joy);

//Sends a virtual key release, or a virtual gamepad button release
//@param Key Platform specific key or gamepad button value
//@param Joy (ppjoy) device number. Value ranges 0-15. Ignored if (key) is not a gamepad button value.
void Release(uint16_t key, unsigned char joy);

//Moves cursor relative to it's current position
void Move(signed long int x, signed long int y);

//Moves cursor to (X), (Y)
void MoveAbsolute(signed long int x, signed long int y);

}}//namespace D2K::Input
