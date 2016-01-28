#include <string>	//std::string
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

UDP::DS2KeySettingsPacket settings;
WindowClass *Window;
Button *Button1;
Button *Button2;
Button *Button3;
Button *Button4;
Button *Button5;
Button *Button6;
Button *Button7;
Button *Button8;
Button *Button9;
Button *Button10;
Button *Button11;
Button *Button12;

//public
WindowClass::WindowClass() : Window()
{
	Screen = 0;
	int x = 16;
	int y = 24;
	int w = 64;
	int h = 24;
	int gap = 12;
	Rect buttonRects[12];
	buttonRects[0] = Rect(x,y,w,h);
		y += h + gap;
	buttonRects[1] = Rect(x,y,w,h);
		y += h + gap;
	buttonRects[2] = Rect(x,y,w,h);
		y += h + gap;
	buttonRects[3] = Rect(x,y,w,h);
		x = x + w + gap;
		y = 24;

	buttonRects[4] = Rect(x,y,w,h);
		y += h + gap;
	buttonRects[5] = Rect(x,y,w,h);
		y += h + gap;
	buttonRects[6] = Rect(x,y,w,h);
		y += h + gap;
	buttonRects[7] = Rect(x,y,w,h);
		y += h + gap;
		x = x + w + gap;
		y = 24;

	buttonRects[8] = Rect(x,y,w,h);
		y += h + gap;
	buttonRects[9] = Rect(x,y,w,h);
		y += h + gap;
	buttonRects[10] = Rect(x,y,w,h);
		y += h + gap;
	buttonRects[11] = Rect(x,y,w,h);
	settings = UDP::GetCommandSettings();
	if(settings.type == UDP::PACKET::COMMAND_SETTINGS)//if settings packet was received
	{
		for(int i = 0; i <= 11; i++)
		{
			buttonRects[i] = Rect(settings.x_1[i], settings.x_2[i], settings.y_1[i], settings.y_1[i]);
		}
	}

	AppendObject(Button1 = new Button(Screen, buttonRects[0], "Command 0", &button1Function));
	AppendObject(Button2 = new Button(Screen, buttonRects[1], "Command 1", &button2Function));
	AppendObject(Button3 = new Button(Screen, buttonRects[2], "Command 2", &button3Function));
	AppendObject(Button4 = new Button(Screen, buttonRects[3], "Command 3", &button4Function));

	AppendObject(Button5 = new Button(Screen, buttonRects[4], "Command 4", &button5Function));
	AppendObject(Button6 = new Button(Screen, buttonRects[5], "Command 5", &button6Function));
	AppendObject(Button7 = new Button(Screen, buttonRects[6], "Command 6", &button7Function));
	AppendObject(Button8 = new Button(Screen, buttonRects[7], "Command 7", &button8Function));

	AppendObject(Button9 = new Button(Screen, buttonRects[8], "Command 8", &button9Function));
	AppendObject(Button10 = new Button(Screen, buttonRects[9], "Command 9", &button10Function));
	AppendObject(Button11 = new Button(Screen, buttonRects[10], "Command 10", &button11Function));
	AppendObject(Button12 = new Button(Screen, buttonRects[11], "Command 11", &button12Function));
}
WindowClass::~WindowClass() { }
void WindowClass::SetVisible(bool visible)
{
	Rect buttonRects[12];
	settings = UDP::GetCommandSettings();
	if(settings.type == UDP::PACKET::COMMAND_SETTINGS)//if settings packet was received
	{
		for(int i = 0; i <= 11; i++)
		{
			buttonRects[i] = Rect(settings.x_1[i], settings.y_1[i], settings.x_2[i], settings.y_2[i]);
		}
		Button1->SetRect(buttonRects[0]);
		Button2->SetRect(buttonRects[1]);
		Button3->SetRect(buttonRects[2]);
		Button4->SetRect(buttonRects[3]);
		Button5->SetRect(buttonRects[4]);
		Button6->SetRect(buttonRects[5]);
		Button7->SetRect(buttonRects[6]);
		Button8->SetRect(buttonRects[7]);
		Button9->SetRect(buttonRects[8]);
		Button10->SetRect(buttonRects[9]);
		Button11->SetRect(buttonRects[10]);
		Button12->SetRect(buttonRects[11]);

		Button1->SetText(settings.text[0]);
		Button2->SetText(settings.text[1]);
		Button3->SetText(settings.text[2]);
		Button4->SetText(settings.text[3]);
		Button5->SetText(settings.text[4]);
		Button6->SetText(settings.text[5]);
		Button7->SetText(settings.text[6]);
		Button8->SetText(settings.text[7]);
		Button9->SetText(settings.text[8]);
		Button10->SetText(settings.text[9]);
		Button11->SetText(settings.text[10]);
		Button12->SetText(settings.text[11]);
	}
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
