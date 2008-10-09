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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "keyboard.h"
#include "keyboard.hit.h"
#include "keyboardImage.h"

//keyboard function variables
u8 keyboardOffset = 6;
uint16 *keyboardMapPointer;
char *keyText = (char *)NULL;
int *keyNumber;
char keyNumberText[7];
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

void initKeyboardString(char *text, unsigned int length, char enter)
{
	keyLength = length;
	keyEnter = enter;
	keyNumber = (int*)NULL;
	keyText = text;
}

void initKeyboardInt(int *number, unsigned int length, char enter)
{
	keyLength = length;
	keyEnter = enter;
	keyNumber = number;
	memset(keyNumberText, 0, 6);
	if(*keyNumber < 100000)
	{
		sprintf(keyNumberText, "%i", *keyNumber);
	}
	keyText = keyNumberText;
	if(strlen(keyText) == 1 && keyText[0] == '0')
	{
		keyText[0] = 0;
	}
}

void deInitKeyboard()
{
	keyLength = 0;
	keyEnter = 0;
	keyNumber = (int*)NULL;
	if(keyText == keyNumberText)
	{
		memset(keyNumberText, 0, 6);
	}
	keyText = (char *)NULL;
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
			if(!keyNumber)
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
		}
		else if(c == BSP)
		{
			if(keyText[0] != 0)
			{
				keyText[strlen(keyText)-1] = 0;
				returnVal = 1;
			}
		}
		else if(c == RET)
		{
			unsigned int strlenKeyText = strlen(keyText);
			if(strlenKeyText < keyLength)
			{
				keyText[strlenKeyText] = keyEnter;
			}
			returnVal = 2;
		}
		else if(c)
		{
			unsigned int strlenKeyText = strlen(keyText);
			if(strlenKeyText < keyLength)
			{
				if(keyNumber && ((c < '0' || c > '9') || (strlenKeyText < 2 && c == '0')))
				{
				}
				else
				{
					if(m_Mode == keyboardShift)
					{
						m_Mode = keyboardNormal;
						drawKeyboard();
					}

					keyText[strlenKeyText] = c;
					returnVal = 1;
				}
			}
		}
	}
	if(keyNumber)
	{
		*keyNumber = atoi(keyText);
	}

	return returnVal;
}
