#pragma once

#include "gui/window.h"
#include "common/udp.h"

namespace D2K {namespace GUI {namespace Touch {

class WindowClass : public GUI::Window
{
public:
	WindowClass();
	~WindowClass();
	
	void SetVisible(bool visible);
	bool Update();
};
extern GUI::Touch::WindowClass g_window;

}}} // namespace D2K::GUI::Touch
