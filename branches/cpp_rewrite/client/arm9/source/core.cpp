/*
	NDS Setup() and Update()
*/
#include <time.h>
#include <stdio.h>
#include <nds.h>
#include <fat.h>
#include <dswifi9.h>

#include "gui/gui.h"//D2K::GUI::Screen
#include "udp.h"
#include "config.h"

namespace D2K {
	namespace Core {
		touchPosition StylusPos;
		bool InputChange = false;

		///return (char*) containing the current time
		char* GetTime() {
			static char timeChar[12];
			time_t unixTime = time(NULL);
			struct tm* timeStruct = gmtime((const time_t *)&unixTime);
			int hour = timeStruct->tm_hour;
			if(hour == 0)
				hour = 12;
			if(hour > 12)
				sprintf(timeChar, "%02i:%02i:%02i PM", hour - 12, timeStruct->tm_min, timeStruct->tm_sec);
			else
				sprintf(timeChar, "%02i:%02i:%02i AM", hour, timeStruct->tm_min, timeStruct->tm_sec);
				timeChar[11]=0;

			return timeChar;
		}

		///resets stylus x/y. probably garbage
		void ResetStylus() {
			StylusPos.px =
			StylusPos.py = 299;
		}

		///updates input values
		void UpdateInputs() {
			if(InputChange) {
				InputChange = false;
				scanKeys();
				if(guitarGripIsInserted()) {
					guitarGripScanKeys();
				}
				if(!keysUp()&&KEY_TOUCH) {
					touchRead(&StylusPos);
				}
			}
		}
		///updates backlights depending on touch and lid status
		void UpdateLid() {
			static uint32_t vblCount = 0;
			static const uint32_t vblCountMax = (60 * 4);		//4 seconds

			if((keysUp()&KEY_LID) ||							//if lid just opened OR
				(keysHeld()&KEY_TOUCH)) {						//screen is touched
				vblCount = 0;									//reset timer
				powerOn(PM_BACKLIGHT_BOTTOM|PM_BACKLIGHT_TOP);	//lights on
			}
			else if((keysDown()&KEY_LID) ||						//if lid just closed OR
				keysDown() ||									//a button pressed OR
					vblCount == vblCountMax) {					//enough time passed
				powerOff(PM_BACKLIGHT_BOTTOM|PM_BACKLIGHT_TOP);	//lights off
			}
			vblCount++;											//increment timer
		}
		///vblank function we assign in Setup()
		void VBLFunction() {
			UpdateInputs();
			UpdateLid();
		}

		///setup the nds system. start fat, wifi, and our udp system
		bool Setup() {
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

			iprintf("Connecting via WFC data\n");
			if(!EMULATOR)
			if(!Wifi_InitDefault(WFC_CONNECT)) {			///this needs replaced
				printf("Error (Wifi_InitDefault): Failed to connect\n");
				return 1;									//return with error
			}

			irqSet(IRQ_VBLANK, VBLFunction);				//setup vblank function

			ResetStylus();									///is this relavent anymore?

			Core::UDP = new Core::C::UDP();					//initilize udp
			Core::Config = new Core::C::Config();			//load udp settings
			Core::UDP->Connect();							//connect with settings

			return 0;										//return without error
		}

		///loop function that calls VBLFunction()
		void Loop() {
			InputChange = true;
			swiWaitForVBlank();
		}
	}
}
