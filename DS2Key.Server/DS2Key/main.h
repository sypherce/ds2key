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
#else //WIN32
#define INPUT_MOUSE 0
#define INPUT_KEYBOARD 1
#define bool unsigned char
#define stricmp	strcasecmp
#define strnicmp strncasecmp
#endif //WIN32

//Normal Defines
#define MAX_MSG 12

//Variables
#ifndef WIN32
extern int screen;
extern Display *display;
#endif //WIN32
extern char currentIP[16];

//Functions
extern void doInput(unsigned int type, unsigned int key, bool state);
extern char *longToIP(unsigned long longIP, char *charIP);

#endif //__MAIN_H__
