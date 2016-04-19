//NDS Init() and Update()

#include <time.h>
#include <cstdio>
#include <iostream>
#include <errno.h>

#if defined(_3DS)
#include <3ds.h>
#include <malloc.h>
#include <stdio.h>
#elif defined(_NDS)
#include <nds.h>
#include <fat.h>
#include <dswifi9.h>
#endif

#include "gui/gui.h"//D2K::GUI::Screen

#include "common/udp.h"
#include "common/misc.h"
#include "config.h"

namespace D2K {

touchPosition g_stylus_position;
bool input_changed = false;
bool enable_input_timeout = true;
bool toggle_both_lights = false;

#ifdef _3DS
void displayKeys()
{
	static char keysNames[32][32] = {
		"KEY_A", "KEY_B", "KEY_SELECT", "KEY_START",
		"KEY_DRIGHT", "KEY_DLEFT", "KEY_DUP", "KEY_DDOWN",
		"KEY_R", "KEY_L", "KEY_X", "KEY_Y",
		"", "", "KEY_ZL", "KEY_ZR",
		"", "", "", "",
		"KEY_TOUCH", "", "", "",
		"KEY_CSTICK_RIGHT", "KEY_CSTICK_LEFT", "KEY_CSTICK_UP", "KEY_CSTICK_DOWN",
		"KEY_CPAD_RIGHT", "KEY_CPAD_LEFT", "KEY_CPAD_UP", "KEY_CPAD_DOWN"
	};
	static u32 kDownOld = 0;
	static u32 kHeldOld = 0;
	static u32 kUpOld = 0;

	u32 kDown = hidKeysDown();
	u32 kHeld = hidKeysHeld();
	u32 kUp = hidKeysUp();
	if (kDown != kDownOld || kHeld != kHeldOld || kUp != kUpOld)
	{
		consoleClear();

		printf("\x1b[3;0H");
		for(int i = 0; i < 32; i++)
		{
			if (kDown & BIT(i)) printf("%s down\n", keysNames[i]);
			if (kHeld & BIT(i)) printf("%s held\n", keysNames[i]);
			if (kUp & BIT(i)) printf("%s up\n", keysNames[i]);
		}
		kDownOld = kDown;
		kHeldOld = kHeld;
		kUpOld = kUp;
	}
	circlePosition pos;
	hidCircleRead(&pos);
	printf("\x1b[2;0H%04d; %04d", pos.dx, pos.dy);
	printf("\x1b[4;0H%04d; %04d", g_stylus_position.px, g_stylus_position.py);
	printf("\x1b[8;0HHold L + R + START + SELECT to exit");
}
#endif

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

#ifdef _3DS
static uint8_t lid_open{};
uint8_t old_lid_open{};
void ScanLid()
{
	old_lid_open = lid_open;
	PTMU_GetShellState(&lid_open);
}

//just closed
bool LidDown()
{
	return lid_open == 0 && old_lid_open == 1;
}
//still closed
bool LidHeld()
{
	return lid_open == 0 && old_lid_open == 0;
}
//just opened
bool LidUp()
{
	return lid_open == 1 && old_lid_open == 0;
}
//no event for still being opened
#endif

///updates input values
void UpdateInputs()
{
	if(input_changed)
	{
		input_changed = false;
		scanKeys();
#ifdef _NDS
		if(guitarGripIsInserted())
		{
			guitarGripScanKeys();
		}
#endif
#ifdef _3DS
		ScanLid();
#endif
		if(!(keysUp()&KEY_TOUCH))
		{
			touchRead(&g_stylus_position);
		}
	}
}

// Updates backlights depending on touch and lid status
void UpdateLid()
{
	static uint32_t s_vblank_count = 0;
	static const uint32_t VBLANK_MAX = (60 * 4);			// 4 seconds

#if defined(_NDS)
	if((keysUp()&KEY_LID)						// If lid just opened OR
	|| (keysHeld()&KEY_TOUCH))					// Screen is touched
	{
		s_vblank_count = 0;					// Reset timer
		powerOn(PM_BACKLIGHT_BOTTOM);				// Lights on
		if(toggle_both_lights)					// Turn on top light only if enabled
			powerOn(PM_BACKLIGHT_TOP);	
	}
	else if((keysDown()&KEY_LID)					// If lid just closed OR
	|| keysDown() 							// A button pressed OR
	|| s_vblank_count == VBLANK_MAX)				// Enough time passed
	{
		powerOff(PM_BACKLIGHT_BOTTOM);				// Lights off
		if(toggle_both_lights)
			powerOff(PM_BACKLIGHT_TOP);			// Turn off top light only if enabled
	}
#elif defined(_3DS)
	if(LidUp()							// If lid just opened OR
	|| keysHeld()&KEY_TOUCH)					// Screen is touched
	{
		s_vblank_count = 0;					// Reset timer
		GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTTOM);		// Lights on
		if(toggle_both_lights)					// Turn on top light only if enabled
			GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_TOP);	
	}
	else if(keysDown() 						// A button pressed OR
	|| s_vblank_count == VBLANK_MAX)				// Enough time passed
	{
		GSPLCD_PowerOffBacklight(GSPLCD_SCREEN_BOTTOM);		// Lights off
		if(toggle_both_lights)
			GSPLCD_PowerOffBacklight(GSPLCD_SCREEN_TOP);	// Turn off top light only if enabled
	}
