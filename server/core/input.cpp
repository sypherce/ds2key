/*
	Virtual input, includes Keyboard, Mouse, and Joystick support
*/

#ifdef WIN32
#include <windows.h>
#include "PPJioctl.h"
#endif//WIN32

#include "input.h"

namespace D2K
{
	namespace Core
	{
		Input::Input()
		{
		#ifdef WIN32
			for(int i = 0; i < 15; i++)
				ppjoy[i] = 0;
		#else//LINUX
			display = XOpenDisplay(NULL);
		#endif//WIN32
		}

		Input::~Input()
		{
		#ifdef WIN32
			for(int i = 0; i < 15; i++)
				if(ppjoy[i] != 0)
				{
					delete(ppjoy[i]);
					ppjoy[i] = 0;
				}
		#else//LINUX
		#endif//WIN32
		}

		void Input::Keyboard(uint16_t key, bool state)
		{
		#ifdef WIN32
			INPUT input;
			if(key == VK_LBUTTON || key == VK_RBUTTON || key == VK_MBUTTON)
			{
				input.type = INPUT_MOUSE;
				input.mi.dx = 0;
				input.mi.dy = 0;
				input.mi.dwExtraInfo = 0;
				input.mi.mouseData = 0;
				input.mi.time = 0;

				if(key == VK_LBUTTON)
				{
					if(state)
					{
						input.mi.dwFlags = MOUSEEVENTF_LEFTUP | MOUSEEVENTF_MOVE;
					}
					else
					{
						input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_MOVE;
					}
				}
				else if(key == VK_RBUTTON)
				{
					if(state)
					{
						input.mi.dwFlags = MOUSEEVENTF_RIGHTUP | MOUSEEVENTF_MOVE;
					}
					else
					{
						input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_MOVE;
					}
				}
				else if(key == VK_MBUTTON)
				{
					if(state)
					{
						input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP | MOUSEEVENTF_MOVE;
					}
					else
					{
						input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MOVE;
					}
				}
			}
			else
			{
				input.type = INPUT_KEYBOARD;
				input.ki.wVk = key;
				input.ki.dwFlags = KEYEVENTF_SCANCODE;

				if(isExtended(key))
				{
					input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
				}

				if(state)
				{
					input.ki.dwFlags |= KEYEVENTF_KEYUP;
				}

				input.ki.wScan = MapVirtualKey(key, 0);
				input.ki.time = 0;
				input.ki.dwExtraInfo = 0;
			}

			SendInput(1, (LPINPUT)&input, sizeof(INPUT));
		#else//LINUX
			int code = XKeysymToKeycode(display, key);
			XTestFakeKeyEvent(display, code, !state, 0);
			XFlush(display);
		#endif//WIN32
		}

		void Input::Mouse(unsigned short type, signed long int X, signed long int Y)
		{
		#ifdef WIN32
			INPUT input;

			input.type = INPUT_MOUSE;
			input.mi.dx = X;//-16 border
			input.mi.dy = Y;//-16 border
			input.mi.dwFlags = type | MOUSEEVENTF_MOVE;
			input.mi.dwExtraInfo = 0;
			input.mi.mouseData = 0;
			input.mi.time = 0;

			SendInput(1, (LPINPUT)&input, sizeof(INPUT));
		#endif//WIN32
		}

		void Input::Press(uint16_t key, unsigned char joy)
		{
			if(key >= 0x100 && key < 0x100 + NUM_DIGITAL)
			{
		#ifdef WIN32
				if(ppjoy[joy] == 0)
					ppjoy[joy] = new PPJoy(joy);
				ppjoy[joy]->SetButton(key - 0x100, 1);
				ppjoy[joy]->Update();
				//printf("pressing joy button %i\n", key - 0x100);
		#endif//WIN32
			}
			else
			{
				Keyboard(key, 0);
			}
		}

		void Input::Release(uint16_t key, unsigned char joy)
		{
			if(key >= 0x100 && key < 0x100 + NUM_DIGITAL)
			{
		#ifdef WIN32
				if(ppjoy[joy] == 0)
					ppjoy[joy] = new PPJoy(joy);
				ppjoy[joy]->SetButton(key - 0x100, 0);
				ppjoy[joy]->Update();
		#endif//WIN32
			}
			else
			{
				Keyboard(key, 1);
			}
		}

		void Input::Move(signed long int X, signed long int Y)
		{
			Mouse(0, X, Y);
		}

		void Input::MoveAbsolute(signed long int X, signed long int Y)
		{
		#ifdef WIN32
			Mouse(MOUSEEVENTF_ABSOLUTE, X, Y);
		#endif//WIN32
		}
	}
}

