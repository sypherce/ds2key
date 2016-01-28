#pragma once

#include "gui/window.h"

namespace D2K {namespace GUI {namespace Command {

class WindowClass : public GUI::Window
{
	public:
		WindowClass();
		~WindowClass();

		void SetVisible(bool visible);
};
extern GUI::Command::WindowClass* g_window;

}}}//namespace D2K::GUI::Command