#endif
	if(s_vblank_count < VBLANK_MAX)
		s_vblank_count++;					// Increment timer
	if(!enable_input_timeout)					// This avoids the screen turning off after 4 seconds
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
	// Screen setup
#if defined(_3DS)
	gfxInitDefault(); //Graphics
	gspLcdInit();     //Backlight
	ptmuInit();       //Lid
	consoleInit(GFX_TOP, NULL);
	gfxSetDoubleBuffering(GFX_BOTTOM, false);

	//Get the bottom screen's frame buffer
	D2K::GUI::g_screen[0] = (uint16_t*)gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
#elif defined(_NDS)
	// PowerOff(PM_BACKLIGHT_TOP);
	videoSetModeSub(MODE_0_2D);

	// Console setup
	consoleDemoInit();
	vramSetPrimaryBanks(VRAM_A_LCD, VRAM_B_MAIN_SPRITE, VRAM_C_SUB_BG, VRAM_D_LCD);

	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	int background_3_id = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
	D2K::GUI::g_screen[0] = bgGetGfxPtr(background_3_id);
	lcdSwap();

	consoleClear();
#endif

	if(EMULATOR)  // If we're running in emulator mode, let the user know
		std::cout << " - Emulator Mode";

	std::cout << "\n-\n";

#ifdef _NDS
	if(!EMULATOR
	&& !fatInitDefault())
		std::clog << "Error (fatInitDefault): Failed to access storage\n";

	std::cout << "Connecting via WFC data\n";
	if(!EMULATOR
	&& !Wifi_InitDefault(WFC_CONNECT))
	{
		std::clog << "Error (Wifi_InitDefault): Failed to connect\n";
		return true;				// Return with error
	}
	
	irqSet(IRQ_VBLANK, VBlankFunction);		// Setup vblank function

#endif

#ifdef _3DS
	static uint32_t SOC_ALIGN = 0x1000;
	static uint32_t SOC_BUFFERSIZE = 0x100000;

	// allocate buffer for SOC service
	static uint32_t *SOC_buffer = (uint32_t*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

	if(SOC_buffer == nullptr) {
		std::clog << "memalign: failed to allocate\n";
		return true;
	}

	int soc_init = socInit(SOC_buffer, SOC_BUFFERSIZE);
	// Now intialise soc:u service
	if(soc_init != 0)
	{
    		std::clog << "socInit: " << (unsigned int)soc_init << "\n";
		return true;
	}
#endif


	UDP::Init();					// Initilize UDP
	Config::Load();					// Load UDP settings
	UDP::Connect();					// Connect with settings
#ifdef _NDS
	if(!toggle_both_lights)
		powerOff(PM_BACKLIGHT_TOP);
#endif

	return false;					// Return without error
}

void DeInit()
{
#ifdef _3DS
	GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTH);
	gspLcdExit();
	socExit();
	gfxExit();
#endif
}

int Loop()
{
#if defined(_3DS)
	displayKeys();
	if(
	(keysHeld()&KEY_START)&&
	(keysHeld()&KEY_SELECT)&&
	(keysHeld()&KEY_L)&&
	(keysHeld()&KEY_R)
	) return 1;
#endif
	input_changed = true;
#if defined(_3DS)
	
        // Flush and swap framebuffers
        gfxFlushBuffers();
        gfxSwapBuffers();

	gspWaitForVBlank();VBlankFunction();
#elif defined(_NDS)
	swiWaitForVBlank();
#endif

	return 0;
}

}//namespace D2K
