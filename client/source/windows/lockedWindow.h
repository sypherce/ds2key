#pragma once

#include "gui/window.h"

namespace D2K {namespace GUI {namespace Locked {

class WindowClass : public GUI::Window
{
public:
	WindowClass();
	~WindowClass();

	void SetVisible(bool visible);

	bool Update();
};
extern GUI::Locked::WindowClass g_window;

}}} // namespace D2K::GUI::Command
