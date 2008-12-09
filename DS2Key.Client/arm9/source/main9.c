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
//includes
#include <fat.h>
#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//wifi includes
#include <dswifi9.h>
#include <netinet/in.h>

//project includes
#include "main9.h"
#include "config.h"
#include "wifi.h"
#include "sprite.h"
#include "keyboard.h"
#include "gh.h"

//common includes
#include "fifo.h"

//image includes
#include "keyboardImage.h"
#include "spritesImage.h"

//variables
bool screenPos = 0;
bool mouseLock = false;
bool settings = false;
int lightCounter = 0;
char lightTimeout = 4;
struct sockaddr_in sain;

void toggleScreen()
{
	if(screenPos)
	{
		lcdMainOnBottom();
	}
	else
	{
		lcdMainOnTop();
	}

	screenPos = !screenPos;
}

void vblfunction()
{
	lightCounter++;
}

int main(int argc, char *argv[])
{
	int i;
	int lastProfile;
	int my_socket;
	char cProfile[6];
	char *cIP;
	lcdMainOnBottom();
	{   //setup keyboard screen
		DISPLAY_CR = MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_SPR_1D | DISPLAY_SPR_ACTIVE;
		BG0_CR = BG_COLOR_16 | BG_32x32 | BG_MAP_BASE(0) | BG_TILE_BASE(1);
		VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_SPRITE;
		VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_BG_0x06000000;

		//copy keyboard palette to ram
		dmaCopy((uint16 *)keyboardImagePal, (uint16 *)BG_PALETTE, keyboardImagePalLen);
		keyboardMapPointer = (uint16 *)(BG_MAP_RAM(0) + (0x40 * keyboardOffset));
		deInitKeyboard();

		//copy keyboard tiles to ram
		dmaCopy((uint16 *)keyboardImageTiles, (uint16 *)BG_TILE_RAM(1), keyboardImageTilesLen);

		//dirty hack to make tile 0 the same as tile 1
		dmaCopy((uint16 *)BG_TILE_RAM(1) + 16, (uint16 *)BG_TILE_RAM(1), 32);

		initSprites();
	}   //setup keyboard screen
	{   //setup console screen
		SUB_DISPLAY_CR = MODE_0_2D | DISPLAY_BG0_ACTIVE;
		SUB_BG0_CR = BG_MAP_BASE(31);
		VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG_0x06200000;

		BG_PALETTE_SUB[0] = RGB15(8, 20, 29);
		BG_PALETTE_SUB[255] = RGB15(0, 0, 0);

		consoleInitDefault((u16 *)SCREEN_BASE_BLOCK_SUB(31), (u16 *)CHAR_BASE_BLOCK_SUB(0), 16);
	}   //setup console screen

	printf("DS2Key\n-\n");

	{   //setup systems
		irqInit();

		//setup vblank
		irqSet(IRQ_VBLANK, vblfunction); //setup vblank IRQ
		irqEnable(IRQ_VBLANK);

		//init fat disk system
		if(isRunningOnHW())
		{
			fatInitDefault();
		}

		//setup timer3
		TIMER3_CR = 0;  //disable timer3
		irqSet(IRQ_TIMER3, Timer_50ms); //setup timer IRQ
		irqEnable(IRQ_TIMER3);
		TIMER3_DATA = TIMER_FREQ_256(50);
		TIMER3_CR = TIMER_ENABLE | TIMER_IRQ_REQ | TIMER_DIV_256;

		//setup fifo IRQ
		irqSet(IRQ_FIFO_NOT_EMPTY, fifo);
		irqEnable(IRQ_FIFO_NOT_EMPTY);
		REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR | IPC_FIFO_RECV_IRQ;

		if(isRunningOnHW())
		{
			initWifi();
		}
		else
		{
			Wifi_Init(0);
		}
	}   //setup systems

	printf("Connecting via WFC data\n");
	wifiConnect();

	my_socket = socket(AF_INET, SOCK_DGRAM, 0);

	sain.sin_family = AF_INET;
	sain.sin_addr.s_addr = htonl(ip);
	sain.sin_port = htons(port);
	i = bind(my_socket, (struct sockaddr *)&sain, sizeof(sain));

	readConfig();
	lastProfile = profile;
	cIP = inet_ntoa(sain.sin_addr);

	if(i < 0)
	{
		printf("cannot bind port number %i\n", port);
		while(1)
		{
		}
	}

	i = 1;
	ioctl(my_socket, FIONBIO, &i);

	printf("Connected to server!\n");

	sprintf(cProfile, "/p%i", profile);
	sendCommand(cProfile);
	printf("%s\n", cProfile);

	{   //sprites
		int spriteGFXPos = 0;

		//mouse
		sprites[SPRITE_MOUSE].attribute[0] = ATTR0_COLOR_16 | ATTR0_NORMAL | SPRITE_MOUSE_Y;
		sprites[SPRITE_MOUSE].attribute[1] = ATTR1_SIZE_16 | SPRITE_MOUSE_X;
		sprites[SPRITE_MOUSE].attribute[2] = ATTR2_PALETTE(0) | (spriteGFXPos >> 4);
		{
			dmaCopy((uint16 *)spritesImagePal, &SPRITE_PALETTE[0], spritesImagePalLen);
			dmaCopy((uint16 *)spritesImageTiles, &SPRITE_GFX[spriteGFXPos], (spritesImageTilesLen << 1));
			spriteGFXPos += 8 * 8;
		}

		//settings
		sprites[SPRITE_SETTINGS].attribute[0] = ATTR0_COLOR_16 | ATTR0_ROTSCALE | SPRITE_SETTINGS_Y;
		sprites[SPRITE_SETTINGS].attribute[1] = ATTR1_SIZE_16 | SPRITE_SETTINGS_X;
		sprites[SPRITE_SETTINGS].attribute[2] = ATTR2_PALETTE(0) | (spriteGFXPos >> 4);
		{
			dmaCopy((uint16 *)spritesImageTiles + spriteGFXPos, &SPRITE_GFX[spriteGFXPos], (spritesImageTilesLen << 1));
			spriteGFXPos += i;
		}

		//rotation
		spriteRotations[0].hdx = 256;
		spriteRotations[0].hdy = 0;
		spriteRotations[0].vdx = 0;
		spriteRotations[0].vdy = 256;
	}   //sprites

	initGHPad();
	lightCounter = 0;
	while(1)
	{
		unsigned int lastx = 299;
		unsigned int lasty = 299;
		touchPosition stylusPos = touchReadXY();
		unsigned char x = stylusPos.px >> 3;
		unsigned char y = (stylusPos.py >> 3) - keyboardOffset;
		int n;
		int cliLen = sizeof(sain);
		char msg[4];
		scanKeys();
		updateGHPad();

		{
			spriteRotations[0].hdx = COS[spriteRotationAngle & 0x1FF] >> 4;
			spriteRotations[0].hdy = SIN[spriteRotationAngle & 0x1FF] >> 4;
			spriteRotations[0].vdx = -spriteRotations[0].hdy;
			spriteRotations[0].vdy = spriteRotations[0].hdx;
			spriteRotationAngle++;
		}

		updateOAM();

		if((!screenPos && !mouseLock && !settings && lightCounter >= lightTimeout * 60) || keysHeld() & KEY_LID)
		{
			turnOffBothBacklights();
		}
		else
		{
			turnOnBothBacklights();
		}

		if((keysHeld() & KEY_A) | (keysHeld() & KEY_B) | (keysHeld() & KEY_X) | (keysHeld() & KEY_Y) | (keysHeld() & KEY_L) | (keysHeld() & KEY_R) | (keysHeld() & KEY_START) | (keysHeld() & KEY_SELECT) | (keysHeld() & KEY_UP) | (keysHeld() & KEY_DOWN) | (keysHeld() & KEY_LEFT) | (keysHeld() & KEY_RIGHT) | (keysDownGH(GH_BLUE)) | (keysDownGH(GH_YELLOW)) | (keysDownGH(GH_RED)) | (keysDownGH(GH_GREEN)))
		{
			lightCounter = lightTimeout * 60;
		}
		else if(keysUp() & KEY_LID)
		{
			lightCounter = 0;
		}

		n = recvfrom(my_socket, msg, 4, 0, (struct sockaddr *)&sain, &cliLen);

		if(n > 0)
		{
			if(!strcmp(msg, "/p?"))
			{
				sprintf(cProfile, "/p%i", profile);
				sendCommand(cProfile);
			}
		}

		if(!screenPos && !settings)
		{
			if(keysDown() & KEY_A)
			{
				sendCommand("/ba0");
			}

			if(keysUp() & KEY_A)
			{
				sendCommand("/ba1");
			}

			if(keysDown() & KEY_B)
			{
				sendCommand("/bb0");
			}

			if(keysUp() & KEY_B)
			{
				sendCommand("/bb1");
			}

			if(keysDown() & KEY_X)
			{
				sendCommand("/bx0");
			}

			if(keysUp() & KEY_X)
			{
				sendCommand("/bx1");
			}

			if(keysDown() & KEY_Y)
			{
				sendCommand("/by0");
			}

			if(keysUp() & KEY_Y)
			{
				sendCommand("/by1");
			}

			if(keysDown() & KEY_L)
			{
				sendCommand("/bl0");
			}

			if(keysUp() & KEY_L)
			{
				sendCommand("/bl1");
			}

			if(keysDown() & KEY_R)
			{
				sendCommand("/br0");
			}

			if(keysUp() & KEY_R)
			{
				sendCommand("/br1");
			}

			if(keysDown() & KEY_START)
			{
				sendCommand("/bt0");
			}

			if(keysUp() & KEY_START)
			{
				sendCommand("/bt1");
			}

			if(keysDown() & KEY_SELECT)
			{
				sendCommand("/be0");
			}

			if(keysUp() & KEY_SELECT)
			{
				sendCommand("/be1");
			}

			if(keysDown() & KEY_UP)
			{
				sendCommand("/du0");
			}

			if(keysUp() & KEY_UP)
			{
				sendCommand("/du1");
			}

			if(keysDown() & KEY_DOWN)
			{
				sendCommand("/dd0");
			}

			if(keysUp() & KEY_DOWN)
			{
				sendCommand("/dd1");
			}

			if(keysDown() & KEY_LEFT)
			{
				sendCommand("/dl0");
			}

			if(keysUp() & KEY_LEFT)
			{
				sendCommand("/dl1");
			}

			if(keysDown() & KEY_RIGHT)
			{
				sendCommand("/dr0");
			}

			if(keysUp() & KEY_RIGHT)
			{
				sendCommand("/dr1");
			}

			if(keysDownGH(GH_BLUE))
			{
				sendCommand("/gb0");
			}

			if(keysUpGH(GH_BLUE))
			{
				sendCommand("/gb1");
			}

			if(keysDownGH(GH_YELLOW))
			{
				sendCommand("/gy0");
			}

			if(keysUpGH(GH_YELLOW))
			{
				sendCommand("/gy1");
			}

			if(keysDownGH(GH_RED))
			{
				sendCommand("/gr0");
			}

			if(keysUpGH(GH_RED))
			{
				sendCommand("/gr1");
			}

			if(keysDownGH(GH_GREEN))
			{
				sendCommand("/gg0");
			}

			if(keysUpGH(GH_GREEN))
			{
				sendCommand("/gg1");
			}
		}

		if(keysDown() & KEY_TOUCH)
		{
			lightCounter = 0;

			if(!screenPos)
			{
				if(mouseLock)
				{
					if(stylusBetween(255 - 16, 191 - 16, 255, 191))
					{
						//printf("Mouse Lock - Off");
						sprites[SPRITE_MOUSE].posX = SPRITE_MOUSE_X;
						sprites[SPRITE_MOUSE].posY = SPRITE_MOUSE_Y;
						sprites[SPRITE_SETTINGS].posX = SPRITE_SETTINGS_X;
						sprites[SPRITE_SETTINGS].posY = SPRITE_SETTINGS_Y;
						mouseLock = false;
						lightCounter = 0;
					}
				}
				else
				{
					unsigned char keyboard = updateKeyboard(x, y);

					if(keyboard)
					{
						if(keyboard == 1)
						{
							settingsPrint();
						}

						if(keyboard == 2)
						{
							if(lastProfile != profile)
							{
								sprintf(cProfile, "/p%i", profile);
								sendCommand(cProfile);
								lastProfile = profile;
							}

							toggleScreen();
							deInitKeyboard();
							settingsPrintButtons();
						}
					}
					else if(!settings)
					{
						if(stylusBetween(255 - 16, 191 - 16, 255, 191))
						{
							//printf("Mouse Lock - On");
							sprites[SPRITE_SETTINGS].posX = SPRITE_HIDDEN_X;
							sprites[SPRITE_SETTINGS].posY = SPRITE_HIDDEN_Y;
							mouseLock = true;
						}
						else if(stylusBetween(0, 0, 15, 15))
						{
							sprites[SPRITE_MOUSE].posX = SPRITE_HIDDEN_X;
							sprites[SPRITE_MOUSE].posY = SPRITE_HIDDEN_Y;
							sprites[SPRITE_SETTINGS].posX = SPRITE_HIDDEN_X;
							sprites[SPRITE_SETTINGS].posY = SPRITE_HIDDEN_Y;
							settings = true;
							toggleScreen();
							settingsPrintButtons();
						}
					}
				}
			}
			else    //if(screenPos)
			{
				bool exit = false;

				if(stylusBetween(0, 120, 48, 128))  //done
				{
					sprites[SPRITE_MOUSE].posX = SPRITE_MOUSE_X;
					sprites[SPRITE_MOUSE].posY = SPRITE_MOUSE_Y;
					sprites[SPRITE_SETTINGS].posX = SPRITE_SETTINGS_X;
					sprites[SPRITE_SETTINGS].posY = SPRITE_SETTINGS_Y;
					deInitKeyboard();
					settings = false;
					exit = true;
					inet_aton(cIP, &sain.sin_addr);

	bind(my_socket, (struct sockaddr *)&sain, sizeof(sain));
					ip = sain.sin_addr.s_addr;
					cIP = inet_ntoa(sain.sin_addr);
					writeConfig();
				}
				else if(stylusBetween(0, 24, 48, 32))   //ip
				{
					initKeyboardString(cIP, 15, 0);
					exit = true;
				}
				else if(stylusBetween(0, 48, 48, 56))   //port
				{
					initKeyboardInt(&port, 5, 0);
					exit = true;
				}
				else if(stylusBetween(0, 72, 48, 80))   //profile
				{
					initKeyboardInt(&profile, 3, 0);
					exit = true;
				}

				//else
				//{
				//	iprintf("\x1b[16;0H     x: %i   \ntile x: %i   \n     y: %i   \ntile y: %i   ", stylusPos.px, x, stylusPos.py, y);
				//}
				if(exit)
				{
					toggleScreen();

					if(settings)
					{
						drawKeyboard();
					}

					settingsPrint();
					lightCounter = 0;
				}
			}
		}

		if(keysHeld() & KEY_TOUCH)
		{
			lightCounter = 0;
		}

		if(mouseLock && !settings && keysHeld() & KEY_TOUCH && ((lastx != stylusPos.px || lasty != stylusPos.py) || (lastx == 299 || lasty == 299)))
		{
			char command[12];
			sprintf(command, "/m%i.%i.1", stylusPos.px, stylusPos.py);
			sendCommand(command);
		}

		if(mouseLock && !settings && keysUp() & KEY_TOUCH)
		{
			char command[12];
			sprintf(command, "/m%i.%i.0", stylusPos.px, stylusPos.py);
			sendCommand(command);
		}

		lastx = stylusPos.px;
		lasty = stylusPos.py;
		swiWaitForVBlank();
	}

	shutdown(my_socket, 0); //good practice to shutdown the socket.
	closesocket(my_socket); //remove the socket.

	//shut off DS
	return 0;
}
