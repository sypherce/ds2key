#pragma once

#if defined(_3DS)
#include <3ds/types.h>
#include <3ds/services/hid.h>// touchPosition
#elif defined(_NDS)
#include <nds/ndstypes.h>
#include <nds/input.h>
#include <nds/arm9/guitarGrip.h>
#include <nds/touch.h>// touchPosition
#endif

namespace D2K {

extern uint32_t g_keys_held, g_keys_down, g_keys_up;
extern touchPosition g_stylus_position;

// @return The current time
extern char* GetTime();

// Wait for VBlank and process inputs, lid
void WaitForVBlank();

// Setup the nds system. Start fat, wifi, and our udp system.
// @return (0) upon success, else (1)
extern bool Init();

// Shutdown the system. Currently only 3DS.
extern void DeInit();

// Loop function that calls VBlankFunction()
// @return false on system shutdown
extern int Loop();

}//namespace D2K
