#pragma once

#include "gui/window.h"

namespace D2K {namespace GUI {namespace Turbo {

class WindowClass : public GUI::Window
{
public:
	WindowClass();
	~WindowClass();

	void SetVisible(bool visible);
	bool Update();
};
extern uint32_t GetKeys();
extern GUI::Turbo::WindowClass g_window;

}}} // namespace D2K::GUI::Turbo
