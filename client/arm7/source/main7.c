#include <nds.h>
#include <dswifi7.h>
//#include <maxmod7.h>

volatile bool exitflag = false;

void VblankHandler(void)
{
	Wifi_Update();
}

void VcountHandler()
{
	inputGetAndSend();
}

void powerButtonCB()
{
	exitflag = true;
}

int main()
{
	readUserSettings();

	irqInit();
	// Start the RTC tracking IRQ
	initClockIRQ();
	fifoInit();

	//mmInstall(FIFO_MAXMOD);

	SetYtrigger(80);

	installWifiFIFO();
	installSoundFIFO();

	installSystemFIFO();

	irqSet(IRQ_VCOUNT, VcountHandler);
	irqSet(IRQ_VBLANK, VblankHandler);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);

	setPowerButtonCB(powerButtonCB);

	// Keep the ARM7 mostly idle
	while(!exitflag)
	{
		swiWaitForVBlank();
	}
	return 0;
}
