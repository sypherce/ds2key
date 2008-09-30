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
#ifndef __FIFO_H__
#define __FIFO_H__

#define PM_BACKLIGHTS	(PM_BACKLIGHT_BOTTOM | PM_BACKLIGHT_TOP)

//lights
#define MSG_BOTH_BACKLIGHTS_ON		0x10000001
#define MSG_TOP_BACKLIGHT_ON		0x10000002
#define MSG_BOTTOM_BACKLIGHT_ON		0x10000003
#define MSG_BOTH_BACKLIGHTS_OFF		0x10000004
#define MSG_TOP_BACKLIGHT_OFF		0x10000005
#define MSG_BOTTOM_BACKLIGHT_OFF	0x10000006
#define MSG_BOTH_BACKLIGHTS_TOGGLE	0x10000007
#define MSG_TOP_BACKLIGHT_TOGGLE	0x10000008
#define MSG_BOTTOM_BACKLIGHT_TOGGLE 0x10000009

//wifi
#define MSG_WIFI_INITIALIZE 0x10000010
#define MSG_WIFI_SYNC		0x10000011

#ifdef ARM7
#endif //ARM7
#ifdef ARM9

//lights
void turnOnBothBacklights();
void turnOnsetTopBacklight();
void turnOnBottomBacklight();
void turnOffBothBacklights();
void turnOffTopBacklight();
void turnOffBottomBacklight();
void toggleBothBacklights();
void toggleTopBacklight();
void toggleBottomBacklight();

//wifi
extern void initWifi();
#endif //ARM9
extern void cpuSync();
extern void fifo();

#endif //__FIFO_H__
