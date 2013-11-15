/*
	main file
*/

#ifdef WIN32GUI
#include <windows.h>
#include "gui/mainWindow.h"
#include "core/core.h"

void D2K::Core::Print(int Level, string Message) {
	if(D2K::Core::Debug <= Level && Level > D2K::Core::dNone)
		if(D2K::GUI::MainWindow::Log::listView)
			D2K::GUI::MainWindow::Log::listView->Append(Message);
}

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow) {
	D2K::Core::Setup(0, (char**)"");
	D2K::GUI::Setup();
	if(D2K::GUI::MainWindow::Setup(hThisInstance, nCmdShow) == 0) {
		D2K::GUI::MainWindow::Profile::checkButton1->SetVisible(false);//disabled until implemented
#else//console mode
#include "core/core.h"

int main(int argc, char *argv[]) {
		D2K::Core::Setup(argc, argv);
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
