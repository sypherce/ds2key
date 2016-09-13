#include <string> // std::string
#include "gamepadWindow.h"
#include "configWindow.h"
#include "turboWindow.h"
#include "gui/gui.h"
#include "common/udp.h"
#include "config.h"
#include "core.h"

namespace D2K {namespace GUI {namespace Gamepad {

// this was copied from server/core/key.h
namespace Key {
enum
{

JOY = 0x100,
// Check key.cpp for final JOY value
JOY_MAX = JOY + 31,

JOY_HAT,
JOY_HAT_UP = JOY_HAT,
JOY_HAT_DOWN,
JOY_HAT_LEFT,
JOY_HAT_RIGHT,
JOY_HAT_MAX = JOY_HAT_RIGHT,

JOY_AXIS1,
JOY_AXIS_Y_MINUS = JOY_AXIS1,
JOY_AXIS_Y_PLUS,
JOY_AXIS_X_MINUS,
JOY_AXIS_X_PLUS,
JOY_AXIS1_MAX = JOY_AXIS_X_PLUS,

JOY_AXIS2,
JOY_AXIS_RX_MINUS = JOY_AXIS2,
JOY_AXIS_RX_PLUS,
JOY_AXIS_Z_MINUS,
JOY_AXIS_Z_PLUS,
JOY_AXIS2_MAX = JOY_AXIS_Z_PLUS,

JOY_AXIS3,
JOY_AXIS_RZ_MINUS = JOY_AXIS3,
JOY_AXIS_RZ_PLUS,
JOY_AXIS_RY_MINUS,
JOY_AXIS_RY_PLUS,
JOY_AXIS3_MAX = JOY_AXIS_RY_PLUS,

JOY_AXIS4,
JOY_AXIS_SL1_MINUS = JOY_AXIS4,
JOY_AXIS_SL1_PLUS,
JOY_AXIS_SL0_MINUS,
JOY_AXIS_SL0_PLUS,
JOY_AXIS4_MAX = JOY_AXIS_SL0_PLUS,

JOY_AXIS_WHL_MINUS,
JOY_AXIS_WHL_PLUS,
JOY_AXIS5_MAX = JOY_AXIS_WHL_PLUS,

};
}


WindowClass g_window;

Label* label_title;
Button* button_l;
Button* button_zl;
Button* button_zr;
Button* button_r;
Button* button_left;
Button* button_right;
Button* button_up;
Button* button_down;
Button* button_cpad_left;
Button* button_cpad_right;
Button* button_cpad_up;
Button* button_cpad_down;
Button* button_cstick_left;
Button* button_cstick_right;
Button* button_cstick_up;
Button* button_cstick_down;
Button* button_a;
Button* button_b;
Button* button_x;
Button* button_y;
Button* button_start;
Button* button_select;
Button* button_enable;
Button* button_slider_volume;
Button* button_slider_3d;
Button* button_blue;
Button* button_yellow;
Button* button_red;
Button* button_green;
Button* button_lid;

Edit* edit_entry;
Label* label_entry;

std::string Buffer;
size_t BufferLen = 0;
void append(uint16_t KEY)
{
	ConfigWindow::current_pressed_key = KEY;
	char c = (char)KEY;
	switch(c)
	{
	// TODO::UPDATE COMMMENTED SECTION
	/*case ';':
	{
		ConfigWindow::current_pressed_key = 0xba;
		break;
	}
	case '\'':
	{
		ConfigWindow::current_pressed_key = 0xde;
		break;
	}
	case '.':
	{
		ConfigWindow::current_pressed_key = 0xbe;
		break;
	}
	case ',':
	{
		ConfigWindow::current_pressed_key = 0xbc;
		break;
	}
	case '=':
	{
		ConfigWindow::current_pressed_key = 0xbb;
		break;
	}*/
	case WINKEY_BACK:
	{
		if(Buffer.length() > 0)
			Buffer.resize(Buffer.length() - 1);
		return;
	}
	}
	if(BufferLen > Buffer.size())
	{
		Buffer += c;
	}
}

void ButtonCloseFunction()
{
	Gamepad::g_window.SetVisible(false);
}

void ButtonEnterFunction()
{
	append(WINKEY_RETURN);
	ButtonCloseFunction();
}

void ButtonAFunction()              { append(Key::JOY+0); }
void ButtonBFunction()              { append(Key::JOY+1); }
void ButtonXFunction()              { append(Key::JOY+2); }
void ButtonYFunction()              { append(Key::JOY+3); }

#if defined(_3DS)
void ButtonZLFunction()             { append(Key::JOY+4); }
void ButtonZRFunction()             { append(Key::JOY+5); }
#endif

void ButtonLFunction()              { append(Key::JOY_AXIS_Z_MINUS); }  //z
void ButtonRFunction()              { append(Key::JOY_AXIS_Z_PLUS); }   //z

void ButtonSelectFunction()         { append(Key::JOY+6); }
void ButtonStartFunction()          { append(Key::JOY+7); }

#if defined(_3DS)
void ButtonSliderVolumeFunction()   { append(WINKEY_VOLUME_UP); }
void ButtonSlider3DFunction()       { append(Key::JOY+9); }
#elif defined(_NDS)
void ButtonBlueFunction()           { append(Key::JOY+4); }
void ButtonYellowFunction()         { append(Key::JOY+5); }
void ButtonRedFunction()            { append(Key::JOY+8); }
void ButtonGreenFunction()          { append(Key::JOY+9); }
#endif

void ButtonLidFunction()            { append(Key::JOY+10); }

void ButtonLeftFunction()           { append(Key::JOY_HAT_LEFT); }
void ButtonRightFunction()          { append(Key::JOY_HAT_RIGHT); }
void ButtonUpFunction()             { append(Key::JOY_HAT_UP); }
void ButtonDownFunction()           { append(Key::JOY_HAT_DOWN); }

void ButtonLeftCPadFunction()       { append(Key::JOY_AXIS_X_MINUS); }  //x
void ButtonRightCPadFunction()      { append(Key::JOY_AXIS_X_PLUS); }   //x
void ButtonUpCPadFunction()         { append(Key::JOY_AXIS_Y_MINUS); }  //y
void ButtonDownCPadFunction()       { append(Key::JOY_AXIS_Y_PLUS); }   //y

void ButtonLeftCStickFunction()     { append(Key::JOY_AXIS_RX_MINUS); } //rx
void ButtonRightCStickFunction()    { append(Key::JOY_AXIS_RX_PLUS); }  //rx
void ButtonUpCStickFunction()       { append(Key::JOY_AXIS_RY_MINUS); } //ry
void ButtonDownCStickFunction()     { append(Key::JOY_AXIS_RY_PLUS); }  //ry

WindowClass::WindowClass() : Window()
{
	m_screen = 0;
	const int gap = 1;
	int x = gap;
	int y = gap;
	const int w = 15;
	const int h = 15;
	const int gamepad_base_x = 8;
	const int gamepad_base_y = 30;
	
	const int print_scroll_pause_base_x = gamepad_base_x;
	const int print_scroll_pause_base_y = gap + (h + gap) * 6;
	const int ins_home_pgup_base_x      = gamepad_base_x;
	const int ins_home_pgup_base_y      = gap + (h + gap) * 8;
	const int up_down_left_right_base_x = gamepad_base_x;
	const int up_down_left_right_base_y = gap + (h + gap) * 10;


	m_screen = 0;

	AppendObject(new Button(m_screen, Rect(229, 1, 10, 10), "Close", ButtonCloseFunction));

	AppendObject(label_title = new Label(m_screen, Rect(24,0+3,128,10), "Gamepad"));
				
	static uint16_t lx, ly;
	lx = gamepad_base_x + 9;
	ly = gamepad_base_y + 15;
	AppendObject(button_l  = new Button(m_screen, Rect(lx,ly,10,10), "L", &ButtonLFunction));
#ifdef _3DS
	AppendObject(button_zl = new Button(m_screen, Rect(lx,ly+13,10,10), "ZL", &ButtonZLFunction));
#endif
		
	static uint16_t rx, ry;
	rx = gamepad_base_x + 235;
	ry = gamepad_base_y + ly;
	AppendObject(button_r  = new Button(m_screen, Rect(rx,ry,10,10), "R", &ButtonRFunction));
#ifdef _3DS
	AppendObject(button_zr = new Button(m_screen, Rect(rx,ry+13,10,10), "ZR", &ButtonZRFunction));
#endif

	static uint16_t dpad_x, dpad_y;
	dpad_x = gamepad_base_x + 9;
	dpad_y = gamepad_base_y + 80;
	AppendObject(button_left  = new Button(m_screen, Rect(dpad_x,      dpad_y + 15, 10, 10), "<", &ButtonLeftFunction));
	AppendObject(button_right = new Button(m_screen, Rect(dpad_x + 60, dpad_y + 15, 10, 10), ">", &ButtonRightFunction));
	AppendObject(button_up    = new Button(m_screen, Rect(dpad_x + 30, dpad_y,      10, 10), "^", &ButtonUpFunction));
	AppendObject(button_down  = new Button(m_screen, Rect(dpad_x + 30, dpad_y + 30, 10, 10), "v", &ButtonDownFunction));
	
#ifdef _3DS
	static uint16_t cpad_x, cpad_y;
	cpad_x = gamepad_base_x + 30;
	cpad_y = gamepad_base_y + 15;
	AppendObject(button_left  = new Button(m_screen, Rect(cpad_x,      cpad_y + 15, 10, 10), "CP <", &ButtonLeftCPadFunction));
	AppendObject(button_right = new Button(m_screen, Rect(cpad_x + 60, cpad_y + 15, 10, 10), "CP >", &ButtonRightCPadFunction));
	AppendObject(button_up    = new Button(m_screen, Rect(cpad_x + 30, cpad_y,      10, 10), "CP ^", &ButtonUpCPadFunction));
	AppendObject(button_down  = new Button(m_screen, Rect(cpad_x + 30, cpad_y + 30, 10, 10), "CP v", &ButtonDownCPadFunction));
	
	static uint16_t cstick_x, cstick_y;
	cstick_x = gamepad_base_x + 135;
	cstick_y = gamepad_base_y + 15;
	AppendObject(button_left  = new Button(m_screen, Rect(cstick_x,      cstick_y + 15, 10, 10), "CS <", &ButtonLeftCStickFunction));
	AppendObject(button_right = new Button(m_screen, Rect(cstick_x + 60, cstick_y + 15, 10, 10), "CS >", &ButtonRightCStickFunction));
	AppendObject(button_up    = new Button(m_screen, Rect(cstick_x + 30, cstick_y,      10, 10), "CS ^", &ButtonUpCStickFunction));
	AppendObject(button_down  = new Button(m_screen, Rect(cstick_x + 30, cstick_y + 30, 10, 10), "CS v", &ButtonDownCStickFunction));
#endif
	
	static uint16_t abxy_x, abxy_y;
	abxy_x = gamepad_base_x + 175;
	abxy_y = gamepad_base_y + 80;
	AppendObject(button_a = new Button(m_screen, Rect(abxy_x + 60, abxy_y + 15, 10, 10), "A", &ButtonAFunction));
	AppendObject(button_b = new Button(m_screen, Rect(abxy_x + 30, abxy_y + 30, 10, 10), "B", &ButtonBFunction));
	AppendObject(button_x = new Button(m_screen, Rect(abxy_x + 30, abxy_y,      10, 10), "X", &ButtonXFunction));
	AppendObject(button_y = new Button(m_screen, Rect(abxy_x     , abxy_y + 15, 10, 10), "Y", &ButtonYFunction));
	
	static uint16_t start_select_x, start_select_y;
	start_select_x = gamepad_base_x + 85;
	start_select_y = gamepad_base_y + 115;
	AppendObject(button_start  = new Button(m_screen, Rect(start_select_x + 60, start_select_y, 10, 10), "Start", &ButtonStartFunction));
	AppendObject(button_select = new Button(m_screen, Rect(start_select_x,     start_select_y, 10, 10), "Select", &ButtonSelectFunction));
	
#if defined(_3DS)
	static uint16_t sliders_x, sliders_y;
	sliders_x = gamepad_base_x + 40;
	sliders_y = gamepad_base_y + 130;
	AppendObject(button_slider_volume = new Button(m_screen, Rect(sliders_x,       sliders_y, 10, 10), "Volume", &ButtonSliderVolumeFunction));
	AppendObject(button_slider_3d     = new Button(m_screen, Rect(sliders_x + 100, sliders_y, 10, 10), "3D Slider", &ButtonSlider3DFunction));
#elif defined(_NDS)
	static uint16_t byrg_x, byrg_y;
	byrg_x = gamepad_base_x + 40;
	byrg_y = gamepad_base_y + 130;
	AppendObject(button_blue   = new Button(m_screen, Rect(byrg_x,       byrg_y, 10, 10), "Blue", &ButtonBlueFunction));
	AppendObject(button_yellow = new Button(m_screen, Rect(byrg_x + 50,  byrg_y, 10, 10), "Yellow", &ButtonYellowFunction));
	AppendObject(button_red    = new Button(m_screen, Rect(byrg_x + 100, byrg_y, 10, 10), "Red", &ButtonRedFunction));
	AppendObject(button_green  = new Button(m_screen, Rect(byrg_x + 150, byrg_y, 10, 10), "Green", &ButtonGreenFunction));
#endif

	static uint16_t lid_x, lid_y;
	lid_x = gamepad_base_x + 115;
	lid_y = gamepad_base_y + 145;
	AppendObject(button_lid  = new Button(m_screen, Rect(lid_x, lid_y, 10, 10), "Lid", &ButtonLidFunction));
}
WindowClass::~WindowClass() { }

// configWindow function
std::string GetString(Label* label, Edit* edit, std::string text, int maxLength)
{
	Gamepad::label_entry->SetRect(label->GetRect());
	Gamepad::label_entry->SetText(label->GetText());

	Gamepad::edit_entry->SetRect(edit->GetRect());
	Gamepad::edit_entry->SetText(edit->GetText());

	Buffer = text;
	BufferLen = maxLength;

	ConfigWindow::g_window.SetVisible(false);                      // Hide config window
	Gamepad::g_window.SetVisible(true);                            // Show gamepad

	Gamepad::g_window.Draw();                                      // And actually draw it
	while(D2K::Loop()
	   && Gamepad::g_window.IsVisible())
	{
		UDP::Update(g_keys_held, Turbo::GetKeys(), nullptr, 
			        &g_circle_position, &g_cstick_position,
			        &g_accel_status, &g_gyro_status,
			        &g_slider_volume_status, &g_slider_3d_status,
			        ConfigWindow::current_pressed_key);    // Update keys and press active gamepad key
		if(Gamepad::g_window.Update())                         // If pressed
		{
			ConfigWindow::current_pressed_key = 0;
			Gamepad::edit_entry->SetText(Buffer);          // Set text
		}
	}
	Gamepad::g_window.SetVisible(false);                           // Hide gamepad

	ConfigWindow::g_window.SetVisible(true);                       // Go back to config window
	return Buffer;
}

//configWindow function
uint16_t GetKey()
{
	Buffer = "";
	BufferLen = 0;

	ConfigWindow::g_window.SetVisible(false); // Hide config window
	Gamepad::g_window.SetVisible(true);       // Show gamepad

	Gamepad::g_window.Draw();                 // And actually draw it
	while(D2K::Loop()
	   && Gamepad::g_window.IsVisible())
	{
		if(Gamepad::g_window.Update())    // If pressed
		{
			break;
		}
	}
	Gamepad::g_window.SetVisible(false);      // Hide gamepad

	ConfigWindow::g_window.SetVisible(true);  // Go back to config window
	return ConfigWindow::current_pressed_key;
}

}}} // namespace D2K::GUI::Gamepad
