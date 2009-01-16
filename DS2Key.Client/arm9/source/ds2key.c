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
#include <dswifi9.h>
#include <netinet/in.h>

#include <stdio.h>
#include <string.h>

#include "gh.h"
#include "ds2key.h"

//variables
touchPosition stylusPos;
touchPosition lastStylusPos;
touchPosition dummyStylusPos;

//functions
unsigned long ds2keyGetLongIP(DS2Key *client)
{
	inet_aton(client->ip, &client->sockaddr.sin_addr);

	return client->sockaddr.sin_addr.s_addr;
}

unsigned int ds2keyGetIntPort(DS2Key *client)
{
	return atoi(client->port);
}

unsigned int ds2keyGetIntProfile(DS2Key *client)
{
	return atoi(client->profile);
}

void sendCommand(DS2Key *client, char* command)
{
	client->sockaddr.sin_addr.s_addr = ds2keyGetLongIP(client);

	if(sendto(client->socket, command, strlen(command), 0, (struct sockaddr *)&client->sockaddr, sizeof(client->sockaddr)) >= 0)
	{
		iprintf("\x1b[22;0HSent: %s                     ", command);
	}
	else
	{
		iprintf("\x1b[22;0HFailed to send: %s           ", command);
	}
}

void initD2K(DS2Key *client)
{
	int i;
	char command[6];

	client->socket = socket(AF_INET,SOCK_DGRAM,0);

	client->sockaddr.sin_family = AF_INET;
	client->sockaddr.sin_port = htons(ds2keyGetIntPort(client));
	client->sockaddr.sin_addr.s_addr = INADDR_ANY;

	i = bind(client->socket, (struct sockaddr *)&client->sockaddr, sizeof(client->sockaddr));
    if(i < 0)
	{
		printf("\nError: Can't bind port number %s\n", client->port);
		while(1)
		{
		    swiWaitForVBlank();
		}
	}

	i = 1;
	ioctl(client->socket, FIONBIO, &i);

	sprintf(command, "/p%s", client->profile);
	sendCommandDefault(command);
}

void deinitD2K(DS2Key *client)
{
	shutdown(client->socket, 0);
	closesocket(client->socket);

	client->socket = 0;
	client->sockaddr.sin_family = 0;
	client->sockaddr.sin_port = 0;
	client->sockaddr.sin_addr.s_addr = 0;
}

void updateD2KKeys(DS2Key *client, uint32 down, uint32 up)
{
	if(down&KEY_A) sendCommandDefault("/ba0");
	else if(up&KEY_A) sendCommandDefault("/ba1");
	if(down&KEY_B) sendCommandDefault("/bb0");
	else if(up&KEY_B) sendCommandDefault("/bb1");
	if(down&KEY_X) sendCommandDefault("/bx0");
	else if(up&KEY_X) sendCommandDefault("/bx1");
	if(down&KEY_Y) sendCommandDefault("/by0");
	else if(up&KEY_Y) sendCommandDefault("/by1");
	if(down&KEY_L) sendCommandDefault("/bl0");
	else if(up&KEY_L) sendCommandDefault("/bl1");
	if(down&KEY_R) sendCommandDefault("/br0");
	else if(up&KEY_R) sendCommandDefault("/br1");
	if(down&KEY_START) sendCommandDefault("/bt0");
	else if(up&KEY_START) sendCommandDefault("/bt1");
	if(down&KEY_SELECT) sendCommandDefault("/be0");
	else if(up&KEY_SELECT) sendCommandDefault("/be1");
	if(down&KEY_UP) sendCommandDefault("/du0");
	else if(up&KEY_UP) sendCommandDefault("/du1");
	if(down&KEY_DOWN) sendCommandDefault("/dd0");
	else if(up&KEY_DOWN) sendCommandDefault("/dd1");
	if(down&KEY_LEFT) sendCommandDefault("/dl0");
	else if(up&KEY_LEFT) sendCommandDefault("/dl1");
	if(down&KEY_RIGHT) sendCommandDefault("/dr0");
	else if(up&KEY_RIGHT) sendCommandDefault("/dr1");
}

void updateD2KGH(DS2Key *client, uint32 down, uint32 up)
{
	if(down&GH_BLUE) sendCommandDefault("/gb0");
	else if(up&GH_BLUE) sendCommandDefault("/gb1");
	if(down&GH_YELLOW) sendCommandDefault("/gy0");
	else if(up&GH_YELLOW) sendCommandDefault("/gy1");
	if(down&GH_RED) sendCommandDefault("/gr0");
	else if(up&GH_RED) sendCommandDefault("/gr1");
	if(down&GH_GREEN) sendCommandDefault("/gg0");
	else if(up&GH_GREEN) sendCommandDefault("/gg1");
}

void updateD2KStylus(DS2Key *client, uint32 held, uint32 up, touchPosition *pos, touchPosition *lastPos)
{
	if(held&KEY_TOUCH && (lastPos->px != pos->px || lastPos->py != pos->py))
	{
		char command[12];
		sprintf(command, "/m%i.%i.1", pos->px, pos->py);
		sendCommandDefault(command);
	}
	else if(up&KEY_TOUCH && (lastPos->px != 299 || lastPos->py != 299))
	{
		char command[12];
		sprintf(command, "/m%i.%i.0", pos->px, pos->py);
		sendCommandDefault(command);
		lastPos->px = 299;
		lastPos->py = 299;
	}
}

void profileQuery(DS2Key *client)
{
    char msg[4];
	char cProfile[6];
    int cliLen = sizeof(client->sockaddr);
    int n = recvfrom(client->socket, msg, 4, 0, (struct sockaddr *)&client->sockaddr, &cliLen);

    if(n > 0)
    {
        if(!strcmp(msg, "/p?"))
        {
            sprintf(cProfile, "/p%s", client->profile);
            sendCommandDefault(cProfile);
        }
    }
}

void updateD2K(DS2Key *client, uint32 down, uint32 held, uint32 up, touchPosition *pos, touchPosition *lastPos)
{
    profileQuery(client);
	updateD2KKeys(client, down, up);
	updateD2KGH(client, GH_keysDown, GH_keysUp);
	updateD2KStylus(client, held, up, pos, lastPos);
}
