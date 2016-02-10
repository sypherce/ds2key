// main - it all begins and ends here

#include "common/udp.h"
#include "windows/mainWindow.h"
#include "windows/keypadWindow.h"
#include "windows/commandWindow.h"
#include "windows/turboWindow.h"
#include "core.h"
#include "apsearch.h"
#include "config.h"

int main()
{
	if(D2K::Init())					// DS hardware setup
		return 1;				// halt on error

	D2K::GUI::Main::g_window.SetVisible(true);	// Show main window

	// TODO: We never actually get out of this loop. If we can, and go back
	// to our loader or something we should
	while(true)
	{
		D2K::Loop();				// DS hardware loop

		D2K::UDP::Update(keysHeld(),		// Update ds2key network
			    D2K::GUI::Turbo::GetKeys(),
			    guitarGripKeysHeld() * guitarGripIsInserted(),
			    D2K::GUI::Turbo::GHGetKeys() * guitarGripIsInserted(),
			    nullptr);

		D2K::GUI::Main::g_window.Update();	// Update the window
	}

	D2K::UDP::DeInit();				// Disconnect network
	D2K::Config::Save();				// Save settings

	return 0;
}
