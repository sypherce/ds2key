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
unsigned int profile[256][pEND];

bool writeConfig()
{
	FILE *file;
	file = fopen("ds2key.ini", "w");

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

bool writeDefaultConfig()
{
	serverPort = DEFAULT_SERVER_PORT;

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
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pUp]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pDown]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pLeft]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pRight]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pA]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pB]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pX]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pY]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pL]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pR]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pStart]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pSelect]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pBlue]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pYellow]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pRed]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pGreen]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pTouch0X0Y]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pTouch1X0Y]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pTouch2X0Y]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pTouch3X0Y]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pTouch0X1Y]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pTouch1X1Y]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pTouch2X1Y]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pTouch3X1Y]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pTouch0X2Y]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pTouch1X2Y]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pTouch2X2Y]));
		fprintf(file, "%s\n", getKeyString(profile[profileNumber][pTouch3X2Y]));

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
	{
		char buffer[256];

		if(fread(buffer, 1, 255, file))
		{
			fclose(file);
			serverPort = atoi(buffer);

			if(serverPort < 1 || serverPort > 65535)
			{
				writeDefaultConfig();

				return 1;
			}
			else
			{
				writeConfig();
			}
		}
		else
		{
			fclose(file);
			writeDefaultConfig();

			return 1;
		}
	}
	else
	{
		writeDefaultConfig();

		return 1;
	}

	return 0;
}

bool readProfileConfig(unsigned char profileNumber)
{
#define readProfileKey(key)	\
	{ \
		if(tmpBuffer[0] != 0) \
		{ \
			int i = 0; \
			getLine(tmpBuffer);	\
			profile[profileNumber][key] = getKeyNumber(tmpBuffer); \
			tmpBuffer = tmpBuffer + strlen(tmpBuffer) + 1; \
			while(tmpBuffer[i] == (char)0xa || tmpBuffer[i] == (char)0xd) \
			{ \
				i++; \
			} \
 \
			tmpBuffer = &tmpBuffer[i]; \
		} \
	}

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
		readProfileKey(pTouch0X0Y);
		readProfileKey(pTouch1X0Y);
		readProfileKey(pTouch2X0Y);
		readProfileKey(pTouch3X0Y);
		readProfileKey(pTouch0X1Y);
		readProfileKey(pTouch1X1Y);
		readProfileKey(pTouch2X1Y);
		readProfileKey(pTouch3X1Y);
		readProfileKey(pTouch0X2Y);
		readProfileKey(pTouch1X2Y);
		readProfileKey(pTouch2X2Y);
		readProfileKey(pTouch3X2Y);

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
