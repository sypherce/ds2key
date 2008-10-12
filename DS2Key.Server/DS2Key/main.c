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
#define sockaddr_in__address(sockaddr_in) sockaddr_in.sin_addr.S_un.S_addr
#else //WIN32
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <X11/Xlib.h>
#define sockaddr_in__address(sockaddr_in) sockaddr_in.sin_addr.s_addr
#endif //WIN32

#ifdef GUI
#include "iup.h"
#include "gui.h"
#endif //GUI

#include "main.h"
#include "config.h"
#include "key.h"

//Variables
#ifdef WIN32
INPUT input;
#else //WIN32
int screen;
Display *display;
#endif //WIN32
bool mouseKeys[13];
bool mouseKeysLast[13];
struct sockaddr_in servAddr;
int sd;
char displayText[1024];

void doInput(unsigned int type, unsigned int key, bool state)
{
#ifdef WIN32
	input.type = type;

	if(input.type == INPUT_KEYBOARD)
	{
		if(key == KEY_LBUTTON || key == KEY_RBUTTON || key == KEY_MBUTTON)
		{
			input.type = INPUT_MOUSE;
			input.mi.dx = 0;
			input.mi.dy = 0;
			input.mi.dwExtraInfo = 0;
			input.mi.mouseData = 0;
			input.mi.time = 0;

			if(key == KEY_LBUTTON)
			{
				if(state)
				{
					input.mi.dwFlags = MOUSEEVENTF_LEFTUP | MOUSEEVENTF_MOVE;
				}
				else
				{
					input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_MOVE;
				}
			}
			else if(key == KEY_RBUTTON)
			{
				if(state)
				{
					input.mi.dwFlags = MOUSEEVENTF_RIGHTUP | MOUSEEVENTF_MOVE;
				}
				else
				{
					input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_MOVE;
				}
			}
			else if(key == KEY_MBUTTON)
			{
				if(state)
				{
					input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP | MOUSEEVENTF_MOVE;
				}
				else
				{
					input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MOVE;
				}
			}
		}
		else
		{
			input.ki.wVk = key;
			input.ki.dwFlags = KEYEVENTF_SCANCODE;

			if(state)
			{
				input.ki.dwFlags |= KEYEVENTF_KEYUP;
			}

			input.ki.wScan = MapVirtualKey(key, 0);
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
		}
	}
	else if(input.type == INPUT_MOUSE)
	{
		input.mi.dx = 65535 * (key & 0xff) / 256;
		input.mi.dy = 65535 * ((key >> 8) & 0xff) / 192;
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		input.mi.dwExtraInfo = 0;
		input.mi.mouseData = 0;
		input.mi.time = 0;
	}

	SendInput(1, (LPINPUT)&input, sizeof(INPUT));
#else
	int code = XKeysymToKeycode(display, key);

	XTestFakeKeyEvent(display, code, !state, CurrentTime);
	XFlush(display);
#endif
}

