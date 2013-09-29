#ifndef __CORE_H__
#define __CORE_H__
#ifdef WIN32GUI
#include "gui/gui.h"
#endif//GUI
#include "udp.h"
#include "input.h"
#include "client.h"

namespace D2K {
	namespace Core {
		extern void _processPackets(Core::Client *Client, Input *input);
		extern Config *config;
		extern UDP *udp;
		extern Input *input;
		extern Client *ClientArray[256];
		extern bool running;
		extern bool console;
		extern uint16_t port;
		extern bool block;
		extern int debug;
		extern int Setup(int argc, char *argv[]);
		extern void loop();
		extern void Print(int Level, std::string Message);
	}
}
#endif//__CORE_H__

