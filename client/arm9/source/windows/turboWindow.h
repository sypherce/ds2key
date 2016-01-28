#pragma once

#include "gui/window.h"

namespace D2K {namespace GUI {namespace Turbo {

class WindowClass : public GUI::Window {
	public:
		WindowClass();
		~WindowClass();
};
extern uint16_t GetKeys();
extern uint8_t GHGetKeys();
extern GUI::Turbo::WindowClass* g_window;

}}}//namespace D2K::GUI::Turbo
