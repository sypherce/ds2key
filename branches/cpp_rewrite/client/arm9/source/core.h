#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <nds.h>//touchPosition

namespace D2K {
	namespace Core {
		extern touchPosition StylusPos;
		extern bool InputChange;

		//@return The current time
		extern char* GetTime();
		
		//Setup the nds system. Start fat, wifi, and our udp system.
        //@return (0) upon success, else (1)
		extern bool Setup();
		
		//Loop function that calls VBLFunction()
		extern void Loop();
	}
}

#endif//__SYSTEM_H__
