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
#include <fat.h>
#include <dswifi9.h>
#include <nds.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "ds2key.h"
#include "keyboard.h"
#include "sprite.h"
#include "gh.h"
#include "main9.h"

//variables
char d2kMode = iDefault;
bool inputChange = false;
uint32 kDown = 0;
uint32 kHeld = 0;
uint32 kUp = 0;
uint32 vblCount = 0;
uint32 vblLid = 0;

//functions
void updateInputs()
{
	if(inputChange)
	{
		inputChange = false;
		scanKeys();
		updateGHPad();
		kDown = keysDown();
		kHeld = keysHeld();
		kUp = keysUp();
		if(!kUp&&KEY_TOUCH)
		{
			lastStylusPos = stylusPos;
			touchRead(&stylusPos);
		}
	}
}

void updateLid()
{
	if((keysUp()&KEY_LID && vblLid)|| keysHeld()&KEY_TOUCH)
	{
	    if(keysUp()&KEY_LID)
	    {
	        vblLid = vblCount = 0;
	    }
	    else
	    {
            vblCount = 0;
	    }
	}
	else if((keysDown()&KEY_LID && !vblLid) || keysDown())
	{
	    if(keysDown()&KEY_LID)
	    {
            vblLid = vblCount = vblCountMax;
	    }
	    else
	    {
            vblCount = vblCountMax;
	    }
	}
	if(vblCount >= vblCountMax)
	{
        powerOff(PM_BACKLIGHT_BOTTOM);
	}
	else if(vblCount == 0)
	{
	    powerOn(PM_BACKLIGHT_BOTTOM);
	}
	vblCount++;
}

void vblfunction()
{
	updateInputs();
	updateLid();
	updateSprites();
}

int main()
{
	DS2Key ds2key;
	CustomKeyboard *keyboard;;
	//screen setup
	powerOff(PM_BACKLIGHT_TOP);
	videoSetModeSub(MODE_0_2D);

	vramSetMainBanks(VRAM_A_LCD, VRAM_B_LCD, VRAM_C_SUB_BG, VRAM_D_SUB_SPRITE);

    initSprites();

	//console setup
	consoleInit(0,0, BgType_Text4bpp, BgSize_T_256x256, 20, 0, false);

	//keyboard setup
	keyboard = &customKeyboard;
	keyboard->keyboard = keyboardGetDefault();
	keyboard->keyboard->OnKeyPressed = onKeyPressed;
	keyboardInit(keyboard->keyboard);

	consoleClear();

	iprintf("DS2Key 1.0 (SVN)\n-\n");

	fatInitDefault();
	readConfig(&ds2key);

	iprintf("Connecting via WFC data\n");

	if(!Wifi_InitDefault(WFC_CONNECT))
	{
		iprintf("Failed to connect to server!");
	}
	else
	{
		consoleClear();
	}

	initGHPad();

	//setup vblank IRQ
	irqSet(IRQ_VBLANK, vblfunction);

	resetStylus();
	dummyStylusPos = stylusPos;
	initD2K(&ds2key);

	while(true)
	{
		swiWaitForVBlank();
		showSprites();

		settingsPrint();

		while(1)
		{
			inputChange = true;
			swiWaitForVBlank();
			if(d2kMode == iDefault)
			{
				if(kDown&KEY_TOUCH && stylusBetween(255 - 16, 191 - 16, 255, 191))
				{
					d2kMode = iMouse;
					hideSettingsSprite();
				}
				else if(kDown&KEY_TOUCH && stylusBetween(0, 0, 15, 15))
				{
					d2kMode = iSettings;
					hideSprites();
					settingsPrintButtons();
				}
				else
				{
					updateD2K(&ds2key, kDown, kHeld, kUp, &dummyStylusPos, &dummyStylusPos);
				}
			}
			else if(d2kMode == iMouse)
			{
				if(kDown&KEY_TOUCH && stylusBetween(255 - 16, 191 - 16, 255, 191))
				{
					d2kMode = iDefault;
					showSettingsSprite();
				}
				else
				{
					updateD2K(&ds2key, kDown, kHeld, kUp, &stylusPos, &lastStylusPos);
				}
			}
			else if(d2kMode == iSettings && kDown&KEY_TOUCH)
			{
				if(stylusBetween(0, 120, 48, 128))  //done
				{
				    DS2Key tempDS2Key = ds2key;
					d2kMode = iDefault;
					showSprites();
					settingsPrint();
					writeConfig(&ds2key);
                    deinitD2K(&ds2key);
                    ds2key = tempDS2Key;
                    initD2K(&ds2key);
				}
				else if(stylusBetween(0, 24, 48, 32))   //ip
				{
					settingsPrintIP();
					keyboard->buffer = ds2key.ip;
					customKeyboardGetString(keyboard, LENGTH_IP + 1, mIP, 12, 3);
					ds2keyGetLongIP(&ds2key);
					strncpy(ds2key.ip, inet_ntoa(ds2key.sockaddr.sin_addr), 15);
					settingsPrintButtons();
					//iprintf("\x1b[3;12H%s", ds2key.ip);
					resetStylus();
				}
				else if(stylusBetween(0, 48, 48, 56))   //port
				{
					unsigned int iPort = 0;
					settingsPrintPort();
					keyboard->buffer = ds2key.port;
					customKeyboardGetString(keyboard, LENGTH_PORT + 1, mNumber, 14, 6);
					iPort = ds2keyGetIntPort(&ds2key);
					if(iPort < 0)
					{
						iPort = 0;
					}
					else if(iPort > 65535)
					{
						iPort = 65535;
					}
					sprintf(ds2key.port, "%i", iPort);
					settingsPrintButtons();
					//iprintf("\x1b[6;14H%s", ds2key.port);
					resetStylus();
				}
				else if(stylusBetween(0, 72, 48, 80))   //profile
				{
					unsigned int iProfile = 0;
					settingsPrintProfile();
					keyboard->buffer = ds2key.profile;
					customKeyboardGetString(keyboard, LENGTH_PROFILE + 1, mNumber, 17, 9);
					iProfile = ds2keyGetIntProfile(&ds2key);
					if(iProfile < 0)
					{
						iProfile = 0;
					}
					else if(iProfile > 255)
					{
						iProfile = 255;
					}
					sprintf(ds2key.profile, "%i", iProfile);
					settingsPrintButtons();
					//iprintf("\x1b[9;17H%s", ds2key.profile);
					resetStylus();
				}
			}
		}
	}
	deinitD2K(&ds2key);

	return 0;
}
