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
