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
