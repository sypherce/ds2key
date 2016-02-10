#include <nds.h>
#include <dswifi7.h>

volatile bool exit_flag = false;

void VBlankHandler(void)
{
	Wifi_Update();
	resyncClock();  // Fixes libnds bug for 3DS
}

void VCountHandler()
{
	inputGetAndSend();
}

void PowerButtonCallback()
{
	exit_flag = true;
}

int main()
{
	readUserSettings();

	irqInit();

	initClockIRQ(); // Start the RTC tracking IRQ
	fifoInit();

	SetYtrigger(80);

	installWifiFIFO();
	installSoundFIFO();

	installSystemFIFO();

	irqSet(IRQ_VCOUNT, VCountHandler);
	irqSet(IRQ_VBLANK, VBlankHandler);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);

	setPowerButtonCB(PowerButtonCallback);

	// Keep the ARM7 mostly idle
	while(exit_flag == false)
	{
		swiWaitForVBlank();
	}
	return 0;
}
