#pragma once

#include <string> // std::string
#include "gui/window.h"
#include "gui/edit.h"
#include "gui/label.h"

namespace D2K {namespace GUI {namespace Keyboard {

const char WINKEY_NONE = 0;
const char WINKEY_VOID = WINKEY_NONE;
const char WINKEY_LBUTTON = 0x01;
const char WINKEY_RBUTTON = 0x02;
const char WINKEY_CANCEL = 0x03;
const char WINKEY_MBUTTON = 0x04;
const char WINKEY_XBUTTON1 = 0x05;
const char WINKEY_XBUTTON2 = 0x06;
const char WINKEY_BACK = 0x08;
const char WINKEY_TAB = 0x09;
const char WINKEY_CLEAR = 0x0C;
const char WINKEY_RETURN = 0x0D;
const char WINKEY_SHIFT = 0x10;
const char WINKEY_CONTROL = 0x11;
const char WINKEY_MENU = 0x12;
const char WINKEY_PAUSE = 0x13;
const char WINKEY_CAPITAL = 0x14;
const char WINKEY_KANA = 0x15;
const char WINKEY_HANGUL = 0x15;
const char WINKEY_JUNJA = 0x17;
const char WINKEY_FINAL = 0x18;
const char WINKEY_KANJI = 0x19;
const char WINKEY_ESCAPE = 0x1B;
const char WINKEY_CONVERT = 0x1C;
const char WINKEY_NONCONVERT = 0x1D;
const char WINKEY_ACCEPT = 0x1E;
const char WINKEY_MODECHANGE = 0x1F;
const char WINKEY_SPACE = 0x20;
const char WINKEY_PRIOR = 0x21;
const char WINKEY_NEXT = 0x22;
const char WINKEY_END = 0x23;
const char WINKEY_HOME = 0x24;
const char WINKEY_LEFT = 0x25;
const char WINKEY_UP = 0x26;
const char WINKEY_RIGHT = 0x27;
const char WINKEY_DOWN = 0x28;
const char WINKEY_SELECT = 0x29;
const char WINKEY_PRINT = 0x2A;
const char WINKEY_EXECUTE = 0x2B;
const char WINKEY_SNAPSHOT = 0x2C;
const char WINKEY_INSERT = 0x2D;
const char WINKEY_DELETE = 0x2E;
const char WINKEY_HELP = 0x2F;
const char WINKEY_0 = '0';
const char WINKEY_1 = '1';
const char WINKEY_2 = '2';
const char WINKEY_3 = '3';
const char WINKEY_4 = '4';
const char WINKEY_5 = '5';
const char WINKEY_6 = '6';
const char WINKEY_7 = '7';
const char WINKEY_8 = '8';
const char WINKEY_9 = '9';
const char WINKEY_A = 'A';
const char WINKEY_B = 'B';
const char WINKEY_C = 'C';
const char WINKEY_D = 'D';
const char WINKEY_E = 'E';
const char WINKEY_F = 'F';
const char WINKEY_G = 'G';
const char WINKEY_H = 'H';
const char WINKEY_I = 'I';
const char WINKEY_J = 'J';
const char WINKEY_K = 'K';
const char WINKEY_L = 'L';
const char WINKEY_M = 'M';
const char WINKEY_N = 'N';
const char WINKEY_O = 'O';
const char WINKEY_P = 'P';
const char WINKEY_Q = 'Q';
const char WINKEY_R = 'R';
const char WINKEY_S = 'S';
const char WINKEY_T = 'T';
const char WINKEY_U = 'U';
const char WINKEY_V = 'V';
const char WINKEY_W = 'W';
const char WINKEY_X = 'X';
const char WINKEY_Y = 'Y';
const char WINKEY_Z = 'Z';
const char WINKEY_LWIN = 0x5B;
const char WINKEY_RWIN = 0x5C;
const char WINKEY_APPS = 0x5D;
const char WINKEY_SLEEP = 0x5F;
const char WINKEY_NUMPAD0 = 0x60;
const char WINKEY_NUMPAD1 = 0x61;
const char WINKEY_NUMPAD2 = 0x62;
const char WINKEY_NUMPAD3 = 0x63;
const char WINKEY_NUMPAD4 = 0x64;
const char WINKEY_NUMPAD5 = 0x65;
const char WINKEY_NUMPAD6 = 0x66;
const char WINKEY_NUMPAD7 = 0x67;
const char WINKEY_NUMPAD8 = 0x68;
const char WINKEY_NUMPAD9 = 0x69;
const char WINKEY_MULTIPLY = 0x6A;
const char WINKEY_ADD = 0x6B;
const char WINKEY_SEPARATOR = 0x6C;
const char WINKEY_SUBTRACT = 0x6D;
const char WINKEY_DECIMAL = 0x6E;
const char WINKEY_DIVIDE = 0x6F;
const char WINKEY_F1 = 0x70;
const char WINKEY_F2 = 0x71;
const char WINKEY_F3 = 0x72;
const char WINKEY_F4 = 0x73;
const char WINKEY_F5 = 0x74;
const char WINKEY_F6 = 0x75;
const char WINKEY_F7 = 0x76;
const char WINKEY_F8 = 0x77;
const char WINKEY_F9 = 0x78;
const char WINKEY_F10 = 0x79;
const char WINKEY_F11 = 0x7A;
const char WINKEY_F12 = 0x7B;
const char WINKEY_F13 = 0x7C;
const char WINKEY_F14 = 0x7D;
const char WINKEY_F15 = 0x7E;
const char WINKEY_F16 = 0x7F;
const char WINKEY_F17 = 0x80;
const char WINKEY_F18 = 0x81;
const char WINKEY_F19 = 0x82;
const char WINKEY_F20 = 0x83;
const char WINKEY_F21 = 0x84;
const char WINKEY_F22 = 0x85;
const char WINKEY_F23 = 0x86;
const char WINKEY_F24 = 0x87;
const char WINKEY_NUMLOCK = 0x90;
const char WINKEY_SCROLL = 0x91;
const char WINKEY_LSHIFT = 0xA0;
const char WINKEY_RSHIFT = 0xA1;
const char WINKEY_LCONTROL = 0xA2;
const char WINKEY_RCONTROL = 0xA3;
const char WINKEY_LMENU = 0xA4;
const char WINKEY_RMENU = 0xA5;
const char WINKEY_BROWSER_BACK = 0xA6;
const char WINKEY_BROWSER_FORWARD = 0xA7;
const char WINKEY_BROWSER_REFRESH = 0xA8;
const char WINKEY_BROWSER_STOP = 0xA9;
const char WINKEY_BROWSER_SEARCH = 0xAA;
const char WINKEY_BROWSER_FAVORITES = 0xAB;
const char WINKEY_BROWSER_HOME = 0xAC;
const char WINKEY_VOLUME_MUTE = 0xAD;
const char WINKEY_VOLUME_DOWN = 0xAE;
const char WINKEY_VOLUME_UP = 0xAF;
const char WINKEY_MEDIA_NEXT_TRACK = 0xB0;
const char WINKEY_MEDIA_PREV_TRACK = 0xB1;
const char WINKEY_MEDIA_STOP = 0xB2;
const char WINKEY_MEDIA_PLAY_PAUSE = 0xB3;
const char WINKEY_LAUNCH_MAIL = 0xB4;
const char WINKEY_LAUNCH_MEDIA_SELECT = 0xB5;
const char WINKEY_LAUNCH_APP1 = 0xB6;
const char WINKEY_LAUNCH_APP2 = 0xB7;
const char WINKEY_OEM_1 = 0xBA; // ;
const char WINKEY_OEM_PLUS = 0xBB; // =
const char WINKEY_OEM_COMMA = 0xBC; // ,
const char WINKEY_OEM_MINUS = 0xBD; // _
const char WINKEY_OEM_PERIOD = 0xBE; // >
const char WINKEY_OEM_2 = 0xBF; // ?
const char WINKEY_OEM_3 = 0xC0; // ``
const char WINKEY_OEM_4 = 0xDB; // [
const char WINKEY_OEM_5 = 0xDC; // |
const char WINKEY_OEM_6 = 0xDD; // ]
const char WINKEY_OEM_7 = 0xDE; // '
const char WINKEY_OEM_8 = 0xDF;
const char WINKEY_PACKET = 0xE7;
const char WINKEY_ATTN = 0xF6;
const char WINKEY_CRSEL = 0xF7;
const char WINKEY_EXSEL = 0xF8;
const char WINKEY_EREOF = 0xF9;
const char WINKEY_PLAY = 0xFA;
const char WINKEY_ZOOM = 0xFB;
const char WINKEY_NONAME = 0xFC;
const char WINKEY_PA1 = 0xFD;
const char WINKEY_OEM_CLEAR = 0xFE;

class WindowClass : public GUI::Window
{
public:
	WindowClass();
	~WindowClass();
};
extern GUI::Keyboard::WindowClass g_window;
extern std::string GetString(GUI::Label* label, GUI::Edit* edit, std::string text, int maxLength);
extern uint16_t GetKey();

}}} // namespace D2K::GUI::Keyboard
