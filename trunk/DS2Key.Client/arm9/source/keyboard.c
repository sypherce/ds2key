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
#include <nds.h>
#include <string.h>
#include "keyboard.h"
#include "keyboard.hit.h"
#include "keyboardImage.h"

//keyboard function variables
u8 keyboardOffset = 6;
uint16 *keyboardMapPointer;
char *keyTextPos = (char *)NULL;
char *keyText = (char *)NULL;
char keyEnter = 0;
unsigned int keyLength = 0;
unsigned char m_Mode = keyboardNormal;

void drawKeyboard()
{
	if(m_Mode == keyboardNormal)
	{
		dmaCopy((uint16 *)keyboardImageMap, keyboardMapPointer, keyboardImageMapLen >> 1);
	}
	else
	{
		dmaCopy((uint16 *)keyboardImageMap + (keyboardImageMapLen >> 2), keyboardMapPointer, keyboardImageMapLen >> 1);
	}
}

char *initKeyboard(char *text, unsigned int length, char enter)
{
	keyLength = length;
	keyEnter = enter;
	keyText = text;
	keyTextPos = text + strlen(text);

	return keyText;
}

void deInitKeyboard()
{
	keyLength = 0;
	keyEnter = 0;
	keyText = keyTextPos = (char *)NULL;
	m_Mode = keyboardNormal;
	memset(keyboardMapPointer, 0, keyboardImageMapLen >> 1);
}

unsigned char updateKeyboard(unsigned char x, unsigned char y)
{
	unsigned char returnVal = 0;
	char c = 0;

	if(keyLength > 0 && x >= 4 && x <= 27 && y <= 12)
	{
		if(m_Mode == keyboardNormal)
		{
			c = keyboard_Hit[x + (y * 32)];
		}
		else
		{
			c = keyboard_Hit_Shift[x + (y * 32)];
		}

		if(c == CAP || c == SHF)
		{
			if(m_Mode == keyboardNormal)
			{
				if(c == SHF)
				{
					m_Mode = keyboardShift;
				}
				else
				{
					m_Mode = keyboardCapsLock;
				}
			}
			else
			{
				m_Mode = keyboardNormal;
			}

			drawKeyboard();
		}
		else if(c)
		{
			if(c == BSP)
			{
				if((int)(keyTextPos - keyText) > 0)
				{
					keyTextPos--;
					*keyTextPos = ' ';
					returnVal = 1;
				}
			}
			else if((int)(keyTextPos - keyText) < keyLength)
			{
				if(c == RET)
				{
					*keyTextPos = keyEnter;
					returnVal = 2;
				}
				else
				{
					if(m_Mode == keyboardShift)
					{
						m_Mode = keyboardNormal;
						drawKeyboard();
					}

					*keyTextPos = c;
					returnVal = 1;
				}

				keyTextPos++;
			}
		}
	}

	return returnVal;
}
