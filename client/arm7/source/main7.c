#include <nds.h>
#include <dswifi7.h>

volatile bool exitflag = false;

void updateTime() {//this fixes a bug in libnds for 3DS
	//static int frame60 = 0;
	//if(frame60++ == 60) {
	//	frame60 = 0;
		resyncClock();
	//}
}

void VblankHandler(void) {
	Wifi_Update();
	updateTime();
}

void VcountHandler() {
	inputGetAndSend();
}

void powerButtonCB() {
	exitflag = true;
}

int main() {
	readUserSettings();

	irqInit();
	// Start the RTC tracking IRQ
	initClockIRQ();
	fifoInit();

	SetYtrigger(80);

	installWifiFIFO();
	installSoundFIFO();

	installSystemFIFO();

	irqSet(IRQ_VCOUNT, VcountHandler);
	irqSet(IRQ_VBLANK, VblankHandler);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);

	setPowerButtonCB(powerButtonCB);

	// Keep the ARM7 mostly idle
	while(!exitflag) {
		swiWaitForVBlank();
	}
	return 0;
}
