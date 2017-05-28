#include <string> // std::string

// windows
#include "configWindow.h"
#include "barWindow.h"
#include "keyboardWindow.h"
#include "turboWindow.h"

// controls
#include "gui/Button.h"
#include "gui/label.h"

// core
#include "core.h"
#include "common/enum_keys.h"
#include "common/key.h"
#include "common/udp.h"
#include "config.h"

#ifdef _3DS
#include "gui/gui.h"
#endif

// TODO: Remove
#include "common/misc.h"

namespace D2K {namespace GUI {namespace ConfigWindow {

uint16_t current_pressed_key = Key::KEY_NONE;
bool g_config_type{};

WindowClass g_window;
Button* button_slider_volume;
Button* button_slider_3d;
Button* button_lid;

Label* instructions_label;
Label* label[KEYS::KEYS_BUTTON_COUNT];

void ProcessInputSettingsPacket(UDP::DS2KeyInputSettingsPacket settings)
{
	for(int KeyCounter = KEYS::_START_OF_BUTTONS_; KeyCounter <= KEYS::_END_OF_BUTTONS_; KeyCounter++)
	{
		int SettingsCounter = KeyCounter - KEYS::_START_OF_BUTTONS_;

		std::string temporary_string = D2K::Key::GetString(settings.value[SettingsCounter]);
		if(temporary_string.find("KEY_NONE") == 0)
			temporary_string = "(None)";
		else if(temporary_string.find("KEY_") == 0)
			temporary_string.erase(0, 3);
		temporary_string = KEYS::GetKeyName(KeyCounter) + ": " + temporary_string;

		label[SettingsCounter]->SetText(temporary_string);
	}
}

void SendNewSetting(uint16_t setting, uint16_t value)
{
	UDP::DS2KeySingleInputSettingPacket settings = UDP::DS2KeySingleInputSettingPacket{};
	settings.profile = UDP::GetProfile();
	settings.type = UDP::PACKET::SINGLE_INPUT_SETTING;
	settings.setting = setting;
	settings.value = value;

	// Send settings packet
	UDP::SendNormalSetting(settings);
}

void SendNewSetting(uint16_t setting)
{
	uint16_t value = Keyboard::GetButtonOrKey(g_config_type);
	if(value != Key::KEY_NONE)
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

	AppendObject(new Button(m_screen, Rect(220, 1, 10, 10), "Close", ButtonCloseFunction));
	AppendObject(instructions_label = new Label(m_screen, Rect(15, 20, 55, 10), "Press a button to configure."));


	static uint16_t sliders_x, sliders_y;
	sliders_x = 40;
	sliders_y = 130;
	AppendObject(button_slider_volume = new Button(m_screen, Rect(sliders_x,       sliders_y, 10, 10), "Volume", &ButtonSliderVolumeFunction));
	AppendObject(button_slider_3d     = new Button(m_screen, Rect(sliders_x + 100, sliders_y, 10, 10), "3D Slider", &ButtonSlider3DFunction));

	static uint16_t lid_x, lid_y;
	lid_x = 115;
	lid_y = 145;
	AppendObject(button_lid  = new Button(m_screen, Rect(lid_x, lid_y, 10, 10), "Lid", &ButtonLidFunction));

	for(int i = 0; i < KEYS::KEYS_BUTTON_COUNT; i++)
	{
		static uint16_t x = 0;
		static uint16_t y = 0;

		AppendObject(label[i] = new Label(m_screen, Rect(x, y, 35, 10), ""));
		y += 10;
		if(y > 165)
		{
			y = 0;
			x += 128;
		}
	}
}
WindowClass::~WindowClass() { }

void WindowClass::SetVisible(bool visible)
{
	if(visible)
		UDP::RequestInputSettings();

	Window::SetVisible(visible);

	if(!visible)
		return;

	Window::SetVisible(visible);
	Bar::SetText(g_config_type ? "[Gamepad]" : "[Keyboard]");
	Bar::g_window.SetVisible(true);

	ForceBacklightsOn(true);          // Lock backlights on
}
bool WindowClass::Update()
{
	static uint8_t last_slider_volume_status = g_slider_volume_status;
	static uint8_t last_slider_3d_status = g_slider_3d_status;
	
	UDP::Update(0, 0, nullptr,
	            nullptr, nullptr,
	            nullptr, nullptr,
	            nullptr, nullptr,
	            0);
	ConfigWindow::current_pressed_key = Key::KEY_NONE;

	if(g_keys_down&KEY_L)
		ButtonLFunction();
#ifdef _3DS
	else if(g_keys_down&KEY_ZL)
		ButtonZLFunction();
#endif
	else if(g_keys_down&KEY_R)
		ButtonRFunction();
#ifdef _3DS
	else if(g_keys_down&KEY_ZR)
		ButtonZRFunction();
#endif
	else if(g_keys_down&KEY_LEFT)
		ButtonLeftFunction();
	else if(g_keys_down&KEY_RIGHT)
		ButtonRightFunction();
	else if(g_keys_down&KEY_UP)
		ButtonUpFunction();
	else if(g_keys_down&KEY_DOWN)
		ButtonDownFunction();
#ifdef _3DS
	else if(g_keys_down&KEY_CPAD_LEFT)
		ButtonLeftCPadFunction();
	else if(g_keys_down&KEY_CPAD_RIGHT)
		ButtonRightCPadFunction();
	else if(g_keys_down&KEY_CPAD_UP)
		ButtonUpCPadFunction();
	else if(g_keys_down&KEY_CPAD_DOWN)
		ButtonDownCPadFunction();
	else if(g_keys_down&KEY_CSTICK_LEFT)
		ButtonLeftCStickFunction();
	else if(g_keys_down&KEY_CSTICK_RIGHT)
		ButtonRightCStickFunction();
	else if(g_keys_down&KEY_CSTICK_UP)
		ButtonUpCStickFunction();
	else if(g_keys_down&KEY_CSTICK_DOWN)
		ButtonDownCStickFunction();
#endif
	else if(g_keys_down&KEY_A)
		ButtonAFunction();
	else if(g_keys_down&KEY_B)
		ButtonBFunction();
	else if(g_keys_down&KEY_X)
		ButtonXFunction();
	else if(g_keys_down&KEY_Y)
		ButtonYFunction();
	else if(g_keys_down&KEY_START)
		ButtonStartFunction();
	else if(g_keys_down&KEY_SELECT)
		ButtonSelectFunction();
#if defined(_3DS)
	else if(last_slider_volume_status != g_slider_volume_status)
	{
		ButtonSliderVolumeFunction();
		last_slider_volume_status = g_slider_volume_status;
	}
	else if(last_slider_3d_status != g_slider_3d_status)
	{
		ButtonSlider3DFunction();
		last_slider_3d_status = g_slider_3d_status;
	}
#elif defined(_NDS)
	else if(g_keys_down&KEY_BLUE)
		ButtonBlueFunction();
	else if(g_keys_down&KEY_YELLOW)
		ButtonYellowFunction();
	else if(g_keys_down&KEY_RED)
		ButtonRedFunction();
	else if(g_keys_down&KEY_GREEN)
		ButtonGreenFunction();
#endif
	else if(g_keys_down&KEY_LID)
		ButtonLidFunction();

	return Window::Update()
	    || Bar::g_window.Update();
}

}}} // namespace D2K::GUI::ConfigWindow
