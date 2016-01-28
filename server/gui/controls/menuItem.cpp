/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include <iostream>//std::cout, std::clog
#include "gui/gui.h"
#include "gui/resource.h"
#include "menuItem.h"

namespace D2K
{
	namespace GUI
	{
		MenuItem::MenuItem(std::string text) : Label(text, 0, 0, 0, 0)
		{
			hwnd = (HWND)NULL_VALUE;
		}
		MenuItem::~MenuItem() { }
		void MenuItem::Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam))
		{
			if(this && object->Attach(this))
			{
				events[object->GetID()].object = object;
				events[object->GetID()].function = function;
				events[object->GetID()].function2 = &GUI::voidFunction;
			}
		}
		void MenuItem::Append(Object *object)
		{
			MenuItem::Append(object, &voidFunction);
		}
		bool MenuItem::Attach(Object *parentObject)
		{
			if(Object::Attach(parentObject))
			{
				std::cout << "Parent:" << (unsigned int)GetParentHWND() << "\n";
				std::cout << "MenuItem:" << (unsigned int)hwnd << "\n";
				std::cout << "Text:" << Text << "\n";
				if(Text == "-")//if it's a seperator
					return AppendMenu((HMENU)GetParentHWND(), MF_SEPARATOR, ID, Text.c_str());

				return AppendMenu((HMENU)GetParentHWND(), MF_STRING, ID, Text.c_str());
			}
			return false;
		}
	}
}
