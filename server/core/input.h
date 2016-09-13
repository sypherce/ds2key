#pragma once

#if defined(__linux__)
#include <X11/extensions/XTest.h>
#endif
#include <cstdint> // uint8_t, etc

// Performs Virtual Keyboard, Mouse, and Gamepad events
namespace D2K {namespace Input {

// Prepares the display for touch input functions (in linux)
void Init();

// Deletes each active VJoy joystick (in windows)
void DeInit();

// Sends a virtual key press and release, or a virtual gamepad button press and release
//  param: Key Platform specific key or gamepad button value
//  param: Joy (vjoy) device number, Value ranges 0-15, Ignored if (key) is not a gamepad button value
// TODO currently if NULL/0 is passed to 'joy' it'll still use joystick 0, This could be replaced with an invalid amount -1, 16? Probably use an value similar to const uint8 JOY_NONE=-1
// TODO also, we're using unsigned char, and uint8_t elsewhere I believe
void Tap(uint16_t key, unsigned char joy);

// Sends a virtual key press, or a virtual gamepad button press
//  param: Key Platform specific key or gamepad button value
//  param: Joy (vjoy) device number, Value ranges 1-16, or Input::Joystick::NO_JOYSTICK (0), Ignored if (key) is not a gamepad button value
void Press(uint16_t key, uint8_t joy);

// Sends a virtual key release, or a virtual gamepad button release
//  param: Key Platform specific key or gamepad button value
//  param: Joy (vjoy) device number, Value ranges 0-15, Ignored if (key) is not a gamepad button value
// TODO currently if NULL/0 is passed to 'joy' it'll still use joystick 0, This could be replaced with an invalid amount -1, 16? Probably use an value similar to const uint8 JOY_NONE=-1
// TODO also, we're using unsigned char, and uint8_t elsewhere I believe
void Release(uint16_t key, unsigned char joy);

// Moves cursor relative to it's current position
void Move(signed long int x, signed long int y);

// Moves cursor to (X), (Y)
void MoveAbsolute(signed long int x, signed long int y);

}} // namespace D2K::Input
