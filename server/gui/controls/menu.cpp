/*
Windows GUI
*/

#include <iostream>//std::cout, std::clog
#include <windows.h>
#include <commctrl.h>
#include "gui/gui.h"
#include "gui/resource.h"
#include "menu.h"

namespace D2K
{
namespace GUI
{
Menu::Menu() : Label("", 0, 0, 0, 0)
{
	hwnd = (HWND)CreateMenu();
}
Menu::~Menu() { }
void Menu::Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam))
{
	if(this && object->Attach(this))
	{
		events[object->GetID()].object = object;
		events[object->GetID()].function = function;
		events[object->GetID()].function2 = &GUI::voidFunction;
	}
}
void Menu::Append(Object *object)
{
	Menu::Append(object, &voidFunction);
}
bool Menu::Attach(Object *parentObject)
{
	if(Object::Attach(parentObject))
	{
		std::cout << "Parent:#" << (unsigned int)GetParentHWND() << "\n";
		std::cout << "Menu:#" << (unsigned int)hwnd << "\n";
		return Update();
	}
	return false;
}
bool Menu::Update()
{
	std::cout << "Parent:#" << (unsigned int)GetParentHWND() << "\n";
	std::cout << "Menu:#" << (unsigned int)hwnd << "\n";
	return SetMenu(GetParentHWND(), (HMENU)hwnd);
}

}
}
