#pragma once

#include "gui/window.h"
#include "common/enum_keys.h"
#include "common/udp.h"

namespace D2K {namespace GUI {namespace ConfigWindow {

class WindowClass : public GUI::Window
{
public:
	WindowClass();
	~WindowClass();

	void SetVisible(bool visible);
	bool Update();
};
extern bool g_config_type; // true = gamepad, false = keyboard
extern GUI::ConfigWindow::WindowClass g_window;
extern uint16_t current_pressed_key;

// TODO: organize/documentation
void ProcessInputSettingsPacket(UDP::DS2KeyInputSettingsPacket settings);

}}} // namespace D2K::GUI::ConfigWindow
