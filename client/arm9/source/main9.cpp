/*
	main - it all begins and ends here
*/
#include "ds2key.h"
#include "mainWindow.h"
#include "keypadWindow.h"
#include "commandWindow.h"
#include "turboWindow.h"
#include "system.h"
#include "apsearch.h"

using namespace D2K;
using namespace D2K::GUI;

int main() {
	System::Setup();

	if(false) {//AP test
		AP::Init();
		for(int i = 0; i < AP::count; i++)
			AP::Print(i);
	}

	DS2Key::Init();
	Main::Window = new Main::WindowClass();
	Keypad::Window = new Keypad::WindowClass();
	Command::Window = new Command::WindowClass();
	Turbo::Window = new Turbo::WindowClass();

	Main::Window->setVisible(true);
	while(true) {
		System::Update(true);

		DS2Key::Update(keysHeld(), Turbo::GetKeys(), guitarGripKeysHeld() * guitarGripIsInserted(), 0 * guitarGripIsInserted(), (touchPosition*)NULL);

		Main::Window->Update();
	}

	DS2Key::DeInit();

	return 0;
}
