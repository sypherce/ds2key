#pragma once

#if defined(_3DS)
#include <3ds.h>
#elif defined(_NDS)
#include <nds.h>
#include "dummy_types.h" // 3ds types: circlePosition, etc
#endif

namespace D2K {

extern uint32_t g_keys_held, g_keys_down, g_keys_up;
extern touchPosition g_stylus_position;
extern circlePosition g_circle_position;
extern circlePosition g_cstick_position;
extern accelVector g_accel_status;
extern angularRate g_gyro_status;
extern float g_slider_3d_status;
extern uint8_t g_slider_volume_status;
extern int8_t wifi_status;
const int8_t WIFI_NOT_CONNECTED = -1;
extern uint8_t battery_level;
extern uint8_t charging_status;

// return: The current time
char* GetTime();

// Wait for VBlank and process inputs, lid
void WaitForVBlank();

// Setup the nds system. Start fat, wifi, and our udp system.
// return: (0) upon success, else (1)
bool Init(int argc, char* argv[]);

// Shutdown the system. Currently only 3DS.
void DeInit();

// Loop function that calls VBlankFunction()
// return: false on system shutdown
int Loop();

// Checks if the magic key combo is held
// return: true if keys are held
bool MagicKeys();

// Force backlights on if (value) == true
void ForceBacklightsOn(bool value);

} // namespace D2K
