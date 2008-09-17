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
#include <windows.h>
#include <winsock.h>
#define stricmp _stricmp
#define strnicmp _strnicmp
#define sscanf sscanf_s
typedef unsigned char bool;
#else//WIN32
#include <sys/types.h>
#include <string.h>//memset()
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>//close()
#endif//WIN32
#include "main.h"

void doInput(INPUT *input, unsigned int type, unsigned int key, bool state)
{
	input->type = type;
	if(input->type == INPUT_KEYBOARD)
	{
		input->ki.wVk = key;
		input->ki.dwFlags = KEYEVENTF_SCANCODE;
		if(state)
			input->ki.dwFlags |= KEYEVENTF_KEYUP;
		input->ki.wScan = MapVirtualKey(key, 0);
		input->ki.time = 0;
		input->ki.dwExtraInfo = 0;
	}
	else if(input->type == INPUT_MOUSE)
	{
		input->mi.dx = 65535*(key&0xff) / 256;
		input->mi.dy = 65535*((key >> 8)&0xff) / 192;
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
	SendInput(1,(LPINPUT)input,sizeof(INPUT));
}

int main(int argc, char *argv[])
{
	int sd, rc, n, cliLen, serverPort;
	struct sockaddr_in cliAddr, servAddr;
	char msg[MAX_MSG];
#ifdef WIN32
	WSADATA wsaData;
	INPUT input;
#else//WIN32
#endif//WIN32
	enum pKeys{pUp, pDown, pLeft, pRight, pA, pB, pX, pY, pL, pR, pStart, pSelect, pBlue, pYellow, pRed, pGreen, pEND};
	unsigned int profile[1][pEND];
	profile[0][pUp] = VK_UP;
	profile[0][pDown] = VK_DOWN;
	profile[0][pLeft] = VK_LEFT;
	profile[0][pRight] = VK_RIGHT;
	profile[0][pA] = 'A';
	profile[0][pB] = 'B';
	profile[0][pX] = 'X';
	profile[0][pY] = 'Y';
	profile[0][pL] = 'L';
	profile[0][pR] = 'R';
	profile[0][pStart] = VK_RETURN;
	profile[0][pSelect] = VK_SELECT;
	profile[0][pBlue] = '1';
	profile[0][pYellow] = '2';
	profile[0][pRed] = '3';
	profile[0][pGreen] = '4';
	profile[1][pUp] = 'Q';
	profile[1][pDown] = 'W';
	profile[1][pLeft] = 'E';
	profile[1][pRight] = 'T';
	profile[1][pA] = 'U';
	profile[1][pB] = 'I';
	profile[1][pX] = 'O';
	profile[1][pY] = 'P';
	profile[1][pL] = 'S';
	profile[1][pR] = 'D';
	profile[1][pStart] = 'F';
	profile[1][pSelect] = 'G';
	profile[1][pBlue] = '5';
	profile[1][pYellow] = '6';
	profile[1][pRed] = '7';
	profile[1][pGreen] = '8';

	{//read Arguments
		if(argc > 1)
		{
			unsigned int i;
			unsigned int strlenargv1 = strlen(argv[1]);
			for(i = 0; i < strlenargv1; i++)
			{
				if(argv[1][i] < '0' || argv[1][i] > '9' || strlenargv1 > 5)
				{
					printf("Usage: %s [port]\n", argv[0]);
					exit(1);
				}
			}
			serverPort = atoi(argv[1]);
		}
		else
		{
			serverPort = LOCAL_SERVER_PORT;
		}

		if(serverPort <= 0)
		{
			fprintf(stderr,"%i: bad port number\n", serverPort);
			exit(1);
		}
	}//read Arguments

	{//setup connections
	#ifdef WIN32
		WSAStartup(0x0202, &wsaData);//windows socket startup
	#endif//WIN32

		//socket creation
		sd=socket(AF_INET, SOCK_DGRAM, 0);
		if(sd < 0)
		{
			fprintf(stderr,"%s: cannot open socket\n", argv[0]);
			exit(1);
		}

		//bind local server port
		servAddr.sin_family = AF_INET;
		servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servAddr.sin_port = htons((unsigned short)serverPort);
		rc = bind(sd, (struct sockaddr*) &servAddr, sizeof(servAddr));
		if(rc < 0)
		{
			fprintf(stderr,"%s: cannot bind port number %d\n", argv[0], serverPort);
			exit(1);
		}

		printf("%s: waiting for data on port UDP %u\n", argv[0], serverPort);
	}//setup connections

	//server infinite loop
	while(1)
	{
		//init buffer
		memset(msg, 0x0, MAX_MSG);

		//receive message
		cliLen = sizeof(cliAddr);
		n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr*) &cliAddr, &cliLen);

		if(n < 0)
		{
			printf("%s: cannot receive data\n", argv[0]);
			continue;
		}

		if(!strnicmp(msg, "/p", 2	))
		{
			printf("%s: profile set to %s\n", msg, &msg[2]);
		}
		else if(!stricmp(msg, "/dl0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pLeft], 0);
			printf("%s: left button pressed\n", msg);
		}
		else if(!stricmp(msg, "/dl1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pLeft], 1);
			printf("%s: left button released\n", msg);
		}
		else if(!stricmp(msg, "/dr0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pRight], 0);
			printf("%s: right button pressed\n", msg);
		}
		else if(!stricmp(msg, "/dr1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pRight], 1);
			printf("%s: right button released\n", msg);
		}
		else if(!stricmp(msg, "/du0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pUp], 0);
			printf("%s: up button pressed\n", msg);
		}
		else if(!stricmp(msg, "/du1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pUp], 1);
			printf("%s: up button released\n", msg);
		}
		else if(!stricmp(msg, "/dd0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pDown], 0);
			printf("%s: down button pressed\n", msg);
		}
		else if(!stricmp(msg, "/dd1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pDown], 1);
			printf("%s: down button released\n", msg);
		}
		else if(!stricmp(msg, "/ba0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pA], 0);
			printf("%s: a button pressed\n", msg);
		}
		else if(!stricmp(msg, "/ba1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pA], 1);
			printf("%s: a button released\n", msg);
		}
		else if(!stricmp(msg, "/bb0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pB], 0);
			printf("%s: b button pressed\n", msg);
		}
		else if(!stricmp(msg, "/bb1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pB], 1);
			printf("%s: b button released\n", msg);
		}
		else if(!stricmp(msg, "/bx0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pX], 0);
			printf("%s: x button pressed\n", msg);
		}
		else if(!stricmp(msg, "/bx1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pX], 1);
			printf("%s: x button released\n", msg);
		}
		else if(!stricmp(msg, "/by0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pY], 0);
			printf("%s: y button pressed\n", msg);
		}
		else if(!stricmp(msg, "/by1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pY], 1);
			printf("%s: y button released\n", msg);
		}
		else if(!stricmp(msg, "/bl0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pL], 0);
			printf("%s: l button pressed\n", msg);
		}
		else if(!stricmp(msg, "/bl1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pL], 1);
			printf("%s: l button released\n", msg);
		}
		else if(!stricmp(msg, "/br0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pR], 0);
			printf("%s: r button pressed\n", msg);
		}
		else if(!stricmp(msg, "/br1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pR], 1);
			printf("%s: r button released\n", msg);
		}
		else if(!stricmp(msg, "/bt0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pStart], 0);
			printf("%s: start button pressed\n", msg);
		}
		else if(!stricmp(msg, "/bt1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pStart], 1);
			printf("%s: start button released\n", msg);
		}
		else if(!stricmp(msg, "/be0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pSelect], 0);
			printf("%s: select button pressed\n", msg);
		}
		else if(!stricmp(msg, "/be1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pSelect], 1);
			printf("%s: select button released\n", msg);
		}
		else if(!stricmp(msg, "/gb0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pBlue], 0);
			printf("%s: blue button pressed\n", msg);
		}
		else if(!stricmp(msg, "/gb1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pBlue], 1);
			printf("%s: blue button released\n", msg);
		}
		else if(!stricmp(msg, "/gy0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pYellow], 0);
			printf("%s: yellow button pressed\n", msg);
		}
		else if(!stricmp(msg, "/gy1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pYellow], 1);
			printf("%s: yellow button released\n", msg);
		}
		else if(!stricmp(msg, "/gr0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pRed], 0);
			printf("%s: red button pressed\n", msg);
		}
		else if(!stricmp(msg, "/gr1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pRed], 1);
			printf("%s: red button released\n", msg);
		}
		else if(!stricmp(msg, "/gg0"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pGreen], 0);
			printf("%s: green button pressed\n", msg);
		}
		else if(!stricmp(msg, "/gg1"))
		{
			doInput(&input, INPUT_KEYBOARD, profile[0][pGreen], 1);
			printf("%s: green button released\n", msg);
		}
		else if(!strnicmp(msg, "/m", 2))
		{
			char *xc = &msg[2];
			char *yc = strchr(xc,'.');
			char *zc = strchr(yc, '.');
			unsigned char x;
			unsigned char y;
			bool z;
			yc[0] = 0;
			yc++;
			zc[0] = 0;
			zc++;
			x = (unsigned char)atoi(xc);
			y = (unsigned char)atoi(yc);
			z = (unsigned char)atoi(zc);
			yc--;
			zc--;
			yc[0] = '.';
			zc[0] = '.';
			doInput(&input, INPUT_MOUSE, (y << 8) | x, 0);
			if(z == 0)
				printf("%s: touch screen pressed at %i, %i\n", msg, x, y);
			else//z != 0
				printf("%s: touch screen released at %i, %i\n", msg, x, y);
		}
		else
			printf("%s: undefined command\n", msg);

	}//end of server infinite loop

	return 0;
}
