#pragma once

#include "gui/window.h"
#include "common/enum_keys.h"

namespace D2K {namespace GUI {namespace ConfigWindow {

class WindowClass : public GUI::Window
{
public:
	WindowClass();
	~WindowClass();
};
extern bool g_config_type;//true = gamepad, false = keyboard
extern GUI::ConfigWindow::WindowClass g_window;
extern uint16_t current_pressed_key;

}}}//namespace D2K::GUI::ConfigWindow
