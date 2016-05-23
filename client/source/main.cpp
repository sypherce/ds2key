// main - it all begins and ends here

#include "common/udp.h"
#include "windows/mainWindow.h"
#include "windows/keypadWindow.h"
#include "windows/commandWindow.h"
#include "windows/turboWindow.h"
#include "core.h"
#include "config.h"

#ifdef _3DS
#include "gui.h" // this has guitarGrip stuff in it for now
#endif

int main()
{
	if(D2K::Init() && !EMULATOR)               // DS hardware setup
	{
		D2K::DeInit();

		return 1;                          // halt on error
	}
	D2K::GUI::Main::g_window.SetVisible(true); // Show main window

	while(D2K::Loop())                         // DS hardware loop
	{
		D2K::UDP::Update(D2K::g_keys_held, // Update ds2key network
			    D2K::GUI::Turbo::GetKeys(),
			    nullptr,
			    NULL_VALUE);

		D2K::GUI::Main::g_window.Update(); // Update the window
	}

	D2K::UDP::DeInit();                        // Disconnect network
	D2K::Config::Save();                       // Save settings
	D2K::DeInit();

	return 0;
}
