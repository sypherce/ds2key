#pragma once

#include "gui/window.h"
#include "common/udp.h"

namespace D2K {namespace GUI {namespace Settings {

class WindowClass : public GUI::Window
{
public:
	WindowClass();
	~WindowClass();

	void SetVisible(bool visible);
	bool Update();
};
extern GUI::Settings::WindowClass g_window;

}}} // namespace D2K::GUI::Command
