#pragma once

#include "gui/window.h"

namespace D2K {namespace GUI {namespace IP {

class WindowClass : public GUI::Window
{
public:
	WindowClass();
	~WindowClass();
	bool Update();
};
extern GUI::IP::WindowClass *Window;

}}}//namespace D2K::GUI::IP
