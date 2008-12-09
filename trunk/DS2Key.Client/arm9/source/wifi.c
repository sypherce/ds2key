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
#include <stdio.h>
#include <string.h>
#include <dswifi9.h>
#include <netinet/in.h>

//wifi timer function, to update internals of sgIP
void Timer_50ms()
{
	Wifi_Timer(50);
}

unsigned long iptoi(char *ipchar, struct sockaddr_in sockaddr)
{
	inet_aton(ipchar, &sockaddr.sin_addr);

	return sockaddr.sin_addr.s_addr;
}

void sendCommand(int socket, struct sockaddr_in sockaddr, unsigned long ip, unsigned int port, char *command)
{
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = ip;

	if(sendto(socket, command, strlen(command), 0, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) >= 0)
	{
		iprintf("\x1b[22;0HSent: %s          ", command);
	}
	else
	{
		iprintf("\x1b[22;0HFailed to send: %s", command);
	}
}

void wifiConnect()
{
	//simple WFC connect:
	Wifi_AutoConnect(); //request connect
	while(1)
	{
		int i = Wifi_AssocStatus(); //check status

		if(i == ASSOCSTATUS_ASSOCIATED)
		{
			printf("Connected successfully!\n");
			//wifiConnected = false;
			break;
		}
		else if(i == ASSOCSTATUS_CANNOTCONNECT)
		{
			printf("Could not connect!\n");
			//wifiConnected = true;
			break;
		}
	}
}
