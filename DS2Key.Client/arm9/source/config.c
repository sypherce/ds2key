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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <nds/jtypes.h>
#include "config.h"
#include "wifi.h"

//Variables
int ip = DEFAULT_IP;
int port = DEFAULT_PORT;
int profile = DEFAULT_PROFILE;

bool writeConfig()
{
	FILE *file;
	file = fopen(DS2KEY_INI, "w");

	if(file)
	{
		sain.sin_addr.s_addr = ip;
		fprintf(file, "%s\n", inet_ntoa(sain.sin_addr));
		fprintf(file, "%i\n", port);
		fprintf(file, "%i\n", profile);

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
	ip = DEFAULT_IP;
	port = DEFAULT_PORT;
	profile = DEFAULT_PROFILE;

	return writeConfig();
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
#define readString()	\
	{ \
		if(tmpBuffer[0] != 0) \
		{ \
			int i = 0; \
			getLine(tmpBuffer);	\
			tmpString = tmpBuffer; \
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
	file = fopen(DS2KEY_INI, "r");

	if(file)
	{   //file opened, read from it.
		unsigned long int size;
		char *buffer;
		char *tmpBuffer;
		char *tmpString = 0;
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

		//this is for the readString define
		tmpBuffer = buffer;

		//set variables
		readString();
		inet_aton(tmpString, &sain.sin_addr);
		ip = sain.sin_addr.s_addr;

		readString();
		port = atoi(tmpString);

		readString();
		profile = atoi(tmpString);

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
