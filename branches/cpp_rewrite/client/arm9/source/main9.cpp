/*
	main - it all begins and ends here
*/
#include "ds2key.h"
#include "mainWindow.h"
#include "keypadWindow.h"
#include "system.h"

using namespace D2K;

int main() {
	System::Setup();
	DS2Key::Init();
	GUI::mainWindow = new GUI::MainWindow();
	GUI::keypadWindow = new GUI::KeypadWindow();
	//GUI::MainWindow::Setup();
	GUI::mainWindow->setVisible(true);
	//GUI::MainWindow::setVisible(true);

	while(true) {
		System::Update(true);

		DS2Key::Update(keysHeld(), guitarGripKeysHeld() * guitarGripIsInserted(), (touchPosition*)NULL);

		GUI::mainWindow->Update();
		//GUI::MainWindow::Update();
	}
	DS2Key::DeInit();

	return 0;
}
