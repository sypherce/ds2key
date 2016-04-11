#pragma once

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "label.h"

namespace D2K {namespace GUI {

class MenuPopUp : public Label
{
public:
	MenuPopUp(std::string text);
	~MenuPopUp();
	void Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam));
	void Append(Object *object);
	bool Attach(Object *parentObject);
};

}}//namespace D2K::GUI
