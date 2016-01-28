/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include <iostream>//std::cout, std::clog
#include "gui/gui.h"
#include "gui/resource.h"
#include "menuPopUp.h"

namespace D2K {namespace GUI {
MenuPopUp::MenuPopUp(std::string text) : Label(text, 0, 0, 0, 0)
{
	hwnd = (HWND)CreatePopupMenu();
}
MenuPopUp::~MenuPopUp() { }
void MenuPopUp::Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam))
{
	if(this && object->Attach(this))
	{
		events[object->GetID()].object = object;
		events[object->GetID()].function = function;
		events[object->GetID()].function2 = &GUI::voidFunction;
	}
}
void MenuPopUp::Append(Object *object)
{
	MenuPopUp::Append(object, &voidFunction);
}
bool MenuPopUp::Attach(Object *parentObject)
{
	if(Object::Attach(parentObject))
	{
		std::cout << "Parent:" << (unsigned int)GetParentHWND() << "\n";
		std::cout << "MenuItem:" << (unsigned int)hwnd << "\n";
		std::cout << "Text:" << Text << "\n";
		return AppendMenu((HMENU)GetParentHWND(), MF_STRING | MF_POPUP, (UINT)hwnd, Text.c_str());
	}
	return false;
}

}}//namespace D2K::GUI
