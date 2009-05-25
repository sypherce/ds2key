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
#ifndef HEADER_DS2KEYNETWORK
#define HEADER_DS2KEYNETWORK

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define sockaddr_in__address(sockaddr_in) sockaddr_in.sin_addr.S_un.S_addr
#else//WIN32
#define sockaddr_in__address(sockaddr_in) sockaddr_in.sin_addr.s_addr
#endif//WIN32
#define getIP() ((char *)inet_ntoa(cliAddr.sin_addr))//char *ip;
#define MAX_PROFILE 256
#define MAX_MSG 12//sizeof(ds2keyPacket)
#define OLD_HEADER '/'
#define NEW_HEADER OLD_HEADER + 1

extern int ds2keyConnect(int port);
extern int ds2keyDisconnect();
extern int ds2keyUpdate();

#ifdef __cplusplus
}//extern "C" end
#endif

#endif//HEADER_DS2KEYNETWORK
