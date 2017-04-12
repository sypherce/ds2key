#include <string> // std::string

// windows
#include "connectionWindow.h"
#include "barWindow.h"

// controls
#include "gui/button.h"
#include "gui/letterButton.h"

// core
#include "core.h" // g_keys_down
#include "common/udp.h"

namespace D2K {namespace GUI {namespace Main {

extern void EditIPFunction();
extern void EditPortFunction();

}}}

namespace D2K {namespace GUI {namespace Connection {

// private
extern void button1Function();
extern void button2Function();
extern void button3Function();

WindowClass g_window;
LetterButton* button_1;
LetterButton* button_2;
LetterButton* button_3;

WindowClass::WindowClass() : Window()
{
	m_screen = 0;
	int x = 16;
	int y = 24;
	int w = 64;
	int h = 24;
	int gap = 12;
	Rect button_rects[2];
	button_rects[0] = Rect(x,y,w,h);
		y += h + gap;
	button_rects[1] = Rect(x,y,w,h);
		y += h + gap;

	AppendObject(button_1 = new LetterButton(m_screen, Rect(  2, 2, 122, 42), "A", "Server IP:", UDP::GetRemoteIPString(), &button1Function));
	AppendObject(button_2 = new LetterButton(m_screen, Rect(130, 2, 122, 42), "B", "Port:", UDP::GetPortString(), &button2Function));
	AppendObject(button_3 = new LetterButton(m_screen, Rect(  2, 150, 252, 22), "Start", "Exit", "", &button3Function));
}
WindowClass::~WindowClass() { }
void WindowClass::SetVisible(bool visible)
{
	if(visible)
	{
		button_1->SetSubText(UDP::GetRemoteIPString());
		button_2->SetSubText(UDP::GetPortString());
	}

	Window::SetVisible(visible);

	if(!visible)
		return;
	
	Bar::SetText("[Connection]");
	Bar::g_window.SetVisible(true);

	ForceBacklightsOn(true); // Lock backlights on
}
bool WindowClass::Update()
{
	// If (A) is pressed
	if(g_keys_down&KEY_A)
		D2K::GUI::Main::EditIPFunction();
	else if(g_keys_down&KEY_B)
		D2K::GUI::Main::EditPortFunction();
// TODO: this interacts badly, search
	else if(g_keys_up&KEY_START)
		Connection::g_window.SetVisible(false);

	UDP::Update(0, 0, nullptr,
	            nullptr, nullptr,
	            nullptr, nullptr,
	            nullptr, nullptr,
	            0);

	return Window::Update()
	    || Bar::g_window.Update();
}

void button1Function()
{
	D2K::GUI::Main::EditIPFunction();
	button_1->SetSubText(UDP::GetRemoteIPString());
}
void button2Function()
{
	D2K::GUI::Main::EditPortFunction();
	button_2->SetSubText(UDP::GetPortString());
}
void button3Function()
{
// TODO: this is really hacky
	g_window.SetVisible(false);
	D2K::g_keys_up &= KEY_START;
}

}}} // namespace D2K::GUI::Command
