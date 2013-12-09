#ifndef __INPUT_H__
#define __INPUT_H__

#ifdef _WIN32
#include "PPJoy.h"
#elif defined __linux__
#include <X11/extensions/XTest.h>
#endif//_WIN32
#include <cstdint>//uint8_t, etc

namespace D2K {
	namespace Core {
		namespace C {
			//Performs Virtual Keyboard, Mouse, and Gamepad events
			class Input {
				public:
					//Assigns each ppjoy pointer to NULL in windows
					Input();
					
					//Deletes each active ppjoy pointer in windows
					~Input();
					
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

				private:
			#ifdef _WIN32
					PPJoy *ppjoy[16];
			#elif defined __linux__
					Display *display;
			#endif//_WIN32

					//Presses or releases (key) depending on (state)
					//@param Key Platform specific value
					//@param State true = released, false = pressed
					void Keyboard(uint16_t Key, bool State);

					//Moves cursor position
					//@param Type true = absolute, false = relative
					void Mouse(bool Type, signed long int X, signed long int Y);
			};
		}
		extern C::Input *Input;
	}
}
#endif//__UDP_H__
