#pragma once

#include <string>//std::string
#include "gui/window.h"
#include "gui/edit.h"
#include "gui/label.h"

namespace D2K {namespace GUI {namespace Keypad {

class WindowClass : public GUI::Window {
public:
	WindowClass();
	~WindowClass();
};
extern GUI::Keypad::WindowClass* g_window;
extern std::string Entry(GUI::Label* label, GUI::Edit* edit, std::string text, int maxLength);

}}}//namespace D2K::GUI::Keypad
