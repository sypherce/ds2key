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
#include <stdio.h>
#include <string.h>
#include "keyboard.h"

//variables
CustomKeyboard customKeyboard;

//functions
void customKeyboardGetString(CustomKeyboard *keyboard, int maxLen, int mode, unsigned char x, unsigned char y)
{
	keyboard->maxLen = maxLen;
	keyboard->mode = mode;
	keyboard->position = strlen(keyboard->buffer);

	keyboardShow();
	while(1)
	{
		keyboardUpdate();
		if(keyboard->mode != mNone)
		{
			iprintf("\x1b[%i;%iH%s ", y, x, keyboard->buffer);
		}
		else
		{
			break;
		}
	}
	keyboardHide();
	keyboard->position = 0;
	keyboard->maxLen = 0;
}

void onKeyPressed(int key)
{
	CustomKeyboard *keyboard = &customKeyboard;
	if(key == DVK_ENTER)
	{
		keyboard->mode = mNone;//break;
	}
	else if(key == DVK_BACKSPACE)
	{
		if(keyboard->position > 0)
		{
			keyboard->buffer[--keyboard->position] = 0;
		}
	}
	else if((key >= '0' && key <= '9') || (keyboard->mode == mIP && key == '.') || (keyboard->mode == mDefault && key > 0))
	{
		if(keyboard->position + 1 < keyboard->maxLen)
		{
			keyboard->buffer[keyboard->position++] = key;
		}
	}
}
