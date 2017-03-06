#include <string> // std::string

// windows
#include "settingsWindow.h"
#include "barWindow.h"
#include "turboWindow.h"

// controls
#include "gui/button.h"
#include "gui/letterButton.h"

// core
#include "core.h" // g_keys_down
#include "common/udp.h"

namespace D2K {namespace GUI {namespace Main {

extern void EditIPFunction();
extern void EditPortFunction();
extern void ButtonConnectionFunction();
extern void ButtonTurboFunction();
extern void ButtonConfigKeyboardWindowFunction();
extern void ButtonConfigGamepadWindowFunction();
extern void EditProfileFunction();

}}}

namespace D2K {namespace GUI {namespace Settings {

// private
extern void button4Function();

WindowClass g_window;
LetterButton* button_1;
LetterButton* button_2;
LetterButton* button_3;
LetterButton* button_4;
LetterButton* button_5;
LetterButton* button_6;
LetterButton* button_7;

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

	// TODO: this should open a new window offering configuration setup and profile selection
	AppendObject(button_1 = new LetterButton(m_screen, Rect(  2,   2,  61,  42), "A",     "Connection", "", &Main::ButtonConnectionFunction));
	AppendObject(button_2 = new LetterButton(m_screen, Rect( 65,   2,  61,  42), "B",     "Profile", "", &Main::EditProfileFunction));
	AppendObject(button_3 = new LetterButton(m_screen, Rect(130,   2,  61,  42), "X",     "Turbo", "", &Main::ButtonTurboFunction));
	AppendObject(button_4 = new LetterButton(m_screen, Rect(195,   2,  61,  42), "Y",     "", "", &button4Function));
	AppendObject(button_5 = new LetterButton(m_screen, Rect(  2,  44, 122,  42), "L",     "Button - Gamepad", "", &Main::ButtonConfigGamepadWindowFunction));
	AppendObject(button_6 = new LetterButton(m_screen, Rect(130,  44, 122,  42), "R",     "Button - Keyboard", "", &Main::ButtonConfigKeyboardWindowFunction));
	AppendObject(button_7 = new LetterButton(m_screen, Rect(  2, 150, 252,  22), "Start", "Exit", "", &button4Function));
}
WindowClass::~WindowClass() { }
void WindowClass::SetVisible(bool visible)
{
	Window::SetVisible(visible);

	if(!visible)
		return;

	Bar::SetText("[Settings]");
	Bar::g_window.SetVisible(true);

	ForceBacklightsOn(true);          // Lock backlights on
}
bool WindowClass::Update()
{
	if(g_keys_down&KEY_A)
		D2K::GUI::Main::ButtonConnectionFunction();
	else if(g_keys_down&KEY_B)
		D2K::GUI::Main::EditProfileFunction();
	else if(g_keys_down&KEY_X)
		D2K::GUI::Main::ButtonTurboFunction();
	else if(g_keys_down&KEY_Y)
		Settings::g_window.SetVisible(false);
	else if(g_keys_down&KEY_L)
		Main::ButtonConfigGamepadWindowFunction();
	else if(g_keys_down&KEY_R)
		Main::ButtonConfigKeyboardWindowFunction();
// TODO: this interacts badly, search
	else if(g_keys_up&KEY_START)
		Settings::g_window.SetVisible(false);

	return Window::Update()
	    || Bar::g_window.Update();
}

void button4Function()
{
	g_window.SetVisible(false);
}

}}} // namespace D2K::GUI::Command
