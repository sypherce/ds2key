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
#ifndef HEADER_DS2KEYKEYS
#define HEADER_DS2KEYKEYS

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ARM9
#include <arm9/input.h>
#else
#include <stdint.h>
#define BIT(n) (1 << (n))
#define DS2KEY_A BIT(0)
#define DS2KEY_B BIT(1)
#define DS2KEY_SELECT BIT(2)
#define DS2KEY_START BIT(3)
#define DS2KEY_RIGHT BIT(4)
#define DS2KEY_LEFT BIT(5)
#define DS2KEY_UP BIT(6)
#define DS2KEY_DOWN BIT(7)
#define DS2KEY_R BIT(8)
#define DS2KEY_L BIT(9)
#define DS2KEY_X BIT(10)
#define DS2KEY_Y BIT(11)
#define DS2KEY_TOUCH BIT(12)
#define DS2KEY_LID BIT(13)
#endif
#define GH_BLUE	0x08
#define GH_YELLOW 0x10
#define GH_RED 0x20
#define GH_GREEN 0x40

#pragma pack(1)
typedef struct ds2keyPacket
{
	uint8_t type;
	uint8_t profile;
	uint16_t keys;
	uint8_t ghKeys;
	uint8_t touchX;
	uint8_t touchY;
} ds2keyPacket;
#pragma pack()
extern ds2keyPacket packet;

extern void ds2keyScan(void);
extern void ds2keySetTouchPos(uint8_t x, uint8_t y);
extern void ds2keyPress(uint16_t key);
extern void ds2keyRelease(uint16_t key);
extern uint16_t ds2keyHeld(void);
extern uint16_t ds2keyDown(void);
extern uint16_t ds2keyUp(void);

extern void ds2keyGHPress(uint8_t key);
extern void ds2keyGHRelease(uint8_t key);
extern uint8_t ds2keyGHHeld(void);
extern uint8_t ds2keyGHDown(void);
extern uint8_t ds2keyGHUp(void);

extern uint8_t ds2keyTouchX();
extern uint8_t ds2keyTouchY();

extern uint8_t ds2keyProfile();

#ifdef __cplusplus
}//extern "C" end
#endif

#endif//HEADER_DS2KEYKEYS
