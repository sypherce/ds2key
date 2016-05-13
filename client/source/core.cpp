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

#include "core.h"

#include "gui/gui.h"//D2K::GUI::Screen

#include "common/udp.h"
#include "common/misc.h"
#include "config.h"

namespace D2K {

touchPosition g_stylus_position;
bool input_changed = false;
bool enable_input_timeout = true;
bool toggle_both_lights = true;

void BacklightsOn()
{
#if defined(_NDS)
	powerOn(PM_BACKLIGHT_BOTTOM);
#elif defined(_3DS)
	GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTTOM);
#endif
	// Turn on top light only if enabled
	if(toggle_both_lights)
	{
#if defined(_NDS)
		powerOn(PM_BACKLIGHT_TOP);	
#elif defined(_3DS)
		GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_TOP);	
#endif
	}
}

void BacklightsOff()
{
#if defined(_NDS)
	powerOff(PM_BACKLIGHT_BOTTOM);
#elif defined(_3DS)
	GSPLCD_PowerOffBacklight(GSPLCD_SCREEN_BOTTOM);
#endif
	// Turn off top light only if enabled
	if(toggle_both_lights)
	{
#if defined(_NDS)
		powerOff(PM_BACKLIGHT_TOP);
#elif defined(_3DS)
		GSPLCD_PowerOffBacklight(GSPLCD_SCREEN_TOP);
#endif
	}
}


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
enum
{
	KEY_LID = BIT(13)
};

static uint8_t lid_open{};
uint8_t old_lid_open{};

void ScanLid()
{
	old_lid_open = lid_open;
	PTMU_GetShellState(&lid_open);
}

//just closed
uint32_t LidDown()
{
	if(lid_open == 0 && old_lid_open == 1)
		return KEY_LID;

	return 0;
}
//still closed
uint32_t LidHeld()
{
	if(lid_open == 0 && old_lid_open == 0)
		return KEY_LID;

	return 0;
}
//just opened
uint32_t LidUp()
{
	if(lid_open == 1 && old_lid_open == 0)
		return KEY_LID;

	return 0;
}
//no event for still being opened
#endif

uint32_t g_keys_held, g_keys_down, g_keys_up{ };

///updates input values
void UpdateInputs()
{
	if(input_changed)
	{
		input_changed = false;

		scanKeys();
		g_keys_held = keysHeld();
		g_keys_down = keysDown();
		g_keys_up   = keysUp();

#if defined(_3DS)
		ScanLid();
		g_keys_held |= LidHeld();
		g_keys_down |= LidDown();
		g_keys_up   |= LidUp();
#elif defined(_NDS)
		guitarGripScanKeys();
		if(guitarGripIsInserted())
		{
			g_keys_held |= (guitarGripKeysHeld() << 24) >> 13;
			g_keys_down |= (guitarGripKeysDown() << 24) >> 13;
			g_keys_up   |= (guitarGripKeysUp()   << 24) >> 13;
		}
#endif

		if(!(g_keys_up&KEY_TOUCH))
		{
			touchRead(&g_stylus_position);
		}
	}
}

// Updates backlights depending on touch and lid status
void UpdateLid()
{
	static uint32_t s_vblank_count = 0;
	static const uint32_t VBLANK_MAX = (60 * 4); // 4 seconds
	
	if(g_keys_up&KEY_LID                         // If lid just opened OR
	|| g_keys_held&KEY_TOUCH)                    // Screen is touched
	{
		s_vblank_count = 0;                  // Reset timer
		BacklightsOn();                      // Backlights on
	}
	else if(g_keys_down                          // A button pressed, possibly the lid OR
	|| s_vblank_count == VBLANK_MAX)             // Enough time passed
	{
		BacklightsOff();                     // Backlights off
	}

	if(s_vblank_count < VBLANK_MAX)
		s_vblank_count++;                    // Increment timer
	if(!enable_input_timeout)                    // This avoids the screen turning off after 4 seconds
		s_vblank_count = 0;
	//while(g_keys_held&KEY_LID)                  // Wait here while the lid is closed
	//	WaitForVBlank();
}

///vblank function we assign in Init()
void VBlankFunction()
{
	UpdateInputs();
	UpdateLid();
}

void WaitForVBlank()
{
#if defined(_3DS)
	gspWaitForVBlank();
	VBlankFunction();
#elif defined(_NDS)
	swiWaitForVBlank();
#endif
}

bool Init()
{
	// Screen setup
#if defined(_3DS)
	gfxInitDefault(); //Graphics
	gspLcdInit();     //Backlight
	ptmuInit();       //Lid
	consoleInit(GFX_TOP, NULL_VALUE);
	gfxSetDoubleBuffering(GFX_BOTTOM, false);

	//Get the bottom screen's frame buffer
	D2K::GUI::g_screen[0] = (uint16_t*)gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL_VALUE, NULL_VALUE);
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
	
	irqSet(IRQ_VBLANK, VBlankFunction); // Setup vblank function
#endif

	if(EMULATOR)  // If we're running in emulator mode, let the user know
		std::cout << " - Emulator Mode";

	std::cout << "\n-\n";

#ifdef _NDS
	if(!fatInitDefault())
		std::clog << "Error (fatInitDefault): Failed to access storage\n";

	std::cout << "Connecting via WFC data\n";
	if(EMULATOR
	|| !Wifi_InitDefault(WFC_CONNECT))
	{
		std::clog << "Error (Wifi_InitDefault): Failed to connect\n";
		return true;                // Return with error
	}

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


	UDP::Init();    // Initilize UDP
	Config::Load(); // Load UDP settings
	UDP::Connect(); // Connect with settings
#ifdef _NDS
	if(!toggle_both_lights)
		powerOff(PM_BACKLIGHT_TOP);
#endif

	return false;   // Return without error
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
	if(!aptMainLoop())
		return 0;

	if((g_keys_held&KEY_START)
	&& (g_keys_held&KEY_SELECT)
	&& (g_keys_held&KEY_L)
	&& (g_keys_held&KEY_R))
		return 0;

	// Flush and swap framebuffers
	gfxFlushBuffers();
	gfxSwapBuffers();
#endif

	input_changed = true;
	WaitForVBlank();

	return 1;
}

}//namespace D2K
