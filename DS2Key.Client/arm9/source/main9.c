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
#include <stdlib.h>
#include <string.h>

#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "keyboard.h"

#include "keyboardTiles_bin.h"
#include "keyboardMap_bin.h"
#include "keyboardPal_bin.h"
#include "spritesTiles_bin.h"
#include "spritesPal_bin.h"
#include "gh.h"

#define waitForVBL() \
	{ \
		while(REG_VCOUNT > SCREEN_HEIGHT); \
		while(REG_VCOUNT < SCREEN_HEIGHT); \
	}

#define MSG_WIFI_INITIALIZE				0x10000001
#define MSG_WIFI_SYNC					0x10000002

#define min(a, b)						(((a) < (b)) ? (a) : (b))
#define max(a, b)						(((a) > (b)) ? (a) : (b))
#define stylusBetween(x1, y1, x2, y2)	(stylusPos.px >= min((x1), (x2)) && stylusPos.px <= max((x1), (x2)) && stylusPos.py >= min((y1), (y2)) && stylusPos.py <= max((y1), (y2)))

#define SPRITE_MOUSE					0
#define SPRITE_SETTINGS					1
#define SPRITE_HIDDEN_X					SCREEN_WIDTH
#define SPRITE_HIDDEN_Y					SCREEN_HEIGHT
#define SPRITE_SETTINGS_X				0
#define SPRITE_SETTINGS_Y				0
#define SPRITE_MOUSE_X					244
#define SPRITE_MOUSE_Y					175

bool screenPos = 0;
bool mouseLock = false;
bool settings = false;

//a global copy of sprite attribute memory
SpriteEntry sprites[128];

//rotation attributes overlap so assign then to the same location
pSpriteRotation spriteRotations = (pSpriteRotation) sprites;
int spriteRotationAngle = 0;

//copy our sprite to object attribute memory
void updateOAM(void)
{
	DC_FlushRange(sprites, 128 * sizeof(SpriteEntry));
	dmaCopy(sprites, OAM, 128 * sizeof(SpriteEntry));
}

//turn off all the sprites
void initSprites(void)
{
	int i;
	for(i = 0; i < SPRITE_COUNT; i++)
	{
		sprites[i].attribute[0] = ATTR0_DISABLED;
		sprites[i].attribute[1] = 0;
		sprites[i].attribute[2] = 0;
	}

	waitForVBL();
	updateOAM();
}

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

//---------------------------------------------------------------------------------
//Dswifi helper functions
//wifi timer function, to update internals of sgIP
void Timer_50ms(void)
{
	Wifi_Timer(50);
}

//notification function to send fifo message to arm7
void arm9_synctoarm7()	//send fifo message
{
	REG_IPC_FIFO_TX = MSG_WIFI_SYNC;
}

//interrupt handler to receive fifo messages from arm7
void arm9_fifo()	//check incoming fifo messages
{
	u32 message = REG_IPC_FIFO_RX;

	switch(message)
	{
		case MSG_WIFI_SYNC:
			Wifi_Sync();
			break;
	}
}

void vblfunction()
{

}

unsigned long iptoi(char *ipchar)
{
	int i1 = 0;
	int i2 = 0;
	unsigned long iplong = 0;
	while(i2 < 4)
	{
		if(*ipchar == '.' || *ipchar == 0)
		{
			iplong = (iplong >> 8) | (i1 << 24);
			i1 = 0;
			i2++;
			if(*ipchar == 0 && i2 < 4)
			{
				iplong = iplong >> ((4 - i2) * 8);
				i2 = 4;
			}
		}
		else
		{
			if(*ipchar >= '0' && *ipchar <= '9')
			{
				i1 = i1 * 10 + (*ipchar - '0');
			}
		}

		ipchar++;
	}

	return iplong;
}

void sendCommand(int socket, struct sockaddr_in sockaddr, unsigned long ip, unsigned int port, char *command)
{
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = ip;
	if(sendto(socket, command, strlen(command), 0, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) >= 0)
	{
		iprintf("\x1b[22;0HSent: %s          ", command);
	}
	else
	{
		iprintf("\x1b[22;0HFailed to send: %s", command);
	}
}

#define sendCommand(a)	sendCommand(my_socket, sain, iptoi(ip), atoi(port), a)

