// Virtual input, includes Keyboard, Mouse, and Joystick support

#ifdef _WIN32
#include <windows.h>
#include <winuser.h>
#include "PPJIoctl.h"
#include "key.h"
#elif defined __linux__
#define NUM_DIGITAL 20
#endif//_WIN32

#include "input.h"

namespace D2K {namespace Input {

#ifdef _WIN32
PPJoy* g_ppjoy[MAX_JOYSTICKS] = { };
#endif//_WIN32
#ifdef __linux__
Display* g_display;
#endif//__linux__

void Init()
{
#ifdef _WIN32
	for (int i = 0; i < MAX_JOYSTICKS; i++)
	{
		g_ppjoy[i] = 0;
	}
#endif//_WIN32
#ifdef __linux__
	g_display = XOpenDisplay(nullptr);
#endif//__linux__
}

void DeInit()
{
#ifdef _WIN32
	for(int i = 0; i < MAX_JOYSTICKS; i++)
		if(g_ppjoy[i] != 0)
		{
			delete(g_ppjoy[i]);
			g_ppjoy[i] = 0;
		}
#endif//_WIN32
}

enum KeyState
{
	pressed = false,
	released = true,
};

//Checks if (key) is an "extended" key
//@param key Platform specific value
//@return true if (key) is an "extended" key
bool IsExtended(uint16_t key)
{
	switch (key)
	{
	case VK_INSERT:
	case VK_DELETE:
	case VK_HOME:
	case VK_END:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_NUMLOCK:
	case VK_PAUSE:
	case VK_PRINT:
	case VK_DIVIDE:
	case VK_RCONTROL:
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		return true;
	default:
		return false;
	}
}

//Presses or releases (key) depending on (state)
//@param key Platform specific value
//@param state true = released, false = pressed
void Keyboard(uint16_t key, KeyState state)
{
	static uint16_t s_press_counter[65535] = { };//this allows 1 or more profile to press the same key, instead of going crazy
	if((s_press_counter[key] == 0 && state == KeyState::pressed)
		|| (s_press_counter[key] == 1 && state == KeyState::released))
	{
#ifdef _WIN32
		INPUT input;
		if(key == VK_LBUTTON || key == VK_RBUTTON || key == VK_MBUTTON)
		{
			input.type = INPUT_MOUSE;
			input.mi.dx = 0;
			input.mi.dy = 0;
			input.mi.dwExtraInfo = 0;
			input.mi.mouseData = 0;
			input.mi.time = 0;
			input.mi.dwFlags = MOUSEEVENTF_MOVE;

			if(key == VK_LBUTTON)
			{
				if(state == KeyState::released)
					input.mi.dwFlags |= MOUSEEVENTF_LEFTUP;
				else
					input.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
			}
			else if(key == VK_RBUTTON)
			{
				if(state == KeyState::released)
					input.mi.dwFlags |= MOUSEEVENTF_RIGHTUP;
				else
					input.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN;
			}
			else if(key == VK_MBUTTON)
			{
				if(state == KeyState::released)
					input.mi.dwFlags |= MOUSEEVENTF_MIDDLEUP;
				else
					input.mi.dwFlags |= MOUSEEVENTF_MIDDLEDOWN;
			}
		}
		else
		{
			input.type = INPUT_KEYBOARD;
			input.ki.wVk = key;
			input.ki.dwFlags = KEYEVENTF_SCANCODE;

			if(IsExtended(key))
				input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;

			if(state == KeyState::released)
				input.ki.dwFlags |= KEYEVENTF_KEYUP;

			input.ki.wScan = MapVirtualKey(key, 0);
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
		}

		SendInput(1, (LPINPUT)&input, sizeof(INPUT));
#elif defined __linux__
		int linux_state = (state != KeyState::released);//linux press values are the opposite of windows
		int code = XKeysymToKeycode(g_display, key);
		XTestFakeKeyEvent(g_display, code, linux_state, 0);
		XFlush(g_display);
#endif//_WIN32
    }

    //doesn't check boundaries
    if(state == false)
		s_press_counter[key]++;
    else
		s_press_counter[key]--;
}

//Moves cursor position
//@param type true = absolute, false = relative
void Mouse(bool type, signed long int x, signed long int y)
{
#ifdef _WIN32
	INPUT input;

	input.type = INPUT_MOUSE;
	input.mi.dx = x;//-16 border
	input.mi.dy = y;//-16 border
	input.mi.dwFlags = (type * MOUSEEVENTF_ABSOLUTE) | MOUSEEVENTF_MOVE;
	input.mi.dwExtraInfo = 0;
	input.mi.mouseData = 0;
	input.mi.time = 0;

	SendInput(1, (LPINPUT)&input, sizeof(INPUT));
#elif defined __linux__
	Window dummyWin;
	unsigned int dummyInt;
	unsigned int width, height;

	int screen = DefaultScreen(g_display);
	Window rootwindow = RootWindow(g_display, screen);

	if(XGetGeometry(g_display, rootwindow, &dummyWin, &dummyInt, &dummyInt, &width, &height, &dummyInt, &dummyInt))
	{
		if(type)
			XTestFakeMotionEvent(g_display, screen, (m_x * width) / 65535, (m_y * height) / 65535, 0);
		else
			XTestFakeRelativeMotionEvent(g_display, m_x, m_y, 0);
	}
#endif//_WIN32
}

void Press(uint16_t key, unsigned char joy)
{
	if(key >= KEY_JOY && key < KEY_JOY + NUM_DIGITAL) //virtual gamepad buttons
	{
#ifdef _WIN32
		if(g_ppjoy[joy] == 0)
			g_ppjoy[joy] = new PPJoy(joy);
		g_ppjoy[joy]->SetButton(key - KEY_JOY, 1);
		g_ppjoy[joy]->Update();
#endif//_WIN32
	}
	else
	{
		Keyboard(key, KeyState::pressed);
	}
}

void Release(uint16_t key, unsigned char joy)
{
	if(key >= KEY_JOY && key < KEY_JOY + NUM_DIGITAL) //virtual gamepad buttons
	{
#ifdef _WIN32
		if(g_ppjoy[joy] == 0)
			g_ppjoy[joy] = new PPJoy(joy);
		g_ppjoy[joy]->SetButton(key - KEY_JOY, 0);
		g_ppjoy[joy]->Update();
#endif//_WIN32
	}
	else
	{
		Keyboard(key, KeyState::released);
	}
}

void Move(signed long int x, signed long int y)
{
	Mouse(false, x, y);
}

void MoveAbsolute(signed long int x, signed long int y)
{
	Mouse(true, x, y);
}

}}//namespace D2K::Input