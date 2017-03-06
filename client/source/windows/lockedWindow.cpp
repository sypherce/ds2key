#include <string> // std::string

// windows
#include "lockedWindow.h"
#include "mainWindow.h"
#include "barWindow.h"

// controls
#include "gui/label.h"

// core
#include "core.h" // g_keys_down

namespace D2K {namespace GUI {namespace Locked {

WindowClass g_window;
Label* label_1;

WindowClass::WindowClass() : Window()
{
	AppendObject(label_1 = new Label(m_screen, Rect(2, 2, 61, 42), "Press L+R+START+SELECT to unlock screen"));
}
WindowClass::~WindowClass() { }
void WindowClass::SetVisible(bool visible)
{
	Window::SetVisible(visible);

	if(!visible)
		return;

	Bar::SetText("[Locked]");
	Bar::g_window.SetVisible(true);
	
	ForceBacklightsOn(false); // Let backlights timeout
}
bool WindowClass::Update()
{
	
	if(MagicKeys())
	{
		while(D2K::Loop() 
		   && MagicKeys())
		{
			// Update window while held
			Window::Update();
			Bar::g_window.Update();
		}

		Locked::g_window.SetVisible(false); // Hide main window
		Main::g_window.SetVisible(true);

		while(D2K::Loop() // Loop while Turbo window is visible
		   && Main::g_window.IsVisible())
		{
			// Update window, also process networking code
			Main::g_window.Update();
		}

		Locked::g_window.SetVisible(true); // Show main window
	}

	return Window::Update()
	    || Bar::g_window.Update();
}

}}} // namespace D2K::GUI::Command
