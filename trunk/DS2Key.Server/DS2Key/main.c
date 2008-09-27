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
	#define WINVER	0x0500
	#include <windows.h>
	#include <winsock.h>
	#ifdef _MSC_VER
		#define stricmp		_stricmp
		#define strnicmp	_strnicmp
		#define sscanf		sscanf_s
	#endif
typedef unsigned char bool;
#else //WIN32
	#include <sys/types.h>
	#include <string.h> //memset()
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <unistd.h> //close()
#endif //WIN32
#include "main.h"
#include "VK.h"

enum pKeys { pIP, pIP2, pUp, pDown, pLeft, pRight, pA, pB, pX, pY, pL, pR, pStart, pSelect, pBlue, pYellow, pRed, pGreen, pEND };
unsigned int profile[256][pEND];

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

	SendInput(1, (LPINPUT) input, sizeof(INPUT));
}

char *longToIP(unsigned long longIP, char *charIP)
{
	#ifdef _MSC_VER
	sprintf_s(charIP, 16, "%i.%i.%i.%i", (((unsigned char *) &longIP)[0]), ((unsigned char *) &longIP)[1], ((unsigned char *) &longIP)[2], ((unsigned char *) &longIP)[3]);
	#else //_MSC_VER
	sprintf(charIP, "%i.%i.%i.%i", (((unsigned char *) &longIP)[0]), ((unsigned char *) &longIP)[1], ((unsigned char *) &longIP)[2], ((unsigned char *) &longIP)[3]);
	#endif //_MSC_VER
	return charIP;
}

int serverPort;

bool writeDefaultConfig()
{
	FILE *file;
	serverPort = LOCAL_SERVER_PORT;
	file = fopen("ds2key.ini", "w+");
	if(file)
	{
		fprintf(file, "%i\n", serverPort);
		fclose(file);
	}
	else
	{
		return 1;
	}

	return 0;
}

bool writeDefaultProfileConfig(unsigned char profileNumber)
{
	FILE *file;
	char filename[16];
	#ifdef _MSC_VER
	sprintf_s(filename, 16, "ds2key.p%i.ini", profileNumber);
	#else //_MSC_VER
	sprintf(filename, "ds2key.p%i.ini", profileNumber);
	#endif //_MSC_VER
	file = fopen(filename, "w+");
	if(file)
	{
		printf("\n\n%s\n\n", filename);
		fprintf(file, "%s\n", getVKString(VK_UP));
		fprintf(file, "%s\n", getVKString(VK_DOWN));
		fprintf(file, "%s\n", getVKString(VK_LEFT));
		fprintf(file, "%s\n", getVKString(VK_RIGHT));
		fprintf(file, "%s\n", getVKString('A'));
		fprintf(file, "%s\n", getVKString('B'));
		fprintf(file, "%s\n", getVKString('X'));
		fprintf(file, "%s\n", getVKString('Y'));
		fprintf(file, "%s\n", getVKString('L'));
		fprintf(file, "%s\n", getVKString('R'));
		fprintf(file, "%s\n", getVKString(VK_RETURN));
		fprintf(file, "%s\n", getVKString(VK_RSHIFT));
		fprintf(file, "%s\n", getVKString('1'));
		fprintf(file, "%s\n", getVKString('2'));
		fprintf(file, "%s\n", getVKString('3'));
		fprintf(file, "%s\n", getVKString('4'));
		fclose(file);
	}
	else
	{
		return 1;
	}

	return 0;
}

bool getLine(char *buffer)
{
	char *d = strchr(buffer, '\n');
	if(!d)
	{
		return 1;
	}

	d[0] = 0;

	return 1;
}

