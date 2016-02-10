#include <string>  // std::string

//windows
#include "mainWindow.h"
#include "keypadWindow.h"
#include "turboWindow.h"
#include "commandWindow.h"

//controls
#include "gui/gui.h"
#include "gui/edit.h"
#include "gui/button.h"
#include "gui/label.h"

//core
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
extern void ButtonIPFunction();
extern void ButtonCommandsFunction();

WindowClass* g_window;

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
Button* button_ip;
Button* button_commands;

WindowClass::WindowClass() : Window()
{
	Color[COLOR_BACKGROUND] 		= ARGB16(1, 19, 22, 25);

	Color[COLOR_LABEL_TEXT] 		= ARGB16(1, 0, 0, 0);
	Color[COLOR_LABEL_BACKGROUND]		= Color[COLOR_BACKGROUND];

	Color[COLOR_BUTTON_TEXT]		= Color[COLOR_LABEL_TEXT];
	Color[COLOR_BUTTON_BACKGROUND]		= ARGB16(1, 31, 30, 15);
	Color[COLOR_BUTTON_OUTLINE]		= Color[COLOR_BUTTON_TEXT];
	Color[COLOR_BUTTON_OUTLINE_ACTIVE]	= ARGB16(1, 31, 0, 0);

	Color[COLOR_EDIT_TEXT]			= Color[COLOR_LABEL_TEXT];
	Color[COLOR_EDIT_BACKGROUND]		= ARGB16(1, 31, 31, 31);
	Color[COLOR_EDIT_OUTLINE]		= Color[COLOR_EDIT_TEXT];
	Color[COLOR_EDIT_OUTLINE_ACTIVE]	= Color[COLOR_BUTTON_OUTLINE_ACTIVE];

	m_screen = 0;

	AppendObject(label_title		= new Label(m_screen, Rect(24,0+3,128,10), D2K::VERSION_STRING));
	AppendObject(label_clock		= new Label(m_screen, Rect(160,0+3,70,10), D2K::GetTime()));
	AppendObject(label_ip			= new Label(m_screen, Rect(64,24+3,10,10), "IP:"));
	AppendObject(label_port			= new Label(m_screen, Rect(64,48+3,10,10), "Port:"));
	AppendObject(label_profile		= new Label(m_screen, Rect(64,72+3,10,10), "Profile:"));

	AppendObject(edit_ip			= new Edit(m_screen, Rect(96,24,95,10), UDP::GetRemoteIPString(), &EditIPFunction));
	AppendObject(button_ip			= new Button(m_screen, Rect(204,24,35,10), "Find IP", &ButtonIPFunction));
	AppendObject(edit_port			= new Edit(m_screen, Rect(112,48,79,10), UDP::GetPortString(), &EditPortFunction));
	AppendObject(edit_profile		= new Edit(m_screen, Rect(136,72,55,10), UDP::GetProfileString(), &EditProfileFunction));
	AppendObject(button_turbo		= new Button(m_screen, Rect(0,177,35,10), "Turbo", &ButtonTurboFunction));
	AppendObject(button_commands		= new Button(m_screen, Rect(95,177,35,10), "Commands", &ButtonCommandsFunction));
	AppendObject(button_touch		= new Button(m_screen, Rect(217,177,35,10), "Touch", &ButtonTouchFunction));
}
WindowClass::~WindowClass() { }
bool WindowClass::Update()
{
	static char seconds = 0;

	const int UPDATE_COUNTER_MAX = 30;
	static int update_counter = 0;
	update_counter++;
	if((update_counter >= UPDATE_COUNTER_MAX)
	&&(label_clock->IsVisible()))
	{
		update_counter = 0;
		char* timePointer = D2K::GetTime();
		if(seconds != timePointer[7])  // If seconds differ
		{
			seconds = timePointer[7];
			label_clock->SetText(timePointer);
		}
	}
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
	// Hide main window
	Main::g_window->SetVisible(false);
	label_title->SetText("Touch Mode");
	label_title->SetVisible(true);
	button_touch->SetVisible(true);
	Main::g_window->Draw();

	while(true)
	{
		D2K::Loop();

		if(Main::g_window->CheckClick(button_touch))  // If pressed again, break
			break;

		UDP::Update(keysHeld(), 0, guitarGripKeysHeld() * guitarGripIsInserted(), Turbo::GHGetKeys() * guitarGripIsInserted(), &D2K::g_stylus_position);
	}

	label_title->SetText(D2K::VERSION_STRING);
	Main::g_window->SetVisible(true);
}

void ButtonTurboFunction()
{
	Main::g_window->SetVisible(false);  // Hide main window
	button_turbo->SetVisible(true);
	Turbo::g_window->SetVisible(true);

	while(true)
	{
		D2K::Loop();
		UDP::Update(keysHeld(), Turbo::GetKeys(), guitarGripKeysHeld() * guitarGripIsInserted(), Turbo::GHGetKeys() * guitarGripIsInserted(), nullptr);
		Turbo::g_window->Update();
		button_turbo->Draw();

		if(Main::g_window->CheckClick(button_turbo))  // If pressed again, break
			break;
	}

	Turbo::g_window->SetVisible(false);
	Main::g_window->SetVisible(true);
}
void ButtonIPFunction()
{
	UDP::ServerLookup();
	edit_ip->SetText(UDP::GetRemoteIPString());
}
void ButtonCommandsFunction()
{
	UDP::GetCommandSettings();
	Main::g_window->SetVisible(false);  // Hide main window
	button_commands->SetVisible(true);  // Keep [Commands] button visible
	Command::g_window->SetVisible(true);    // Show command window

	while(true)
	{
		D2K::Loop();
		UDP::Update(keysHeld(), Turbo::GetKeys(), guitarGripKeysHeld() * guitarGripIsInserted(), Turbo::GHGetKeys() * guitarGripIsInserted(), nullptr);
		Command::g_window->Update();  // Update and draw command window
		button_commands->Draw();  // Draw [Commands] button

		if(Main::g_window->CheckClick(button_commands))   // If pressed again, break
			break;							
	}

	Command::g_window->SetVisible(false);
	Main::g_window->SetVisible(true);  // Show main window
}

}}}//namespace D2K::GUI::Main
