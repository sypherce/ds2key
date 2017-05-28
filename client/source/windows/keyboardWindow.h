#pragma once

#include <string> // std::string
#include "gui/window.h"
#include "gui/edit.h"
#include "gui/label.h"

namespace D2K {namespace GUI {namespace Keyboard {

class WindowClass : public GUI::Window
{
public:
	WindowClass();
	~WindowClass();
};
extern GUI::Keyboard::WindowClass g_window;
extern std::string GetString(GUI::Label* label, GUI::Edit* edit, std::string text, int maxLength);
// true = button, false = key
extern uint16_t GetButtonOrKey(bool button_or_key);

}}} // namespace D2K::GUI::Keyboard
