/*
DS2Key Server - An application to use your DS as a PC Gamepad
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
#include <stdio.h>
#ifdef WIN32
#ifdef WINVER
#undef WINVER
#endif
#define WINVER 0x0500
#include <winsock.h>
#else //WIN32
#endif //WIN32
#include "main.h"
#include "config.h"
#include "vk.h"

//Variables
char currentIP[16];
bool mouseKeys[13];
bool mouseKeysLast[13];

void doInput(INPUT *input, unsigned int type, unsigned int key, bool state)
{
	input->type = type;

	if(input->type == INPUT_KEYBOARD)
	{
		input->ki.wVk = key;
		input->ki.dwFlags = KEYEVENTF_SCANCODE;

		if(state)
		{
			input->ki.dwFlags |= KEYEVENTF_KEYUP;
		}

		input->ki.wScan = MapVirtualKey(key, 0);
		input->ki.time = 0;
		input->ki.dwExtraInfo = 0;
	}
	else if(input->type == INPUT_MOUSE)
	{
		input->mi.dx = 65535 * (key & 0xff) / 256;
		input->mi.dy = 65535 * ((key >> 8) & 0xff) / 192;
		input->mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

		//click, not working yet
		/*
		if(state)
		{
		printf("state");
		input->mi.dwFlags |= MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTDOWN;
		}
		*/
		input->mi.dwExtraInfo = 0;
		input->mi.mouseData = 0;
		input->mi.time = 0;
	}

	SendInput(1, (LPINPUT)input, sizeof(INPUT));
}

char *longToIP(unsigned long longIP, char *charIP)
{
	sprintf(charIP, "%i.%i.%i.%i", (((unsigned char *)&longIP)[0]), ((unsigned char *)&longIP)[1], ((unsigned char *)&longIP)[2], ((unsigned char *)&longIP)[3]);

	return charIP;
}

