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
#include <nds.h>
#include <dswifi9.h>
#include "fifo.h"

#ifdef ARM9

//lights
void turnOnBothBacklights()
{
	while(!(REG_IPC_FIFO_CR & IPC_FIFO_SEND_EMPTY));
	REG_IPC_FIFO_TX = MSG_BOTH_BACKLIGHTS_ON;
	swiWaitForVBlank();
}

void turnOnsetTopBacklight()
{
	while(!(REG_IPC_FIFO_CR & IPC_FIFO_SEND_EMPTY));
	REG_IPC_FIFO_TX = MSG_TOP_BACKLIGHT_ON;
	swiWaitForVBlank();
}

void turnOnBottomBacklight()
{
	while(!(REG_IPC_FIFO_CR & IPC_FIFO_SEND_EMPTY));
	REG_IPC_FIFO_TX = MSG_BOTTOM_BACKLIGHT_ON;
	swiWaitForVBlank();
}

void turnOffBothBacklights()
{
	while(!(REG_IPC_FIFO_CR & IPC_FIFO_SEND_EMPTY));
	REG_IPC_FIFO_TX = MSG_BOTH_BACKLIGHTS_OFF;
	swiWaitForVBlank();
}

void turnOffTopBacklight()
{
	while(!(REG_IPC_FIFO_CR & IPC_FIFO_SEND_EMPTY));
	REG_IPC_FIFO_TX = MSG_TOP_BACKLIGHT_OFF;
	swiWaitForVBlank();
}

void turnOffBottomBacklight()
{
	while(!(REG_IPC_FIFO_CR & IPC_FIFO_SEND_EMPTY));
	REG_IPC_FIFO_TX = MSG_BOTTOM_BACKLIGHT_OFF;
	swiWaitForVBlank();
}

void toggleBothBacklights()
{
	while(!(REG_IPC_FIFO_CR & IPC_FIFO_SEND_EMPTY));
	REG_IPC_FIFO_TX = MSG_BOTH_BACKLIGHTS_TOGGLE;
	swiWaitForVBlank();
}

void toggleTopBacklight()
{
	while(!(REG_IPC_FIFO_CR & IPC_FIFO_SEND_EMPTY));
	REG_IPC_FIFO_TX = MSG_TOP_BACKLIGHT_TOGGLE;
	swiWaitForVBlank();
}

void toggleBottomBacklight()
{
	while(!(REG_IPC_FIFO_CR & IPC_FIFO_SEND_EMPTY));
	REG_IPC_FIFO_TX = MSG_BOTTOM_BACKLIGHT_TOGGLE;
	swiWaitForVBlank();
}

//wifi
void initWifi()
{
	u32 Wifi_pass = Wifi_Init(WIFIINIT_OPTION_USELED);
	REG_IPC_FIFO_TX = MSG_WIFI_INITIALIZE;
	REG_IPC_FIFO_TX = Wifi_pass;
	Wifi_SetSyncHandler(cpuSync);
	while(Wifi_CheckInit() == 0)
	{
		swiWaitForVBlank();
	}
}

#endif //ARM9
void cpuSync()
{
	REG_IPC_FIFO_TX = MSG_WIFI_SYNC;
}

void fifo()
{
	u32 message = REG_IPC_FIFO_RX;

	switch(message)
	{
			#ifdef ARM7

		//lights
		case MSG_BOTH_BACKLIGHTS_ON:
			{
				u32 reg = readPowerManagement(PM_CONTROL_REG) | PM_BACKLIGHTS;
				writePowerManagement(PM_CONTROL_REG, reg);
				break;
			}

		case MSG_TOP_BACKLIGHT_ON:
			{
				u32 reg = readPowerManagement(PM_CONTROL_REG) | PM_BACKLIGHT_TOP;
				writePowerManagement(PM_CONTROL_REG, reg);
				break;
			}

		case MSG_BOTTOM_BACKLIGHT_ON:
			{
				u32 reg = readPowerManagement(PM_CONTROL_REG) | PM_BACKLIGHT_BOTTOM;
				writePowerManagement(PM_CONTROL_REG, reg);
				break;
			}

		case MSG_BOTH_BACKLIGHTS_OFF:
			{
				u32 reg = readPowerManagement(PM_CONTROL_REG) &~PM_BACKLIGHTS;
				writePowerManagement(PM_CONTROL_REG, reg & 255);
				break;
			}

		case MSG_TOP_BACKLIGHT_OFF:
			{
				u32 reg = readPowerManagement(PM_CONTROL_REG) &~PM_BACKLIGHT_TOP;
				writePowerManagement(PM_CONTROL_REG, reg & 255);
				break;
			}

		case MSG_BOTTOM_BACKLIGHT_OFF:
			{
				u32 reg = readPowerManagement(PM_CONTROL_REG) &~PM_BACKLIGHT_BOTTOM;
				writePowerManagement(PM_CONTROL_REG, reg & 255);
				break;
			}

		case MSG_BOTH_BACKLIGHTS_TOGGLE:
			{
				u32 reg = readPowerManagement(PM_CONTROL_REG);
				if(reg & PM_BACKLIGHTS)
				{
					reg &= ~PM_BACKLIGHTS;
				}
				else
				{
					reg |= PM_BACKLIGHTS;
				}

				writePowerManagement(PM_CONTROL_REG, reg);
				break;
			}

		case MSG_TOP_BACKLIGHT_TOGGLE:
			{
				u32 reg = readPowerManagement(PM_CONTROL_REG);
				if(reg & PM_BACKLIGHT_TOP)
				{
					reg &= ~PM_BACKLIGHT_TOP;
				}
				else
				{
					reg |= PM_BACKLIGHT_TOP;
				}

				writePowerManagement(PM_CONTROL_REG, reg);
				break;
			}

		case MSG_BOTTOM_BACKLIGHT_TOGGLE:
			{
				u32 reg = readPowerManagement(PM_CONTROL_REG);
				if(reg & PM_BACKLIGHT_BOTTOM)
				{
					reg &= ~PM_BACKLIGHT_BOTTOM;
				}
				else
				{
					reg |= PM_BACKLIGHT_BOTTOM;
				}

				writePowerManagement(PM_CONTROL_REG, reg);
				break;
			}

		//wifi
		case MSG_WIFI_INITIALIZE:
			{
				while(REG_IPC_FIFO_CR & IPC_FIFO_RECV_EMPTY)
				{
					swiWaitForVBlank();
				}

				Wifi_Init(REG_IPC_FIFO_RX);
				Wifi_SetSyncHandler(cpuSync);
				break;
			}

			#endif

		case MSG_WIFI_SYNC:
			{
				Wifi_Sync();
				break;
			}
	}
}
