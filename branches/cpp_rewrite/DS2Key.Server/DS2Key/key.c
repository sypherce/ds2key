/*
DS2Key Server - An application to use your DS as a PC Gamepad
Copyright (C) 2008  Derrick (sypherce) Wirth

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef WIN32
#ifdef WINVER
#undef WINVER
#endif
#define WINVER 0x0500
#include <windows.h>
#else //WIN32
#include <string.h>
#include <X11/keysym.h>
#endif //WIN32
#include "key.h"

char *keyTable[0xffff];

unsigned int getKeyNumber(char *key)
{
	int i;
	for(i = 0; i <= 0xffff; i++)
	{
		if(i == 0xffff || !stricmp(key, keyTable[i])) //keep in this order or will cause bugs
		{
			return i;
		}
	}

	return i;
}

char *getKeyString(unsigned int key)
{
	if(key > 0xffff || keyTable[key][0] == 0) //keep in this order or will cause bugs
	{
		return "KEY_NONE";
	}

	return keyTable[key];
}

void initKeyTable()
{
	int i;
	for(i = 0; i < 0xffff; i++)
	{
		keyTable[i] = "KEY_NONE";
	}

	keyTable[KEY_LBUTTON] = "KEY_LBUTTON";
	keyTable[KEY_RBUTTON] = "KEY_RBUTTON";
	keyTable[KEY_CANCEL] = "KEY_CANCEL";
	keyTable[KEY_MBUTTON] = "KEY_MBUTTON";
	keyTable[KEY_XBUTTON1] = "KEY_XBUTTON1";
	keyTable[KEY_XBUTTON2] = "KEY_XBUTTON2";
	keyTable[KEY_BACK] = "KEY_BACK";
	keyTable[KEY_TAB] = "KEY_TAB";
	keyTable[KEY_CLEAR] = "KEY_CLEAR";
	keyTable[KEY_RETURN] = "KEY_RETURN";
	keyTable[KEY_SHIFT] = "KEY_SHIFT";
	keyTable[KEY_CONTROL] = "KEY_CONTROL";
	keyTable[KEY_MENU] = "KEY_MENU";
	keyTable[KEY_PAUSE] = "KEY_PAUSE";
	keyTable[KEY_CAPITAL] = "KEY_CAPITAL";
	keyTable[KEY_KANA] = "KEY_KANA";
	keyTable[KEY_HANGEUL] = "KEY_HANGEUL";
	keyTable[KEY_HANGUL] = "KEY_HANGUL";
	keyTable[KEY_JUNJA] = "KEY_JUNJA";
	keyTable[KEY_FINAL] = "KEY_FINAL";
	keyTable[KEY_HANJA] = "KEY_HANJA";
	keyTable[KEY_KANJI] = "KEY_KANJI";
	keyTable[KEY_ESCAPE] = "KEY_ESCAPE";
	keyTable[KEY_CONVERT] = "KEY_CONVERT";
	keyTable[KEY_NONCONVERT] = "KEY_NONCONVERT";
	keyTable[KEY_ACCEPT] = "KEY_ACCEPT";
	keyTable[KEY_MODECHANGE] = "KEY_MODECHANGE";
	keyTable[KEY_SPACE] = "KEY_SPACE";
	keyTable[KEY_PRIOR] = "KEY_PRIOR";
	keyTable[KEY_NEXT] = "KEY_NEXT";
	keyTable[KEY_END] = "KEY_END";
	keyTable[KEY_HOME] = "KEY_HOME";
	keyTable[KEY_LEFT] = "KEY_LEFT";
	keyTable[KEY_UP] = "KEY_UP";
	keyTable[KEY_RIGHT] = "KEY_RIGHT";
	keyTable[KEY_DOWN] = "KEY_DOWN";
	keyTable[KEY_SELECT] = "KEY_SELECT";
	keyTable[KEY_PRINT] = "KEY_PRINT";
	keyTable[KEY_EXEC] = "KEY_EXEC";
	keyTable[KEY_SNAPSHOT] = "KEY_SNAPSHOT";
	keyTable[KEY_INSERT] = "KEY_INSERT";
	keyTable[KEY_DELETE] = "KEY_DELETE";
	keyTable[KEY_HELP] = "KEY_HELP";
	keyTable['0'] = "KEY_0";
	keyTable['1'] = "KEY_1";
	keyTable['2'] = "KEY_2";
	keyTable['3'] = "KEY_3";
	keyTable['4'] = "KEY_4";
	keyTable['5'] = "KEY_5";
	keyTable['6'] = "KEY_6";
	keyTable['7'] = "KEY_7";
	keyTable['8'] = "KEY_8";
	keyTable['9'] = "KEY_9";
	keyTable['A'] = "KEY_A";
	keyTable['B'] = "KEY_B";
	keyTable['C'] = "KEY_C";
	keyTable['D'] = "KEY_D";
	keyTable['E'] = "KEY_E";
	keyTable['F'] = "KEY_F";
	keyTable['G'] = "KEY_G";
	keyTable['H'] = "KEY_H";
	keyTable['I'] = "KEY_I";
	keyTable['J'] = "KEY_J";
	keyTable['K'] = "KEY_K";
	keyTable['L'] = "KEY_L";
	keyTable['M'] = "KEY_M";
	keyTable['N'] = "KEY_N";
	keyTable['O'] = "KEY_O";
	keyTable['P'] = "KEY_P";
	keyTable['Q'] = "KEY_Q";
	keyTable['R'] = "KEY_R";
	keyTable['S'] = "KEY_S";
	keyTable['T'] = "KEY_T";
	keyTable['U'] = "KEY_U";
	keyTable['V'] = "KEY_V";
	keyTable['W'] = "KEY_W";
	keyTable['X'] = "KEY_X";
	keyTable['Y'] = "KEY_Y";
	keyTable['Z'] = "KEY_Z";
	keyTable[KEY_LWIN] = "KEY_LWIN";
	keyTable[KEY_RWIN] = "KEY_RWIN";
	keyTable[KEY_APPS] = "KEY_APPS";
	keyTable[KEY_SLEEP] = "KEY_SLEEP";
	keyTable[KEY_NUMPAD0] = "KEY_NUMPAD0";
	keyTable[KEY_NUMPAD1] = "KEY_NUMPAD1";
	keyTable[KEY_NUMPAD2] = "KEY_NUMPAD2";
	keyTable[KEY_NUMPAD3] = "KEY_NUMPAD3";
	keyTable[KEY_NUMPAD4] = "KEY_NUMPAD4";
	keyTable[KEY_NUMPAD5] = "KEY_NUMPAD5";
	keyTable[KEY_NUMPAD6] = "KEY_NUMPAD6";
	keyTable[KEY_NUMPAD7] = "KEY_NUMPAD7";
	keyTable[KEY_NUMPAD8] = "KEY_NUMPAD8";
	keyTable[KEY_NUMPAD9] = "KEY_NUMPAD9";
	keyTable[KEY_MULTIPLY] = "KEY_MULTIPLY";
	keyTable[KEY_ADD] = "KEY_ADD";
	keyTable[KEY_SEPARATOR] = "KEY_SEPARATOR";
	keyTable[KEY_SUBTRACT] = "KEY_SUBTRACT";
	keyTable[KEY_DECIMAL] = "KEY_DECIMAL";
	keyTable[KEY_DIVIDE] = "KEY_DIVIDE";
	keyTable[KEY_F1] = "KEY_F1";
	keyTable[KEY_F2] = "KEY_F2";
	keyTable[KEY_F3] = "KEY_F3";
	keyTable[KEY_F4] = "KEY_F4";
	keyTable[KEY_F5] = "KEY_F5";
	keyTable[KEY_F6] = "KEY_F6";
	keyTable[KEY_F7] = "KEY_F7";
	keyTable[KEY_F8] = "KEY_F8";
	keyTable[KEY_F9] = "KEY_F9";
	keyTable[KEY_F10] = "KEY_F10";
	keyTable[KEY_F11] = "KEY_F11";
	keyTable[KEY_F12] = "KEY_F12";
	keyTable[KEY_F13] = "KEY_F13";
	keyTable[KEY_F14] = "KEY_F14";
	keyTable[KEY_F15] = "KEY_F15";
	keyTable[KEY_F16] = "KEY_F16";
	keyTable[KEY_F17] = "KEY_F17";
	keyTable[KEY_F18] = "KEY_F18";
	keyTable[KEY_F19] = "KEY_F19";
	keyTable[KEY_F20] = "KEY_F20";
	keyTable[KEY_F21] = "KEY_F21";
	keyTable[KEY_F22] = "KEY_F22";
	keyTable[KEY_F23] = "KEY_F23";
	keyTable[KEY_F24] = "KEY_F24";
	keyTable[KEY_NUMLOCK] = "KEY_NUMLOCK";
	keyTable[KEY_SCROLL] = "KEY_SCROLL";
	keyTable[KEY_LSHIFT] = "KEY_LSHIFT";
	keyTable[KEY_RSHIFT] = "KEY_RSHIFT";
	keyTable[KEY_LCONTROL] = "KEY_LCONTROL";
	keyTable[KEY_RCONTROL] = "KEY_RCONTROL";
	keyTable[KEY_LMENU] = "KEY_LMENU";
	keyTable[KEY_RMENU] = "KEY_RMENU";
	keyTable[KEY_BROWSER_BACK] = "KEY_BROWSER_BACK";
	keyTable[KEY_BROWSER_FORWARD] = "KEY_BROWSER_FORWARD";
	keyTable[KEY_BROWSER_REFRESH] = "KEY_BROWSER_REFRESH";
	keyTable[KEY_BROWSER_STOP] = "KEY_BROWSER_STOP";
	keyTable[KEY_BROWSER_SEARCH] = "KEY_BROWSER_SEARCH";
	keyTable[KEY_BROWSER_FAVORITES] = "KEY_BROWSER_FAVORITES";
	keyTable[KEY_BROWSER_HOME] = "KEY_BROWSER_HOME";
	keyTable[KEY_VOLUME_MUTE] = "KEY_VOLUME_MUTE";
	keyTable[KEY_VOLUME_DOWN] = "KEY_VOLUME_DOWN";
	keyTable[KEY_VOLUME_UP] = "KEY_VOLUME_UP";
	keyTable[KEY_MEDIA_NEXT_TRACK] = "KEY_MEDIA_NEXT_TRACK";
	keyTable[KEY_MEDIA_PREV_TRACK] = "KEY_MEDIA_PREV_TRACK";
	keyTable[KEY_MEDIA_STOP] = "KEY_MEDIA_STOP";
	keyTable[KEY_MEDIA_PLAY_PAUSE] = "KEY_MEDIA_PLAY_PAUSE";
	keyTable[KEY_LAUNCH_MAIL] = "KEY_LAUNCH_MAIL";
	keyTable[KEY_LAUNCH_MEDIA_SELECT] = "KEY_LAUNCH_MEDIA_SELECT";
	keyTable[KEY_LAUNCH_APP1] = "KEY_LAUNCH_APP1";
	keyTable[KEY_LAUNCH_APP2] = "KEY_LAUNCH_APP2";
	keyTable[KEY_OEM_1] = "KEY_OEM_1";
	keyTable[KEY_OEM_PLUS] = "KEY_OEM_PLUS";
	keyTable[KEY_OEM_COMMA] = "KEY_OEM_COMMA";
	keyTable[KEY_OEM_MINUS] = "KEY_OEM_MINUS";
	keyTable[KEY_OEM_PERIOD] = "KEY_OEM_PERIOD";
	keyTable[KEY_OEM_2] = "KEY_OEM_2";
	keyTable[KEY_OEM_3] = "KEY_OEM_3";
	keyTable[KEY_OEM_4] = "KEY_OEM_4";
	keyTable[KEY_OEM_5] = "KEY_OEM_5";
	keyTable[KEY_OEM_6] = "KEY_OEM_6";
	keyTable[KEY_OEM_7] = "KEY_OEM_7";
	keyTable[KEY_OEM_8] = "KEY_OEM_8";
	keyTable[KEY_OEM_102] = "KEY_OEM_102";
	keyTable[KEY_PROCESSKEY] = "KEY_PROCESSKEY";
	keyTable[KEY_PACKET] = "KEY_PACKET";
	keyTable[KEY_ATTN] = "KEY_ATTN";
	keyTable[KEY_CRSEL] = "KEY_CRSEL";
	keyTable[KEY_EXSEL] = "KEY_EXSEL";
	keyTable[KEY_EREOF] = "KEY_EREOF";
	keyTable[KEY_PLAY] = "KEY_PLAY";
	keyTable[KEY_ZOOM] = "KEY_ZOOM";
	keyTable[KEY_NONAME] = "KEY_NONAME";
	keyTable[KEY_PA1] = "KEY_PA1";
	keyTable[KEY_OEM_CLEAR] = "KEY_OEM_CLEAR";
	keyTable[KEY_NONE] = "KEY_NONE";
}
