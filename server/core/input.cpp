// Virtual input, includes Keyboard, Mouse, and Joystick support

#ifdef _WIN32
#ifdef __GNUC__
#define _WIN32_WINNT 0x0500
#endif
#include <windows.h>
#include <winuser.h>
#include <iostream>//std::cout, std::clog
#include "vjoy.h"
#endif
#include "key.h"

#include "input.h"

namespace D2K {namespace Input {

#ifdef __linux__
Display* g_display;
#endif

void Init()
{
#ifdef __linux__
	g_display = XOpenDisplay(nullptr);
#endif
}

void DeInit()
{
#ifdef _WIN32
	for(uint8_t i = 1; i < Joystick::MAX_JOYSTICKS; i++)
		if(Joystick::IsActive(i))
			Joystick::DeInit(i);
#endif
}

//TODO check and verify for every extended key
//Checks if (key) is an "extended" key
//@param key Platform specific value
//@return true if (key) is an "extended" key
bool IsExtended(uint16_t key)
{
	switch(key)
	{
#ifdef _WIN32
	case VK_INSERT:
	case VK_DELETE:
	case VK_HOME:
	case VK_END:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_NUMLOCK:
	case VK_PRINT:
	case VK_SCROLL:
	case VK_PAUSE:
	case VK_DIVIDE:
	case VK_LMENU:
	case VK_RMENU:
	case VK_LWIN:
	case VK_RWIN:
	case VK_LCONTROL:
	case VK_RCONTROL:
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
	case VK_MEDIA_NEXT_TRACK:
	case VK_MEDIA_PLAY_PAUSE:
	case VK_MEDIA_PREV_TRACK:
	case VK_MEDIA_STOP:
	//next are untested
	case VK_EXECUTE:
	case VK_SNAPSHOT:
	case VK_APPS:
		return true;
#endif
	default:
		return false;
	}
}

enum KeyState
{
	pressed = false,
	released = true,
};
enum MouseMovement
{
	relative = false,
	absolute = true,
};

static uint16_t s_press_counter[65535] = { };//this allows 1 or more profile to press the same key, instead of going crazy
static uint16_t s_turbo_status [65535] = { };

//Presses or releases (key) depending on (state)
//@param key Platform specific value
//@param state true = released, false = pressed
void Keyboard(uint16_t key, KeyState state)
{
#ifdef _WIN32
	INPUT input{ };
	switch(key)
	{
	case VK_LBUTTON:
	{
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;

		if(state == KeyState::released)
			input.mi.dwFlags |= MOUSEEVENTF_LEFTUP;
		else
			input.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
		break;
	}
	case VK_RBUTTON:
	{
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;

		if(state == KeyState::released)
			input.mi.dwFlags |= MOUSEEVENTF_RIGHTUP;
		else
			input.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN;
		break;
	}
	case VK_MBUTTON:
	{
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		if(state == KeyState::released)
			input.mi.dwFlags |= MOUSEEVENTF_MIDDLEUP;
		else
			input.mi.dwFlags |= MOUSEEVENTF_MIDDLEDOWN;
		break;
	}
	default:
	{
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = key;
		input.ki.dwFlags = KEYEVENTF_SCANCODE;

		if(IsExtended(key))
			input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;

		if(state == KeyState::released)
			input.ki.dwFlags |= KEYEVENTF_KEYUP;

		input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
		break;
	}
	}

	SendInput(1, (LPINPUT)&input, sizeof(INPUT));
#elif defined(__linux__)
	unsigned long int code = XKeysymToKeycode(g_display, key);
	XTestFakeKeyEvent(g_display, code, !state, 0);
	XFlush(g_display);
#endif
}

//Moves cursor position
//@param type true = absolute, false = relative
void Mouse(bool type, signed long int x, signed long int y)
{
#ifdef _WIN32
	INPUT input{ };

	input.type = INPUT_MOUSE;
	input.mi.dx = x;//-16 border
	input.mi.dy = y;//-16 border
	input.mi.dwFlags = (type ? MOUSEEVENTF_ABSOLUTE : 0) | MOUSEEVENTF_MOVE;
	input.mi.dwExtraInfo = 0;
	input.mi.mouseData = 0;
	input.mi.time = 0;

	SendInput(1, (LPINPUT)&input, sizeof(INPUT));
#elif defined(__linux__)
	Window dummyWin;
	int dummySignedInt;
	unsigned int width, height, dummyInt;

	int screen = DefaultScreen(g_display);
	Window rootwindow = RootWindow(g_display, screen);

	if(XGetGeometry(g_display, rootwindow, &dummyWin, &dummySignedInt, &dummySignedInt, &width, &height, &dummyInt, &dummyInt))
	{
		if(type)
			XTestFakeMotionEvent(g_display, screen, (x * width) / 65535, (y * height) / 65535, 0);
		else
			XTestFakeRelativeMotionEvent(g_display, x, y, 0);
	}
#endif
}

void Tap(uint16_t key, uint8_t joy)
{
	if(s_press_counter[key] == 0)
	{
		if(!s_turbo_status[key])
			Press(key, joy);
		else
			Release(key, joy);
	}
	else
	{
		if(!s_turbo_status[key])
			Release(key, joy);
		else
			Press(key, joy);
	}
}

void Press(uint16_t key, uint8_t joy)
{
	if(s_press_counter[key] == 0)
	{
		// GamePad Buttons
		if(key >= Key::JOY && key <= Key::JOY_MAX) //virtual gamepad buttons
		{
#ifdef _WIN32
			Joystick::SetButton(joy, key - Key::JOY, true);
			Joystick::Update(joy);
#endif
		}
		// GamePad DPad
		else if(key >= Key::JOY_HAT && key <= Key::JOY_HAT_MAX) //virtual gamepad buttons
		{
#ifdef _WIN32
			Joystick::SetHat(joy, key - Key::JOY_HAT, true);
			Joystick::Update(joy);
#endif
		}
		// Analog Axis
		else if(key >= Key::JOY_AXIS1 && key <= Key::JOY_AXIS5_MAX) //virtual analog axis
		{
#ifdef _WIN32
			switch(key)
			{
			case Key::JOY_AXIS_X_MINUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_X, 0);
				break;
			case Key::JOY_AXIS_X_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_X, 100);
				break;
			case Key::JOY_AXIS_Y_MINUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_Y, 0);
				break;
			case Key::JOY_AXIS_Y_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_Y, 100);
				break;
			case Key::JOY_AXIS_Z_MINUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_Z, 0);
				break;
			case Key::JOY_AXIS_Z_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_Z, 100);
				break;
			case Key::JOY_AXIS_RX_MINUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_RX, 0);
				break;
			case Key::JOY_AXIS_RX_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_RX, 100);
				break;
			case Key::JOY_AXIS_RY_MINUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_RY, 0);
				break;
			case Key::JOY_AXIS_RY_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_RY, 100);
				break;
			case Key::JOY_AXIS_RZ_MINUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_RZ, 0);
				break;
			case Key::JOY_AXIS_RZ_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_RZ, 100);
				break;
			case Key::JOY_AXIS_SL0_MINUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_SL0, 0);
				break;
			case Key::JOY_AXIS_SL0_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_SL0, 100);
				break;
			case Key::JOY_AXIS_SL1_MINUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_SL1, 0);
				break;
			case Key::JOY_AXIS_SL1_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_SL1, 100);
				break;
			case Key::JOY_AXIS_WHL_MINUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_WHL, 0);
				break;
			case Key::JOY_AXIS_WHL_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_WHL, 100);
				break;
			default:
				break;
			}
			Joystick::Update(joy);
