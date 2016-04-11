// main - it all begins and ends here

#include "common/udp.h"
#ifdef _NDS
#include "windows/mainWindow.h"
#include "windows/keypadWindow.h"
#include "windows/commandWindow.h"
#include "windows/turboWindow.h"
#include "apsearch.h"
#endif
#include "core.h"
#include "config.h"

#ifdef _3DS
#include <stdio.h>
void displayKeys()
{
    static char keysNames[32][32] = {
        "KEY_A", "KEY_B", "KEY_SELECT", "KEY_START",
        "KEY_DRIGHT", "KEY_DLEFT", "KEY_DUP", "KEY_DDOWN",
        "KEY_R", "KEY_L", "KEY_X", "KEY_Y",
        "", "", "KEY_ZL", "KEY_ZR",
        "", "", "", "",
        "KEY_TOUCH", "", "", "",
        "KEY_CSTICK_RIGHT", "KEY_CSTICK_LEFT", "KEY_CSTICK_UP", "KEY_CSTICK_DOWN",
        "KEY_CPAD_RIGHT", "KEY_CPAD_LEFT", "KEY_CPAD_UP", "KEY_CPAD_DOWN"
    };
	static u32 kDownOld = 0;
	static u32 kHeldOld = 0;
	static u32 kUpOld = 0;

	u32 kDown = hidKeysDown();
	u32 kHeld = hidKeysHeld();
	u32 kUp = hidKeysUp();
	if (kDown != kDownOld || kHeld != kHeldOld || kUp != kUpOld)
	{
		consoleClear();

		printf("\x1b[3;0H");
		for(int i = 0; i < 32; i++)
		{
			if (kDown & BIT(i)) printf("%s down\n", keysNames[i]);
			if (kHeld & BIT(i)) printf("%s held\n", keysNames[i]);
			if (kUp & BIT(i)) printf("%s up\n", keysNames[i]);
		}
		kDownOld = kDown;
		kHeldOld = kHeld;
		kUpOld = kUp;
	}
	circlePosition pos;
	hidCircleRead(&pos);
	printf("\x1b[2;0H%04d; %04d", pos.dx, pos.dy);
	printf("\x1b[8;0HHold L + R + START + SELECT to exit");
}
#endif

int main()
{
	if(D2K::Init())					// DS hardware setup
	{
#ifdef _3DS
		socExit();
		gfxExit();
#endif

		return 1;				// halt on error
	}
#ifdef _NDS
	D2K::GUI::Main::g_window.SetVisible(true);	// Show main window
#endif

	// TODO: We never actually get out of this loop. If we can, and go back
	// to our loader or something we should
	while(true)
	{
		D2K::Loop();				// DS hardware loop
#ifdef _3DS
		displayKeys();
#endif
#if defined(_3DS)
		D2K::UDP::Update(keysHeld(),		// Update ds2key network
			    0,
			    0,
			    0,
			    nullptr);
				if(
				(keysHeld()&KEY_START)&&
				(keysHeld()&KEY_SELECT)&&
				(keysHeld()&KEY_L)&&
				(keysHeld()&KEY_R)
				) break;
#elif defined(_NDS)
		D2K::UDP::Update(keysHeld(),		// Update ds2key network
			    D2K::GUI::Turbo::GetKeys(),
			    guitarGripKeysHeld() * guitarGripIsInserted(),
			    D2K::GUI::Turbo::GHGetKeys() * guitarGripIsInserted(),
			    nullptr);

		D2K::GUI::Main::g_window.Update();	// Update the window
#endif

	}

	D2K::UDP::DeInit();				// Disconnect network
	D2K::Config::Save();				// Save settings
#ifdef _3DS
	socExit();
	gfxExit();
#endif

	return 0;
}
