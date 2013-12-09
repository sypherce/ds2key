#ifndef __CORE_H__
#define __CORE_H__

#include "common/udp.h"
#include "input.h"
#include "client.h"

namespace D2K {
	namespace Core {
		extern bool Running;

		extern int Setup(int argc, char *argv[]);//Sets up everything and runs UDP->Connect
		extern void Loop();//Receives and processes data packets. Also updates GUI in Windows.
	}
}
#endif//__CORE_H__
