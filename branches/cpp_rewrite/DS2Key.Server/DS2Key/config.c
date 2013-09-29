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
#else //WIN32
#include <stdlib.h>
#include <string.h>
#include <X11/keysym.h>
#endif //WIN32

#include "config.h"
#include "key.h"

//Variables
int serverPort;
char printDebugLevel;
unsigned int profile[256][pEND];

bool writeConfig()
{
	FILE *file;
	file = fopen("ds2key.ini", "w");

	if(file)
	{
		fprintf(file, "%i\n", serverPort);
		fprintf(file, "%i\n", printDebugLevel);
		fclose(file);
	}
	else
	{
		return 1;
	}

	return 0;
}

bool writeDefaultConfig()
{
	serverPort = DEFAULT_SERVER_PORT;
	printDebugLevel = lDefault;

	return writeConfig();
}

bool writeProfileConfig(unsigned char profileNumber)
{
	FILE *file;
	char filename[16];
	sprintf(filename, "ds2key.p%i.ini", profileNumber);
	file = fopen(filename, "w");

	if(file)
	{
		int i;
		for(i = 1; i < pEND; i++)
		{
			fprintf(file, "%s\n", getKeyString(profile[profileNumber][i]));
		}

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
	profile[profileNumber][pUp] = KEY_UP;
	profile[profileNumber][pDown] = KEY_DOWN;
	profile[profileNumber][pLeft] = KEY_LEFT;
	profile[profileNumber][pRight] = KEY_RIGHT;
	profile[profileNumber][pA] = KEY_A;
	profile[profileNumber][pB] = KEY_B;
	profile[profileNumber][pX] = KEY_X;
	profile[profileNumber][pY] = KEY_Y;
	profile[profileNumber][pL] = KEY_L;
	profile[profileNumber][pR] = KEY_R;
	profile[profileNumber][pStart] = KEY_RETURN;
	profile[profileNumber][pSelect] = KEY_RSHIFT;
	profile[profileNumber][pBlue] = KEY_1;
	profile[profileNumber][pYellow] = KEY_2;
	profile[profileNumber][pRed] = KEY_3;
	profile[profileNumber][pGreen] = KEY_4;
	profile[profileNumber][pTouch0X0Y] = KEY_NUMPAD7;
	profile[profileNumber][pTouch1X0Y] = KEY_NUMPAD8;
	profile[profileNumber][pTouch2X0Y] = KEY_NUMPAD8;
	profile[profileNumber][pTouch3X0Y] = KEY_NUMPAD9;
	profile[profileNumber][pTouch0X1Y] = KEY_NUMPAD4;
	profile[profileNumber][pTouch1X1Y] = KEY_NUMPAD5;
	profile[profileNumber][pTouch2X1Y] = KEY_NUMPAD5;
	profile[profileNumber][pTouch3X1Y] = KEY_NUMPAD6;
	profile[profileNumber][pTouch0X2Y] = KEY_NUMPAD1;
	profile[profileNumber][pTouch1X2Y] = KEY_NUMPAD2;
	profile[profileNumber][pTouch2X2Y] = KEY_NUMPAD2;
	profile[profileNumber][pTouch3X2Y] = KEY_NUMPAD3;

	return writeProfileConfig(profileNumber);
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

bool readConfig()
{
	FILE *file;

	//read Arguments
	file = fopen("ds2key.ini", "r");

	if(file)
	{   //file opened, read from it.
		unsigned long int size;
		char *buffer;
		char *tmpBuffer;
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

		//close the file
		fclose(file);

		//this is for the readProfileKey define
		tmpBuffer = buffer;

		//setup settings
		{
			int i;
			for(i = 0; i <= 1; i++)
			{
				if(tmpBuffer[0] != 0)
				{
					int i2 = 0;
					getLine(tmpBuffer);
					if(i == 0)
					{
						serverPort = atoi(tmpBuffer);
						if(serverPort < 1 || serverPort > 65535)
						{
							serverPort = DEFAULT_SERVER_PORT;
						}
					}
					else if(i == 1)
					{
						printDebugLevel = atoi(tmpBuffer);
						if(printDebugLevel < 0 || printDebugLevel >= lEND)
						{
							printDebugLevel = lDefault;
						}
					}
					tmpBuffer = tmpBuffer + strlen(tmpBuffer) + 1;
					while(tmpBuffer[i2] == (char)0xa || tmpBuffer[i2] == (char)0xd)
					{
						i2++;
					}

					tmpBuffer = &tmpBuffer[i2];
				}
				else
				{
					writeDefaultConfig();
				}
			}
		}

		//free "buffer" memory
		free(buffer);

		//write out read data
		writeConfig();
	}
	else
	{
		//file couldn't be opened, write a default one
		writeDefaultConfig();

		return 1;
	}

	return 0;
}

bool readProfileConfig(unsigned char profileNumber)
{
	FILE *file;
	char filename[16];

	sprintf(filename, "ds2key.p%i.ini", profileNumber);
	file = fopen(filename, "r");

	if(file)
	{   //file opened, read from it.
		unsigned long int size;
		char *buffer;
		char *tmpBuffer;
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

		//close the file
		fclose(file);

		//this is for the readProfileKey define
		tmpBuffer = buffer;

		//set profile keys
		{
			int i;
			for(i = 1; i < pEND; i++)
			{
				if(tmpBuffer[0] != 0)
				{
					int i2 = 0;
					getLine(tmpBuffer);
					profile[profileNumber][i] = getKeyNumber(tmpBuffer);
					tmpBuffer = tmpBuffer + strlen(tmpBuffer) + 1;
					while(tmpBuffer[i2] == (char)0xa || tmpBuffer[i2] == (char)0xd)
					{
						i2++;
					}

					tmpBuffer = &tmpBuffer[i2];
				}
			}
		}

		//free "buffer" memory
		free(buffer);

		//write out read data
		writeProfileConfig(profileNumber);
	}
	else
	{
		//file couldn't be opened, write a default one
		writeDefaultProfileConfig(profileNumber);

		return 1;
	}

	return 0;
}
