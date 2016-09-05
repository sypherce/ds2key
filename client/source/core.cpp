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

// This looks wrong because it's a macro
#include "common/easylogging++Wrapper.h"
INITIALIZE_EASYLOGGINGPP

#include "common/udp.h"
#include "common/misc.h"
#include "config.h"

namespace D2K {

#ifdef _3DS
//TODO: is this filter needed? possibly on server instead? is 8 right?
#define FILTER_SIZE 8
static accelVector accel[FILTER_SIZE]{};
static angularRate gyro[FILTER_SIZE]{};
#endif
circlePosition g_circle_position{};
circlePosition g_cstick_position{};
accelVector g_accel_status{};
angularRate g_gyro_status{};
touchPosition g_stylus_position{};
float g_slider_3d_status{};
uint8_t g_slider_volume_status{};
uint32_t wifi_status{};
uint8_t battery_level{};
uint8_t charging_status{};
bool input_changed = false;
//TODO: this should be configurable?
bool enable_input_timeout = true;
//TODO: this should be configurable?
bool force_backlights_on = false;
//TODO: this should be configurable?
const bool toggle_both_lights = true;

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

void ForceBacklightsOn(bool value)
{
	if(value)
		BacklightsOn();
	force_backlights_on = value;
}

char* GetTime()
{
	static char s_time_char[12];

	time_t unix_time = time(nullptr);
	struct tm* time_struct = gmtime((const time_t*)&unix_time);
	int hour = time_struct->tm_hour;
	int minute = time_struct->tm_min;
	int second = time_struct->tm_sec;
	bool am = false;
	
	if(hour < 12)
		am = true;
	if(hour == 0)
	{
		hour = 12;
	}
	else if(hour > 12)
	{
		hour = hour - 12;
	}

	
	if(am)
		sprintf(s_time_char, "%02i:%02i:%02i AM", hour, minute, second);
	else
		sprintf(s_time_char, "%02i:%02i:%02i PM", hour, minute, second);

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

#ifdef _3DS
//debug printing
//#include <cstdio>
void UpdateGyroAccel()
{
//TODO: is raw usable?
	const bool raw = false;
	if(raw)
	{
		hidAccelRead(&g_accel_status);
		hidGyroRead(&g_gyro_status);
	}
	else
	{
		static size_t array_pos = 0;
		static u32 delay = 0;

		hidAccelRead(&accel[array_pos]);
		hidGyroRead(&gyro[array_pos]);
		array_pos = (array_pos + 1) % FILTER_SIZE;

		if(!delay)
		{
			float accel_x = 0, accel_y = 0, accel_z = 0;
			float gyro_x = 0, gyro_y = 0, gyro_z = 0;
			for(size_t i = 0; i < FILTER_SIZE; ++i)
			{
//TODO: why are we scaling this? should we use float or decimal?
				float scale = 0.1f / FILTER_SIZE;
				accel_x += accel[i].x * scale;
				accel_y += accel[i].y * scale;
				accel_z += accel[i].z * scale;
				gyro_x += gyro[i].x * scale;
				gyro_y += gyro[i].y * scale;
				gyro_z += gyro[i].z * scale;
			}
			g_accel_status.x = accel_x;
			g_accel_status.y = accel_y;
			g_accel_status.z = accel_z;
			g_gyro_status.x = gyro_x;
			g_gyro_status.y = gyro_y;
			g_gyro_status.z = gyro_z;

			//debug printing
			//printf("\x1b[0;0HAccelerometer:\nx = %9.2f\ny = %9.2f\nz = %9.2f", accel_x, accel_y, accel_z);
			//printf("\x1b[5;0H    Gyroscope:\nx = %9.2f\nz = %9.2f\ny = %9.2f", gyro_x, gyro_z, gyro_y);

//TODO: delay was 8 before I changed it
			delay = 1;
		}
		delay--;
	}
}
#endif

uint32_t g_keys_held, g_keys_down, g_keys_up{};

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
		UpdateGyroAccel();
		hidCircleRead(&g_circle_position);
		hidCstickRead(&g_cstick_position);
		g_slider_3d_status = osGet3DSliderState();
		HIDUSER_GetSoundVolume(&g_slider_volume_status);
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

	if(g_keys_up&KEY_LID                     // If lid just opened OR
	|| g_keys_held&KEY_TOUCH)                // Screen is touched
	{
		s_vblank_count = 0;              // Reset timer
		BacklightsOn();                  // Backlights on
	}
	else if(g_keys_down                      // A button pressed, possibly the lid OR
	|| s_vblank_count == VBLANK_MAX)         // Enough time passed
	{
		if(!force_backlights_on)         // If Backlights are NOT forced on
			BacklightsOff();         // Backlights off
	}

	if(s_vblank_count < VBLANK_MAX)
		s_vblank_count++;                // Increment timer
	if(!enable_input_timeout)                // This avoids the screen turning off after 4 seconds
		s_vblank_count = 0;
	//while(g_keys_held&KEY_LID)             // Wait here while the lid is closed
	//	WaitForVBlank();
}

///vblank function we assign in Init()
void VBlankFunction()
{
#ifdef _3DS
	ACU_GetWifiStatus(&wifi_status);
	PTMU_GetBatteryLevel(&battery_level);
	PTMU_GetBatteryChargeState(&charging_status);
#endif
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

bool Init(int argc, char* argv[])
{
	// Screen setup
#if defined(_3DS)
	gfxInitDefault();              //Graphics
	gspLcdInit();                  //Backlight
	ptmuInit();                    //Lid
//TODO: these should only be enabled when used?
	HIDUSER_EnableAccelerometer(); //Accelerometer
	HIDUSER_EnableGyroscope();     //Gyroscope
	consoleInit(GFX_TOP, nullptr);
	gfxSetDoubleBuffering(GFX_BOTTOM, false);

	//Get the bottom screen's frame buffer
	D2K::GUI::g_screen[0] = (uint16_t*)gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, nullptr, nullptr);
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

	D2K::InitLogging(argc, argv);

	if(EMULATOR)  // If we're running in emulator mode, let the user know
		LOG(DEBUG) << " - Emulator Mode";

	LOG(INFO) << "\n-";

#ifdef _NDS
	if(!fatInitDefault())
		LOG(ERROR) << "Error (fatInitDefault): Failed to access storage\n";
	
	LOG(INFO) << "Connecting via WFC data\n";
	if(EMULATOR
	|| !Wifi_InitDefault(WFC_CONNECT))
	{
		LOG(ERROR) << "Error (Wifi_InitDefault): Failed to connect\n";
		if(!EMULATOR)
			return true;                // Return with error
	}

#endif

#ifdef _3DS
	static uint32_t SOC_ALIGN = 0x1000;
	static uint32_t SOC_BUFFERSIZE = 0x100000;

	// allocate buffer for SOC service
	static uint32_t *SOC_buffer = (uint32_t*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

	if(SOC_buffer == nullptr) {
		LOG(ERROR) << "memalign: failed to allocate";
		return true;
	}

	int soc_init = socInit(SOC_buffer, SOC_BUFFERSIZE);
	// Now intialise soc:u service
	if(soc_init != 0)
	{
		LOG(ERROR) << "socInit: " << (unsigned int)soc_init;
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
	D2K::DeInitLogging();
#ifdef _3DS
//TODO: these should only be enabled when used?
	HIDUSER_DisableGyroscope();     //Gyroscope
	HIDUSER_DisableAccelerometer(); //Accelerometer

	ptmuExit();                     //Lid
	GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTH);
	gspLcdExit();
	socExit();
	gfxExit();
#endif
}

int Loop()
{
#if defined(_3DS)
//TODO: we should send a release of all keys if we knowingly disconnect
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
