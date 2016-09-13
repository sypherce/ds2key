#pragma once

#include <cstdint>
#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <string.h>
#include <X11/keysym.h>
#endif

namespace D2K {namespace Key {

enum {

	KEY_NONE = 0,
#ifdef _WIN32
	KEY_VOID = KEY_NONE,
	KEY_LBUTTON = VK_LBUTTON,
	KEY_RBUTTON = VK_RBUTTON,
	KEY_CANCEL = VK_CANCEL,
	KEY_MBUTTON = VK_MBUTTON,
	KEY_BACK = VK_BACK,
	KEY_TAB = VK_TAB,
	KEY_CLEAR = VK_CLEAR,
	KEY_RETURN = VK_RETURN,
	KEY_SHIFT = VK_SHIFT,
	KEY_CONTROL = VK_CONTROL,
	KEY_MENU = VK_MENU,
	KEY_PAUSE = VK_PAUSE,
	KEY_CAPITAL = VK_CAPITAL,
	KEY_KANA = VK_KANA,
	KEY_HANGEUL = VK_HANGEUL,
	KEY_HANGUL = VK_HANGUL,
	KEY_JUNJA = VK_JUNJA,
	KEY_FINAL = VK_FINAL,
	KEY_HANJA = VK_HANJA,
	KEY_KANJI = VK_KANJI,
	KEY_ESCAPE = VK_ESCAPE,
	KEY_CONVERT = VK_CONVERT,
	KEY_NONCONVERT = VK_NONCONVERT,
	KEY_ACCEPT = VK_ACCEPT,
	KEY_MODECHANGE = VK_MODECHANGE,
	KEY_SPACE = VK_SPACE,
	KEY_PRIOR = VK_PRIOR,
	KEY_NEXT = VK_NEXT,
	KEY_END = VK_END,
	KEY_HOME = VK_HOME,
	KEY_LEFT = VK_LEFT,
	KEY_UP = VK_UP,
	KEY_RIGHT = VK_RIGHT,
	KEY_DOWN = VK_DOWN,
	KEY_SELECT = VK_SELECT,
	KEY_PRINT = VK_PRINT,
	KEY_EXEC = VK_EXECUTE, // KEY_EXECUTE is already defined in windows
	KEY_SNAPSHOT = VK_SNAPSHOT,
	KEY_INSERT = VK_INSERT,
	KEY_DELETE = VK_DELETE,
	KEY_HELP = VK_HELP,
	KEY_0 = '0',
	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',
	KEY_5 = '5',
	KEY_6 = '6',
	KEY_7 = '7',
	KEY_8 = '8',
	KEY_9 = '9',
	KEY_A = 'A',
	KEY_B = 'B',
	KEY_C = 'C',
	KEY_D = 'D',
	KEY_E = 'E',
	KEY_F = 'F',
	KEY_G = 'G',
	KEY_H = 'H',
	KEY_I = 'I',
	KEY_J = 'J',
	KEY_K = 'K',
	KEY_L = 'L',
	KEY_M = 'M',
	KEY_N = 'N',
	KEY_O = 'O',
	KEY_P = 'P',
	KEY_Q = 'Q',
	KEY_R = 'R',
	KEY_S = 'S',
	KEY_T = 'T',
	KEY_U = 'U',
	KEY_V = 'V',
	KEY_W = 'W',
	KEY_X = 'X',
	KEY_Y = 'Y',
	KEY_Z = 'Z',
	KEY_LWIN = VK_LWIN,
	KEY_RWIN = VK_RWIN,
	KEY_APPS = VK_APPS,
	KEY_SLEEP = VK_SLEEP,
	KEY_NUMPAD0 = VK_NUMPAD0,
	KEY_NUMPAD1 = VK_NUMPAD1,
	KEY_NUMPAD2 = VK_NUMPAD2,
	KEY_NUMPAD3 = VK_NUMPAD3,
	KEY_NUMPAD4 = VK_NUMPAD4,
	KEY_NUMPAD5 = VK_NUMPAD5,
	KEY_NUMPAD6 = VK_NUMPAD6,
	KEY_NUMPAD7 = VK_NUMPAD7,
	KEY_NUMPAD8 = VK_NUMPAD8,
	KEY_NUMPAD9 = VK_NUMPAD9,
	KEY_MULTIPLY = VK_MULTIPLY,
	KEY_ADD = VK_ADD,
	KEY_SEPARATOR = VK_SEPARATOR,
	KEY_SUBTRACT = VK_SUBTRACT,
	KEY_DECIMAL = VK_DECIMAL,
	KEY_DIVIDE = VK_DIVIDE,
	KEY_F1 = VK_F1,
	KEY_F2 = VK_F2,
	KEY_F3 = VK_F3,
	KEY_F4 = VK_F4,
	KEY_F5 = VK_F5,
	KEY_F6 = VK_F6,
	KEY_F7 = VK_F7,
	KEY_F8 = VK_F8,
	KEY_F9 = VK_F9,
	KEY_F10 = VK_F10,
	KEY_F11 = VK_F11,
	KEY_F12 = VK_F12,
	KEY_F13 = VK_F13,
	KEY_F14 = VK_F14,
	KEY_F15 = VK_F15,
	KEY_F16 = VK_F16,
	KEY_F17 = VK_F17,
	KEY_F18 = VK_F18,
	KEY_F19 = VK_F19,
	KEY_F20 = VK_F20,
	KEY_F21 = VK_F21,
	KEY_F22 = VK_F22,
	KEY_F23 = VK_F23,
	KEY_F24 = VK_F24,
	KEY_NUMLOCK = VK_NUMLOCK,
	KEY_SCROLL = VK_SCROLL,
	KEY_LSHIFT = VK_LSHIFT,
	KEY_RSHIFT = VK_RSHIFT,
	KEY_LCONTROL = VK_LCONTROL,
	KEY_RCONTROL = VK_RCONTROL,
	KEY_LMENU = VK_LMENU,
	KEY_RMENU = VK_RMENU,
	KEY_OEM_1 = VK_OEM_1,
	KEY_OEM_2 = VK_OEM_2,
	KEY_OEM_3 = VK_OEM_3,
	KEY_OEM_4 = VK_OEM_4,
	KEY_OEM_5 = VK_OEM_5,
	KEY_OEM_6 = VK_OEM_6,
	KEY_OEM_7 = VK_OEM_7,
	KEY_OEM_8 = VK_OEM_8,
	KEY_PROCESSKEY = VK_PROCESSKEY,
	KEY_ATTN = VK_ATTN,
	KEY_CRSEL = VK_CRSEL,
	KEY_EXSEL = VK_EXSEL,
	KEY_EREOF = VK_EREOF,
	KEY_PLAY = VK_PLAY,
	KEY_ZOOM = VK_ZOOM,
	KEY_NONAME = VK_NONAME,
	KEY_PA1 = VK_PA1,
	KEY_OEM_CLEAR = VK_OEM_CLEAR,
	KEY_SLASH = VK_OEM_2,
	KEY_BACKSLASH = VK_OEM_5,
#elif defined(__linux__)
	KEY_VOID = XK_VoidSymbol,
	KEY_LBUTTON 0x8000, // LBUTTON
	KEY_RBUTTON 0x8002, // RBUTTON
	KEY_CANCEL = KEY_NONE, // CANCEL
	KEY_MBUTTON 0x8001, // MBUTTON
	KEY_XBUTTON1 = KEY_NONE, // XBUTTON1
	KEY_XBUTTON2 = KEY_NONE, // XBUTTON2
	KEY_BACK = XK_BackSpace,
	KEY_TAB = XK_Tab,
	KEY_CLEAR = XK_Clear,
	KEY_RETURN = XK_Return,
	KEY_SHIFT = XK_Shift_L,
	KEY_CONTROL = XK_Control_L,
	KEY_MENU = KEY_NONE, // MENU
	KEY_PAUSE = XK_Pause,
	KEY_CAPITAL = XK_Caps_Lock,
	KEY_KANA = KEY_NONE, // KANA
	KEY_HANGEUL = KEY_NONE, // HANGEUL
	KEY_HANGUL = KEY_NONE, // HANGUL
	KEY_JUNJA = KEY_NONE, // JUNJA
	KEY_FINAL = KEY_NONE, // FINAL
	KEY_HANJA = KEY_NONE, // HANJA
	KEY_KANJI = KEY_NONE, // KANJI
	KEY_ESCAPE = XK_Escape, // ESCAPE
	KEY_CONVERT = KEY_NONE, // CONVERT
	KEY_NONCONVERT = KEY_NONE, // NONCONVERT
	KEY_ACCEPT = KEY_NONE, // ACCEPT
	KEY_MODECHANGE = KEY_NONE, // MODECHANGE
	KEY_SPACE = XK_space,
	KEY_PRIOR = XK_Prior,
	KEY_NEXT = XK_Next,
	KEY_END = XK_End,
	KEY_HOME = XK_Home,
	KEY_LEFT = XK_Left,
	KEY_UP = XK_Up,
	KEY_RIGHT = XK_Right,
	KEY_DOWN = XK_Down,
	KEY_SELECT = KEY_NONE, // SELECT
	KEY_PRINT = XK_Print,
	KEY_EXEC = XK_Execute, // KEY_EXECUTE is already defined in windows
	KEY_SNAPSHOT = KEY_NONE, // SNAPSHOT
	KEY_INSERT = XK_Insert,
	KEY_DELETE = XK_Delete,
	KEY_HELP = XK_Help,
	KEY_0 = XK_0,
	KEY_1 = XK_1,
	KEY_2 = XK_2,
	KEY_3 = XK_3,
	KEY_4 = XK_4,
	KEY_5 = XK_5,
	KEY_6 = XK_6,
	KEY_7 = XK_7,
	KEY_8 = XK_8,
	KEY_9 = XK_9,
	KEY_A = XK_A,
	KEY_B = XK_B,
	KEY_C = XK_C,
	KEY_D = XK_D,
	KEY_E = XK_E,
	KEY_F = XK_F,
	KEY_G = XK_G,
	KEY_H = XK_H,
	KEY_I = XK_I,
	KEY_J = XK_J,
	KEY_K = XK_K,
	KEY_L = XK_L,
	KEY_M = XK_M,
	KEY_N = XK_N,
	KEY_O = XK_O,
	KEY_P = XK_P,
	KEY_Q = XK_Q,
	KEY_R = XK_R,
	KEY_S = XK_S,
	KEY_T = XK_T,
	KEY_U = XK_U,
	KEY_V = XK_V,
	KEY_W = XK_W,
	KEY_X = XK_X,
	KEY_Y = XK_Y,
	KEY_Z = XK_Z,
	KEY_LWIN = KEY_NONE, // LWIN
	KEY_RWIN = KEY_NONE, // RWIN
	KEY_APPS = KEY_NONE, // APPS
	KEY_SLEEP = KEY_NONE, // SLEEP
	KEY_NUMPAD0 = XK_KP_0,
	KEY_NUMPAD1 = XK_KP_1,
	KEY_NUMPAD2 = XK_KP_2,
	KEY_NUMPAD3 = XK_KP_3,
	KEY_NUMPAD4 = XK_KP_4,
	KEY_NUMPAD5 = XK_KP_5,
	KEY_NUMPAD6 = XK_KP_6,
	KEY_NUMPAD7 = XK_KP_7,
	KEY_NUMPAD8 = XK_KP_8,
	KEY_NUMPAD9 = XK_KP_9,
	KEY_MULTIPLY = XK_KP_Multiply,
	KEY_ADD = XK_KP_Add,
	KEY_SEPARATOR = XK_KP_Separator,
	KEY_SUBTRACT = XK_KP_Subtract,
	KEY_DECIMAL = XK_KP_Decimal,
	KEY_DIVIDE = XK_KP_Divide,
	KEY_F1 = XK_F1,
	KEY_F2 = XK_F2,
	KEY_F3 = XK_F3,
	KEY_F4 = XK_F4,
	KEY_F5 = XK_F5,
	KEY_F6 = XK_F6,
	KEY_F7 = XK_F7,
	KEY_F8 = XK_F8,
	KEY_F9 = XK_F9,
	KEY_F10 = XK_F10,
	KEY_F11 = XK_F11,
	KEY_F12 = XK_F12,
	KEY_F13 = XK_F13,
	KEY_F14 = XK_F14,
	KEY_F15 = XK_F15,
	KEY_F16 = XK_F16,
	KEY_F17 = XK_F17,
	KEY_F18 = XK_F18,
	KEY_F19 = XK_F19,
	KEY_F20 = XK_F20,
	KEY_F21 = XK_F21,
	KEY_F22 = XK_F22,
	KEY_F23 = XK_F23,
	KEY_F24 = XK_F24,
	KEY_NUMLOCK = XK_Num_Lock,
	KEY_SCROLL = XK_Scroll_Lock,
	KEY_LSHIFT = XK_Shift_L,
	KEY_RSHIFT = XK_Shift_R,
	KEY_LCONTROL = XK_Control_L,
	KEY_RCONTROL = XK_Control_R,
	KEY_LMENU = KEY_NONE, // LMENU
	KEY_RMENU = KEY_NONE, // RMENU
	KEY_BROWSER_BACK = KEY_NONE, // BROWSER_BACK
	KEY_BROWSER_FORWARD = KEY_NONE, // BROWSER_FORWARD
	KEY_BROWSER_REFRESH = KEY_NONE, // BROWSER_REFRESH
	KEY_BROWSER_STOP = KEY_NONE, // BROWSER_STOP
	KEY_BROWSER_SEARCH = KEY_NONE, // BROWSER_SEARCH
	KEY_BROWSER_FAVORITES = KEY_NONE, // BROWSER_FAVORITES
	KEY_BROWSER_HOME = KEY_NONE, // BROWSER_HOME
	KEY_VOLUME_MUTE = KEY_NONE, // VOLUME_MUTE
	KEY_VOLUME_DOWN = KEY_NONE, // VOLUME_DOWN
	KEY_VOLUME_UP = KEY_NONE, // VOLUME_UP
	KEY_MEDIA_NEXT_TRACK = KEY_NONE, // MEDIA_NEXT_TRACK
	KEY_MEDIA_PREV_TRACK = KEY_NONE, // MEDIA_PREV_TRACK
	KEY_MEDIA_STOP = KEY_NONE, // MEDIA_STOP
	KEY_MEDIA_PLAY_PAUSE = KEY_NONE, // MEDIA_PLAY_PAUSE
	KEY_LAUNCH_MAIL = KEY_NONE, // LAUNCH_MAIL
	KEY_LAUNCH_MEDIA_SELECT = KEY_NONE, // LAUNCH_MEDIA_SELECT
	KEY_LAUNCH_APP1 = KEY_NONE, // LAUNCH_APP1
	KEY_LAUNCH_APP2 = KEY_NONE, // LAUNCH_APP2
	KEY_OEM_1 = KEY_NONE, // OEM_1
	KEY_OEM_PLUS = KEY_NONE, // OEM_PLUS
	KEY_OEM_COMMA = KEY_NONE, // OEM_COMMA
	KEY_OEM_MINUS = KEY_NONE, // OEM_MINUS
	KEY_OEM_PERIOD = KEY_NONE, // OEM_PERIOD
	KEY_OEM_2 = KEY_NONE, // OEM_2
	KEY_OEM_3 = KEY_NONE, // OEM_3
	KEY_OEM_4 = KEY_NONE, // OEM_4
	KEY_OEM_5 = KEY_NONE, // OEM_5
	KEY_OEM_6 = KEY_NONE, // OEM_6
	KEY_OEM_7 = KEY_NONE, // OEM_7
	KEY_OEM_8 = KEY_NONE, // OEM_8
	KEY_OEM_102 = KEY_NONE, // OEM_102
	KEY_PROCESSKEY = KEY_NONE, // PROCESSKEY
	KEY_PACKET = KEY_NONE, // PACKET
	KEY_ATTN = KEY_NONE, // ATTN
	KEY_CRSEL = KEY_NONE, // CRSEL
	KEY_EXSEL = KEY_NONE, // EXSEL
	KEY_EREOF = KEY_NONE, // EREOF
	KEY_PLAY = KEY_NONE, // PLAY
	KEY_ZOOM = KEY_NONE, // ZOOM
	KEY_NONAME = KEY_NONE, // NONAME
	KEY_PA1 = KEY_NONE, // PA1
	KEY_OEM_CLEAR = KEY_NONE, // OEM_CLEAR
	KEY_SLASH = XK_slash,
	KEY_BACKSLASH = XK_backslash,
#endif

#if (_WIN32_WINNT >= 0x0500)
	KEY_XBUTTON1 = VK_XBUTTON1,
	KEY_XBUTTON2 = VK_XBUTTON2,
	KEY_BROWSER_BACK = VK_BROWSER_BACK,
	KEY_BROWSER_FORWARD = VK_BROWSER_FORWARD,
	KEY_BROWSER_REFRESH = VK_BROWSER_REFRESH,
	KEY_BROWSER_STOP = VK_BROWSER_STOP,
	KEY_BROWSER_SEARCH = VK_BROWSER_SEARCH,
	KEY_BROWSER_FAVORITES = VK_BROWSER_FAVORITES,
	KEY_BROWSER_HOME = VK_BROWSER_HOME,
	KEY_VOLUME_MUTE = VK_VOLUME_MUTE,
	KEY_VOLUME_DOWN = VK_VOLUME_DOWN,
	KEY_VOLUME_UP = VK_VOLUME_UP,
	KEY_MEDIA_NEXT_TRACK = VK_MEDIA_NEXT_TRACK,
	KEY_MEDIA_PREV_TRACK = VK_MEDIA_PREV_TRACK,
	KEY_MEDIA_STOP = VK_MEDIA_STOP,
	KEY_MEDIA_PLAY_PAUSE = VK_MEDIA_PLAY_PAUSE,
	KEY_LAUNCH_MAIL = VK_LAUNCH_MAIL,
	KEY_LAUNCH_MEDIA_SELECT = VK_LAUNCH_MEDIA_SELECT,
	KEY_LAUNCH_APP1 = VK_LAUNCH_APP1,
	KEY_LAUNCH_APP2 = VK_LAUNCH_APP2,
	KEY_OEM_PLUS = VK_OEM_PLUS,
	KEY_OEM_COMMA = VK_OEM_COMMA,
	KEY_OEM_MINUS = VK_OEM_MINUS,
	KEY_OEM_PERIOD = VK_OEM_PERIOD,
	KEY_OEM_102 = VK_OEM_102,
	KEY_PACKET = VK_PACKET,
#elif defined(_WIN32) || defined(__linux__)
	KEY_XBUTTON1 = KEY_NONE,
	KEY_XBUTTON2 = KEY_NONE,
	KEY_BROWSER_BACK = KEY_NONE,
	KEY_BROWSER_FORWARD = KEY_NONE,
	KEY_BROWSER_REFRESH = KEY_NONE,
	KEY_BROWSER_STOP = KEY_NONE,
	KEY_BROWSER_SEARCH = KEY_NONE,
	KEY_BROWSER_FAVORITES = KEY_NONE,
	KEY_BROWSER_HOME = KEY_NONE,
	KEY_VOLUME_MUTE = KEY_NONE,
	KEY_VOLUME_DOWN = KEY_NONE,
	KEY_VOLUME_UP = KEY_NONE,
	KEY_MEDIA_NEXT_TRACK = KEY_NONE,
	KEY_MEDIA_PREV_TRACK = KEY_NONE,
	KEY_MEDIA_STOP = KEY_NONE,
	KEY_MEDIA_PLAY_PAUSE = KEY_NONE,
	KEY_LAUNCH_MAIL = KEY_NONE,
	KEY_LAUNCH_MEDIA_SELECT = KEY_NONE,
	KEY_LAUNCH_APP1 = KEY_NONE,
	KEY_LAUNCH_APP2 = KEY_NONE,
	KEY_OEM_PLUS = KEY_NONE,
	KEY_OEM_COMMA = KEY_NONE,
	KEY_OEM_MINUS = KEY_NONE,
	KEY_OEM_PERIOD = KEY_NONE,
	KEY_OEM_102 = KEY_NONE,
	KEY_PACKET = KEY_NONE,
#endif

};

enum {

JOY           = 0x100,
// Check key.cpp for final JOY value
JOY_MAX       = JOY + 31,

JOY_HAT,
JOY_HAT_UP = JOY_HAT,
JOY_HAT_DOWN,
JOY_HAT_LEFT,
JOY_HAT_RIGHT,
JOY_HAT_MAX = JOY_HAT_RIGHT,

JOY_AXIS1,
JOY_AXIS_Y_MINUS = JOY_AXIS1,
JOY_AXIS_Y_PLUS,
JOY_AXIS_X_MINUS,
JOY_AXIS_X_PLUS,
JOY_AXIS1_MAX = JOY_AXIS_X_PLUS,

JOY_AXIS2,
JOY_AXIS_RX_MINUS = JOY_AXIS2,
JOY_AXIS_RX_PLUS,
JOY_AXIS_Z_MINUS,
JOY_AXIS_Z_PLUS,
JOY_AXIS2_MAX= JOY_AXIS_Z_PLUS,

JOY_AXIS3,
JOY_AXIS_RZ_MINUS = JOY_AXIS3,
JOY_AXIS_RZ_PLUS,
JOY_AXIS_RY_MINUS,
JOY_AXIS_RY_PLUS,
JOY_AXIS3_MAX = JOY_AXIS_RY_PLUS,

JOY_AXIS4,
JOY_AXIS_SL1_MINUS = JOY_AXIS4,
JOY_AXIS_SL1_PLUS,
JOY_AXIS_SL0_MINUS,
JOY_AXIS_SL0_PLUS,
JOY_AXIS4_MAX = JOY_AXIS_SL0_PLUS,

JOY_AXIS_WHL_MINUS,
JOY_AXIS_WHL_PLUS,
JOY_AXIS5_MAX = JOY_AXIS_WHL_PLUS,
VOL_UP,
VOL_DOWN,

};

typedef struct KeyEntry final
{
	uint32_t value;
	const char* text;
} KeyEntry;

extern uint32_t GetNumber(const char* key);
extern const char* GetString(uint32_t key);

}} // namespace D2K::Key
