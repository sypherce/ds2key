// main - it all begins and ends here

// windows
#include "windows/lockedWindow.h"
#include "windows/turboWindow.h" // D2K::GUI::Turbo::GetKeys()

// core
#include "core.h"
#include "common/udp.h"
#include "config.h"

#ifdef _3DS
#include "gui.h" // this has guitarGrip stuff in it for now
#endif

int main(int argc, char* argv[])
{
	if(D2K::Init(argc, argv) && !EMULATOR)       // DS hardware setup
	{
		D2K::DeInit();

		return 1;                            // halt on error
	}
	D2K::GUI::Locked::g_window.SetVisible(true); // Show locked window

	while(D2K::Loop())                           // DS hardware loop
	{
		D2K::UDP::Update(D2K::g_keys_held,   // Update ds2key network
		                 D2K::GUI::Turbo::GetKeys(),
		                 nullptr,            // No touch position
		                 &D2K::g_circle_position,
		                 &D2K::g_cstick_position,
		                 &D2K::g_accel_status,
		                 &D2K::g_gyro_status,
		                 &D2K::g_slider_volume_status,
		                 &D2K::g_slider_3d_status,
		                 0);                 // No keyboard press

		D2K::GUI::Locked::g_window.Update(); // Update the window
	}

	D2K::UDP::DeInit();                          // Disconnect network
	D2K::Config::Save();                         // Save settings
	D2K::DeInit();

	return 0;
}
