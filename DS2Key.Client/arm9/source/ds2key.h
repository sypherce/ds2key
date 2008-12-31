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
#ifndef __D2K_H__
#define __D2K_H__

#include <nds.h>
#include <netinet/in.h>

//struct defines
typedef struct DS2Key
{
	int socket;
	struct sockaddr_in sockaddr;
	char ip[16];
	char port[6];
	char profile[4];
} DS2Key;

//function defines
#define sendCommandDefault(command) sendCommand(client, command)

//variables
extern touchPosition stylusPos;
extern touchPosition lastStylusPos;
extern touchPosition dummyStylusPos;

//functions
extern unsigned long ds2keyGetLongIP(DS2Key *client);
extern unsigned int ds2keyGetIntPort(DS2Key *client);
extern unsigned int ds2keyGetIntProfile(DS2Key *client);
extern void sendCommand(DS2Key *client, char* command);
extern void initD2K(DS2Key *client);
extern void deinitD2K(DS2Key *client);
extern void updateD2KKeys(DS2Key *client, uint32 down, uint32 up);
extern void updateD2KStylus(DS2Key *client, uint32 held, uint32 up, touchPosition *pos, touchPosition *lastPos);
extern void updateD2K(DS2Key *client, uint32 down, uint32 held, uint32 up, touchPosition *pos, touchPosition *lastPos);

#endif//__D2K_H__
