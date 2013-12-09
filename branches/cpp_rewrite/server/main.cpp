/*
	main file
*/

#include "core/core.h"
#ifdef WIN32GUI
#include <windows.h>
#include "gui/mainWindow.h"
#include "common/misc.h"

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow) {
	D2K::Core::Setup(0, (char**)D2K::Core::VERSION_STRING.c_str());
	D2K::GUI::Setup();
	if(D2K::GUI::MainWindow::Setup(hThisInstance, nCmdShow) == 0) {
		D2K::GUI::MainWindow::Profile::checkButton1->SetVisible(false); //disabled until implemented
#else//console mode

int main(int argc, char *argv[]) {
		if(D2K::Core::Setup(argc, argv) )                                //exit if we don't connect properly
            return 1;
#endif//WIN32GUI
		while(D2K::Core::Running) {
			D2K::Core::Loop();
		}
#ifdef WIN32GUI
		D2K::GUI::MainWindow::TrayIcon->Delete();
	}
#endif//WIN32GUI

	return 0;
}
