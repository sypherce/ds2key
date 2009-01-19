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
#ifndef __MAIN_H__
#define __MAIN_H__

//Windows Defines
#ifdef WIN32
#ifdef _MSC_VER //less warnings from microsoft
#define stricmp	_stricmp
#define strnicmp _strnicmp
#endif
#define bool BOOL
#define sockaddr_in__address(sockaddr_in) sockaddr_in.sin_addr.S_un.S_addr
#define _sleep(time) Sleep(time)
#else //WIN32
#define INPUT_MOUSE 0
#define INPUT_KEYBOARD 1
#define stricmp	strcasecmp
#define strnicmp strncasecmp
#define bool unsigned char
#define sockaddr_in__address(sockaddr_in) sockaddr_in.sin_addr.s_addr
#define _sleep(time) usleep(time * 1000)
#endif //WIN32

//Function Defines
#define isExtended(key) \
((key == KEY_INSERT) || \
(key == KEY_DELETE) || \
(key == KEY_HOME) || \
(key == KEY_END) || \
(key == KEY_PRIOR) || \
(key == KEY_NEXT) || \
(key == KEY_NUMLOCK) || \
(key == KEY_PAUSE) || \
(key == KEY_PRINT) || \
(key == KEY_DIVIDE) || \
(key == KEY_RCONTROL) || \
(key == KEY_UP) || \
(key == KEY_DOWN) || \
(key == KEY_LEFT) || \
(key == KEY_RIGHT))

//(key == KEY_NUMPADENTER) || \
//(key == KEY_RALT) || \

//Normal Defines
#define MAX_MSG 12

//Variables
#ifdef WIN32
extern HINSTANCE hInst;
//extern INPUT input;
#else //WIN32
extern int screen;
extern Display *display;
#endif //WIN32

extern bool mouseKeys[13];
extern bool mouseKeysLast[13];
extern struct sockaddr_in servAddr;
extern int sd;
extern bool connectedDS2Key;

//Functions
//extern int _printf(const char *format, ...);
extern void doInput(unsigned int type, unsigned int key, bool state);
extern void connectDS2Key();
extern void disconnectDS2Key();
extern void serverLoop();

#endif //__MAIN_H__
