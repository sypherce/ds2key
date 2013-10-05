/*
	NDS Setup() and Update()
*/
#include <time.h>
#include <stdio.h>
#include <nds.h>
#include <fat.h>
#include <dswifi9.h>
#include "system.h"
#include "config.h"
#include "gui.h"

namespace D2K {
	namespace System {
		const uint32_t vblCountMax = (60 * 4);
		touchPosition stylusPos;
		bool inputChange = false;
		uint32_t vblCount = 0;
		uint32_t vblLid = 0;

		char* getTime() {
			static char timeChar[13];
			time_t unixTime = time(NULL);
			struct tm* timeStruct = gmtime((const time_t *)&unixTime);
			int hour = timeStruct->tm_hour;
			if(hour == 0)
				hour = 12;
			if(hour > 12)
				sprintf(timeChar, "%02i:%02i:%02i PM", hour - 12, timeStruct->tm_min, timeStruct->tm_sec);
			else
				sprintf(timeChar, "%02i:%02i:%02i AM", hour, timeStruct->tm_min, timeStruct->tm_sec);

			return timeChar;
		}

		void resetStylus() {
			stylusPos.px = 299;
			stylusPos.py = 299;
		}

		void updateInputs() {
			if(inputChange) {
				inputChange = false;
				scanKeys();
				if(guitarGripIsInserted()) {
					guitarGripScanKeys();
				}
				if(!keysUp()&&KEY_TOUCH) {
					touchRead(&System::stylusPos);
				}
			}
		}
		void updateLid() {
			if((keysUp()&KEY_LID) ||							//if lid just opened OR
				(keysHeld()&KEY_TOUCH))	{						//screen is being touched
				vblCount = 0;									//reset timer
				powerOn(PM_BACKLIGHT_BOTTOM|PM_BACKLIGHT_TOP);	//lights on
			}
			else if((keysDown()&KEY_LID) ||						//if lid just closed OR
				keysDown() ||									//a button was pressed OR
					vblCount == vblCountMax) {					//enough time has passed
				powerOff(PM_BACKLIGHT_BOTTOM|PM_BACKLIGHT_TOP);	//lights off
			}
			vblCount++;											//increment timer
		}
		void vblfunction() {
			updateInputs();
			updateLid();
		}

		/*bool isScreenOn() {
			return !(vblCount >= vblCountMax);
		}*/

		void Setup() {
			//screen setup
			//powerOff(PM_BACKLIGHT_TOP);
			videoSetModeSub(MODE_0_2D);

			//console setup
			consoleDemoInit();
			vramSetPrimaryBanks(VRAM_A_LCD, VRAM_B_MAIN_SPRITE, VRAM_C_SUB_BG, VRAM_D_LCD);

			videoSetMode(MODE_5_2D);
			vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
			int bg3 = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
			D2K::GUI::Screen[0] = bgGetGfxPtr(bg3);
			lcdSwap();

			consoleClear();

			printf("%s\n-\n", VERSION_STRING);

			if(!EMULATOR)
			if(!fatInitDefault())
				printf("Error (fatInitDefault): Failed to access storage\n");
			Config::Load();

			iprintf("Connecting via WFC data\n");
			if(!EMULATOR)
			if(!Wifi_InitDefault(WFC_CONNECT)) {
				printf("Error (Wifi_InitDefault): Failed to connect\n");
			}

			//setup vblank IRQ
			irqSet(IRQ_VBLANK, D2K::System::vblfunction);

			resetStylus();
		}

		void Update(bool change) {
			inputChange = change;
			swiWaitForVBlank();
		}
	}
}
