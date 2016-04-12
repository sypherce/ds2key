//NDS Init() and Update()

#include <time.h>
#include <cstdio>
#include <iostream>
#include <errno.h>

#if defined(_3DS)
#include <3ds.h>
#include <malloc.h>
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
#ifdef _NDS
		if(guitarGripIsInserted())
		{
			guitarGripScanKeys();
		}
#endif
		if(!keysUp()&&KEY_TOUCH)
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

#ifdef _NDS
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
	gfxInitDefault();
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
	#define SOC_ALIGN       0x1000
	#define SOC_BUFFERSIZE  0x100000

	// allocate buffer for SOC service
	static u32 *SOC_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

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

void Loop()
{
	input_changed = true;
#if defined(_3DS)
	
        // Flush and swap framebuffers
        gfxFlushBuffers();
        gfxSwapBuffers();

	gspWaitForVBlank();VBlankFunction();
#elif defined(_NDS)
	swiWaitForVBlank();
#endif
}

}//namespace D2K
