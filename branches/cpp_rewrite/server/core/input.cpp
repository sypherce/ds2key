/*
	Virtual input, includes Keyboard, Mouse, and Joystick support
*/

#ifdef _WIN32
#include <windows.h>
#include <winuser.h>
#include "PPJIoctl.h"
#elif defined __linux__
#define NUM_DIGITAL 20
#endif//_WIN32

#include "input.h"

#define isExtended(key) \
((key == VK_INSERT) || \
(key == VK_DELETE) || \
(key == VK_HOME) || \
(key == VK_END) || \
(key == VK_PRIOR) || \
(key == VK_NEXT) || \
(key == VK_NUMLOCK) || \
(key == VK_PAUSE) || \
(key == VK_PRINT) || \
(key == VK_DIVIDE) || \
(key == VK_RCONTROL) || \
(key == VK_UP) || \
(key == VK_DOWN) || \
(key == VK_LEFT) || \
(key == VK_RIGHT))

namespace D2K {
	namespace Input {
		//private
		#ifdef _WIN32
		PPJoy *ppjoy[MAX_JOYSTICKS] = {0};
		#elif defined __linux__
		Display *display;
		#endif//_WIN32

		void Init() {
		#ifdef _WIN32
			for(int i = 0; i < MAX_JOYSTICKS; i++)
				ppjoy[i] = 0;
		#elif defined __linux__
			display = XOpenDisplay(NULL);
		#endif//_WIN32
		}

		void DeInit() {
		#ifdef _WIN32
			for(int i = 0; i < MAX_JOYSTICKS; i++)
				if(ppjoy[i] != 0) {
					delete(ppjoy[i]);
					ppjoy[i] = 0;
				}
		#endif//_WIN32
		}

		//Presses or releases (key) depending on (state)
		//@param Key Platform specific value
		//@param State true = released, false = pressed
		void Keyboard(uint16_t Key, bool State) {
		#ifdef _WIN32
			INPUT input;
			if(Key == VK_LBUTTON || Key == VK_RBUTTON || Key == VK_MBUTTON) {
				input.type = INPUT_MOUSE;
				input.mi.dx = 0;
				input.mi.dy = 0;
				input.mi.dwExtraInfo = 0;
				input.mi.mouseData = 0;
				input.mi.time = 0;

				if(Key == VK_LBUTTON) {
					if(State)
						input.mi.dwFlags = MOUSEEVENTF_LEFTUP | MOUSEEVENTF_MOVE;
					else
						input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_MOVE;
				}
				else if(Key == VK_RBUTTON) {
					if(State)
						input.mi.dwFlags = MOUSEEVENTF_RIGHTUP | MOUSEEVENTF_MOVE;
					else
						input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_MOVE;
				}
				else if(Key == VK_MBUTTON) {
					if(State)
						input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP | MOUSEEVENTF_MOVE;
					else
						input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MOVE;
				}
			}
			else {
				input.type = INPUT_KEYBOARD;
				input.ki.wVk = Key;
				input.ki.dwFlags = KEYEVENTF_SCANCODE;

				if(isExtended(Key))
					input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;

				if(State)
					input.ki.dwFlags |= KEYEVENTF_KEYUP;

				input.ki.wScan = MapVirtualKey(Key, 0);
				input.ki.time = 0;
				input.ki.dwExtraInfo = 0;
			}

			SendInput(1, (LPINPUT)&input, sizeof(INPUT));
		#elif defined __linux__
			int code = XKeysymToKeycode(display, Key);
			XTestFakeKeyEvent(display, code, !State, 0);
			XFlush(display);
		#endif//_WIN32
		}

		//Moves cursor position
		//@param Type true = absolute, false = relative
		void Mouse(bool Type, signed long int X, signed long int Y) {
		#ifdef _WIN32
			INPUT input;
			int absolute = false;
			if(Type != 0)
				absolute = MOUSEEVENTF_ABSOLUTE;

			input.type = INPUT_MOUSE;
			input.mi.dx = X;//-16 border
			input.mi.dy = Y;//-16 border
			input.mi.dwFlags = absolute | MOUSEEVENTF_MOVE;
			input.mi.dwExtraInfo = 0;
			input.mi.mouseData = 0;
			input.mi.time = 0;

			SendInput(1, (LPINPUT)&input, sizeof(INPUT));
		#elif defined __linux__
			Window dummyWin;
			int dummyInt;
			unsigned int width, height;

			int screen = DefaultScreen(display);
			Window rootwindow = RootWindow(display, screen);

			if(XGetGeometry(display, rootwindow, &dummyWin, &dummyInt, &dummyInt, &width, &height, (unsigned int*)&dummyInt, (unsigned int*)&dummyInt)) {
				if(!Type)
					XTestFakeRelativeMotionEvent(display, X, Y, 0);
				else
					XTestFakeMotionEvent(display, screen, (X * width) / 65535, (Y * height) / 65535, 0);
			}
		#endif//_WIN32
		}

		void Press(uint16_t Key, unsigned char Joy) {
			if(Key >= 0x100 && Key < 0x100 + NUM_DIGITAL) {//0x100 to (0x100 + NUM_DIGITAL) are virtual gamepad buttons
		#ifdef _WIN32
				if(ppjoy[Joy] == 0)
					ppjoy[Joy] = new PPJoy(Joy);
				ppjoy[Joy]->SetButton(Key - 0x100, 1);
				ppjoy[Joy]->Update();
		#endif//_WIN32
			}
			else
				Keyboard(Key, 0);
		}

		void Release(uint16_t Key, unsigned char Joy) {
			if(Key >= 0x100 && Key < 0x100 + NUM_DIGITAL) {//0x100 to (0x100 + NUM_DIGITAL) are virtual gamepad buttons
		#ifdef _WIN32
				if(ppjoy[Joy] == 0)
					ppjoy[Joy] = new PPJoy(Joy);
				ppjoy[Joy]->SetButton(Key - 0x100, 0);
				ppjoy[Joy]->Update();
		#endif//_WIN32
			}
			else
				Keyboard(Key, 1);
		}

		void Move(signed long int X, signed long int Y) {
			Mouse(false, X, Y);
		}

		void MoveAbsolute(signed long int X, signed long int Y) {
			Mouse(true, X, Y);
		}
	}
}
