#include <string>  // std::string
#include "commandWindow.h"
#include "gui/button.h"
#include "common/udp.h"

namespace D2K {namespace GUI {namespace Command {

//private
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

// Request a settings packet, use the info if we get any

void ProcessCommandSettingsPacket(UDP::DS2KeyCommandSettingsPacket settings)
{
	Rect button_rects[12];
	for(int i = 0; i <= 11; i++)
	{
		button_rects[i] = Rect(settings.x_1[i], settings.y_1[i], settings.x_2[i], settings.y_2[i]);
	}
	button_1->SetRect(button_rects[0]);
	button_2->SetRect(button_rects[1]);
	button_3->SetRect(button_rects[2]);
	button_4->SetRect(button_rects[3]);
	button_5->SetRect(button_rects[4]);
	button_6->SetRect(button_rects[5]);
	button_7->SetRect(button_rects[6]);
	button_8->SetRect(button_rects[7]);
	button_9->SetRect(button_rects[8]);
	button_10->SetRect(button_rects[9]);
	button_11->SetRect(button_rects[10]);
	button_12->SetRect(button_rects[11]);

	button_1->SetText(settings.text[0]);
	button_2->SetText(settings.text[1]);
	button_3->SetText(settings.text[2]);
	button_4->SetText(settings.text[3]);
	button_5->SetText(settings.text[4]);
	button_6->SetText(settings.text[5]);
	button_7->SetText(settings.text[6]);
	button_8->SetText(settings.text[7]);
	button_9->SetText(settings.text[8]);
	button_10->SetText(settings.text[9]);
	button_11->SetText(settings.text[10]);
	button_12->SetText(settings.text[11]);
}

WindowClass::WindowClass() : Window()
{
	m_screen = 0;
	int x = 16;
	int y = 24;
	int w = 64;
	int h = 24;
	int gap = 12;
	Rect button_rects[12];
	button_rects[0] = Rect(x,y,w,h);
		y += h + gap;
	button_rects[1] = Rect(x,y,w,h);
		y += h + gap;
	button_rects[2] = Rect(x,y,w,h);
		y += h + gap;
	button_rects[3] = Rect(x,y,w,h);
		x = x + w + gap;
		y = 24;

	button_rects[4] = Rect(x,y,w,h);
		y += h + gap;
	button_rects[5] = Rect(x,y,w,h);
		y += h + gap;
	button_rects[6] = Rect(x,y,w,h);
		y += h + gap;
	button_rects[7] = Rect(x,y,w,h);
		y += h + gap;
		x = x + w + gap;
		y = 24;

	button_rects[8] = Rect(x,y,w,h);
		y += h + gap;
	button_rects[9] = Rect(x,y,w,h);
		y += h + gap;
	button_rects[10] = Rect(x,y,w,h);
		y += h + gap;
	button_rects[11] = Rect(x,y,w,h);

	AppendObject(button_1 = new Button(m_screen, button_rects[0], "Command 0", &button1Function));
	AppendObject(button_2 = new Button(m_screen, button_rects[1], "Command 1", &button2Function));
	AppendObject(button_3 = new Button(m_screen, button_rects[2], "Command 2", &button3Function));
	AppendObject(button_4 = new Button(m_screen, button_rects[3], "Command 3", &button4Function));

	AppendObject(button_5 = new Button(m_screen, button_rects[4], "Command 4", &button5Function));
	AppendObject(button_6 = new Button(m_screen, button_rects[5], "Command 5", &button6Function));
	AppendObject(button_7 = new Button(m_screen, button_rects[6], "Command 6", &button7Function));
	AppendObject(button_8 = new Button(m_screen, button_rects[7], "Command 7", &button8Function));

	AppendObject(button_9 = new Button(m_screen, button_rects[8], "Command 8", &button9Function));
	AppendObject(button_10 = new Button(m_screen, button_rects[9], "Command 9", &button10Function));
	AppendObject(button_11 = new Button(m_screen, button_rects[10], "Command 10", &button11Function));
	AppendObject(button_12 = new Button(m_screen, button_rects[11], "Command 11", &button12Function));
}
WindowClass::~WindowClass() { }
void WindowClass::SetVisible(bool visible)
{
	if(visible)
		UDP::RequestSettingsCommand();

	return Window::SetVisible(visible);
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

}}}//namespace D2K::GUI::Command
