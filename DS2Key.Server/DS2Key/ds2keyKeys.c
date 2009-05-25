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
#include <stdio.h>
#include "ds2keyKeys.h"

ds2keyPacket packet = { 0 };
static uint16_t keys = 0;
static uint16_t keysOld = 0;
static uint8_t ghKeys = 0;
static uint8_t ghKeysOld = 0;

void ds2keyScan(void)
{
	keysOld = keys;
	keys = packet.keys;

	ghKeysOld = ghKeys;
	ghKeys = packet.ghKeys;
}

void ds2keySetTouchPos(uint8_t x, uint8_t y)
{
	packet.touchX = x;
	packet.touchY = y;
}

void ds2keyPress(uint16_t key)
{
	packet.keys |= key;
}

void ds2keyRelease(uint16_t key)
{
	packet.keys &= ~key;
}

uint16_t ds2keyHeld(void)
{
	return keys;
}

uint16_t ds2keyDown(void)
{
	return (keys &~ keysOld);
}

uint16_t ds2keyUp(void)
{
	return (keys ^ keysOld) & (~keys);
}

void ds2keyGHPress(uint8_t key)
{
	packet.ghKeys |= key;
}

void ds2keyGHRelease(uint8_t key)
{
	packet.ghKeys &= ~key;
}

uint8_t ds2keyGHHeld(void)
{
	return ghKeys;
}

uint8_t ds2keyGHDown(void)
{
	return (ghKeys &~ ghKeysOld);
}

uint8_t ds2keyGHUp(void)
{
	return (ghKeys ^ ghKeysOld) & (~ghKeys);
}

uint8_t ds2keyTouchX()
{
	return packet.touchX;
}

uint8_t ds2keyTouchY()
{
	return packet.touchY;
}

uint8_t ds2keyProfile()
{
	return packet.profile;
}
