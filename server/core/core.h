#pragma once

#include "common/udp.h"
#include "input.h"
#include "client.h"

namespace D2K
{
extern bool g_running;

int Setup(int argc, char* argv[]);//Sets up everything and runs UDP::Connect
void Loop();//Receives and processes data packets. Also updates GUI in Windows.
}
