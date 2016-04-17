// main - it all begins and ends here

#include "common/udp.h"
#include "windows/mainWindow.h"
#include "windows/keypadWindow.h"
#include "windows/commandWindow.h"
#include "windows/turboWindow.h"
#include "apsearch.h"
#include "core.h"
#include "config.h"

#ifdef _3DS
#include "gui.h"//this has guitarGrip stuff in it for now
#endif

int main()
{
	if(D2K::Init())					// DS hardware setup
	{
		D2K::DeInit();

		return 1;				// halt on error
	}
	D2K::GUI::Main::g_window.SetVisible(true);	// Show main window

#if defined(_3DS)
	while(aptMainLoop())
#elif defined(_NDS)
	// TODO: We never actually get out of this loop. If we can, and go back
	// to our loader or something we should
	while(true)
#endif
	{
		if(D2K::Loop() != 0)			// DS hardware loop
			break;				
		D2K::UDP::Update(keysHeld(),		// Update ds2key network
			    D2K::GUI::Turbo::GetKeys(),
			    guitarGripKeysHeld() * guitarGripIsInserted(),
			    D2K::GUI::Turbo::GHGetKeys() * guitarGripIsInserted(),
			    nullptr);

		D2K::GUI::Main::g_window.Update();	// Update the window

	}

	D2K::UDP::DeInit();				// Disconnect network
	D2K::Config::Save();				// Save settings
	D2K::DeInit();

	return 0;
}
