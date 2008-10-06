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
#include <stdlib.h>
#include <dswifi7.h>
#include "fifo.h"

void startSound(int sampleRate, const void *data, u32 bytes, u8 channel, u8 vol, u8 pan, u8 format)
{
	SCHANNEL_TIMER(channel) = SOUND_FREQ(sampleRate);
	SCHANNEL_SOURCE(channel) = (u32)data;
	SCHANNEL_LENGTH(channel) = bytes >> 2;
	SCHANNEL_CR(channel) = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(vol) | SOUND_PAN(pan) | (format == 1 ? SOUND_8BIT : SOUND_16BIT);
}

s32 getFreeSoundChannel()
{
	int i;
	for(i = 0; i < 16; i++)
	{
		if((SCHANNEL_CR(i) & SCHANNEL_ENABLE) == 0)
		{
			return i;
		}
	}

	return -1;
}

int vcount;
touchPosition first, tempPos;

void VcountHandler()
{
	static int lastbut = -1;

	uint16 but = 0, x = 0, y = 0, xpx = 0, ypx = 0, z1 = 0, z2 = 0;

	but = REG_KEYXY;

	if(!((but ^ lastbut) & (1 << 6)))
	{
		tempPos = touchReadXY();

		if(tempPos.x == 0 || tempPos.y == 0)
		{
			but |= (1 << 6);
			lastbut = but;
		}
		else
		{
			x = tempPos.x;
			y = tempPos.y;
			xpx = tempPos.px;
			ypx = tempPos.py;
			z1 = tempPos.z1;
			z2 = tempPos.z2;
		}
	}
	else
	{
		lastbut = but;
		but |= (1 << 6);
	}

	if(vcount == 80)
	{
		first = tempPos;
	}
	else
	{
		if(abs(xpx - first.px) > 10 || abs(ypx - first.py) > 10 || (but & (1 << 6)))
		{
			but |= (1 << 6);
			lastbut = but;
		}
		else
		{
			IPC->mailBusy = 1;
			IPC->touchX = x;
			IPC->touchY = y;
			IPC->touchXpx = xpx;
			IPC->touchYpx = ypx;
			IPC->touchZ1 = z1;
			IPC->touchZ2 = z2;
			IPC->mailBusy = 0;
		}
	}

	IPC->buttons = but;
	vcount ^= (80 ^ 130);
	SetYtrigger(vcount);
}

void VblankHandler(void)
{
	u32 i;

	//sound code  :)
	TransferSound *snd = IPC->soundData;
	IPC->soundData = 0;

	if(0 != snd)
	{
		for(i = 0; i < snd->count; i++)
		{
			s32 chan = getFreeSoundChannel();

			if(chan >= 0)
			{
				startSound(snd->data[i].rate, snd->data[i].data, snd->data[i].len, chan, snd->data[i].vol, snd->data[i].pan, snd->data[i].format);
			}
		}
	}

	Wifi_Update();
}

int main(int argc, char **argv)
{
	REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR;

	rtcReset();

	//enable sound
	powerON(POWER_SOUND);
	SOUND_CR = SOUND_ENABLE | SOUND_VOL(0x7F);
	IPC->soundData = 0;
	IPC->mailBusy = 0;

	irqInit();
	irqSet(IRQ_VBLANK, VblankHandler);
	SetYtrigger(80);
	vcount = 80;
	irqSet(IRQ_VCOUNT, VcountHandler);
	irqEnable(IRQ_VBLANK | IRQ_VCOUNT);
	irqSet(IRQ_WIFI, Wifi_Interrupt);
	irqEnable(IRQ_WIFI);

	irqSet(IRQ_FIFO_NOT_EMPTY, fifo);
	irqEnable(IRQ_FIFO_NOT_EMPTY);
	REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_RECV_IRQ;

	while(1)
	{
		swiWaitForVBlank();
	}
}
