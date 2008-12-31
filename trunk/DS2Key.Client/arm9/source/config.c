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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <nds/ndstypes.h>
#include "config.h"
#include "ds2key.h"

//functions
bool writeConfig(DS2Key *ds2key)
{
	FILE *file;
	file = fopen(DS2KEY_INI, "w");

	if(file)
	{
		fprintf(file, "%s\n", ds2key->ip);
		fprintf(file, "%s\n", ds2key->port);
		fprintf(file, "%s\n", ds2key->profile);

		fclose(file);
	}
	else
	{
		return 1;
	}

	return 0;
}

bool writeDefaultConfig(DS2Key *ds2key)
{
	strncpy(ds2key->ip, DEFAULT_IP, LENGTH_IP);
	strncpy(ds2key->port, DEFAULT_PORT, LENGTH_PORT);
	strncpy(ds2key->profile, DEFAULT_PROFILE, LENGTH_PROFILE);

	return writeConfig(ds2key);
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

bool readConfig(DS2Key *ds2key)
{
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
		strncpy(ds2key->ip, tmpString, LENGTH_IP);

		readString();
		strncpy(ds2key->port, tmpString, LENGTH_PORT);

		readString();
		strncpy(ds2key->profile, tmpString, LENGTH_PROFILE);

		//free "buffer" memory
		free(buffer);

		//write out read data
		writeConfig(ds2key);
	}
	else
	{
		//file couldn't be opened, write a default one
		writeDefaultConfig(ds2key);

		return 1;
	}

	return 0;
}
