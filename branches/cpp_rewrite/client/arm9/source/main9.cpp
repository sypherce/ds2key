/*
	main - it all begins and ends here
*/
#include "udp.h"
#include "windows/mainWindow.h"
#include "windows/keypadWindow.h"
#include "windows/commandWindow.h"
#include "windows/turboWindow.h"
#include "core.h"
#include "apsearch.h"
#include "config.h"

using namespace D2K;
using namespace D2K::GUI;

int main() {
	if(Core::Setup())								//DS hardware setup
		return 1;

	//setup our windows
	Keypad::Window = new Keypad::WindowClass();
	Command::Window = new Command::WindowClass();
	Turbo::Window = new Turbo::WindowClass();
	Main::Window = new Main::WindowClass();
		Main::Window->SetVisible(true);				//show main window

	while(true) {
		Core::Loop();								//DS hardware loop

		Core::UDP->Update(keysHeld(),				//update ds2key network
			Turbo::GetKeys(),
			guitarGripKeysHeld() * guitarGripIsInserted(),
			0 * guitarGripIsInserted(),				//guitar grip is disabled
			(touchPosition*)NULL);

		Main::Window->Update();						//update the window
	}

	//delete windows
	delete Turbo::Window;
	delete Command::Window;
	delete Keypad::Window;
	delete Main::Window;

	delete Core::UDP;		//disconnect network
	delete Core::Config;	//save settings

	return 0;
}
