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
	if(D2K::Init())								//DS hardware setup
		return 1;									//halt on error

	//setup our windows
	Keypad::Window = new Keypad::WindowClass();
	Command::Window = new Command::WindowClass();
	Turbo::Window = new Turbo::WindowClass();
	Main::Window = new Main::WindowClass();
		Main::Window->SetVisible(true);				//show main window

	while(true)
	{
		D2K::Loop();								//DS hardware loop

		UDP::Update(keysHeld(),						//update ds2key network
			Turbo::GetKeys(),
			guitarGripKeysHeld() * guitarGripIsInserted(),
			Turbo::GHGetKeys() * guitarGripIsInserted(),
			nullptr);

		Main::Window->Update();						//update the window
	}

	//delete windows
	delete Turbo::Window;
	delete Command::Window;
	delete Keypad::Window;
	delete Main::Window;

	UDP::DeInit();	//disconnect network
	Config::Save();	//save settings

	return 0;
}
