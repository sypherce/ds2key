/*
DS2Key Client - An application to use your DS as a PC Gamepad
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
#ifndef D2K__KEYBOARD_H__
#define D2K__KEYBOARD_H__

//struct defines
typedef struct CustomKeyboard
{
	Keyboard *keyboard;
	char *buffer;
	int maxLen;
	int position;
	char mode;
} CustomKeyboard;

//normal defines
enum { mNone, mDefault, mNumber, mIP };
enum { DVK_BACKSPACE = 8, DVK_ENTER = 10 };

//variables
extern CustomKeyboard customKeyboard;

//functions
extern void keyboardShiftState();//not defined in libnds includes
extern void customKeyboardGetString(CustomKeyboard *keyboard, int maxLen, int mode, unsigned char x, unsigned char y);
extern void onKeyPressed(int key);

#endif //D2K__KEYBOARD_H__
