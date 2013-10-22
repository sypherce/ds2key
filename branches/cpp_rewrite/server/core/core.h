#ifndef __CORE_H__
#define __CORE_H__
#include "udp.h"
#include "input.h"
#include "client.h"

namespace D2K {
	namespace Core {
		extern bool Running;
		extern uint8_t Debug;

		extern int Setup(int argc, char *argv[]);
		extern void Loop();
		extern void Print(int Level, std::string Message);
	}
}
#endif//__CORE_H__

