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
#ifndef __MAIN9_H__
#define __MAIN9_H__

//defines
#define settingsPrint() iprintf("\x1b[3;0H        IP: %s \n\n\n        Port: %i \n\n\n        Profile: %i \n\n\n\n\n\n      ", cIP, port, profile)

#define settingsPrintButtons() iprintf("\x1b[3;0H[Edit]  IP: %s\n\n\n[Edit]  Port: %i \n\n\n[Edit]  Profile: %i \n\n\n\n\n\n[Done]", cIP, port, profile)

//Variables
extern bool screenPos;
extern bool mouseLock;
extern bool settings;
extern int lightCounter;
extern char lightTimeout;

//Functions
extern void toggleScreen();
extern void vblfunction();

//Function Defines
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define stylusBetween(x1, y1, x2, y2) (stylusPos.px >= min((x1), (x2)) && stylusPos.px <= max((x1), (x2)) && stylusPos.py >= min((y1), (y2)) && stylusPos.py <= max((y1), (y2)))
#define emulator() (!PersonalData->name[0])

#endif //__MAIN9_H__
