#ifndef __INPUT_H__
#define __INPUT_H__

#ifdef _WIN32
#include "PPJoy.h"
#elif defined __linux__
#include <X11/extensions/XTest.h>
#endif//_WIN32
#include <cstdint>//uint8_t, etc

namespace D2K {
	//Performs Virtual Keyboard, Mouse, and Gamepad events
	namespace Input {
		//Assigns each ppjoy pointer to NULL in windows
		void Init();

		//Deletes each active ppjoy pointer in windows
		void DeInit();

		//Sends a virtual key press, or a virtual gamepad button press
		//@param Key Platform specific key or gamepad button value
		//@param Joy (ppjoy) device number. Value ranges 0-15. Ignored if (key) is not a gamepad button value.
		void Press(uint16_t Key, unsigned char Joy);

		//Sends a virtual key release, or a virtual gamepad button release
		//@param Key Platform specific key or gamepad button value
		//@param Joy (ppjoy) device number. Value ranges 0-15. Ignored if (key) is not a gamepad button value.
		void Release(uint16_t Key, unsigned char Joy);

		//Moves cursor relative to it's current position
		void Move(signed long int X, signed long int Y);

		//Moves cursor to (X), (Y)
		void MoveAbsolute(signed long int X, signed long int Y);
	}
}
#endif//__UDP_H__
