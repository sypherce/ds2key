#pragma once

#include <nds.h>//touchPosition

namespace D2K {

extern touchPosition StylusPos;
extern bool InputChange;

//@return The current time
extern char* GetTime();

//Setup the nds system. Start fat, wifi, and our udp system.
//@return (0) upon success, else (1)
extern bool Init();

//Loop function that calls VBLFunction()
extern void Loop();

}//namespace D2K
