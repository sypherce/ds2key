#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stdint.h>

namespace D2K {
	namespace System {
		const int LENGTH_IP = 15;
		const int LENGTH_PORT = 5;
		const int LENGTH_PROFILE = 3;

		extern touchPosition stylusPos;
		extern bool inputChange;

		extern char* getTime();
		//extern bool isScreenOn();
		extern void Setup();
		extern void Update(bool change);
	}
}

#endif//__SYSTEM_H__
