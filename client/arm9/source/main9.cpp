//main - it all begins and ends here

#include "common/udp.h"
#include "windows/mainWindow.h"
#include "windows/keypadWindow.h"
#include "windows/commandWindow.h"
#include "windows/turboWindow.h"
#include "core.h"
#include "apsearch.h"
#include "config.h"

using namespace D2K;
using namespace D2K::GUI;

int main()
{
	if(D2K::Init())							//DS hardware setup
		return 1;						//halt on error

	//setup our windows
	Keypad::g_window = new Keypad::WindowClass();
	Command::g_window = new Command::WindowClass();
	Turbo::g_window = new Turbo::WindowClass();
	Main::g_window = new Main::WindowClass();
	Main::g_window->SetVisible(true);				//show main window

	while(true)
	{
		D2K::Loop();						//DS hardware loop

		UDP::Update(keysHeld(),					//update ds2key network
			    Turbo::GetKeys(),
			    guitarGripKeysHeld() * guitarGripIsInserted(),
			    Turbo::GHGetKeys() * guitarGripIsInserted(),
			    nullptr);

		Main::g_window->Update();				//update the window
	}

	//delete windows
	delete Turbo::g_window;
	delete Command::g_window;
	delete Keypad::g_window;
	delete Main::g_window;

	UDP::DeInit();							//disconnect network
	Config::Save();							//save settings

	return 0;
}
