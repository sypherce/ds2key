#include <string> // std::string

// windows
#include "touchWindow.h"
#include "barWindow.h"
#include "turboWindow.h"

// controls
#include "gui/button.h"

// core
#include "core.h"
#include "common/udp.h"

namespace D2K {namespace GUI {namespace Touch {

WindowClass g_window;
Label* label_1{};

WindowClass::WindowClass() : Window()
{
	m_screen = 0;
	
	AppendObject(label_1 = new Label(m_screen, Rect(2, 2, 61, 42), "Press L+R+START+SELECT to unlock screen"));
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
	            &D2K::g_slider_volume_status, &D2K::g_slider_3d_status, 0);

	return Window::Update()
	    || Bar::g_window.Update();
}

}}} // namespace D2K::GUI::Touch