void serverLoop()
{
	int n, cliLen;
	struct sockaddr_in cliAddr;
	char msg[MAX_MSG];
	char *ip;
	unsigned int currentProfile;

	//init buffer
	memset(msg, 0x0, MAX_MSG);

	//receive message
	cliLen = sizeof(cliAddr);
	n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *)&cliAddr, &cliLen);
	ip = inet_ntoa(cliAddr.sin_addr);

	if(n < 0)
	{
		sprintf(displayText, "Cannot receive data");
		return;
	}

	if(!strnicmp(msg, "/p", 2))
	{
		for(currentProfile = 0; currentProfile <= 255; currentProfile++)
		{
			if(sockaddr_in__address(cliAddr) == ((unsigned long *)&profile[currentProfile])[0])
			{
				((unsigned long *)&profile[currentProfile])[0] = 0;
			}
		}

		readProfileConfig(atoi(&msg[2]));

		sprintf(displayText, "%s: [%s] profile set to %s", ip, msg, &msg[2]);
		((unsigned long *)&profile[atoi(&msg[2])])[0] = sockaddr_in__address(cliAddr);
	}
	else
	{
		bool noProfile = 0;
		for(currentProfile = 0; currentProfile <= 255; currentProfile++)
		{
			if(sockaddr_in__address(cliAddr) == ((unsigned long *)&profile[currentProfile])[0])
			{
				break;
			}
			else if(currentProfile == 255)
			{
				char *command = "/p?";

				if(sendto(sd, command, strlen(command), 0, (struct sockaddr *)&cliAddr, sizeof(cliAddr)) >= 0)
				{
					sprintf(displayText, "Sent: %s", command);
				}
				else
				{
					sprintf(displayText, "Failed to send: %s", command);
				}

				noProfile = 1;

				break;
			}
		}

		if(noProfile)
		{
		}
		else if(!stricmp(msg, "/dl0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pLeft], 0);
			sprintf(displayText, "%s: [%s] left button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/dl1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pLeft], 1);
			sprintf(displayText, "%s: [%s] left button released", ip, msg);
		}
		else if(!stricmp(msg, "/dr0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pRight], 0);
			sprintf(displayText, "%s: [%s] right button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/dr1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pRight], 1);
			sprintf(displayText, "%s: [%s] right button released", ip, msg);
		}
		else if(!stricmp(msg, "/du0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pUp], 0);
			sprintf(displayText, "%s: [%s] up button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/du1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pUp], 1);
			sprintf(displayText, "%s: [%s] up button released", ip, msg);
		}
		else if(!stricmp(msg, "/dd0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pDown], 0);
			sprintf(displayText, "%s: [%s] down button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/dd1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pDown], 1);
			sprintf(displayText, "%s: [%s] down button released", ip, msg);
		}
		else if(!stricmp(msg, "/ba0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pA], 0);
			sprintf(displayText, "%s: [%s] a button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/ba1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pA], 1);
			sprintf(displayText, "%s: [%s] a button released", ip, msg);
		}
		else if(!stricmp(msg, "/bb0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pB], 0);
			sprintf(displayText, "%s: [%s] b button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/bb1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pB], 1);
			sprintf(displayText, "%s: [%s] b button released", ip, msg);
		}
		else if(!stricmp(msg, "/bx0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pX], 0);
			sprintf(displayText, "%s: [%s] x button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/bx1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pX], 1);
			sprintf(displayText, "%s: [%s] x button released", ip, msg);
		}
		else if(!stricmp(msg, "/by0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pY], 0);
			sprintf(displayText, "%s: [%s] y button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/by1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pY], 1);
			sprintf(displayText, "%s: [%s] y button released", ip, msg);
		}
		else if(!stricmp(msg, "/bl0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pL], 0);
			sprintf(displayText, "%s: [%s] l button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/bl1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pL], 1);
			sprintf(displayText, "%s: [%s] l button released", ip, msg);
		}
		else if(!stricmp(msg, "/br0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pR], 0);
			sprintf(displayText, "%s: [%s] r button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/br1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pR], 1);
			sprintf(displayText, "%s: [%s] r button released", ip, msg);
		}
		else if(!stricmp(msg, "/bt0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pStart], 0);
			sprintf(displayText, "%s: [%s] start button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/bt1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pStart], 1);
			sprintf(displayText, "%s: [%s] start button released", ip, msg);
		}
		else if(!stricmp(msg, "/be0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pSelect], 0);
			sprintf(displayText, "%s: [%s] select button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/be1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pSelect], 1);
			sprintf(displayText, "%s: [%s] select button released", ip, msg);
		}
		else if(!stricmp(msg, "/gb0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pBlue], 0);
			sprintf(displayText, "%s: [%s] blue button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/gb1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pBlue], 1);
			sprintf(displayText, "%s: [%s] blue button released", ip, msg);
		}
		else if(!stricmp(msg, "/gy0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pYellow], 0);
			sprintf(displayText, "%s: [%s] yellow button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/gy1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pYellow], 1);
			sprintf(displayText, "%s: [%s] yellow button released", ip, msg);
		}
		else if(!stricmp(msg, "/gr0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pRed], 0);
			sprintf(displayText, "%s: [%s] red button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/gr1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pRed], 1);
			sprintf(displayText, "%s: [%s] red button released", ip, msg);
		}
		else if(!stricmp(msg, "/gg0"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pGreen], 0);
			sprintf(displayText, "%s: [%s] green button pressed", ip, msg);
		}
		else if(!stricmp(msg, "/gg1"))
		{
			doInput(INPUT_KEYBOARD, profile[currentProfile][pGreen], 1);
			sprintf(displayText, "%s: [%s] green button released", ip, msg);
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
					if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch0X0Y])))
					{
						mouseKeys[0] = 1; //doInput(INPUT_KEYBOARD, profile[currentProfile][pTouch0X0Y], 0);
						commandSent = 1;
					}
				}
				else if(buttonY == 1)
				{
					if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch0X1Y])))
					{
						mouseKeys[4] = 1;
						commandSent = 1;
					}
				}
				else if(buttonY == 2)
				{
					if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch0X2Y])))
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
					if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch1X0Y])))
					{
						mouseKeys[1] = 1;
						commandSent = 1;
					}
				}
				else if(buttonY == 1)
				{
					if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch1X1Y])))
					{
						mouseKeys[5] = 1;
						commandSent = 1;
					}
				}
				else if(buttonY == 2)
				{
					if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch1X2Y])))
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
					if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch2X0Y])))
					{
						mouseKeys[2] = 1;
						commandSent = 1;
					}
				}
				else if(buttonY == 1)
				{
					if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch2X1Y])))
					{
						mouseKeys[6] = 1;
						commandSent = 1;
					}
				}
				else if(buttonY == 2)
				{
					if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch2X2Y])))
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
					if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch3X0Y])))
					{
						mouseKeys[3] = 1;
						commandSent = 1;
					}
				}
				else if(buttonY == 1)
				{
					if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch3X1Y])))
					{
						mouseKeys[7] = 1;
						commandSent = 1;
					}
				}
				else if(buttonY == 2)
				{
					if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch3X2Y])))
					{
						mouseKeys[11] = 1;
						commandSent = 1;
					}
				}
			}

			if(commandSent == 0)
			{
				doInput(INPUT_MOUSE, (y << 8) | x, 0);

				if(z)
				{
					sprintf(displayText, "%s: [%s] touch screen pressed at %i, %i", ip, msg, x, y);
				}
				else
				{
					sprintf(displayText, "%s: [%s] touch screen released at %i, %i", ip, msg, x, y);
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
						doInput(INPUT_KEYBOARD, profile[currentProfile][pTouch + key], !mouseKeys[key]);
						mouseKeysLast[key] = 0;
						status = 1;
					}
					else if(mouseKeys[key] != mouseKeysLast[key])
					{
						doInput(INPUT_KEYBOARD, profile[currentProfile][pTouch + key], !mouseKeys[key]);
						mouseKeysLast[key] = mouseKeys[key];
						status = mouseKeys[key] + 1;
					}

					if(status == 1)
					{
						sprintf(displayText, "%s: [%s] released touch screen button %i", ip, msg, key);
					}
					else if(status == 2)
					{
						sprintf(displayText, "%s: [%s] pressed touch screen button %i", ip, msg, key);
					}

					mouseKeys[key] = 0;
				}
			}
		}
		else
		{
			sprintf(displayText, "%s: [%s] undefined command", ip, msg);
		}
	}

	if(displayText[0] != 0)
	{
#ifdef GUI
		char *position = IupGetAttribute(ml, IUP_CARET);
		while(displayText[strlen(displayText) - 2] == 0xd && displayText[strlen(displayText) - 1] == 0x2)
		{
			displayText[strlen(displayText) - 2] = 0;
		}

		IupSetAttribute(ml, IUP_APPEND, displayText);
		IupSetAttribute(ml, IUP_CARET, position);
#else
		printf("%s\n", displayText);
#endif
		displayText[0] = 0;
	}
}

