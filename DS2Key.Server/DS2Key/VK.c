#include <windows.h>
#include "VK.h"

#ifdef _MSC_VER //less warnings from microsoft
#define stricmp _stricmp
#endif //_MSC_VER
char *vkTable[0xff];

unsigned char getVKNumber(unsigned char *key)
{
	int i;
	for(i = 0; i < 0xff; i++)
	{
		if(!stricmp(key, vkTable[i]))
		{
			return i;
		}
	}

	return i;
}

unsigned char *getVKString(unsigned char key)
{
	if(strlen(vkTable[key]) == 0)
	{
		return "KEY_NONE";
	}

	return vkTable[key];
}

void initVKTable()
{
	int i;
	for(i = 0; i <= 0xff; i++)
	{
		vkTable[i] = "";
	}

	vkTable[VK_LBUTTON] = "KEY_LBUTTON";
	vkTable[VK_RBUTTON] = "KEY_RBUTTON";
	vkTable[VK_CANCEL] = "KEY_CANCEL";
	vkTable[VK_MBUTTON] = "KEY_MBUTTON";
#if (_WIN32_WINNT >= 0x0500)
	vkTable[VK_XBUTTON1] = "KEY_XBUTTON1";
	vkTable[VK_XBUTTON2] = "KEY_XBUTTON2";
#endif
	vkTable[VK_BACK] = "KEY_BACK";
	vkTable[VK_TAB] = "KEY_TAB";
	vkTable[VK_CLEAR] = "KEY_CLEAR";
	vkTable[VK_RETURN] = "KEY_RETURN";
	vkTable[VK_SHIFT] = "KEY_SHIFT";
	vkTable[VK_CONTROL] = "KEY_CONTROL";
	vkTable[VK_MENU] = "KEY_MENU";
	vkTable[VK_PAUSE] = "KEY_PAUSE";
	vkTable[VK_CAPITAL] = "KEY_CAPITAL";
	vkTable[VK_KANA] = "KEY_KANA";
	vkTable[VK_HANGEUL] = "KEY_HANGEUL";
	vkTable[VK_HANGUL] = "KEY_HANGUL";
	vkTable[VK_JUNJA] = "KEY_JUNJA";
	vkTable[VK_FINAL] = "KEY_FINAL";
	vkTable[VK_HANJA] = "KEY_HANJA";
	vkTable[VK_KANJI] = "KEY_KANJI";
	vkTable[VK_ESCAPE] = "KEY_ESCAPE";
	vkTable[VK_CONVERT] = "KEY_CONVERT";
	vkTable[VK_NONCONVERT] = "KEY_NONCONVERT";
	vkTable[VK_ACCEPT] = "KEY_ACCEPT";
	vkTable[VK_MODECHANGE] = "KEY_MODECHANGE";
	vkTable[VK_SPACE] = "KEY_SPACE";
	vkTable[VK_PRIOR] = "KEY_PRIOR";
	vkTable[VK_NEXT] = "KEY_NEXT";
	vkTable[VK_END] = "KEY_END";
	vkTable[VK_HOME] = "KEY_HOME";
	vkTable[VK_LEFT] = "KEY_LEFT";
	vkTable[VK_UP] = "KEY_UP";
	vkTable[VK_RIGHT] = "KEY_RIGHT";
	vkTable[VK_DOWN] = "KEY_DOWN";
	vkTable[VK_SELECT] = "KEY_SELECT";
	vkTable[VK_PRINT] = "KEY_PRINT";
	vkTable[VK_EXECUTE] = "KEY_EXECUTE";
	vkTable[VK_SNAPSHOT] = "KEY_SNAPSHOT";
	vkTable[VK_INSERT] = "KEY_INSERT";
	vkTable[VK_DELETE] = "KEY_DELETE";
	vkTable[VK_HELP] = "KEY_HELP";
	vkTable['0'] = "KEY_0";
	vkTable['1'] = "KEY_1";
	vkTable['2'] = "KEY_2";
	vkTable['3'] = "KEY_3";
	vkTable['4'] = "KEY_4";
	vkTable['5'] = "KEY_5";
	vkTable['6'] = "KEY_6";
	vkTable['7'] = "KEY_7";
	vkTable['8'] = "KEY_8";
	vkTable['9'] = "KEY_9";
	vkTable['A'] = "KEY_A";
	vkTable['B'] = "KEY_B";
	vkTable['C'] = "KEY_C";
	vkTable['D'] = "KEY_D";
	vkTable['E'] = "KEY_E";
	vkTable['F'] = "KEY_F";
	vkTable['G'] = "KEY_G";
	vkTable['H'] = "KEY_H";
	vkTable['I'] = "KEY_I";
	vkTable['J'] = "KEY_J";
	vkTable['K'] = "KEY_K";
	vkTable['L'] = "KEY_L";
	vkTable['M'] = "KEY_M";
	vkTable['N'] = "KEY_N";
	vkTable['O'] = "KEY_O";
	vkTable['P'] = "KEY_P";
	vkTable['Q'] = "KEY_Q";
	vkTable['R'] = "KEY_R";
	vkTable['S'] = "KEY_S";
	vkTable['T'] = "KEY_T";
	vkTable['U'] = "KEY_U";
	vkTable['V'] = "KEY_V";
	vkTable['W'] = "KEY_W";
	vkTable['X'] = "KEY_X";
	vkTable['Y'] = "KEY_Y";
	vkTable['Z'] = "KEY_Z";
	vkTable[VK_LWIN] = "KEY_LWIN";
	vkTable[VK_RWIN] = "KEY_RWIN";
	vkTable[VK_APPS] = "KEY_APPS";
	vkTable[VK_SLEEP] = "KEY_SLEEP";
	vkTable[VK_NUMPAD0] = "KEY_NUMPAD0";
	vkTable[VK_NUMPAD1] = "KEY_NUMPAD1";
	vkTable[VK_NUMPAD2] = "KEY_NUMPAD2";
	vkTable[VK_NUMPAD3] = "KEY_NUMPAD3";
	vkTable[VK_NUMPAD4] = "KEY_NUMPAD4";
	vkTable[VK_NUMPAD5] = "KEY_NUMPAD5";
	vkTable[VK_NUMPAD6] = "KEY_NUMPAD6";
	vkTable[VK_NUMPAD7] = "KEY_NUMPAD7";
	vkTable[VK_NUMPAD8] = "KEY_NUMPAD8";
	vkTable[VK_NUMPAD9] = "KEY_NUMPAD9";
	vkTable[VK_MULTIPLY] = "KEY_MULTIPLY";
	vkTable[VK_ADD] = "KEY_ADD";
	vkTable[VK_SEPARATOR] = "KEY_SEPARATOR";
	vkTable[VK_SUBTRACT] = "KEY_SUBTRACT";
	vkTable[VK_DECIMAL] = "KEY_DECIMAL";
	vkTable[VK_DIVIDE] = "KEY_DIVIDE";
	vkTable[VK_F1] = "KEY_F1";
	vkTable[VK_F2] = "KEY_F2";
	vkTable[VK_F3] = "KEY_F3";
	vkTable[VK_F4] = "KEY_F4";
	vkTable[VK_F5] = "KEY_F5";
	vkTable[VK_F6] = "KEY_F6";
	vkTable[VK_F7] = "KEY_F7";
	vkTable[VK_F8] = "KEY_F8";
	vkTable[VK_F9] = "KEY_F9";
	vkTable[VK_F10] = "KEY_F10";
	vkTable[VK_F11] = "KEY_F11";
	vkTable[VK_F12] = "KEY_F12";
	vkTable[VK_F13] = "KEY_F13";
	vkTable[VK_F14] = "KEY_F14";
	vkTable[VK_F15] = "KEY_F15";
	vkTable[VK_F16] = "KEY_F16";
	vkTable[VK_F17] = "KEY_F17";
	vkTable[VK_F18] = "KEY_F18";
	vkTable[VK_F19] = "KEY_F19";
	vkTable[VK_F20] = "KEY_F20";
	vkTable[VK_F21] = "KEY_F21";
	vkTable[VK_F22] = "KEY_F22";
	vkTable[VK_F23] = "KEY_F23";
	vkTable[VK_F24] = "KEY_F24";
	vkTable[VK_NUMLOCK] = "KEY_NUMLOCK";
	vkTable[VK_SCROLL] = "KEY_SCROLL";
	vkTable[VK_LSHIFT] = "KEY_LSHIFT";
	vkTable[VK_RSHIFT] = "KEY_RSHIFT";
	vkTable[VK_LCONTROL] = "KEY_LCONTROL";
	vkTable[VK_RCONTROL] = "KEY_RCONTROL";
	vkTable[VK_LMENU] = "KEY_LMENU";
	vkTable[VK_RMENU] = "KEY_RMENU";
#if (_WIN32_WINNT >= 0x0500)
	vkTable[VK_BROWSER_BACK] = "KEY_BROWSER_BACK";
	vkTable[VK_BROWSER_FORWARD] = "KEY_BROWSER_FORWARD";
	vkTable[VK_BROWSER_REFRESH] = "KEY_BROWSER_REFRESH";
	vkTable[VK_BROWSER_STOP] = "KEY_BROWSER_STOP";
	vkTable[VK_BROWSER_SEARCH] = "KEY_BROWSER_SEARCH";
	vkTable[VK_BROWSER_FAVORITES] = "KEY_BROWSER_FAVORITES";
	vkTable[VK_BROWSER_HOME] = "KEY_BROWSER_HOME";
	vkTable[VK_VOLUME_MUTE] = "KEY_VOLUME_MUTE";
	vkTable[VK_VOLUME_DOWN] = "KEY_VOLUME_DOWN";
	vkTable[VK_VOLUME_UP] = "KEY_VOLUME_UP";
	vkTable[VK_MEDIA_NEXT_TRACK] = "KEY_MEDIA_NEXT_TRACK";
	vkTable[VK_MEDIA_PREV_TRACK] = "KEY_MEDIA_PREV_TRACK";
	vkTable[VK_MEDIA_STOP] = "KEY_MEDIA_STOP";
	vkTable[VK_MEDIA_PLAY_PAUSE] = "KEY_MEDIA_PLAY_PAUSE";
	vkTable[VK_LAUNCH_MAIL] = "KEY_LAUNCH_MAIL";
	vkTable[VK_LAUNCH_MEDIA_SELECT] = "KEY_LAUNCH_MEDIA_SELECT";
	vkTable[VK_LAUNCH_APP1] = "KEY_LAUNCH_APP1";
	vkTable[VK_LAUNCH_APP2] = "KEY_LAUNCH_APP2";
#endif
	vkTable[VK_OEM_1] = "KEY_OEM_1";
#if (_WIN32_WINNT >= 0x0500)
	vkTable[VK_OEM_PLUS] = "KEY_OEM_PLUS";
	vkTable[VK_OEM_COMMA] = "KEY_OEM_COMMA";
	vkTable[VK_OEM_MINUS] = "KEY_OEM_MINUS";
	vkTable[VK_OEM_PERIOD] = "KEY_OEM_PERIOD";
#endif
	vkTable[VK_OEM_2] = "KEY_OEM_2";
	vkTable[VK_OEM_3] = "KEY_OEM_3";
	vkTable[VK_OEM_4] = "KEY_OEM_4";
	vkTable[VK_OEM_5] = "KEY_OEM_5";
	vkTable[VK_OEM_6] = "KEY_OEM_6";
	vkTable[VK_OEM_7] = "KEY_OEM_7";
	vkTable[VK_OEM_8] = "KEY_OEM_8";
#if (_WIN32_WINNT >= 0x0500)
	vkTable[VK_OEM_102] = "KEY_OEM_102";
#endif
	vkTable[VK_PROCESSKEY] = "KEY_PROCESSKEY";
#if (_WIN32_WINNT >= 0x0500)
	vkTable[VK_PACKET] = "KEY_PACKET";
#endif
	vkTable[VK_ATTN] = "KEY_ATTN";
	vkTable[VK_CRSEL] = "KEY_CRSEL";
	vkTable[VK_EXSEL] = "KEY_EXSEL";
	vkTable[VK_EREOF] = "KEY_EREOF";
	vkTable[VK_PLAY] = "KEY_PLAY";
	vkTable[VK_ZOOM] = "KEY_ZOOM";
	vkTable[VK_NONAME] = "KEY_NONAME";
	vkTable[VK_PA1] = "KEY_PA1";
	vkTable[VK_OEM_CLEAR] = "KEY_OEM_CLEAR";
}
