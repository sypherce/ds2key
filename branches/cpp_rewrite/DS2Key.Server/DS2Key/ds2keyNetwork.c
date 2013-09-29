/*
DS2Key Server - An application to use your DS as a PC Gamepad
Copyright (C) 2008, 2009  Derrick (sypherce) Wirth

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
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#ifdef WIN32
#include <winsock.h>
#else//WIN32
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#ifdef NGC
#include <gctypes.h>
#include <network.h>
#define socket net_socket
#define bind net_bind
#define recvfrom net_recvfrom
#define sendto net_sendto
#define close net_close
#else//NGC
#include <netinet/in.h>
#endif//NGC
#endif//WIN32
#include "ds2keyNetwork.h"
#include "ds2keyKeys.h"

int ds2keySocket;
bool ds2keyConnected = false;
unsigned int profile[MAX_PROFILE];

void setProfile(int profileNumber, struct sockaddr_in sockAddr)
{
	unsigned int currentProfile;
	for(currentProfile = 0; currentProfile < MAX_PROFILE; currentProfile++)
	{
		if(sockaddr_in__address(sockAddr) == profile[currentProfile])
		{
			profile[currentProfile] = 0;
		}
	}

	//printf("%s: [/p%i] profile set to %i\n", getIP(), profileNumber, profileNumber);
	profile[profileNumber] = sockaddr_in__address(sockAddr);
	packet.profile = profileNumber;

	return;
}

int getProfile(struct sockaddr_in sockAddr)
{
	uint16_t currentProfile;
	for(currentProfile = 0; currentProfile < MAX_PROFILE; currentProfile++)
	{
		if(sockaddr_in__address(sockAddr) == profile[currentProfile])
		{
			packet.profile = currentProfile;
			return currentProfile;
		}
	}
	
	if(currentProfile == MAX_PROFILE)//0 to 255 = 256 profiles
	{
		char *command = "/p?";

		if(sendto(ds2keySocket, command, strlen(command), 0, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) >= 0)
		{
			//printf("Sent: %s\n", command);
		}
		else
		{
			//printf("Failed to send: %s, Error: %i\n", command, errno);
		}
	}

	return currentProfile;
}

bool processOldHeader(char *msg, struct sockaddr_in cliAddr)
{
	if(!strnicmp(msg, "/p", 2))
	{
		setProfile(atoi(&msg[2]), cliAddr);
	}
	else
	{
		if(getProfile(cliAddr) == MAX_PROFILE)
		{
			return 1;
		}
		else if(!stricmp(msg, "/dl0"))
		{
			ds2keyPress(DS2KEY_LEFT);
		}
		else if(!stricmp(msg, "/dl1"))
		{
			ds2keyRelease(DS2KEY_LEFT);
		}
		else if(!stricmp(msg, "/dr0"))
		{
			ds2keyPress(DS2KEY_RIGHT);
		}
		else if(!stricmp(msg, "/dr1"))
		{
			ds2keyRelease(DS2KEY_RIGHT);
		}
		else if(!stricmp(msg, "/du0"))
		{
			ds2keyPress(DS2KEY_UP);
		}
		else if(!stricmp(msg, "/du1"))
		{
			ds2keyRelease(DS2KEY_UP);
		}
		else if(!stricmp(msg, "/dd0"))
		{
			ds2keyPress(DS2KEY_DOWN);
		}
		else if(!stricmp(msg, "/dd1"))
		{
			ds2keyRelease(DS2KEY_DOWN);
		}
		else if(!stricmp(msg, "/ba0"))
		{
			ds2keyPress(DS2KEY_A);
		}
		else if(!stricmp(msg, "/ba1"))
		{
			ds2keyRelease(DS2KEY_A);
		}
		else if(!stricmp(msg, "/bb0"))
		{
			ds2keyPress(DS2KEY_B);
		}
		else if(!stricmp(msg, "/bb1"))
		{
			ds2keyRelease(DS2KEY_B);
		}
		else if(!stricmp(msg, "/bx0"))
		{
			ds2keyPress(DS2KEY_X);
		}
		else if(!stricmp(msg, "/bx1"))
		{
			ds2keyRelease(DS2KEY_X);
		}
		else if(!stricmp(msg, "/by0"))
		{
			ds2keyPress(DS2KEY_Y);
		}
		else if(!stricmp(msg, "/by1"))
		{
			ds2keyRelease(DS2KEY_Y);
		}
		else if(!stricmp(msg, "/bl0"))
		{
			ds2keyPress(DS2KEY_L);
		}
		else if(!stricmp(msg, "/bl1"))
		{
			ds2keyRelease(DS2KEY_L);
		}
		else if(!stricmp(msg, "/br0"))
		{
			ds2keyPress(DS2KEY_R);
		}
		else if(!stricmp(msg, "/br1"))
		{
			ds2keyRelease(DS2KEY_R);
		}
		else if(!stricmp(msg, "/bt0"))
		{
			ds2keyPress(DS2KEY_START);
		}
		else if(!stricmp(msg, "/bt1"))
		{
			ds2keyRelease(DS2KEY_START);
		}
		else if(!stricmp(msg, "/be0"))
		{
			ds2keyPress(DS2KEY_SELECT);
		}
		else if(!stricmp(msg, "/be1"))
		{
			ds2keyRelease(DS2KEY_SELECT);
		}
		else if(!stricmp(msg, "/gb0"))
		{
			ds2keyGHPress(GH_BLUE);
		}
		else if(!stricmp(msg, "/gb1"))
		{
			ds2keyGHRelease(GH_BLUE);
		}
		else if(!stricmp(msg, "/gy0"))
		{
			ds2keyGHPress(GH_YELLOW);
		}
		else if(!stricmp(msg, "/gy1"))
		{
			ds2keyGHRelease(GH_YELLOW);
		}
		else if(!stricmp(msg, "/gr0"))
		{
			ds2keyGHPress(GH_RED);
		}
		else if(!stricmp(msg, "/gr1"))
		{
			ds2keyGHRelease(GH_RED);
		}
		else if(!stricmp(msg, "/gg0"))
		{
			ds2keyGHPress(GH_GREEN);
		}
		else if(!stricmp(msg, "/gg1"))
		{
			ds2keyGHRelease(GH_GREEN);
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
			x = (unsigned char)atoi(xc);
			y = (unsigned char)atoi(&yc[1]);
			z = (bool)atoi(&zc[1]);
			buttonX = (x / 64);
			buttonY = (y / 64);

			if(z)
			{
				ds2keyPress(DS2KEY_TOUCH);
				ds2keySetTouchPos(x, y);
				//printf("%s: [%s] touch screen pressed at %i, %i\n", getIP(), msg, x, y);
			}
			else
			{
				ds2keyRelease(DS2KEY_TOUCH);
				//printf("%s: [%s] touch screen released at %i, %i\n", getIP(), msg, x, y);
			}
		}
		else
		{
			//printf("%s: [%s] undefined command\n", getIP(), msg);
		}
	}

	return 0;
}

int ds2keyConnect(int port)
{   //setup connections
    if(!ds2keyConnected)
    {
		struct sockaddr_in servAddr;
		int returnVal;
#ifdef WIN32
        unsigned long ioctlsocketCommand = 1;
        WSADATA wsaData;
        returnVal = WSAStartup(0x0202, &wsaData);   //windows socket startup
        if(returnVal != 0)
		{
			return returnVal;
		}
#endif//WIN32

        //socket creation
        ds2keySocket = socket(AF_INET, SOCK_DGRAM, 0);

        if(ds2keySocket == -1)
        {
            //fprintf(stderr, "Cannot open socket\n");

            return errno;
        }

        ds2keyConnected = true;

        //bind local server port
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servAddr.sin_port = htons((unsigned short)port);

#ifdef WIN32
        if(ioctlsocket(ds2keySocket, FIONBIO, &ioctlsocketCommand) == SOCKET_ERROR)
		{
			returnVal = WSAGetLastError();
#else//WIN32
#ifdef NGC
        returnVal = net_fcntl(ds2keySocket, F_GETFL, 0);
        if(returnVal >= 0)
		{
			returnVal = net_fcntl(ds2keySocket, F_SETFL, returnVal | 4);
		}
		if(returnVal < 0)
		{
#else//NGC
        if(fcntl(ds2keySocket, F_SETFL, O_NONBLOCK) == -1)
        {
			returnVal = errno;
#endif//NGC
#endif//WIN32
            //fprintf(stderr, "Setting to non-blocking failed\n");
			ds2keyDisconnect();

            return returnVal;
        }

        if(bind(ds2keySocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
        {
            //fprintf(stderr, "Cannot bind port number %d\n", port);
			ds2keyDisconnect();

            return errno;//what to do for windows?
        }
		
		//zero out profiles
		memset(profile, 0, MAX_PROFILE);

#ifdef GUI
        //sprintf(mlDefaultTxt, "Waiting for data on port UDP %u", port);
#else//GUI
        //printf("Waiting for data on port UDP %u\n", port);
#endif//GUI
    }
	return 0;
}   //setup connections

int ds2keyDisconnect()
{
    if(ds2keyConnected)
    {
		int returnVal;
#ifdef WIN32
		returnVal = closesocket(ds2keySocket);
        if(returnVal != 0)
		{
			if(returnVal == SOCKET_ERROR)
			{
				returnVal = WSAGetLastError();
			}

			return returnVal;
		}
        returnVal = WSACleanup();
        if(returnVal != 0)
		{
			if(returnVal == SOCKET_ERROR)
			{
				returnVal = WSAGetLastError();
			}

			return returnVal;
		}
#else//WIN32
        returnVal = close(ds2keySocket);
        if(returnVal != 0)
		{
			if(returnVal == -1)
			{
				returnVal = errno;
			}

			return returnVal;
		}
#endif//WIN32
        ds2keyConnected = false;
    }

	return 0;
}

int ds2keyUpdateNoScan()
{
    if(ds2keyConnected)
    {
        ssize_t msgSize;
		struct sockaddr_in cliAddr;
        socklen_t cliLen = sizeof(cliAddr);
        char msg[MAX_MSG];

        //init buffer
        memset(msg, 0, MAX_MSG);

        //receive message
        msgSize = recvfrom(ds2keySocket, msg, MAX_MSG, 0, (struct sockaddr *)&cliAddr, &cliLen);

        if(msgSize == 0)
        {
			//no data to be received
            return -1;
        }
		else if(msgSize == -1)
		{
            //Cannot receive data
			return errno;
		}
		
		if(msg[0] == NEW_HEADER)
		{
			ds2keyPacket *pPacket = (ds2keyPacket*)msg;
			packet = *pPacket;
			setProfile(packet.profile, cliAddr);//patch for old DS2Key protocol
			#ifdef NGC
			uint8_t *keyPointer = (uint8_t*)&packet.keys;
			uint8_t tempValue;
			tempValue = keyPointer[0];
			keyPointer[0] = keyPointer[1];
			keyPointer[1] = tempValue;
			#endif//NGC
		}
		else if(msg[0] == OLD_HEADER)//old protocol
		{
			if(processOldHeader(msg, cliAddr))
			{
				return -2;
			}
		}
		else
		{
			return -3;
		}
	}
	else
	{
		return -4;
	}

	return 0;
}

int ds2keyUpdate()
{
	int returnVal = ds2keyUpdateNoScan();
	ds2keyScan();

	return returnVal;
}