int main(int argc, char *argv[])
{
#ifdef WIN32
	WSADATA wsaData;
	unsigned long ioctlsocketCommand = 1;
#else //WIN32
#endif //WIN32
	int pI;
	for(pI = 0; pI <= 255; pI++)
	{
		((unsigned long *)&profile[pI])[pIP] = 0;
	}

#ifdef WIN32
#else //WIN32
	display = XOpenDisplay(NULL);

	if(display == NULL)
	{
		fprintf(stderr, "Unable to connect to display: %s\n", XDisplayName(NULL));
		exit(1);
	}

	screen = DefaultScreen(display);
#endif

	initKeyTable();

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
			fprintf(stderr, "Cannot open socket\n");
			exit(1);
		}

		//bind local server port
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons((unsigned short)serverPort);

#ifdef WIN32

		if(ioctlsocket(sd, FIONBIO, &ioctlsocketCommand) == -1)
#else //WIN32

		if(fcntl(sd, F_SETFL, O_NONBLOCK) == -1)
#endif //WIN32
		{
			fprintf(stderr, "Setting to non-blocking failed\n");
			exit(1);
		}

		if(bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
		{
			fprintf(stderr, "Cannot bind port number %d\n", serverPort);
			exit(1);
		}

		sprintf(displayText, "Waiting for data on port UDP %u", serverPort);
	}   //setup connections

	//infinite loop
	#ifdef GUI
	initGui();
	loopGui();
	#else
	printf("%s\n", displayText);
	while(1)
	{
		serverLoop();
	}

	#endif
	//end of infinite loop

	return 0;
}
