#include <string> // std::string

// windows
#include "touchWindow.h"
#include "barWindow.h"
#include "turboWindow.h"
#include "keyboardWindow.h"

// controls
#include "gui/button.h"

// core
#include "core.h"
#include "common/udp.h"

namespace D2K {namespace GUI {namespace Touch {

WindowClass g_window;
Label* label_1{};
Button* button_left{};
Button* button_right{};

uint16_t key_press{};

extern void ButtonLeftFunction();
extern void ButtonRightFunction();

WindowClass::WindowClass() : Window()
{
	m_screen = 0;

	AppendObject(label_1 = new Label(m_screen, Rect(2, 2, 61, 10), "Press L+R+START+SELECT to unlock screen"));

	int button_width = 124;
	AppendObject(button_left = new Button(m_screen, Rect(2, 8, button_width, 12), "Left Click", &ButtonLeftFunction));
	AppendObject(button_right = new Button(m_screen, Rect(button_width + 4, 8, button_width + 1, 12), "Right Click", &ButtonRightFunction));
}
WindowClass::~WindowClass() { }
void WindowClass::SetVisible(bool visible)
{
	Window::SetVisible(visible);

	if(!visible)
		return;

	Bar::SetText("[Touch Mode]");
	Bar::g_window.SetVisible(true);

	ForceBacklightsOn(false); // Let backlights shut off
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
		Touch::g_window.SetVisible(false);

// TODO: this return value needs inspected
		return 0;
	}

	UDP::Update(D2K::g_keys_held, Turbo::GetKeys(), &D2K::g_stylus_position,
	            &D2K::g_circle_position, &D2K::g_cstick_position,
	            &D2K::g_accel_status, &D2K::g_gyro_status,
	            &D2K::g_slider_volume_status, &D2K::g_slider_3d_status, key_press);

	key_press = 0;

	return Window::Update()
	    || Bar::g_window.Update();
}

void ButtonLeftFunction()
{
	key_press = Keyboard::WINKEY_LBUTTON;
}

void ButtonRightFunction()
{
	key_press = Keyboard::WINKEY_RBUTTON;
}

}}} // namespace D2K::GUI::Touch
