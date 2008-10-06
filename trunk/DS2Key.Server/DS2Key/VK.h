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
#ifndef __VK_H__
#define __VK_H__

//Windows Defines
#ifdef _MSC_VER //less warnings from microsoft
#define stricmp _stricmp
#endif //_MSC_VER

//Variables
extern char *vkTable[0xff];

//Functions
extern void initVKTable();
extern unsigned char getVKNumber(char *key);
extern char *getVKString(unsigned char key);

#endif //__VK_H__
