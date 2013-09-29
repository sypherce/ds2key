#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <nds/ndstypes.h>

namespace D2K {
	namespace System {
		const int LENGTH_IP = 15;
		const int LENGTH_PORT = 5;
		const int LENGTH_PROFILE = 3;

		extern touchPosition stylusPos;
		extern bool inputChange;
		extern uint32 vblCount;
		extern uint32 vblLid;

		extern char* getTime();
		extern void updateInputs();
		extern void updateLid();
		extern void vblfunction();
		extern void resetStylus();
		extern bool isScreenOn();
		extern void Setup();
		extern void Update(bool change);
	}
}

#endif//__SYSTEM_H__