bool readProfileConfig(unsigned char profileNumber)
{
	FILE *file;
	char filename[16];
	#define readProfileKey(key) \
		{ \
			if(tmpBuffer[0] != 0) \
			{ \
				int i = 0; \
				getLine(tmpBuffer); \
				profile[profileNumber][key] = getVKNumber(tmpBuffer); \
				tmpBuffer = tmpBuffer + strlen(tmpBuffer) + 1; \
				while(tmpBuffer[i] == (char)0xa || tmpBuffer == (char)0xd) \
				{ \
					i++; \
				} \
 \
				tmpBuffer = &tmpBuffer[i]; \
			} \
		}

	#ifdef _MSC_VER
	sprintf_s(filename, 16, "ds2key.p%i.ini", profileNumber);
	#else //_MSC_VER
	sprintf(filename, "ds2key.p%i.ini", profileNumber);
	#endif //_MSC_VER
	file = fopen(filename, "r");
	if(file)
	{
		unsigned long int size;
		char *buffer;
		char *tmpBuffer;
		char *currentKey;
		char *key0d;
		char *key0a;
		char *keyEnd;
		size_t result;
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		rewind(file);

		// allocate memory to contain the whole file:
		buffer = (char *)malloc(sizeof(char) * size);
		if(buffer == NULL)
		{
			fprintf(stderr, "Memory error");
			return 1;
		}

		result = fread(buffer, 1, size, file);
		if(result != size)
		{
			buffer[result] = 0;

			//fprintf(stderr, "Reading error");
			//return 1;
		}

		fclose(file);

		tmpBuffer = buffer;

		readProfileKey(pUp);
		readProfileKey(pDown);
		readProfileKey(pLeft);
		readProfileKey(pRight);
		readProfileKey(pA);
		readProfileKey(pB);
		readProfileKey(pX);
		readProfileKey(pY);
		readProfileKey(pL);
		readProfileKey(pR);
		readProfileKey(pStart);
		readProfileKey(pSelect);
		readProfileKey(pBlue);
		readProfileKey(pYellow);
		readProfileKey(pRed);
		readProfileKey(pGreen);
		free(buffer);
	}
	else
	{
		writeDefaultProfileConfig(profileNumber);

		return 1;
	}

	return 0;
}

unsigned char currentIP[16];

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
		((unsigned long *) &profile[pI])[pIP] = 0;
	}

	initVKTable();

	{	//read Arguments
		FILE *file = fopen("ds2key.ini", "r");
		if(file)
		{
			char buffer[256];
			if(fread(buffer, 1, 255, file))
			{
				fclose(file);
				serverPort = atoi(buffer);
				if(serverPort == 0)
				{
					writeDefaultConfig();
				}
			}
			else
			{
				fclose(file);
				writeDefaultConfig();
			}
		}
		else
		{
			writeDefaultConfig();
		}

		if(serverPort <= 0)
		{
			fprintf(stderr, "%i: bad port number\n", serverPort);
			exit(1);
		}
	}	//read Arguments
	{	//setup connections
		#ifdef WIN32
		WSAStartup(0x0202, &wsaData);	//windows socket startup
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
		rc = bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
		if(rc < 0)
		{
			fprintf(stderr, "%s: cannot bind port number %d\n", argv[0], serverPort);
			exit(1);
		}

		printf("%s: waiting for data on port UDP %u\n", argv[0], serverPort);
	}	//setup connections

	//server infinite loop
	while(1)
	{
		unsigned char *ip;
		unsigned int currentProfile;

		//init buffer
		memset(msg, 0x0, MAX_MSG);

		//receive message
		cliLen = sizeof(cliAddr);
		n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *) &cliAddr, &cliLen);
		ip = longToIP(cliAddr.sin_addr.S_un.S_addr, currentIP);

		if(n < 0)
		{
			printf("%s: cannot receive data\n", argv[0]);
			continue;
		}

		for(currentProfile = 0; currentProfile <= 255; currentProfile++)
		{
			if(cliAddr.sin_addr.S_un.S_addr == ((unsigned long *) &profile[currentProfile])[0])
			{
				break;
			}
			else if(currentProfile == 255)
			{
				//send /p?, for now it sets profile to 0
				currentProfile = 0;
				readProfileConfig(currentProfile);
				((unsigned long *) &profile[currentProfile])[0] = cliAddr.sin_addr.S_un.S_addr;
				break;
			}
		}

		if(!strnicmp(msg, "/p", 2))
		{
			for(currentProfile = 0; currentProfile <= 255; currentProfile++)
			{
				if(cliAddr.sin_addr.S_un.S_addr == ((unsigned long *) &profile[currentProfile])[0])
				{
					((unsigned long *) &profile[currentProfile])[0] = 0;
				}
			}

			readProfileConfig(atoi(&msg[2]));

			printf("%s: [%s] profile set to %s\n", ip, msg, &msg[2]);
			((unsigned long *) &profile[atoi(&msg[2])])[0] = cliAddr.sin_addr.S_un.S_addr;
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
			doInput(&input, INPUT_KEYBOARD, profile[currentProfile][pY], 1);
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
			bool z;
			x = (unsigned char)atoi(xc);
			y = (unsigned char)atoi(&yc[1]);
			z = (bool) atoi(&zc[1]);
			doInput(&input, INPUT_MOUSE, (y << 8) | x, 0);
			if(z)
			{
				printf("%s: [%s] touch screen pressed at %i, %i\n", ip, msg, x, y);
			}
			else
			{	//!z
				printf("%s: [%s] touch screen released at %i, %i\n", ip, msg, x, y);
			}
		}
		else
		{
			printf("%s: [%s] undefined command\n", ip, msg);

		}

	}	//end of server infinite loop

	return 0;
}
