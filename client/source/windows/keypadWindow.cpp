#include <string> // std::string
#include "keypadWindow.h"
#include "mainWindow.h"
#include "gui/gui.h"
#include "common/udp.h"
#include "config.h"
#include "core.h"

namespace D2K {namespace GUI {namespace Keypad {

extern void Button1Function();
extern void Button2Function();
extern void Button3Function();
extern void Button4Function();
extern void Button5Function();
extern void Button6Function();
extern void Button7Function();
extern void Button8Function();
extern void Button9Function();
extern void ButtonPeriodFunction();
extern void Button0Function();
extern void ButtonBackspaceFunction();
extern void ButtonEnterFunction();

WindowClass g_window;
Button* button_1;
Button* button_2;
Button* button_3;
Button* button_4;
Button* button_5;
Button* button_6;
Button* button_7;
Button* button_8;
Button* button_9;
Button* button_period;
Button* button_0;
Button* button_backspace;
Button* button_enter;

Edit* edit_entry;
Label* label_entry;

WindowClass::WindowClass() : Window()
{
	m_screen = 0;
	int x = 97;
	int y = 90;
	int w = 15;
	int h = 15;
	int gap = 6;

	AppendObject(label_entry      = new Label(m_screen, Rect(64,24+3,40,10), ""));
	AppendObject(edit_entry       = new Edit(m_screen, Rect(96,24,95,10), "", &VoidFunction));

	AppendObject(button_1         = new Button(m_screen, Rect(x + 0, y, w, h), "1", &Button1Function));
	AppendObject(button_2         = new Button(m_screen, Rect(x + w + gap, y, w, h), "2", &Button2Function));
	AppendObject(button_3         = new Button(m_screen, Rect(x + (w * 2) + (gap * 2), y, w, h), "3", &Button3Function));
	y += h + gap;
	AppendObject(button_4         = new Button(m_screen, Rect(x + 0, y, w, h), "4", &Button4Function));
	AppendObject(button_5         = new Button(m_screen, Rect(x + w + gap, y, w, h), "5", &Button5Function));
	AppendObject(button_6         = new Button(m_screen, Rect(x + (w * 2) + (gap * 2), y, w, h), "6", &Button6Function));
	y += h + gap;
	AppendObject(button_7         = new Button(m_screen, Rect(x + 0, y, w, h), "7", &Button7Function));
	AppendObject(button_8         = new Button(m_screen, Rect(x + w + gap, y, w, h), "8", &Button8Function));
	AppendObject(button_9         = new Button(m_screen, Rect(x + (w * 2) + (gap * 2), y, w, h), "9", &Button9Function));
	y += h + gap;
	AppendObject(button_period    = new Button(m_screen, Rect(x + 0, y, w, h), ".", &ButtonPeriodFunction));
	AppendObject(button_0         = new Button(m_screen, Rect(x + w + gap, y, w, h), "0", &Button0Function));
	AppendObject(button_backspace = new Button(m_screen, Rect(x + (w * 2) + (gap * 2), y, w, h), "<", &ButtonBackspaceFunction));
	y += h + gap;
	AppendObject(button_enter     = new Button(m_screen, Rect(x + 0, y, 42, h), "Enter", &ButtonEnterFunction));
}
WindowClass::~WindowClass() { }

std::string Buffer;
size_t BufferLen = 0;
void append(char c)
{
	if(BufferLen > Buffer.size())
		Buffer += c;
}

void ButtonEnterFunction()
{
	Keypad::g_window.SetVisible(false);
	Main::g_window.SetVisible(true);
}

void Button1Function()      { append('1'); }
void Button2Function()      { append('2'); }
void Button3Function()      { append('3'); }
void Button4Function()      { append('4'); }
void Button5Function()      { append('5'); }
void Button6Function()      { append('6'); }
void Button7Function()      { append('7'); }
void Button8Function()      { append('8'); }
void Button9Function()      { append('9'); }
void ButtonPeriodFunction() { append('.'); }
void Button0Function()      { append('0'); }
void ButtonBackspaceFunction()
{
	if(Buffer.length() > 0)
		Buffer.resize(Buffer.length() - 1);
}

std::string Entry(Label* label, Edit* edit, std::string text, int maxLength)
{
	ForceBacklightsOn(true);                             // Lock backlights on
	Keypad::label_entry->SetRect(label->GetRect());
	Keypad::label_entry->SetText(label->GetText());

	Keypad::edit_entry->SetRect(edit->GetRect());
	Keypad::edit_entry->SetText(edit->GetText());

	Buffer = text;
	BufferLen = maxLength;

	Main::g_window.SetVisible(false);                    // Hide main window
	Keypad::g_window.SetVisible(true);                   // Show keypad

	Keypad::g_window.Draw();                             // And actually draw it
	while(D2K::Loop()
	   && Keypad::g_window.IsVisible())
	{
		if(Keypad::g_window.Update())                // If pressed
			Keypad::edit_entry->SetText(Buffer); // Set text
	}
	Keypad::g_window.SetVisible(false);                  // Hide keypad

	Main::g_window.SetVisible(true);                     // Go back to main window
	return Buffer;
}

}}} // namespace D2K::GUI::Keypad
