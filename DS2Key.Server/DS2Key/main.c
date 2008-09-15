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
#include <winsock.h>
#define stricmp _stricmp
#define strnicmp _strnicmp
#define sscanf sscanf_s
typedef unsigned char bool;
#else//WIN32
#include <sys/types.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h> /* close() */
#endif//WIN32
#include "main.h"

int main(int argc, char *argv[])
{
	int sd, rc, n, cliLen, serverPort;
	struct sockaddr_in cliAddr, servAddr;
	char msg[MAX_MSG];
#ifdef WIN32
	WSADATA wsaData;
#endif//WIN32

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

#ifdef WIN32
	WSAStartup(0x0202, &wsaData); /*windows socket startup */
#endif//WIN32

	/* socket creation */
	sd=socket(AF_INET, SOCK_DGRAM, 0);
	if(sd < 0)
	{
		fprintf(stderr,"%s: cannot open socket\n", argv[0]);
		exit(1);
	}

	/* bind local server port */
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

	/* server infinite loop */
	while(1)
	{
		/* init buffer */
		memset(msg, 0x0, MAX_MSG);

		/* receive message */
		cliLen = sizeof(cliAddr);
		n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr*) &cliAddr, &cliLen);

		if(n < 0)
		{
			printf("%s: cannot receive data\n", argv[0]);
			continue;
		}

		if(!strnicmp(msg, "/p", 2	))
			printf("%s: profile set to %s\n", msg, &msg[2]);
		else if(!stricmp(msg, "/dl0"))
			printf("%s: left button pressed\n", msg);
		else if(!stricmp(msg, "/dl1"))
			printf("%s: left button released\n", msg);
		else if(!stricmp(msg, "/dr0"))
			printf("%s: right button pressed\n", msg);
		else if(!stricmp(msg, "/dr1"))
			printf("%s: right button released\n", msg);
		else if(!stricmp(msg, "/du0"))
			printf("%s: up button pressed\n", msg);
		else if(!stricmp(msg, "/du1"))
			printf("%s: up button released\n", msg);
		else if(!stricmp(msg, "/dd0"))
			printf("%s: down button pressed\n", msg);
		else if(!stricmp(msg, "/dd1"))
			printf("%s: down button released\n", msg);
		else if(!stricmp(msg, "/ba0"))
			printf("%s: a button pressed\n", msg);
		else if(!stricmp(msg, "/ba1"))
			printf("%s: a button released\n", msg);
		else if(!stricmp(msg, "/bb0"))
			printf("%s: b button pressed\n", msg);
		else if(!stricmp(msg, "/bb1"))
			printf("%s: b button released\n", msg);
		else if(!stricmp(msg, "/bx0"))
			printf("%s: x button pressed\n", msg);
		else if(!stricmp(msg, "/bx1"))
			printf("%s: x button released\n", msg);
		else if(!stricmp(msg, "/by0"))
			printf("%s: y button pressed\n", msg);
		else if(!stricmp(msg, "/by1"))
			printf("%s: y button released\n", msg);
		else if(!stricmp(msg, "/bl0"))
			printf("%s: l button pressed\n", msg);
		else if(!stricmp(msg, "/bl1"))
			printf("%s: l button released\n", msg);
		else if(!stricmp(msg, "/br0"))
			printf("%s: r button pressed\n", msg);
		else if(!stricmp(msg, "/br1"))
			printf("%s: r button released\n", msg);
		else if(!stricmp(msg, "/bt0"))
			printf("%s: start button pressed\n", msg);
		else if(!stricmp(msg, "/bt1"))
			printf("%s: start button released\n", msg);
		else if(!stricmp(msg, "/be0"))
			printf("%s: select button pressed\n", msg);
		else if(!stricmp(msg, "/be1"))
			printf("%s: select button released\n", msg);
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
			if(z == 0)
				printf("%s: touch screen pressed at %i, %i\n", msg, x, y);
			else//z == 0
				printf("%s: touch screen released at %i, %i\n", msg, x, y);
		}
		else
		/* print received message */
		printf("%s: undefined command\n", msg);

	}/* end of server infinite loop */

	return 0;
}
