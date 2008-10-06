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
#ifndef __GH_H__
#define __GH_H__

#define GH_BLUE	0x08
#define GH_YELLOW 0x10
#define GH_RED 0x20
#define GH_GREEN 0x40
#define GH_ANY 0x78

#define GH_Any(pad)	(GH_Blue(pad) | GH_Yellow(pad) | GH_Red(pad) | GH_Green(pad))
#define GH_Blue(pad) (pad & GH_BLUE)
#define GH_Yellow(pad) (pad & GH_YELLOW)
#define GH_Red(pad) (pad & GH_RED)
#define GH_Green(pad) (pad & GH_GREEN)
#define GH_keysHeld	(keysGH)
#define GH_keysDown	((keysGH ^ oldKeysGH) & keysGH)
#define GH_keysUp ((keysGH ^ oldKeysGH) & (~keysGH))

#define insertedGHPad (GBAROM[0xFFFF] == 0xF9FF)

extern unsigned short int keysGH;
extern unsigned short int oldKeysGH;

extern unsigned short int keysDownGH(unsigned char color);
extern unsigned short int keysHeldGH(unsigned char color);
extern unsigned short int keysUpGH(unsigned char color);
extern void initGHPad();
extern void updateGHPad();

#endif //__GH_H__
