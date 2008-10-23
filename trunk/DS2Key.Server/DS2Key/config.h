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
#ifndef __CONFIG_H__
#define __CONFIG_H__

//Windows Defines
#ifdef WIN32
#define bool BOOL
#else //WIN32
#define bool unsigned char
#endif //WIN32

//Normal Defines
#define DEFAULT_SERVER_PORT 9501
#define pTouch pTouch0X0Y
enum pKeys { pIP, pUp, pDown, pLeft, pRight, pA, pB, pX, pY, pL, pR, pStart, pSelect, pBlue, pYellow, pRed, pGreen, pTouch0X0Y, pTouch1X0Y, pTouch2X0Y, pTouch3X0Y, pTouch0X1Y, pTouch1X1Y, pTouch2X1Y, pTouch3X1Y, pTouch0X2Y, pTouch1X2Y, pTouch2X2Y, pTouch3X2Y, pEND };

//Variables
extern int serverPort;
extern unsigned int profile[256][pEND];

//Functions
extern bool writeConfig();
extern bool writeDefaultConfig();
extern bool writeProfileConfig(unsigned char profileNumber);
extern bool writeDefaultProfileConfig(unsigned char profileNumber);
extern bool getLine(char *buffer);
extern bool readConfig();
extern bool readProfileConfig(unsigned char profileNumber);

#endif //__CONFIG_H__
