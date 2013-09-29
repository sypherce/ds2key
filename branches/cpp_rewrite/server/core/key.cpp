/*
	Conversion between system key codes and strings

	in windows
	VK_RETURN = "KEY_RETURN"

	in linux
	XK_Return = "KEY_RETURN"
*/

#ifndef WIN32
#include <string.h>
#include <X11/keysym.h>
#endif //WIN32

#include "key.h"

namespace D2K {
	namespace Core {
		namespace Key {
			keyEntry Table[] = {
				{KEY_LBUTTON, "KEY_LBUTTON"},
				{KEY_RBUTTON, "KEY_RBUTTON"},
				{KEY_CANCEL, "KEY_CANCEL"},
				{KEY_MBUTTON, "KEY_MBUTTON"},
				{KEY_XBUTTON1, "KEY_XBUTTON1"},
				{KEY_XBUTTON2, "KEY_XBUTTON2"},
				{KEY_BACK, "KEY_BACK"},
				{KEY_TAB, "KEY_TAB"},
				{KEY_CLEAR, "KEY_CLEAR"},
				{KEY_RETURN, "KEY_RETURN"},
				{KEY_SHIFT, "KEY_SHIFT"},
				{KEY_CONTROL, "KEY_CONTROL"},
				{KEY_MENU, "KEY_MENU"},
				{KEY_PAUSE, "KEY_PAUSE"},
				{KEY_CAPITAL, "KEY_CAPITAL"},
				{KEY_KANA, "KEY_KANA"},
				{KEY_HANGEUL, "KEY_HANGEUL"},
				{KEY_HANGUL, "KEY_HANGUL"},
				{KEY_JUNJA, "KEY_JUNJA"},
				{KEY_FINAL, "KEY_FINAL"},
				{KEY_HANJA, "KEY_HANJA"},
				{KEY_KANJI, "KEY_KANJI"},
				{KEY_ESCAPE, "KEY_ESCAPE"},
				{KEY_CONVERT, "KEY_CONVERT"},
				{KEY_NONCONVERT, "KEY_NONCONVERT"},
				{KEY_ACCEPT, "KEY_ACCEPT"},
				{KEY_MODECHANGE, "KEY_MODECHANGE"},
				{KEY_SPACE, "KEY_SPACE"},
				{KEY_PRIOR, "KEY_PRIOR"},
				{KEY_NEXT, "KEY_NEXT"},
				{KEY_END, "KEY_END"},
				{KEY_HOME, "KEY_HOME"},
				{KEY_LEFT, "KEY_LEFT"},
				{KEY_UP, "KEY_UP"},
				{KEY_RIGHT, "KEY_RIGHT"},
				{KEY_DOWN, "KEY_DOWN"},
				{KEY_SELECT, "KEY_SELECT"},
				{KEY_PRINT, "KEY_PRINT"},
				{KEY_EXEC, "KEY_EXEC"},
				{KEY_SNAPSHOT, "KEY_SNAPSHOT"},
				{KEY_INSERT, "KEY_INSERT"},
				{KEY_DELETE, "KEY_DELETE"},
				{KEY_HELP, "KEY_HELP"},
				{KEY_0, "KEY_0"},
				{KEY_1, "KEY_1"},
				{KEY_2, "KEY_2"},
				{KEY_3, "KEY_3"},
				{KEY_4, "KEY_4"},
				{KEY_5, "KEY_5"},
				{KEY_6, "KEY_6"},
				{KEY_7, "KEY_7"},
				{KEY_8, "KEY_8"},
				{KEY_9, "KEY_9"},
				{KEY_A, "KEY_A"},
				{KEY_B, "KEY_B"},
				{KEY_C, "KEY_C"},
				{KEY_D, "KEY_D"},
				{KEY_E, "KEY_E"},
				{KEY_F, "KEY_F"},
				{KEY_G, "KEY_G"},
				{KEY_H, "KEY_H"},
				{KEY_I, "KEY_I"},
				{KEY_J, "KEY_J"},
				{KEY_K, "KEY_K"},
				{KEY_L, "KEY_L"},
				{KEY_M, "KEY_M"},
				{KEY_N, "KEY_N"},
				{KEY_O, "KEY_O"},
				{KEY_P, "KEY_P"},
				{KEY_Q, "KEY_Q"},
				{KEY_R, "KEY_R"},
				{KEY_S, "KEY_S"},
				{KEY_T, "KEY_T"},
				{KEY_U, "KEY_U"},
				{KEY_V, "KEY_V"},
				{KEY_W, "KEY_W"},
				{KEY_X, "KEY_X"},
				{KEY_Y, "KEY_Y"},
				{KEY_Z, "KEY_Z"},
				{KEY_LWIN, "KEY_LWIN"},
				{KEY_RWIN, "KEY_RWIN"},
				{KEY_APPS, "KEY_APPS"},
				{KEY_SLEEP, "KEY_SLEEP"},
				{KEY_NUMPAD0, "KEY_NUMPAD0"},
				{KEY_NUMPAD1, "KEY_NUMPAD1"},
				{KEY_NUMPAD2, "KEY_NUMPAD2"},
				{KEY_NUMPAD3, "KEY_NUMPAD3"},
				{KEY_NUMPAD4, "KEY_NUMPAD4"},
				{KEY_NUMPAD5, "KEY_NUMPAD5"},
				{KEY_NUMPAD6, "KEY_NUMPAD6"},
				{KEY_NUMPAD7, "KEY_NUMPAD7"},
				{KEY_NUMPAD8, "KEY_NUMPAD8"},
				{KEY_NUMPAD9, "KEY_NUMPAD9"},
				{KEY_MULTIPLY, "KEY_MULTIPLY"},
				{KEY_ADD, "KEY_ADD"},
				{KEY_SEPARATOR, "KEY_SEPARATOR"},
				{KEY_SUBTRACT, "KEY_SUBTRACT"},
				{KEY_DECIMAL, "KEY_DECIMAL"},
				{KEY_DIVIDE, "KEY_DIVIDE"},
				{KEY_F1, "KEY_F1"},
				{KEY_F2, "KEY_F2"},
				{KEY_F3, "KEY_F3"},
				{KEY_F4, "KEY_F4"},
				{KEY_F5, "KEY_F5"},
				{KEY_F6, "KEY_F6"},
				{KEY_F7, "KEY_F7"},
				{KEY_F8, "KEY_F8"},
				{KEY_F9, "KEY_F9"},
				{KEY_F10, "KEY_F10"},
				{KEY_F11, "KEY_F11"},
				{KEY_F12, "KEY_F12"},
				{KEY_F13, "KEY_F13"},
				{KEY_F14, "KEY_F14"},
				{KEY_F15, "KEY_F15"},
				{KEY_F16, "KEY_F16"},
				{KEY_F17, "KEY_F17"},
				{KEY_F18, "KEY_F18"},
				{KEY_F19, "KEY_F19"},
				{KEY_F20, "KEY_F20"},
				{KEY_F21, "KEY_F21"},
				{KEY_F22, "KEY_F22"},
				{KEY_F23, "KEY_F23"},
				{KEY_F24, "KEY_F24"},
				{KEY_NUMLOCK, "KEY_NUMLOCK"},
				{KEY_SCROLL, "KEY_SCROLL"},
				{KEY_LSHIFT, "KEY_LSHIFT"},
				{KEY_RSHIFT, "KEY_RSHIFT"},
				{KEY_LCONTROL, "KEY_LCONTROL"},
				{KEY_RCONTROL, "KEY_RCONTROL"},
				{KEY_LMENU, "KEY_LMENU"},
				{KEY_RMENU, "KEY_RMENU"},
				{KEY_BROWSER_BACK, "KEY_BROWSER_BACK"},
				{KEY_BROWSER_FORWARD, "KEY_BROWSER_FORWARD"},
				{KEY_BROWSER_REFRESH, "KEY_BROWSER_REFRESH"},
				{KEY_BROWSER_STOP, "KEY_BROWSER_STOP"},
				{KEY_BROWSER_SEARCH, "KEY_BROWSER_SEARCH"},
				{KEY_BROWSER_FAVORITES, "KEY_BROWSER_FAVORITES"},
				{KEY_BROWSER_HOME, "KEY_BROWSER_HOME"},
				{KEY_VOLUME_MUTE, "KEY_VOLUME_MUTE"},
				{KEY_VOLUME_DOWN, "KEY_VOLUME_DOWN"},
				{KEY_VOLUME_UP, "KEY_VOLUME_UP"},
				{KEY_MEDIA_NEXT_TRACK, "KEY_MEDIA_NEXT_TRACK"},
				{KEY_MEDIA_PREV_TRACK, "KEY_MEDIA_PREV_TRACK"},
				{KEY_MEDIA_STOP, "KEY_MEDIA_STOP"},
				{KEY_MEDIA_PLAY_PAUSE, "KEY_MEDIA_PLAY_PAUSE"},
				{KEY_LAUNCH_MAIL, "KEY_LAUNCH_MAIL"},
				{KEY_LAUNCH_MEDIA_SELECT, "KEY_LAUNCH_MEDIA_SELECT"},
				{KEY_LAUNCH_APP1, "KEY_LAUNCH_APP1"},
				{KEY_LAUNCH_APP2, "KEY_LAUNCH_APP2"},
				{KEY_OEM_1, "KEY_OEM_1"},
				{KEY_OEM_PLUS, "KEY_OEM_PLUS"},
				{KEY_OEM_COMMA, "KEY_OEM_COMMA"},
				{KEY_OEM_MINUS, "KEY_OEM_MINUS"},
				{KEY_OEM_PERIOD, "KEY_OEM_PERIOD"},
				{KEY_OEM_2, "KEY_OEM_2"},
				{KEY_OEM_3, "KEY_OEM_3"},
				{KEY_OEM_4, "KEY_OEM_4"},
				{KEY_OEM_5, "KEY_OEM_5"},
				{KEY_OEM_6, "KEY_OEM_6"},
				{KEY_OEM_7, "KEY_OEM_7"},
				{KEY_OEM_8, "KEY_OEM_8"},
				{KEY_OEM_102, "KEY_OEM_102"},
				{KEY_PROCESSKEY, "KEY_PROCESSKEY"},
				{KEY_PACKET, "KEY_PACKET"},
				{KEY_ATTN, "KEY_ATTN"},
				{KEY_CRSEL, "KEY_CRSEL"},
				{KEY_EXSEL, "KEY_EXSEL"},
				{KEY_EREOF, "KEY_EREOF"},
				{KEY_PLAY, "KEY_PLAY"},
				{KEY_ZOOM, "KEY_ZOOM"},
				{KEY_NONAME, "KEY_NONAME"},
				{KEY_PA1, "KEY_PA1"},
				{KEY_OEM_CLEAR, "KEY_OEM_CLEAR"},
				{0x100, "JOY_00"},
				{0x101, "JOY_01"},
				{0x102, "JOY_02"},
				{0x103, "JOY_03"},
				{0x104, "JOY_04"},
				{0x105, "JOY_05"},
				{0x106, "JOY_06"},
				{0x107, "JOY_07"},
				{0x108, "JOY_08"},
				{0x109, "JOY_09"},
				{0x10a, "JOY_10"},
				{0x10b, "JOY_11"},
				{0x10c, "JOY_12"},
				{0x10d, "JOY_13"},
				{0x10e, "JOY_14"},
				{0x10f, "JOY_15"},
				{0x110, "JOY_16"},

				{KEY_NONE, "KEY_NONE"},
				{KEY_VOID, "KEY_VOID"}
			};

			uint32_t GetNumber(const char *key) {
				for(keyEntry *entry = Table; entry->value != KEY_VOID; entry++) {
					if(!strcmp(entry->text, key)) {
						return entry->value;
					}
				}

				return KEY_NONE;
			}

			char *GetString(uint32_t key) {
				for(keyEntry *entry = Table; entry->value != KEY_VOID; entry++) {
					if(entry->value == key) {
						return (char*)entry->text;
					}
				}

				return (char*)"KEY_NONE";
			}
		}
	}
}