int main(int argc, char *argv[])
{
	int sd, rc, n, cliLen;
	struct sockaddr_in cliAddr, servAddr;
	char msg[MAX_MSG];
#ifdef WIN32
	WSADATA wsaData;
	INPUT input;
#else //WIN32
#endif //WIN32
	int pI;
	for(pI = 0; pI <= 255; pI++)
	{
		((unsigned long *)&profile[pI])[pIP] = 0;
	}

	initVKTable();

	memset(mouseKeys, 0, 12);
	memset(mouseKeysLast, 0, 12);

	readConfig();
	{   //setup connections
#ifdef WIN32
		WSAStartup(0x0202, &wsaData);   //windows socket startup
#endif //WIN32

		//socket creation
		sd = socket(AF_INET, SOCK_DGRAM, 0);

		if(sd < 0)
		{
			fprintf(stderr, "%s: cannot open socket\n", argv[0]);
			exit(1);
		}

		//bind local server port
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons((unsigned short)serverPort);
		rc = bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));

		if(rc < 0)
		{
			fprintf(stderr, "%s: cannot bind port number %d\n", argv[0], serverPort);
			exit(1);
		}

		printf("%s: waiting for data on port UDP %u\n", argv[0], serverPort);
	}   //setup connections

	//server infinite loop
	while(1)
	{
		char *ip;
		unsigned int currentProfile;

		//init buffer
		memset(msg, 0x0, MAX_MSG);

		//receive message
		cliLen = sizeof(cliAddr);
		n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *)&cliAddr, &cliLen);
		ip = longToIP(cliAddr.sin_addr.S_un.S_addr, currentIP);

		if(n < 0)
		{
			printf("%s: cannot receive data\n", argv[0]);
			continue;
		}

		if(!strnicmp(msg, "/p", 2))
		{
			for(currentProfile = 0; currentProfile <= 255; currentProfile++)
			{
				if(cliAddr.sin_addr.S_un.S_addr == ((unsigned long *)&profile[currentProfile])[0])
				{
					((unsigned long *)&profile[currentProfile])[0] = 0;
				}
			}

			readProfileConfig(atoi(&msg[2]));

			printf("%s: [%s] profile set to %s\n", ip, msg, &msg[2]);
			((unsigned long *)&profile[atoi(&msg[2])])[0] = cliAddr.sin_addr.S_un.S_addr;
		}
		else
		{
			bool noProfile = FALSE;
			for(currentProfile = 0; currentProfile <= 255; currentProfile++)
			{
				if(cliAddr.sin_addr.S_un.S_addr == ((unsigned long *)&profile[currentProfile])[0])
				{
					break;
				}
				else if(currentProfile == 255)
				{
					char *command = "/p?";

					if(sendto(sd, command, strlen(command), 0, (struct sockaddr *)&cliAddr, sizeof(cliAddr)) >= 0)
					{
						printf("Sent: %s\n", command);
					}
					else
					{
						printf("Failed to send: %s\n", command);
					}

					noProfile = TRUE;

					break;
				}
			}

			if(noProfile)
			{
			}
			else if(!stricmp(msg, "/dl0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pLeft], 0);
				printf("%s: [%s] left button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/dl1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pLeft], 1);
				printf("%s: [%s] left button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/dr0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pRight], 0);
				printf("%s: [%s] right button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/dr1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pRight], 1);
				printf("%s: [%s] right button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/du0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pUp], 0);
				printf("%s: [%s] up button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/du1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pUp], 1);
				printf("%s: [%s] up button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/dd0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pDown], 0);
				printf("%s: [%s] down button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/dd1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pDown], 1);
				printf("%s: [%s] down button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/ba0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pA], 0);
				printf("%s: [%s] a button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/ba1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pA], 1);
				printf("%s: [%s] a button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/bb0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pB], 0);
				printf("%s: [%s] b button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/bb1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pB], 1);
				printf("%s: [%s] b button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/bx0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pX], 0);
				printf("%s: [%s] x button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/bx1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pX], 1);
				printf("%s: [%s] x button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/by0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pY], 0);
				printf("%s: [%s] y button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/by1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pY], 0);
				printf("%s: [%s] y button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/bl0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pL], 0);
				printf("%s: [%s] l button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/bl1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pL], 1);
				printf("%s: [%s] l button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/br0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pR], 0);
				printf("%s: [%s] r button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/br1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pR], 1);
				printf("%s: [%s] r button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/bt0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pStart], 0);
				printf("%s: [%s] start button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/bt1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pStart], 1);
				printf("%s: [%s] start button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/be0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pSelect], 0);
				printf("%s: [%s] select button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/be1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pSelect], 1);
				printf("%s: [%s] select button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/gb0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pBlue], 0);
				printf("%s: [%s] blue button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/gb1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pBlue], 1);
				printf("%s: [%s] blue button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/gy0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pYellow], 0);
				printf("%s: [%s] yellow button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/gy1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pYellow], 1);
				printf("%s: [%s] yellow button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/gr0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pRed], 0);
				printf("%s: [%s] red button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/gr1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pRed], 1);
				printf("%s: [%s] red button released\n", ip, msg);
			}
			else if(!stricmp(msg, "/gg0"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pGreen], 0);
				printf("%s: [%s] green button pressed\n", ip, msg);
			}
			else if(!stricmp(msg, "/gg1"))
			{
				doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pGreen], 1);
				printf("%s: [%s] green button released\n", ip, msg);
			}
			else if(!strnicmp(msg, "/m", 2))
			{
				char *xc = &msg[2];
				char *yc = strchr(xc, '.');
				char *zc = strchr(&yc[1], '.');
				unsigned char x;
				unsigned char y;
				unsigned char buttonX;
				unsigned char buttonY;
				bool z;
				bool commandSent = 0;
				x = (unsigned char)atoi(xc);
				y = (unsigned char)atoi(&yc[1]);
				z = (bool)atoi(&zc[1]);
				buttonX = (x / 64);
				buttonY = (y / 64);

				if(buttonX == 0)
				{
					if(buttonY == 0)
					{
						if(strcmp("KEY_NONE", getVKString(profile[currentProfile][pTouch0X0Y])))
						{
							mouseKeys[0] = 1; //doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pTouch0X0Y], 0);
							commandSent = 1;
						}
					}
					else if(buttonY == 1)
					{
						if(strcmp("KEY_NONE", getVKString(profile[currentProfile][pTouch0X1Y])))
						{
							mouseKeys[4] = 1;
							commandSent = 1;
						}
					}
					else if(buttonY == 2)
					{
						if(strcmp("KEY_NONE", getVKString(profile[currentProfile][pTouch0X2Y])))
						{
							mouseKeys[8] = 1;
							commandSent = 1;
						}
					}
				}
				else if(buttonX == 1)
				{
					if(buttonY == 0)
					{
						if(strcmp("KEY_NONE", getVKString(profile[currentProfile][pTouch1X0Y])))
						{
							mouseKeys[1] = 1;
							commandSent = 1;
						}
					}
					else if(buttonY == 1)
					{
						if(strcmp("KEY_NONE", getVKString(profile[currentProfile][pTouch1X1Y])))
						{
							mouseKeys[5] = 1;
							commandSent = 1;
						}
					}
					else if(buttonY == 2)
					{
						if(strcmp("KEY_NONE", getVKString(profile[currentProfile][pTouch1X2Y])))
						{
							mouseKeys[9] = 1;
							commandSent = 1;
						}
					}
				}
				else if(buttonX == 2)
				{
					if(buttonY == 0)
					{
						if(strcmp("KEY_NONE", getVKString(profile[currentProfile][pTouch2X0Y])))
						{
							mouseKeys[2] = 1;
							commandSent = 1;
						}
					}
					else if(buttonY == 1)
					{
						if(strcmp("KEY_NONE", getVKString(profile[currentProfile][pTouch2X1Y])))
						{
							mouseKeys[6] = 1;
							commandSent = 1;
						}
					}
					else if(buttonY == 2)
					{
						if(strcmp("KEY_NONE", getVKString(profile[currentProfile][pTouch2X2Y])))
						{
							mouseKeys[10] = 1;
							commandSent = 1;
						}
					}
				}
				else if(buttonX == 3)
				{
					if(buttonY == 0)
					{
						if(strcmp("KEY_NONE", getVKString(profile[currentProfile][pTouch3X0Y])))
						{
							mouseKeys[3] = 1;
							commandSent = 1;
						}
					}
					else if(buttonY == 1)
					{
						if(strcmp("KEY_NONE", getVKString(profile[currentProfile][pTouch3X1Y])))
						{
							mouseKeys[7] = 1;
							commandSent = 1;
						}
					}
					else if(buttonY == 2)
					{
						if(strcmp("KEY_NONE", getVKString(profile[currentProfile][pTouch3X2Y])))
						{
							mouseKeys[11] = 1;
							commandSent = 1;
						}
					}
				}

				if(commandSent == 0)
				{
					doInput(&input, INPUT_MOUSE, (y << 8) | x, 0);

					if(z)
					{
						printf("%s: [%s] touch screen pressed at %i, %i\n", ip, msg, x, y);
					}
					else
					{
						printf("%s: [%s] touch screen released at %i, %i\n", ip, msg, x, y);
					}
				}
				else
				{
					int key;
					for(key = 0; key < 12; key++)
					{
						int status = 0;

						if(mouseKeys[key] && !z)
						{
							mouseKeys[key] = 0;
							printf("\n\n-key: %i, %i\n", key, mouseKeys[key]);
							doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pTouch + key], !mouseKeys[key]);
							mouseKeysLast[key] = 0;
							status = 1;
						}
						else if(mouseKeys[key] != mouseKeysLast[key])
						{
							printf("\n\n+key: %i, %i\n", key, mouseKeys[key]);
							doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pTouch + key], !mouseKeys[key]);
							mouseKeysLast[key] = mouseKeys[key];
							status = mouseKeys[key] + 1;
						}

						if(status == 1)
						{
							printf("%s: [%s] released touch screen button %i\n", ip, msg, key);
						}
						else if(status == 2)
						{
							printf("%s: [%s] pressed touch screen button %i\n", ip, msg, key);
						}

						mouseKeys[key] = 0;
					}
				}
			}
			else
			{
				printf("%s: [%s] undefined command\n", ip, msg);
			}
		}
	}   //end of server infinite loop

	return 0;
}