#endif
		}
		// Keyboard
		else
		{
			Keyboard(key, KeyState::pressed);
			//TODO:log std::cout << "key" << key << "\n";
		}
	}

	if(s_press_counter[key] < 65535)
		s_press_counter[key]++;
}

void Release(uint16_t key, uint8_t joy)
{
	if(s_press_counter[key] == 1)
	{
		// GamePad Buttons
		if(key >= Key::JOY && key <= Key::JOY_MAX)
		{
#ifdef _WIN32
			Joystick::SetButton(joy, key - Key::JOY, false);
			Joystick::Update(joy);
#endif
		}
		// GamePad DPad
		else if(key >= Key::JOY_HAT && key <= Key::JOY_HAT_MAX)
		{
#ifdef _WIN32
			Joystick::SetHat(joy, key - Key::JOY_HAT, false);
			Joystick::Update(joy);
#endif
		}
		// Analog Axis
		else if(key >= Key::JOY_AXIS1 && key <= Key::JOY_AXIS5_MAX)
		{
#ifdef _WIN32
			switch(key)
			{
			case Key::JOY_AXIS_X_MINUS:
			case Key::JOY_AXIS_X_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_X, 50);
				break;
			case Key::JOY_AXIS_Y_MINUS:
			case Key::JOY_AXIS_Y_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_Y, 50);
				break;
			case Key::JOY_AXIS_Z_MINUS:
			case Key::JOY_AXIS_Z_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_Z, 50);
				break;
			case Key::JOY_AXIS_RX_MINUS:
			case Key::JOY_AXIS_RX_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_RX, 50);
				break;
			case Key::JOY_AXIS_RY_MINUS:
			case Key::JOY_AXIS_RY_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_RY, 50);
				break;
			case Key::JOY_AXIS_RZ_MINUS:
			case Key::JOY_AXIS_RZ_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_RZ, 50);
				break;
			case Key::JOY_AXIS_SL0_MINUS:
			case Key::JOY_AXIS_SL0_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_SL0, 50);
				break;
			case Key::JOY_AXIS_SL1_MINUS:
			case Key::JOY_AXIS_SL1_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_SL1, 50);
				break;
			case Key::JOY_AXIS_WHL_MINUS:
			case Key::JOY_AXIS_WHL_PLUS:
				Joystick::SetAxisPercent(joy, HID_USAGE_WHL, 50);
				break;
			default:
				break;
			}
			Joystick::Update(joy);
#endif
		}
		// Keyboard
		else
		{
			Keyboard(key, KeyState::released);
		}
	}

	if(s_press_counter[key] > 0)
		s_press_counter[key]--;
}

void Move(signed long int x, signed long int y)
{
	Mouse(MouseMovement::relative, x, y);
}

void MoveAbsolute(signed long int x, signed long int y)
{
	Mouse(MouseMovement::absolute, x, y);
}

}}//namespace D2K::Input
