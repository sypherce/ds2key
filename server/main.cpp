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

#include <chrono>
namespace D2K {

void ExecuteCommand(std::string Command);
void SendClient()
{
	using namespace std::chrono;
	static  high_resolution_clock::time_point time_previous = high_resolution_clock::now();
		high_resolution_clock::time_point time_current  = high_resolution_clock::now();
	long long time_difference = duration_cast<milliseconds>(time_current - time_previous).count();
	if(time_difference >= 5000)
	{
		D2K::ExecuteCommand("C:\\devkitPro\\devkitARM\\bin\\3dslink.exe C:\\Root\\Programming\\ds2key\\master\\ds2key\\client\\ds2key.3dsx");
		time_previous = time_current;
	}
}

}

int main(int argc, char* argv[])
{
	if(D2K::Setup(argc, argv)) // Exit if we don't connect properly
	{
		return 1;
	}
#endif

		while(D2K::g_running)
		{
			//D2K::SendClient();
			D2K::Loop();
		}

#ifdef WIN32GUI
		D2K::GUI::MainWindow::TrayIcon->Delete();
	}
#endif

	return 0;
}
