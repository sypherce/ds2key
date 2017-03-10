#include <string> // std::string

// windows
#include "barWindow.h"

// controls
#include "gui/gui.h"
#include "gui/edit.h"
#include "gui/button.h"
#include "gui/label.h"
#include "gui/canvas.h"
#include "gui/frame.h"

// core
#include "core.h"
#include "common/udp.h"
#include "common/misc.h"
#include "config.h"

namespace D2K {

extern uint32_t wifi_status;
extern uint8_t battery_level;
extern uint8_t charging_status;

}

namespace D2K {namespace GUI {namespace Bar {

WindowClass g_window;

Canvas* canvas_wifi;
Canvas* canvas_battery;
Label* label_clock;
Label* label_title;
Frame* frame_main;
Frame* frame_title;

uint16_t Battery_0[8][14] = {
	0, 0, 0, 1, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};
uint16_t Battery_1[8][14] = {
	0, 0, 0, 1, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};
uint16_t Battery_2[8][14] = {
	0, 0, 0, 1, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};
uint16_t Battery_3[8][14] = {
	0, 0, 0, 1, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};
uint16_t Battery_4[8][14] = {
	0, 0, 0, 1, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};
uint16_t Battery_5[8][14] = {
	0, 0, 0, 1, 1, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
};

uint16_t Wifi_1[14][14] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
uint16_t Wifi_2[14][14] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
};
uint16_t Wifi_3[14][14] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
	1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
	1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
	1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
	1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
};
uint16_t Wifi_4[14][14] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1,
	0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
	0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
	0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
	1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
	1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
	1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
	1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
};

uint16_t background_color = ARGB16(1, 30, 30, 30);
uint16_t title_background_color = ARGB16(1, 31, 15, 15);
WindowClass::WindowClass() : Window()
{
	m_screen = 0;

	uint16_t header_y  = 177;
	uint16_t wifi_x    = 4;
	uint16_t battery_x = wifi_x + 16 + 4;
	uint16_t clock_x   = battery_x + 10 + 4;
	uint16_t title_x   = clock_x + 120;
	
	AppendObject(frame_main            = new Frame(m_screen,  Rect(0,           header_y, 191, title_x - 5), background_color));
	AppendObject(frame_title           = new Frame(m_screen,  Rect(title_x - 5, header_y, 191, 255 - (title_x - 5)), title_background_color));
	AppendObject(canvas_wifi           = new Canvas(m_screen, wifi_x,      header_y, 14, 14));
	canvas_wifi->SetBuffer((uint16_t*)Wifi_4);
	canvas_wifi->SetColor(background_color);
	AppendObject(canvas_battery        = new Canvas(m_screen, battery_x,   header_y, 8, 14));
	canvas_battery->SetBuffer((uint16_t*)Battery_5);
	canvas_battery->SetColor(background_color);
	AppendObject(label_clock           = new Label(m_screen,  Rect(clock_x,     header_y + 5,  70, 10), D2K::GetTime()));
	label_clock->SetColor(background_color);
	AppendObject(label_title           = new Label(m_screen,  Rect(title_x,     header_y + 5, 128, 10), "[" + D2K::VERSION_STRING + "]"));
	label_title->SetColor(title_background_color);
}
WindowClass::~WindowClass() { }
bool WindowClass::Update()
{
	const unsigned int UPDATE_COUNTER_MAX = 30;
// TODO: this delay's for the clock?
	static unsigned int update_counter = UPDATE_COUNTER_MAX;

	if(!IsVisible())
	{
		update_counter = UPDATE_COUNTER_MAX;
	}
	else if(update_counter < UPDATE_COUNTER_MAX)
	{
		update_counter++;
	}
	else
	{
		update_counter = 0;
		static char seconds = 0;
		char* timePointer = D2K::GetTime();

		if(seconds != timePointer[7])  // If seconds differ
		{
			seconds = timePointer[7];
			label_clock->SetText(timePointer);
		}

		switch(D2K::wifi_status)
		{
		case 0:
			canvas_wifi->SetBuffer((uint16_t*)Wifi_1);
			break;
		case 1:
			canvas_wifi->SetBuffer((uint16_t*)Wifi_2);
			break;
		case 2:
			canvas_wifi->SetBuffer((uint16_t*)Wifi_3);
			break;
		default:
			canvas_wifi->SetBuffer((uint16_t*)Wifi_4);
			break;
		}

		switch(D2K::battery_level)
		{
		case 2:
			canvas_battery->SetBuffer((uint16_t*)Battery_2);
			break;
		case 3:
			canvas_battery->SetBuffer((uint16_t*)Battery_3);
			break;
		case 4:
			canvas_battery->SetBuffer((uint16_t*)Battery_4);
			break;
		case 5:
			canvas_battery->SetBuffer((uint16_t*)Battery_5);
			break;
		default:
			canvas_battery->SetBuffer((uint16_t*)Battery_1);
			break;
		}
	}

	return Window::Update();
}

void SetText(std::string Text)
{
	label_title->SetText(Text);
}

}}} // namespace D2K::GUI::Main
