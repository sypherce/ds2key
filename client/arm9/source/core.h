#pragma once

#if defined(_3DS)
#include <3ds.h>// touchPosition
#elif defined(_NDS)
#include <nds.h>// touchPosition
#endif

namespace D2K {

extern touchPosition g_stylus_position;

// @return The current time
extern char* GetTime();

// Setup the nds system. Start fat, wifi, and our udp system.
// @return (0) upon success, else (1)
extern bool Init();

// Shutdown the system. Currently only 3DS.
extern void DeInit();

// Loop function that calls VBlankFunction()
extern int Loop();

}//namespace D2K
