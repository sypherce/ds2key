#include <string>  // std::string
#include "configWindow.h"
#include "gui/Button.h"
#include "gui/label.h"
#include "core.h"

#include "common/udp.h"
#include "config.h"

#ifdef _3DS
#include "gui/gui.h"
#endif
#include "keyboardWindow.h"

namespace D2K {namespace GUI {namespace ConfigWindow {

uint16_t current_pressed_key = NULL_VALUE;

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

void SendNewSetting(uint16_t setting, uint16_t value)
{//temp udp stuff
	UDP::DS2KeyNormalSettingsPacket settings = UDP::DS2KeyNormalSettingsPacket{ };
	settings.profile = UDP::GetProfile();
	settings.type = UDP::PACKET::NORMAL_SETTING;
	settings.setting = setting;
	settings.value = value;

	// Send settings packet
	UDP::SendNormalSetting(settings);
}

void SendNewSetting(uint16_t setting)
{
	char value = Keyboard::GetKey();
	if(value != 0)
		SendNewSetting(setting, value);
}

void ButtonCloseFunction()
{
	ConfigWindow::g_window.SetVisible(false);
}

void ButtonLFunction()
{
	SendNewSetting(KEYS::L);
}

void ButtonZLFunction()
{
	SendNewSetting(KEYS::ZL);
}
void ButtonRFunction()
{
	SendNewSetting(KEYS::R);
}
void ButtonZRFunction()
{
	SendNewSetting(KEYS::ZR);
}
void ButtonLeftFunction()
{
	SendNewSetting(KEYS::DLEFT);
}
void ButtonRightFunction()
{
	SendNewSetting(KEYS::DRIGHT);
}
void ButtonUpFunction()
{
	SendNewSetting(KEYS::DUP);
}
void ButtonDownFunction()
{
	SendNewSetting(KEYS::DDOWN);
}
void ButtonLeftCPadFunction()
{
	SendNewSetting(KEYS::CPAD_LEFT);
}
void ButtonRightCPadFunction()
{
	SendNewSetting(KEYS::CPAD_RIGHT);
}
void ButtonUpCPadFunction()
{
	SendNewSetting(KEYS::CPAD_UP);
}
void ButtonDownCPadFunction()
{
	SendNewSetting(KEYS::CPAD_DOWN);
}
void ButtonLeftCStickFunction()
{
	SendNewSetting(KEYS::CSTICK_LEFT);
}
void ButtonRightCStickFunction()
{
	SendNewSetting(KEYS::CSTICK_RIGHT);
}
void ButtonUpCStickFunction()
{
	SendNewSetting(KEYS::CSTICK_UP);
}
void ButtonDownCStickFunction()
{
	SendNewSetting(KEYS::CSTICK_DOWN);
}
void ButtonAFunction()
{
	SendNewSetting(KEYS::A);
}
void ButtonBFunction()
{
	SendNewSetting(KEYS::B);
}
void ButtonXFunction()
{
	SendNewSetting(KEYS::X);
}
void ButtonYFunction()
{
	SendNewSetting(KEYS::Y);
}
void ButtonStartFunction()
{
	SendNewSetting(KEYS::START);
}
void ButtonSelectFunction()
{
	SendNewSetting(KEYS::SELECT);
}
void ButtonBlueFunction()
{
	SendNewSetting(KEYS::BLUE);
}
void ButtonYellowFunction()
{
	SendNewSetting(KEYS::YELLOW);
}
void ButtonGreenFunction()
{
	SendNewSetting(KEYS::GREEN);
}
void ButtonSliderVolumeFunction()
{
	SendNewSetting(KEYS::SLIDER_VOLUME);
}
void ButtonSlider3DFunction()
{
	SendNewSetting(KEYS::SLIDER_3D);
}
void ButtonLidFunction()
{
	SendNewSetting(KEYS::LID);
}
void ButtonRedFunction()
{
	SendNewSetting(KEYS::RED);
}

WindowClass::WindowClass() : Window()
{
	m_screen = 0;

	AppendObject(new Button(m_screen, Rect(229, 1, 10, 10), "Close", ButtonCloseFunction));

	AppendObject(label_title = new Label(m_screen, Rect(24,0+3,128,10), "Config Settings"));
				
	static uint16_t lx, ly;
	lx = 9;
	ly = 15;
	AppendObject(button_l  = new Button(m_screen, Rect(lx,ly,10,10), "L", &ButtonLFunction));
#ifdef _3DS
	AppendObject(button_zl = new Button(m_screen, Rect(lx,ly+13,10,10), "ZL", &ButtonZLFunction));
#endif
		
	static uint16_t rx, ry;
	rx = 235;
	ry = ly;
	AppendObject(button_r  = new Button(m_screen, Rect(rx,ry,10,10), "R", &ButtonRFunction));
#ifdef _3DS
	AppendObject(button_zr = new Button(m_screen, Rect(rx,ry+13,10,10), "ZR", &ButtonZRFunction));
#endif

	static uint16_t dpad_x, dpad_y;
	dpad_x = 9;
	dpad_y = 80;
	AppendObject(button_left  = new Button(m_screen, Rect(dpad_x,      dpad_y + 15, 10, 10), "<", &ButtonLeftFunction));
	AppendObject(button_right = new Button(m_screen, Rect(dpad_x + 60, dpad_y + 15, 10, 10), ">", &ButtonRightFunction));
	AppendObject(button_up    = new Button(m_screen, Rect(dpad_x + 30, dpad_y,      10, 10), "^", &ButtonUpFunction));
	AppendObject(button_down  = new Button(m_screen, Rect(dpad_x + 30, dpad_y + 30, 10, 10), "v", &ButtonDownFunction));
	
#ifdef _3DS
	static uint16_t cpad_x, cpad_y;
	cpad_x = 30;
	cpad_y = 15;
	AppendObject(button_left  = new Button(m_screen, Rect(cpad_x,      cpad_y + 15, 10, 10), "CP <", &ButtonLeftCPadFunction));
	AppendObject(button_right = new Button(m_screen, Rect(cpad_x + 60, cpad_y + 15, 10, 10), "CP >", &ButtonRightCPadFunction));
	AppendObject(button_up    = new Button(m_screen, Rect(cpad_x + 30, cpad_y,      10, 10), "CP ^", &ButtonUpCPadFunction));
	AppendObject(button_down  = new Button(m_screen, Rect(cpad_x + 30, cpad_y + 30, 10, 10), "CP v", &ButtonDownCPadFunction));
	
	static uint16_t cstick_x, cstick_y;
	cstick_x = 135;
	cstick_y = 15;
	AppendObject(button_left  = new Button(m_screen, Rect(cstick_x,      cstick_y + 15, 10, 10), "CS <", &ButtonLeftCStickFunction));
	AppendObject(button_right = new Button(m_screen, Rect(cstick_x + 60, cstick_y + 15, 10, 10), "CS >", &ButtonRightCStickFunction));
	AppendObject(button_up    = new Button(m_screen, Rect(cstick_x + 30, cstick_y,      10, 10), "CS ^", &ButtonUpCStickFunction));
	AppendObject(button_down  = new Button(m_screen, Rect(cstick_x + 30, cstick_y + 30, 10, 10), "CS v", &ButtonDownCStickFunction));
#endif
	
	static uint16_t abxy_x, abxy_y;
	abxy_x = 175;
	abxy_y = 80;
	AppendObject(button_a = new Button(m_screen, Rect(abxy_x + 60, abxy_y + 15, 10, 10), "A", &ButtonAFunction));
	AppendObject(button_b = new Button(m_screen, Rect(abxy_x + 30, abxy_y + 30, 10, 10), "B", &ButtonBFunction));
	AppendObject(button_x = new Button(m_screen, Rect(abxy_x + 30, abxy_y,      10, 10), "X", &ButtonXFunction));
	AppendObject(button_y = new Button(m_screen, Rect(abxy_x     , abxy_y + 15, 10, 10), "Y", &ButtonYFunction));
	
	static uint16_t start_select_x, start_select_y;
	start_select_x = 85;
	start_select_y = 115;
	AppendObject(button_start  = new Button(m_screen, Rect(start_select_x + 60, start_select_y, 10, 10), "Start", &ButtonStartFunction));
	AppendObject(button_select = new Button(m_screen, Rect(start_select_x,     start_select_y, 10, 10), "Select", &ButtonSelectFunction));
	
#if defined(_3DS)
	static uint16_t sliders_x, sliders_y;
	sliders_x = 40;
	sliders_y = 130;
	AppendObject(button_slider_volume = new Button(m_screen, Rect(sliders_x,       sliders_y, 10, 10), "Volume", &ButtonSliderVolumeFunction));
	AppendObject(button_slider_3d     = new Button(m_screen, Rect(sliders_x + 100, sliders_y, 10, 10), "3D Slider", &ButtonSlider3DFunction));
#elif defined(_NDS)
	static uint16_t byrg_x, byrg_y;
	byrg_x = 40;
	byrg_y = 130;
	AppendObject(button_blue   = new Button(m_screen, Rect(byrg_x,       byrg_y, 10, 10), "Blue", &ButtonBlueFunction));
	AppendObject(button_yellow = new Button(m_screen, Rect(byrg_x + 50,  byrg_y, 10, 10), "Yellow", &ButtonYellowFunction));
	AppendObject(button_red    = new Button(m_screen, Rect(byrg_x + 100, byrg_y, 10, 10), "Red", &ButtonRedFunction));
	AppendObject(button_green  = new Button(m_screen, Rect(byrg_x + 150, byrg_y, 10, 10), "Green", &ButtonGreenFunction));
#endif

	static uint16_t lid_x, lid_y;
	lid_x = 115;
	lid_y = 145;
	AppendObject(button_lid  = new Button(m_screen, Rect(lid_x, lid_y, 10, 10), "Lid", &ButtonLidFunction));
}
WindowClass::~WindowClass() { }

}}}//namespace D2K::GUI::ConfigWindow
