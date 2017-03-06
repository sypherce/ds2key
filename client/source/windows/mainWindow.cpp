#include <string> // std::string

// windows
#include "mainWindow.h"
#include "barWindow.h"
#include "keypadWindow.h"
#include "keyboardWindow.h"
#include "turboWindow.h"
#include "configWindow.h"
#include "commandWindow.h"
#include "connectionWindow.h"
#include "lockedWindow.h"
#include "settingsWindow.h"
#include "touchWindow.h"

// controls
#include "gui/gui.h"
#include "gui/edit.h"
#include "gui/button.h"
#include "gui/label.h"
#include "gui/letterButton.h"
#include "gui/canvas.h"

// core
#include "core.h"
#include "common/udp.h"
#include "common/misc.h"
#include "config.h"

namespace D2K {namespace GUI {namespace Main {

const int LENGTH_IP = 15;
const int LENGTH_PORT = 5;
const int LENGTH_PROFILE = 3;

extern void EditIPFunction();
extern void EditPortFunction();
extern void EditProfileFunction();
extern void ButtonTouchFunction();
extern void ButtonTurboFunction();
extern void ButtonConfigKeyboardWindowFunction();
extern void ButtonConfigGamepadWindowFunction();
extern void ButtonIPFunction();
extern void ButtonConnectionFunction();
extern void ButtonSettingsFunction();
extern void ButtonCommandsFunction();
extern void LockScreen();

WindowClass g_window;

Label* label_ip;
Label* label_unlock;
Label* label_port;
Label* label_profile;

Edit* edit_ip;
Edit* edit_port;
Edit* edit_profile;

LetterButton* button_touch;
Button* button_ip;
LetterButton* button_settings;
LetterButton* button_commands;

WindowClass::WindowClass() : Window()
{
	Color[COLOR_BACKGROUND]            = ARGB16(1, 19, 22, 25);

	Color[COLOR_LABEL_TEXT]            = ARGB16(1, 0, 0, 0);
	Color[COLOR_LABEL_BACKGROUND]      = Color[COLOR_BACKGROUND];

	Color[COLOR_BUTTON_TEXT]           = Color[COLOR_LABEL_TEXT];
	Color[COLOR_BUTTON_BACKGROUND]     = ARGB16(1, 30, 30, 30);
	Color[COLOR_BUTTON_OUTLINE]        = Color[COLOR_BUTTON_TEXT];
	Color[COLOR_BUTTON_OUTLINE_ACTIVE] = ARGB16(1, 31, 0, 0);

	Color[COLOR_EDIT_TEXT]             = Color[COLOR_LABEL_TEXT];
	Color[COLOR_EDIT_BACKGROUND]       = Color[COLOR_BUTTON_BACKGROUND];
	Color[COLOR_EDIT_OUTLINE]          = Color[COLOR_EDIT_TEXT];
	Color[COLOR_EDIT_OUTLINE_ACTIVE]   = Color[COLOR_BUTTON_OUTLINE_ACTIVE];

	m_screen = 0;
	{ // Hidden
	AppendObject(label_ip              = new Label(m_screen, Rect(64,24+3,10,10), "IP:"));
	AppendObject(label_port            = new Label(m_screen, Rect(64,48+3,10,10), "Port:"));
	AppendObject(label_profile         = new Label(m_screen, Rect(64,72+3,10,10), "Profile:"));

	AppendObject(edit_ip               = new Edit(m_screen, Rect(96,24,95,10), UDP::GetRemoteIPString(), &EditIPFunction));
	AppendObject(edit_port             = new Edit(m_screen, Rect(112,48,79,10), UDP::GetPortString(), &EditPortFunction));
	AppendObject(edit_profile          = new Edit(m_screen, Rect(136,72,55,10), UDP::GetProfileString(), &EditProfileFunction));
	AppendObject(button_ip             = new Button(m_screen, Rect(204,24,35,10), "Find IP", &ButtonIPFunction));
	}

	AppendObject(label_unlock          = new Label(m_screen, Rect(  2, 2, 61, 12), "Press L+R+START+SELECT again to lock screen"));
	
	int button_width = 82;

	AppendObject(button_settings       = new LetterButton(m_screen, Rect(                     2, 8, button_width, 42), "A", "Settings", "", &ButtonSettingsFunction));
	AppendObject(button_touch          = new LetterButton(m_screen, Rect( button_width      + 4, 8, button_width + 1, 42), "B", "Touch", "", &ButtonTouchFunction));
	AppendObject(button_commands       = new LetterButton(m_screen, Rect((button_width) * 2 + 6 + 1, 8, button_width, 42), "X", "Commands", "", &ButtonCommandsFunction));

	Bar::g_window.SetVisible(true);
}
WindowClass::~WindowClass() { }

void WindowClass::SetVisible(bool visible)
{
	Window::SetVisible(visible);

	if(!visible)
		return;

	label_ip->SetVisible(false);
	label_port->SetVisible(false);
	label_profile->SetVisible(false);

	edit_ip->SetVisible(false);
	edit_port->SetVisible(false);
	edit_profile->SetVisible(false);

	button_ip->SetVisible(false);

	Bar::SetText("[" + D2K::VERSION_STRING + "]");
	Bar::g_window.SetVisible(true);
	
	ForceBacklightsOn(false); // Let backlights timeout
}

bool WindowClass::Update()
{
	static std::string ip{};
	static std::string port{};
	static std::string profile{};

	if(MagicKeys())
	{
		while(D2K::Loop() 
	           && MagicKeys())
		{
			// Update window while held
			Window::Update();
			Bar::g_window.Update();
		}
		LockScreen(); // Lock Screen

// TODO: this return value needs inspected
		return 0;
	}

	static bool first_loop = true;
	if(first_loop)
	{
		first_loop = false;
		// Search for server on start
		ButtonIPFunction();
	}
		
	if(edit_ip->IsVisible())
	{
		std::string temp_ip = UDP::GetRemoteIPString();
		if(ip != temp_ip)
		{
			ip = temp_ip;
			edit_ip->SetText(ip);
		}
	}
	if(edit_port->IsVisible())
	{
		std::string temp_port = UDP::GetPortString();
		if(port != temp_port)
		{
			port = temp_port;
			edit_port->SetText(port);
		}
	}
	if(edit_profile->IsVisible())
	{
		std::string temp_profile = UDP::GetProfileString();
		if(profile != temp_profile)
		{
			profile = temp_profile;
			edit_profile->SetText(profile);
		}
	}

	return Window::Update()
	    || Bar::g_window.Update();
}

void EditIPFunction()
{
	std::string entry = Keypad::Entry(label_ip, edit_ip, UDP::GetRemoteIPString(), LENGTH_IP);
	UDP::SetRemoteIP(entry);
	edit_ip->SetText(UDP::GetRemoteIPString());
	Config::Save();
	UDP::Connect();
}
void EditPortFunction()
{
	std::string entry = Keypad::Entry(label_port, edit_port, UDP::GetPortString(), LENGTH_PORT);
	UDP::SetConfigPort(entry);
	edit_port->SetText(UDP::GetPortString());
	Config::Save();
	UDP::Connect();
}
void EditProfileFunction()
{
	std::string entry = Keypad::Entry(label_profile, edit_profile, UDP::GetProfileString(), LENGTH_PROFILE);
	UDP::SetProfile(entry);
	edit_profile->SetText(UDP::GetProfileString());
	Config::Save();
	UDP::Connect();
}

void ButtonIPFunction()
{
	UDP::ServerLookup();
	edit_ip->SetText(UDP::GetRemoteIPString());
}
void ButtonTouchFunction()
{
	Main::g_window.SetVisible(false); // Hide main window
	Touch::g_window.SetVisible(true);

	while(D2K::Loop() // Loop while Turbo window is visible
	   && Touch::g_window.IsVisible())
	{
		// Update window, also process networking code
		Touch::g_window.Update();
	}

	Main::g_window.SetVisible(true); // Show main window
}

void ButtonTurboFunction()
{
	Main::g_window.SetVisible(false); // Hide main window
	Turbo::g_window.SetVisible(true);

	while(D2K::Loop() // Loop while Turbo window is visible
	   && Turbo::g_window.IsVisible())
	{
		// Update window, also process networking code
		Turbo::g_window.Update();
	}

	Main::g_window.SetVisible(true); // Show main window
}
void ButtonConnectionFunction()
{
	Main::g_window.SetVisible(false);      // Hide main window
	Connection::g_window.SetVisible(true); // Show connection window
	
	while(D2K::Loop() // Loop while Connection window is visible
	   && Connection::g_window.IsVisible())
	{
		// Update window, DON'T process networking code
		Connection::g_window.Update();
	}

	Main::g_window.SetVisible(true);       // Show main window

}
void ButtonSettingsFunction()
{
	Main::g_window.SetVisible(false);    // Hide main window
	Settings::g_window.SetVisible(true); // Show Settings window

	while(D2K::Loop() // Loop while Settings window is visible
	   && Settings::g_window.IsVisible())
	{
		// Update window, DON'T process networking code
		Settings::g_window.Update();
	}

	Main::g_window.SetVisible(true);     // Show main window

}
void ButtonCommandsFunction()
{
	Main::g_window.SetVisible(false);   // Hide main window
	Command::g_window.SetVisible(true); // Show command window

	while(D2K::Loop() // Loop while Command window is visible
	   && Command::g_window.IsVisible())
	{
		// Update window, also process networking code
		Command::g_window.Update();
	}

	Main::g_window.SetVisible(true);    // Show main window
}
void ButtonConfigWindowFunction(bool type)
{
	Main::g_window.SetVisible(false);   // Hide main window
	ConfigWindow::g_config_type = type; // true = gamepad, false = keyboard
	ConfigWindow::g_window.SetVisible(true);

	while(D2K::Loop() // Loop while Config window is visible
	   && ConfigWindow::g_window.IsVisible())
	{
		// Update window, also process networking code
		ConfigWindow::g_window.Update();
	}

	Main::g_window.SetVisible(true);    // Show main window
}
void ButtonConfigGamepadWindowFunction()
{
	ButtonConfigWindowFunction(true);      // true = gamepad
}
void ButtonConfigKeyboardWindowFunction()
{
	ButtonConfigWindowFunction(false);      // false = keyboard
}

void LockScreen()
{
	Main::g_window.SetVisible(false);  // Hide main window
	Locked::g_window.SetVisible(true); // Show locked window
}

}}} // namespace D2K::GUI::Main
