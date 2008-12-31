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
#include <nds/fifomessages.h>
#include <dswifi7.h>
#include <maxmod7.h>
#include "main7.h"

//functions
void VcountHandler()
{
    static bool penDown = false;
    touchPosition tempPos = { 0 };
    SystemInputMsg input = { 0 };

    input.keys = REG_KEYXY;

    if(input.keys&KEY_TOUCH)
    {
        penDown = false;
    }
    else
    {
        input.keys |= KEY_TOUCH;

        if(penDown)
        {
            touchReadXY(&tempPos);

            if(tempPos.rawx && tempPos.rawy)
            {
                input.keys &= ~KEY_TOUCH;
                input.touch = tempPos;
            }
            else
            {
                penDown = false;
            }
        }
        else
        {
            penDown = true;
        }
    }

    input.type = SYS_INPUT_MESSAGE;
    fifoSendDatamsg(FIFO_SYSTEM, sizeof(input), (u8*)&input);
}

void VblankHandler(void)
{
    Wifi_Update();
}

int main()
{
    irqInit();
    fifoInit();

    readUserSettings();

    initClockIRQ();

    SetYtrigger(80);

    installWifiFIFO();
    installSoundFIFO();

    mmInstall(FIFO_MAXMOD);

    installSystemFIFO();

    irqSet(IRQ_VCOUNT, VcountHandler);
    irqSet(IRQ_VBLANK, VblankHandler);

    irqEnable(IRQ_VBLANK|IRQ_VCOUNT|IRQ_NETWORK);

    while(1)
    {
        swiWaitForVBlank();
    }
}