int main(int argc, char *argv[])
{
	char cProfile[6];
	int i = 1;
	struct sockaddr_in sain;
	unsigned char portLength = 6;
	char port[portLength + 1];
	unsigned char ipLength = 16;
	char ip[ipLength + 1];
	unsigned char profileLength = 4;
	char profile[profileLength + 1];
	char lastProfile[profileLength + 1];
	int my_socket;
	lcdMainOnBottom();
	{	//setup keyboard screen
		DISPLAY_CR = MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_SPR_1D | DISPLAY_SPR_ACTIVE;
		BG0_CR = BG_COLOR_16 | BG_32x32 | BG_MAP_BASE(0) | BG_TILE_BASE(1);
		VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_SPRITE;
		VRAM_B_CR = VRAM_ENABLE | VRAM_B_MAIN_BG_0x06000000;

		dmaCopy((uint16 *)keyboardPal_bin, (uint16 *)BG_PALETTE, keyboardPal_bin_size);
		keyboardMapPointer = (uint16 *) (BG_MAP_RAM(0) + (0x40 * keyboardOffset));
		deInitKeyboard();
		dmaCopy((uint16 *)keyboardTiles_bin, (uint16 *)BG_TILE_RAM(1), keyboardTiles_bin_size);

		initSprites();
	}	//setup keyboard screen
	{	//setup console screen
		SUB_DISPLAY_CR = MODE_0_2D | DISPLAY_BG0_ACTIVE;
		SUB_BG0_CR = BG_MAP_BASE(31);
		VRAM_C_CR = VRAM_ENABLE | VRAM_C_SUB_BG_0x06200000;

		BG_PALETTE_SUB[255] = RGB15(31, 31, 31);

		consoleInitDefault((u16 *)SCREEN_BASE_BLOCK_SUB(31), (u16 *)CHAR_BASE_BLOCK_SUB(0), 16);
	}	//setup console screen

	printf("DS2Key\n-\n");

	{	//wifi init
		u32 Wifi_pass;

		//send fifo message to initialize the arm7 wifi
		REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR;	//enable & clear FIFO
		Wifi_pass = Wifi_Init(WIFIINIT_OPTION_USELED);
		REG_IPC_FIFO_TX = MSG_WIFI_INITIALIZE;
		REG_IPC_FIFO_TX = Wifi_pass;

		TIMER3_CR = 0;	//disable timer3
		irqInit();

		irqSet(IRQ_VBLANK, vblfunction);
		irqEnable(IRQ_VBLANK);
		irqSet(IRQ_TIMER3, Timer_50ms); //setup timer IRQ
		irqEnable(IRQ_TIMER3);
		irqSet(IRQ_FIFO_NOT_EMPTY, arm9_fifo);	//setup fifo IRQ
		irqEnable(IRQ_FIFO_NOT_EMPTY);

		REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_RECV_IRQ;	//enable FIFO IRQ
		Wifi_SetSyncHandler(arm9_synctoarm7);	//tell wifi lib to use our handler to notify arm7

		//set timer3
		TIMER3_DATA = TIMER_FREQ_256(50);
		TIMER3_CR = TIMER_ENABLE | TIMER_IRQ_REQ | TIMER_DIV_256;

		while(Wifi_CheckInit() == 0)	//wait for arm7 to be initted successfully
		{
			waitForVBL();
		}
	}	//wifi init

	printf("Connecting via WFC data\n");
	{	//wifi connect
		//simple WFC connect:
		Wifi_AutoConnect(); //request connect
		while(1)
		{
			int i = Wifi_AssocStatus(); //check status
			if(i == ASSOCSTATUS_ASSOCIATED)
			{
				printf("Connected successfully!\n");
				break;
			}

			if(i == ASSOCSTATUS_CANNOTCONNECT)
			{
				printf("Could not connect!\n");
				while(1);
			}
		}
	}	//wifi connect

	my_socket = socket(AF_INET, SOCK_DGRAM, 0);
	memset(port, 0, portLength);
	strcpy(port, "9501");
	memset(ip, 0, ipLength);
	strcpy(ip, "192.168.1.2");
	memset(profile, 0, profileLength);
	strcpy(profile, "0");
	memset(lastProfile, 0, profileLength);
	strcpy(lastProfile, "0");

	sain.sin_family = AF_INET;
	sain.sin_addr.s_addr = htonl(INADDR_ANY);
	sain.sin_port = htons(atoi(port));
	i = bind(my_socket, (struct sockaddr *) &sain, sizeof(sain));
	if(i < 0)
	{
		printf("cannot bind port number %i\n", atoi(port));
		while(1);
	}

	i = 1;
	ioctl(my_socket, FIONBIO, &i);

	printf("Connected to server!\n");

	sprintf(cProfile, "/p%i", atoi(profile));
	sendCommand(cProfile);
	printf("%s\n", cProfile);

	{	//sprites
		int spriteGFXPos = 0;

		//mouse
		sprites[SPRITE_MOUSE].attribute[0] = ATTR0_COLOR_16 | ATTR0_NORMAL | SPRITE_MOUSE_Y;
		sprites[SPRITE_MOUSE].attribute[1] = ATTR1_SIZE_16 | SPRITE_MOUSE_X;
		sprites[SPRITE_MOUSE].attribute[2] = ATTR2_PALETTE(0) | (spriteGFXPos >> 4);
		{
			dmaCopy((uint16 *)spritesPal_bin, &SPRITE_PALETTE[0], keyboardPal_bin_size);
			dmaCopy((uint16 *)spritesTiles_bin, &SPRITE_GFX[spriteGFXPos], (keyboardTiles_bin_size << 1));
			spriteGFXPos += 8 * 8;
		}

		//settings
		sprites[SPRITE_SETTINGS].attribute[0] = ATTR0_COLOR_16 | ATTR0_ROTSCALE | SPRITE_SETTINGS_Y;
		sprites[SPRITE_SETTINGS].attribute[1] = ATTR1_SIZE_16 | SPRITE_SETTINGS_X;
		sprites[SPRITE_SETTINGS].attribute[2] = ATTR2_PALETTE(0) | (spriteGFXPos >> 4);
		{
			dmaCopy((uint16 *)spritesTiles_bin + spriteGFXPos, &SPRITE_GFX[spriteGFXPos], (keyboardTiles_bin_size << 1));
			spriteGFXPos += i;
		}

		//rotation
		spriteRotations[0].hdx = 256;
		spriteRotations[0].hdy = 0;
		spriteRotations[0].vdx = 0;
		spriteRotations[0].vdy = 256;

	}	//sprites

	initGHPad();
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

		n = recvfrom(my_socket, msg, 4, 0, (struct sockaddr *) &sain, &cliLen);

		if(n > 0)
		{
			if(!strcmp(msg, "/p?"))
			{
				sprintf(cProfile, "/p%i", atoi(profile));
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
					}
				}
				else
				{
					unsigned char keyboard = updateKeyboard(x, y);
					if(keyboard)
					{
						if(keyboard == 1)
						{
							iprintf("\x1b[3;0H        IP: %s\n\n\n        Port: %s\n\n\n        Profile: %s\n\n\n\n\n\n      ", ip, port, profile);
						}

						if(keyboard == 2)
						{
							if(strcmp(lastProfile, profile) != 0)
							{
								sprintf(cProfile, "/p%i", atoi(profile));
								sendCommand(cProfile);
								strcpy(lastProfile, profile);
							}

							toggleScreen();
							deInitKeyboard();
							iprintf("\x1b[3;0H[Edit]  IP: %s\n\n\n[Edit]  Port: %s\n\n\n[Edit]  Profile: %s\n\n\n\n\n\n[Done]", ip, port, profile);
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
							memset(keyboardMapPointer, 0, keyboardMap_bin_size >> 1);
						}
						else if(stylusBetween(0, 0, 15, 15))
						{
							sprites[SPRITE_MOUSE].posX = SPRITE_HIDDEN_X;
							sprites[SPRITE_MOUSE].posY = SPRITE_HIDDEN_Y;
							sprites[SPRITE_SETTINGS].posX = SPRITE_HIDDEN_X;
							sprites[SPRITE_SETTINGS].posY = SPRITE_HIDDEN_Y;
							settings = true;
							toggleScreen();
							deInitKeyboard();
							iprintf("\x1b[3;0H[Edit]  IP: %s\n\n\n[Edit]  Port: %s\n\n\n[Edit]  Profile: %s\n\n\n\n\n\n[Done]", ip, port, profile);
						}
					}
				}
			}
			else	//if(screenPos)
			{
				bool exit = false;
				if(stylusBetween(0, 120, 48, 128))	//done
				{
					sprites[SPRITE_MOUSE].posX = SPRITE_MOUSE_X;
					sprites[SPRITE_MOUSE].posY = SPRITE_MOUSE_Y;
					sprites[SPRITE_SETTINGS].posX = SPRITE_SETTINGS_X;
					sprites[SPRITE_SETTINGS].posY = SPRITE_SETTINGS_Y;
					deInitKeyboard();
					settings = false;
					exit = true;
				}
				else if(stylusBetween(0, 24, 48, 32))	//ip
				{
					initKeyboard(ip, ipLength, 0);
					exit = true;
				}
				else if(stylusBetween(0, 48, 48, 56))	//port
				{
					initKeyboard(port, portLength, 0);
					exit = true;
				}
				else if(stylusBetween(0, 72, 48, 80))	//profile
				{
					initKeyboard(profile, profileLength, 0);
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
						if(m_Mode == KB_NORMAL)
						{
							dmaCopy((uint16 *)keyboardMap_bin, keyboardMapPointer, keyboardMap_bin_size >> 1);
						}
						else
						{
							dmaCopy((uint16 *)keyboardMap_bin + (keyboardMap_bin_size >> 2), keyboardMapPointer, keyboardMap_bin_size >> 1);
						}
					}

					iprintf("\x1b[3;0H        IP: %s\n\n\n        Port: %s\n\n\n        Profile: %s\n\n\n\n\n\n      ", ip, port, profile);
				}
			}
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
		waitForVBL();
	}

	shutdown(my_socket, 0); //good practice to shutdown the socket.
	closesocket(my_socket); //remove the socket.

	//shut off DS
	return 0;
}
