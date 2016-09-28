#include <string> // std::string

// windows
#include "mainWindow.h"
#include "keypadWindow.h"
#include "keyboardWindow.h"
#include "turboWindow.h"
#include "configWindow.h"
#include "commandWindow.h"
#include "connectionWindow.h"

// controls
#include "gui/gui.h"
#include "gui/edit.h"
#include "gui/button.h"
#include "gui/label.h"

// core
#include "common/udp.h"
#include "common/misc.h"
#include "config.h"
#include "core.h"

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
extern void ButtonCommandsFunction();

WindowClass g_window;

Label* label_title;
Label* label_clock;
Label* label_ip;
Label* label_port;
Label* label_profile;

Edit* edit_ip;
Edit* edit_port;
Edit* edit_profile;

Button* button_touch;
Button* button_turbo;
Button* button_config_keyboard;
Button* button_config_gamepad;
Button* button_ip;
Button* button_connection;
Button* button_commands;

WindowClass::WindowClass() : Window()
{
	Color[COLOR_BACKGROUND]            = ARGB16(1, 19, 22, 25);

	Color[COLOR_LABEL_TEXT]            = ARGB16(1, 0, 0, 0);
	Color[COLOR_LABEL_BACKGROUND]      = Color[COLOR_BACKGROUND];

	Color[COLOR_BUTTON_TEXT]           = Color[COLOR_LABEL_TEXT];
	Color[COLOR_BUTTON_BACKGROUND]     = ARGB16(1, 31, 31, 31);
	Color[COLOR_BUTTON_OUTLINE]        = Color[COLOR_BUTTON_TEXT];
	Color[COLOR_BUTTON_OUTLINE_ACTIVE] = ARGB16(1, 31, 0, 0);

	Color[COLOR_EDIT_TEXT]             = Color[COLOR_LABEL_TEXT];
	Color[COLOR_EDIT_BACKGROUND]       = Color[COLOR_BUTTON_BACKGROUND];
	Color[COLOR_EDIT_OUTLINE]          = Color[COLOR_EDIT_TEXT];
	Color[COLOR_EDIT_OUTLINE_ACTIVE]   = Color[COLOR_BUTTON_OUTLINE_ACTIVE];

	m_screen = 0;

	AppendObject(label_title           = new Label(m_screen, Rect(24,0+3,128,10), D2K::VERSION_STRING));
	AppendObject(label_clock           = new Label(m_screen, Rect(160,0+3,70,10), D2K::GetTime()));
	AppendObject(button_connection     = new Button(m_screen, Rect(0,24+3,10,10), "Connection", &ButtonConnectionFunction));
	AppendObject(label_ip              = new Label(m_screen, Rect(64,24+3,10,10), "IP:"));
	AppendObject(label_port            = new Label(m_screen, Rect(64,48+3,10,10), "Port:"));
	AppendObject(label_profile         = new Label(m_screen, Rect(64,72+3,10,10), "Profile:"));

	AppendObject(edit_ip               = new Edit(m_screen, Rect(96,24,95,10), UDP::GetRemoteIPString(), &EditIPFunction));
	AppendObject(button_ip             = new Button(m_screen, Rect(204,24,35,10), "Find IP", &ButtonIPFunction));
	AppendObject(edit_port             = new Edit(m_screen, Rect(112,48,79,10), UDP::GetPortString(), &EditPortFunction));
	AppendObject(edit_profile          = new Edit(m_screen, Rect(136,72,55,10), UDP::GetProfileString(), &EditProfileFunction));
	AppendObject(button_turbo          = new Button(m_screen, Rect(0,177,35,10), "Turbo", &ButtonTurboFunction));
	AppendObject(button_config_gamepad = new Button(m_screen, Rect(0,137,35,10), "ConfigG", &ButtonConfigGamepadWindowFunction));
	AppendObject(button_config_keyboard= new Button(m_screen, Rect(0,157,35,10), "ConfigK", &ButtonConfigKeyboardWindowFunction));
	AppendObject(button_commands       = new Button(m_screen, Rect(95,177,35,10), "Commands", &ButtonCommandsFunction));
	AppendObject(button_touch          = new Button(m_screen, Rect(217,177,35,10), "Touch", &ButtonTouchFunction));

}
WindowClass::~WindowClass() { }
bool WindowClass::Update()
{
	static char seconds = 0;
	static std::string ip{};
	static std::string port{};
	static std::string profile{};
	static int update_counter = 0;

//TODO: this delay's for the clock?
	const int UPDATE_COUNTER_MAX = 30;

	ForceBacklightsOn(false); // Let backlights timeout

	if(update_counter >= UPDATE_COUNTER_MAX)
	{
		static bool first_loop = true;
		if(first_loop)
		{
			first_loop = false;
			// Search for server on start
			ButtonIPFunction();
		}
		update_counter = 0;
		
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
		if(label_clock->IsVisible())
		{
			char* timePointer = D2K::GetTime();
			if(seconds != timePointer[7])  // If seconds differ
			{
				seconds = timePointer[7];
				label_clock->SetText(timePointer);
			}
		}
	}
	update_counter++;

	return Window::Update();
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
void ButtonTouchFunction()
{
	Main::g_window.SetVisible(false); // Hide main window
	label_title->SetText("Touch Mode");
	label_title->SetVisible(true);
	button_touch->SetVisible(true);
	Main::g_window.Draw();

	while(D2K::Loop())
	{
		// If pressed again, break
		if(Main::g_window.CheckClick(button_touch))
			break;

		UDP::Update(g_keys_held, 0, &D2K::g_stylus_position,
		            &D2K::g_circle_position, &D2K::g_cstick_position,
		            &D2K::g_accel_status, &D2K::g_gyro_status,
		            &D2K::g_slider_volume_status, &D2K::g_slider_3d_status, 0);

		Main::g_window.Update();
	}

	label_title->SetText(D2K::VERSION_STRING);
	Main::g_window.SetVisible(true);
}

void ButtonTurboFunction()
{
	Main::g_window.SetVisible(false); // Hide main window
	button_turbo->SetVisible(true);
	Turbo::g_window.SetVisible(true);
	ForceBacklightsOn(true);          // Lock backlights on

	while(D2K::Loop())
	{
		UDP::Update(g_keys_held, Turbo::GetKeys(), nullptr,
		            &D2K::g_circle_position, &D2K::g_cstick_position,
		            &D2K::g_accel_status, &D2K::g_gyro_status,
		            &D2K::g_slider_volume_status, &D2K::g_slider_3d_status, 0);
		Turbo::g_window.Update();
		button_turbo->Draw();

		// If pressed again, break
		if(Main::g_window.CheckClick(button_turbo))
			break;
	}

	Turbo::g_window.SetVisible(false);
	Main::g_window.SetVisible(true);
}
void ButtonConfigGamepadWindowFunction()
{
	ConfigWindow::g_config_type = true;      // true = gamepad;

	Main::g_window.SetVisible(false);        // Hide main window
	button_config_gamepad->SetVisible(true);
	ConfigWindow::g_window.SetVisible(true);
	ForceBacklightsOn(true);                 // Lock backlights on

	while(D2K::Loop()
	   && ConfigWindow::g_window.IsVisible())
	{
		ConfigWindow::g_window.Update();
			UDP::Update(g_keys_held, Turbo::GetKeys(), nullptr, 
			            &g_circle_position, &g_cstick_position,
			            &g_accel_status, &g_gyro_status,
			            &g_slider_volume_status, &g_slider_3d_status,
			            0);
		ConfigWindow::current_pressed_key = 0;
		button_config_gamepad->Draw();

		// If pressed again, break
		if(Main::g_window.CheckClick(button_config_gamepad))
			break;
	}

	ConfigWindow::g_window.SetVisible(false);
	Main::g_window.SetVisible(true);
}
void ButtonConfigKeyboardWindowFunction()
{
	ConfigWindow::g_config_type = false;      // false = keyboard;

	Main::g_window.SetVisible(false);         // Hide main window
	button_config_keyboard->SetVisible(true);
	ConfigWindow::g_window.SetVisible(true);
	ForceBacklightsOn(true);                  // Lock backlights on

	while(D2K::Loop()
	   && ConfigWindow::g_window.IsVisible())
	{
		ConfigWindow::g_window.Update();
			UDP::Update(g_keys_held, Turbo::GetKeys(), nullptr, 
			            &g_circle_position, &g_cstick_position,
			            &g_accel_status, &g_gyro_status,
			            &g_slider_volume_status, &g_slider_3d_status,
			            0);
		ConfigWindow::current_pressed_key = 0;
		button_config_keyboard->Draw();

		// If pressed again, break
		if(Main::g_window.CheckClick(button_config_keyboard))
			break;
	}

	ConfigWindow::g_window.SetVisible(false);
	Main::g_window.SetVisible(true);
}
void ButtonIPFunction()
{
	UDP::ServerLookup();
	edit_ip->SetText(UDP::GetRemoteIPString());
}
void ButtonConnectionFunction()
{
	Main::g_window.SetVisible(false);      // Hide main window
	button_connection->SetVisible(true);   // Keep [Connection] button visible
	Connection::g_window.SetVisible(true); // Show connection window
	ForceBacklightsOn(true);               // Lock backlights on

	while(D2K::Loop())
	{
		Connection::g_window.Update(); // Update and draw command window
		button_commands->Draw();       // Draw [Commands] button

		// If (A) is pressed
		if(g_keys_down&KEY_A)
			D2K::GUI::Main::EditIPFunction();
		else if(g_keys_down&KEY_B)
			D2K::GUI::Main::EditPortFunction();
		else if(g_keys_down&KEY_START)
			break;
		
		// If pressed again, break
		if(Main::g_window.CheckClick(button_commands))
			break;
	}

	Connection::g_window.SetVisible(false);
	Main::g_window.SetVisible(true);       // Show main window

}
void ButtonCommandsFunction()
{
	Main::g_window.SetVisible(false);   // Hide main window
	button_commands->SetVisible(true);  // Keep [Commands] button visible
	Command::g_window.SetVisible(true); // Show command window
	ForceBacklightsOn(true);            // Lock backlights on

	while(D2K::Loop())
	{
		UDP::Update(g_keys_held, Turbo::GetKeys(), nullptr, 
		            &D2K::g_circle_position, &D2K::g_cstick_position,
		            &D2K::g_accel_status, &D2K::g_gyro_status,
		            &D2K::g_slider_volume_status, &D2K::g_slider_3d_status, 0);
		Command::g_window.Update(); // Update and draw command window
		button_commands->Draw();    // Draw [Commands] button
		
		// If pressed again, break
		if(Main::g_window.CheckClick(button_commands))
			break;
	}

	Command::g_window.SetVisible(false);
	Main::g_window.SetVisible(true);    // Show main window
}

}}} // namespace D2K::GUI::Main
