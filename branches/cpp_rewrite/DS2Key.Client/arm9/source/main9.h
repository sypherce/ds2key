/*
DS2Key Client - An application to use your DS as a PC Gamepad
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
#ifndef __MAIN9_H__
#define __MAIN9_H__

//defines
#define vblCountMax (60 * 4)

//function defines
#define settingsPrint() iprintf("\x1b[0;0H                DS2Key 1.02\n\n        IP: %s \n\n\n        Port: %s \n\n\n        Profile: %s \n\n\n\n\n\n      ", ds2key.ip, ds2key.port, ds2key.profile)
#define settingsPrintButtons() iprintf("\x1b[0;0H                DS2Key 1.02\n\n[Edit]  IP: %s \n\n\n[Edit]  Port: %s \n\n\n[Edit]  Profile: %s \n\n\n\n\n\n[Done]", ds2key.ip, ds2key.port, ds2key.profile)
#define settingsPrintIP() iprintf("\x1b[0;0H                DS2Key 1.02\n\n        IP: %s \n\n\n                    \n\n\n                     \n\n\n\n\n\n      \n\n\n\n\n\n\n                                      ", ds2key.ip)
#define settingsPrintPort() iprintf("\x1b[0;0H                DS2Key 1.02\n\n                           \n\n\n        Port: %s \n\n\n                     \n\n\n\n\n\n      \n\n\n\n\n\n\n                                      ", ds2key.port)
#define settingsPrintProfile() iprintf("\x1b[0;0H                DS2Key 1.02\n\n                           \n\n\n                    \n\n\n        Profile: %s \n\n\n\n\n\n      \n\n\n\n\n\n\n                                      ", ds2key.profile)

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define stylusBetween(x1, y1, x2, y2) (stylusPos.px >= min((x1), (x2)) && stylusPos.px <= max((x1), (x2)) && stylusPos.py >= min((y1), (y2)) && stylusPos.py <= max((y1), (y2)))
#define resetStylus() \
	stylusPos.px = 299; \
	stylusPos.py = 299; \
	lastStylusPos = stylusPos;

//normal defines
enum { iDefault, iMouse, iSettings };

//variables
extern char d2kMode;
extern bool inputChange;
extern uint32 kDown;
extern uint32 kHeld;
extern uint32 kUp;
extern uint32 vblCount;
extern uint32 vblLid;

//functions
extern void updateInputs();
extern void vblfunction();

#endif//__MAIN9_H__

