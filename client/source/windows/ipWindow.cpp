#include <string> // std::string
#include "ipWindow.h"
#include "gui/button.h"
#include "gui/label.h"
#include "common/udp.h"

namespace D2K {namespace GUI {namespace IP {

extern void button1Function();
extern void button2Function();
extern void button3Function();
extern void button4Function();
extern void button5Function();
extern void button6Function();
extern void button7Function();
extern void button8Function();
extern void button9Function();
extern void button10Function();
extern void button11Function();
extern void button12Function();

WindowClass g_window;
Label* label_1;
Button* button_1;
Button* button_2;
Button* button_3;
Button* button_4;
Button* button_5;
Button* button_6;
Button* button_7;
Button* button_8;
Button* button_9;
Button* button_10;
Button* button_11;
Button* button_12;

WindowClass::WindowClass() : Window()
{
	m_screen = 0;
	int x = 16;
	int y = 24;
	int w = 64;
	int h = 24;
	int gap = 12;

	AppendObject(label_1 = new Label(m_screen, Rect(24,0+3,128,10), "IPs"));

	AppendObject(button_1 = new Button(m_screen, Rect(x,y,w,h), "Command 1", &button1Function));
	y += h + gap;
	AppendObject(button_2 = new Button(m_screen, Rect(x,y,w,h), "Command 2", &button2Function));
	y += h + gap;
	AppendObject(button_3 = new Button(m_screen, Rect(x,y,w,h), "Command 3", &button3Function));
	y += h + gap;
	AppendObject(button_4 = new Button(m_screen, Rect(x,y,w,h), "Command 4", &button4Function));

	x = x + w + gap;
	y = 24;

	AppendObject(button_5 = new Button(m_screen, Rect(x,y,w,h), "Command 5", &button5Function));
	y += h + gap;
	AppendObject(button_6 = new Button(m_screen, Rect(x,y,w,h), "Command 6", &button6Function));
	y += h + gap;
	AppendObject(button_7 = new Button(m_screen, Rect(x,y,w,h), "Command 7", &button7Function));
	y += h + gap;
	AppendObject(button_8 = new Button(m_screen, Rect(x,y,w,h), "Command 8", &button8Function));

	x = x + w + gap;
	y = 24;

	AppendObject(button_9 = new Button(m_screen, Rect(x,y,w,h), "Command 9", &button9Function));
	y += h + gap;
	AppendObject(button_10 = new Button(m_screen, Rect(x,y,w,h), "Command 10", &button10Function));
	y += h + gap;
	AppendObject(button_11 = new Button(m_screen, Rect(x,y,w,h), "Command 11", &button11Function));
	y += h + gap;
	AppendObject(button_12 = new Button(m_screen, Rect(x,y,w,h), "Command 12", &button12Function));
}
WindowClass::~WindowClass() { }
bool WindowClass::Update()
{
	return Window::Update();
}

void button1Function()
{
	UDP::SendCommand(0);
}
void button2Function()
{
	UDP::SendCommand(1);
}
void button3Function()
{
	UDP::SendCommand(2);
}
void button4Function()
{
	UDP::SendCommand(3);
}
void button5Function()
{
	UDP::SendCommand(4);
}
void button6Function()
{
	UDP::SendCommand(5);
}
void button7Function()
{
	UDP::SendCommand(6);
}
void button8Function()
{
	UDP::SendCommand(7);
}
void button9Function()
{
	UDP::SendCommand(8);
}
void button10Function()
{
	UDP::SendCommand(9);
}
void button11Function()
{
	UDP::SendCommand(10);
}
void button12Function()
{
	UDP::SendCommand(11);
}

}}} // namespace D2K::GUI::IP
