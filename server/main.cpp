// main file

#include "core/core.h"
#ifdef WIN32GUI
#include <windows.h>
#include "gui/mainWindow.h"
#include "common/misc.h"

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
	D2K::Setup(0, (char**)D2K::VERSION_STRING.c_str());
	D2K::GUI::Setup();
	if(D2K::GUI::MainWindow::Setup(hThisInstance, nCmdShow) == 0)
	{
		D2K::GUI::MainWindow::Profile::checkButton1->SetVisible(false); // Disabled until implemented
#elif defined(_WIN32) || defined(__linux__)

int main(int argc, char* argv[])
{
	if(D2K::Setup(argc, argv)) // Exit if we don't connect properly
	{
		return 1;
	}
#endif

		while(D2K::g_running)
		{
			D2K::Loop();
		}

#ifdef WIN32GUI
		D2K::GUI::MainWindow::TrayIcon->Delete();
	}
#endif

	return 0;
}
