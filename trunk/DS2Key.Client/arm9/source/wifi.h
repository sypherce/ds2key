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
#ifndef __WIFI_H__
#define __WIFI_H__

//Variables
extern struct sockaddr_in sain;

//Functions
extern void Timer_50ms();
extern unsigned long iptoi(char *ipchar, struct sockaddr_in sockaddr);
extern void sendCommand(int socket, struct sockaddr_in sockaddr, unsigned long ip, unsigned int port, char *command);
extern void wifiConnect();

//Function Defines
#define sendCommand(a) sendCommand(my_socket, sain, iptoi(cIP, sain), port, a)

#endif //__WIFI_H__
