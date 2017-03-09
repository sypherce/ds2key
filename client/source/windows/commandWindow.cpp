#include <string> // std::string

// windows
#include "commandWindow.h"
#include "barWindow.h"
#include "turboWindow.h"

// controls
#include "gui/button.h"

// core
#include "core.h"
#include "common/udp.h"

namespace D2K {namespace GUI {namespace Command {

// private
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
Label* label_1{};
Button* command_button[12]{};

void ButtonCloseCommandsFunction()
{
	Command::g_window.SetVisible(false);
}

// Request a settings packet, use the info if we get any

void ProcessCommandSettingsPacket(UDP::DS2KeyCommandSettingsPacket settings)
{
	Rect button_rects[12];
	for(int i = 0; i <= 11; i++)
	{
		command_button[i]->SetRect(Rect(settings.x_1[i], settings.y_1[i], settings.x_2[i], settings.y_2[i]));
		command_button[i]->SetText(settings.text[i]);
	}
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
	
	AppendObject(label_1 = new Label(m_screen, Rect(2, 2, 61, 10), "Press L+R+START+SELECT to unlock screen"));

	AppendObject(command_button[0] = new Button(m_screen, button_rects[0], "Command 0", &button1Function));
	AppendObject(command_button[1] = new Button(m_screen, button_rects[1], "Command 1", &button2Function));
	AppendObject(command_button[2] = new Button(m_screen, button_rects[2], "Command 2", &button3Function));
	AppendObject(command_button[3] = new Button(m_screen, button_rects[3], "Command 3", &button4Function));

	AppendObject(command_button[4] = new Button(m_screen, button_rects[4], "Command 4", &button5Function));
	AppendObject(command_button[5] = new Button(m_screen, button_rects[5], "Command 5", &button6Function));
	AppendObject(command_button[6] = new Button(m_screen, button_rects[6], "Command 6", &button7Function));
	AppendObject(command_button[7] = new Button(m_screen, button_rects[7], "Command 7", &button8Function));

	AppendObject(command_button[8] = new Button(m_screen, button_rects[8], "Command 8", &button9Function));
	AppendObject(command_button[9] = new Button(m_screen, button_rects[9], "Command 9", &button10Function));
	AppendObject(command_button[10] = new Button(m_screen, button_rects[10], "Command 10", &button11Function));
	AppendObject(command_button[11] = new Button(m_screen, button_rects[11], "Command 11", &button12Function));
}
WindowClass::~WindowClass() { }
void WindowClass::SetVisible(bool visible)
{
	if(visible)
		UDP::RequestSettingsCommand();

	Window::SetVisible(visible);

	if(!visible)
		return;

	Window::SetVisible(visible);

	Bar::SetText("[Commands]");
	Bar::g_window.SetVisible(true);

	ForceBacklightsOn(true);            // Lock backlights on
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
		ButtonCloseCommandsFunction();

// TODO: this return value needs inspected
		return 0;
	}

	UDP::Update(D2K::g_keys_held, Turbo::GetKeys(), nullptr,
	            &D2K::g_circle_position, &D2K::g_cstick_position,
	            &D2K::g_accel_status, &D2K::g_gyro_status,
	            &D2K::g_slider_volume_status, &D2K::g_slider_3d_status, 0);

	return Window::Update()
	    || Bar::g_window.Update();
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

}}} // namespace D2K::GUI::Command
