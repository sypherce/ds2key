#pragma once

#include "gui/window.h"

namespace D2K {namespace GUI {namespace Main {

class WindowClass : public GUI::Window
{
public:
	WindowClass();
	~WindowClass();

	bool Update();
};
extern GUI::Main::WindowClass g_window;

}}} // namespace D2K::GUI::Main
