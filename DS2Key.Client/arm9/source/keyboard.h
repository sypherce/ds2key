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
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

enum keyboardState { keyboardNormal, keyboardCapsLock, keyboardShift };

extern u8 keyboardOffset;
extern uint16 *keyboardMapPointer;
extern char *keyTextPos;
extern char *keyText;
extern char keyEnter;
extern unsigned int keyLength;
extern unsigned char m_Mode;

extern void drawKeyboard();
extern char *initKeyboard(char *text, unsigned int length, char enter);
extern void deInitKeyboard();
extern unsigned char updateKeyboard(unsigned char x, unsigned char y);

#endif //__KEYBOARD_H__
