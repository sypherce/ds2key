/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>//printf
#include "gui/gui.h"
#include "gui/resource.h"
#include "core/core.h"//Print
#include "menuPopUp.h"

namespace D2K {
	namespace GUI {
		MenuPopUp::MenuPopUp(std::string text) : Label(text, 0, 0, 0, 0) {
				hwnd = (HWND)CreatePopupMenu();}
		MenuPopUp::~MenuPopUp() {}
		void MenuPopUp::Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)) {
			if(this && object->Attach(this)) {
				events[object->GetID()].object = object;
				events[object->GetID()].function = function;
				events[object->GetID()].function2 = &GUI::voidFunction;
			}
		}
		void MenuPopUp::Append(Object *object) {
			MenuPopUp::Append(object, &voidFunction);
		}
		bool MenuPopUp::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				printf("Parent:%x\n", (unsigned int)GetParentHWND());
				printf("menupopup:%x\n", (unsigned int)hwnd);
				printf("text:%s\n", Text.c_str());
				return AppendMenu((HMENU)GetParentHWND(), MF_STRING | MF_POPUP, (UINT)hwnd, Text.c_str());
			}
			return false;
		}
	}
}
