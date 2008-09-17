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
#include <stdlib.h>
#include <string.h>

#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "gh.h"

//for some odd reason I can't seem to find vcount in ndslib -sigh- (I mean, in a place it doesn't conflict with other things)
#define		VCOUNT		(*((u16 volatile *) 0x04000006))

//---------------------------------------------------------------------------------
//Dswifi helper functions

//wifi timer function, to update internals of sgIP
void Timer_50ms(void)
{
	Wifi_Timer(50);
}

//notification function to send fifo message to arm7
void arm9_synctoarm7()//send fifo message
{
	REG_IPC_FIFO_TX = 0x87654321;
}

//interrupt handler to receive fifo messages from arm7
void arm9_fifo()//check incoming fifo messages
{
	u32 value = REG_IPC_FIFO_RX;
	if(value == 0x87654321) Wifi_Sync();
}

void vblfunction()
{
}

unsigned long iptoi(char *ipchar)
{
	int i1 = 0;
	int i2 = 0;
	unsigned long iplong = 0;
	while(i2 < 4)
	{
		if(*ipchar == '.' || *ipchar == 0)
		{
			iplong = (iplong >> 8) | (i1 << 24);
			i1 = 0;
			i2++;
			if(*ipchar == 0 && i2 < 4)
			{
				iplong = iplong >> ((4 - i2) * 8);
				i2 = 4;
			}
		}
		else
		{
			if(*ipchar >= '0' && *ipchar <= '9')
			{
				i1 = i1 * 10 + (*ipchar - '0');
			}
		}
		ipchar++;
	}

	return iplong;
}

