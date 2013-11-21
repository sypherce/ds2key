#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <nds.h>//touchPosition

namespace D2K {
	namespace Core {
		extern touchPosition StylusPos;
		extern bool InputChange;

		extern char* GetTime();
		extern bool Setup();
		extern void Loop();
	}
}

#endif//__SYSTEM_H__
