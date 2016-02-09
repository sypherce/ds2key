//NDS Init() and Update()

#include <time.h>
#include <cstdio>
#include <iostream>
#include <errno.h>
#include <nds.h>
#include <fat.h>
#include <dswifi9.h>

#include "gui/gui.h"//D2K::GUI::Screen
#include "common/udp.h"
#include "common/misc.h"
#include "config.h"

namespace D2K {

touchPosition g_stylus_position;
bool input_changed = false;
bool enable_input_timeout = false;
bool toggle_both_lights = false;

char* GetTime()
{
	static char s_time_char[12];

	time_t unix_time = time(0);
	struct tm* time_struct = gmtime((const time_t*)&unix_time);
	int hour = time_struct->tm_hour;
	int minute = time_struct->tm_min;
	int second = time_struct->tm_sec;

	if(hour == 0)
		hour = 12;
	else if(hour > 12)
		hour = hour - 12;
	
	sprintf(s_time_char, "%02i:%02i:%02i AM", hour, minute, second);

	return s_time_char;
}

///updates input values
void UpdateInputs()
{
	if(input_changed)
	{
		input_changed = false;
		scanKeys();
		if(guitarGripIsInserted())
		{
			guitarGripScanKeys();
		}
		if(!keysUp()&&KEY_TOUCH)
		{
			touchRead(&g_stylus_position);
		}
	}
}

///updates backlights depending on touch and lid status
void UpdateLid()
{
	static uint32_t s_vblank_count = 0;
	static const uint32_t VBLANK_MAX = (60 * 4);			//4 seconds

	if((keysUp()&KEY_LID)						//if lid just opened OR
	|| (keysHeld()&KEY_TOUCH))					//screen is touched
	{
		s_vblank_count = 0;					//reset timer
		powerOn(PM_BACKLIGHT_BOTTOM);				//lights on
		if(toggle_both_lights)					//turn on top light only if enabled
			powerOn(PM_BACKLIGHT_TOP);	
	}
	else if((keysDown()&KEY_LID)					//if lid just closed OR
	|| keysDown() 							//a button pressed OR
	|| s_vblank_count == VBLANK_MAX)				//enough time passed
	{
		powerOff(PM_BACKLIGHT_BOTTOM);				//lights off
		if(toggle_both_lights)
			powerOff(PM_BACKLIGHT_TOP);			//turn off top light only if enabled
	}
	if(s_vblank_count < VBLANK_MAX)
		s_vblank_count++;					//increment timer
	if(!enable_input_timeout)					//this avoids the screen turning off after 4 seconds
		s_vblank_count = 0;
}

///vblank function we assign in Init()
void VBlankFunction()
{
	UpdateInputs();
	UpdateLid();
}

bool Init()
{
	//screen setup
	//powerOff(PM_BACKLIGHT_TOP);
	videoSetModeSub(MODE_0_2D);

	//console setup
	consoleDemoInit();
	vramSetPrimaryBanks(VRAM_A_LCD, VRAM_B_MAIN_SPRITE, VRAM_C_SUB_BG, VRAM_D_LCD);

	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	int background_3_id = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
	D2K::GUI::g_screen[0] = bgGetGfxPtr(background_3_id);
	lcdSwap();

	consoleClear();

	//if we're running in emulator mode, let the user know
	if(EMULATOR)
		std::cout << " - Emulator Mode";

	std::cout << "\n-\n";

	if(!EMULATOR
	&& !fatInitDefault())
		std::clog << "Error (fatInitDefault): Failed to access storage\n";

	std::cout << "Connecting via WFC data\n";
	if(!EMULATOR
	&& !Wifi_InitDefault(WFC_CONNECT))		///this needs replaced
	{
		std::clog << "Error (Wifi_InitDefault): Failed to connect\n";
		return true;				//return with error
	}

	irqSet(IRQ_VBLANK, VBlankFunction);		//setup vblank function

	UDP::Init();					//initilize udp
	Config::Load();					//load udp settings
	UDP::Connect();					//connect with settings
	if(!toggle_both_lights)
		powerOff(PM_BACKLIGHT_TOP);

	return false;					//return without error
}

void Loop()
{
	input_changed = true;
	swiWaitForVBlank();
}

}//namespace D2K