void sendCommand(int socket, struct sockaddr_in sockaddr, unsigned long ip, unsigned int port, char* command)
{
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = ip;
	if(sendto(socket, command, strlen(command), 0, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1)
		printf("Sent: %s\n", command);
	else
		printf("Failed to send: %s\n", command);
}

#define sendCommand(a) sendCommand(my_socket, sain, iptoi(address), atoi(port), a)

int main(int argc, char *argv[])
{
	{//setup system
		videoSetMode(0);									//not using the main screen
		videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE);	//sub bg 0 will be used to print text
		vramSetBankC(VRAM_C_SUB_BG);

		SUB_BG0_CR = BG_MAP_BASE(31);

		BG_PALETTE_SUB[255] = RGB15(31, 31, 31);			//by default font will be rendered with color 255

		//consoleInit() is a lot more flexible but this gets you up and running quick
		consoleInitDefault((u16*)SCREEN_BASE_BLOCK_SUB(31), (u16*)CHAR_BASE_BLOCK_SUB(0), 16);

		irqInit();
		irqSet(IRQ_VBLANK, vblfunction);
		irqEnable(IRQ_VBLANK);
	}
	printf("DS2Key\n-\n");

	{//send fifo message to initialize the arm7 wifi
		REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR;//enable & clear FIFO
		
		u32 Wifi_pass= Wifi_Init(WIFIINIT_OPTION_USELED);
		REG_IPC_FIFO_TX = 0x12345678;
		REG_IPC_FIFO_TX = Wifi_pass;
		
		*((volatile u16*)0x0400010E) = 0;//disable timer3
		
		irqInit(); 
		irqSet(IRQ_TIMER3, Timer_50ms);//setup timer IRQ
		irqEnable(IRQ_TIMER3);
		irqSet(IRQ_FIFO_NOT_EMPTY, arm9_fifo);//setup fifo IRQ
		irqEnable(IRQ_FIFO_NOT_EMPTY);
		
		REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_RECV_IRQ;//enable FIFO IRQ
		
		Wifi_SetSyncHandler(arm9_synctoarm7);//tell wifi lib to use our handler to notify arm7

		//set timer3
		*((volatile u16*)0x0400010C) = -6553;//6553.1 * 256 cycles = ~50ms;
		*((volatile u16*)0x0400010E) = 0x00C2;//enable, irq, 1/256 clock
		
		while(Wifi_CheckInit() == 0)//wait for arm7 to be initted successfully
		{
			while(VCOUNT > 192);//wait for vblank
			while(VCOUNT < 192);
		}
	}//wifi init complete - wifi lib can now be used!
	
	printf("Connecting via WFC data\n");
	{//simple WFC connect:
		Wifi_AutoConnect();//request connect
		while(1)
		{
			int i = Wifi_AssocStatus();//check status
			if(i == ASSOCSTATUS_ASSOCIATED)
			{
				printf("Connected successfully!\n");
				break;
			}
			if(i==ASSOCSTATUS_CANNOTCONNECT)
			{
				printf("Could not connect!\n");
				while(1);
			}
		}
	}//if connected, you can now use the berkley sockets interface to connect to the internet!
	printf("-\n");

 
	//Create a TCP socket
	int my_socket = my_socket = socket(AF_INET, SOCK_DGRAM, 0);
	printf("Created Socket!\n");

	char *address = "192.168.1.2";
	char *port = "9501";

	//Tell the socket to connect to the IP address we found, on port 80 (HTTP)
	struct sockaddr_in sain;
	sain.sin_family = AF_INET;
	sain.sin_port = htons(atoi(port));
	sain.sin_addr.s_addr = INADDR_ANY;
	int i = 1;
	ioctl(my_socket, FIONBIO, &i);
	printf("Connected to server!\n");
	//send our request
	unsigned char profile = 0;
	char cProfile[5];
	sprintf(cProfile, "/p%i", profile);
	sendCommand(cProfile);
	printf("%s\n", cProfile);
	
	initGHPad();
	while(1)
	{
		unsigned int lastx = 299;
		unsigned int lasty = 299;
		touchPosition stylusPos = touchReadXY();
		scanKeys();
		updateGHPad();
		
		if(keysDown()&KEY_A) sendCommand("/ba0");
		if(keysUp()&KEY_A) sendCommand("/ba1");
		if(keysDown()&KEY_B) sendCommand("/bb0");
		if(keysUp()&KEY_B) sendCommand("/bb1");
		if(keysDown()&KEY_X) sendCommand("/bx0");
		if(keysUp()&KEY_X) sendCommand("/bx1");
		if(keysDown()&KEY_Y) sendCommand("/by0");
		if(keysUp()&KEY_Y) sendCommand("/by1");
		if(keysDown()&KEY_L) sendCommand("/bl0");
		if(keysUp()&KEY_L) sendCommand("/bl1");
		if(keysDown()&KEY_R) sendCommand("/br0");
		if(keysUp()&KEY_R) sendCommand("/br1");
		if(keysDown()&KEY_START) sendCommand("/bt0");
		if(keysUp()&KEY_START) sendCommand("/bt1");
		if(keysDown()&KEY_SELECT) sendCommand("/be0");
		if(keysUp()&KEY_SELECT) sendCommand("/be1");
		if(keysDown()&KEY_UP) sendCommand("/du0");
		if(keysUp()&KEY_UP) sendCommand("/du1");
		if(keysDown()&KEY_DOWN) sendCommand("/dd0");
		if(keysUp()&KEY_DOWN) sendCommand("/dd1");
		if(keysDown()&KEY_LEFT) sendCommand("/dl0");
		if(keysUp()&KEY_LEFT) sendCommand("/dl1");
		if(keysDown()&KEY_RIGHT) sendCommand("/dr0");
		if(keysUp()&KEY_RIGHT) sendCommand("/dr1");
		if(keysDownGH(GH_BLUE)) sendCommand("/gb0");
		if(keysUpGH(GH_BLUE)) sendCommand("/gb1");
		if(keysDownGH(GH_YELLOW)) sendCommand("/gy0");
		if(keysUpGH(GH_YELLOW)) sendCommand("/gy1");
		if(keysDownGH(GH_RED)) sendCommand("/gr0");
		if(keysUpGH(GH_RED)) sendCommand("/gr1");
		if(keysDownGH(GH_GREEN)) sendCommand("/gg0");
		if(keysUpGH(GH_GREEN)) sendCommand("/gg1");
		if(keysHeld()&KEY_TOUCH && ((lastx != stylusPos.px || lasty != stylusPos.py) || (lastx == 299 || lasty == 299)))
		{
			char command[12];
			sprintf(command, "/m%i.%i.1", stylusPos.px, stylusPos.py);
			sendCommand(command);
		}
		if(keysUp()&KEY_TOUCH)
		{
			char command[12];
			sprintf(command, "/m%i.%i.0", stylusPos.px, stylusPos.py);
			sendCommand(command);
		}
		lastx = stylusPos.px;
		lasty = stylusPos.py;
		//swiWaitForVBlank();
	}

	shutdown(my_socket,0);//good practice to shutdown the socket.
	closesocket(my_socket);//remove the socket.
	//shut off DS

	return 0;
}
