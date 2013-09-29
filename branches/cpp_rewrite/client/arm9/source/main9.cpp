/*
	main - it all begins and ends here
*/
#include "ds2key.h"
//#include "mainWindow.h"
#include "mainWindow2.h"
#include "system.h"

using namespace D2K;

int main() {
	System::Setup();
	DS2Key::Init();
	GUI::MainWindow2 *window = new GUI::MainWindow2();
	//GUI::MainWindow::Setup();
	window->setVisible(true);
	//GUI::MainWindow::setVisible(true);

	while(true) {
		System::Update(true);

		DS2Key::Update(keysHeld(), guitarGripKeysHeld() * guitarGripIsInserted(), (touchPosition*)NULL);

		window->Update();
		//GUI::MainWindow::Update();
	}
	DS2Key::DeInit();

	return 0;
}
