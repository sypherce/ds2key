/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>//printf
#include "gui/gui.h"
#include "gui/resource.h"
#include "core/core.h"//Print
#include "menu.h"

namespace D2K {
	namespace GUI {
		Menu::Menu() : Label("", 0, 0, 0, 0) {
				hwnd = (HWND)CreateMenu();}
		Menu::~Menu() {}
		void Menu::Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)) {
			if(this && object->Attach(this)) {
				events[object->GetID()].object = object;
				events[object->GetID()].function = function;
				events[object->GetID()].function2 = &GUI::voidFunction;
			}
		}
		void Menu::Append(Object *object) {
			Menu::Append(object, &voidFunction);
		}
		bool Menu::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				printf("Parent:%x\n", (unsigned int)GetParentHWND());
				printf("menu:%x\n", (unsigned int)hwnd);
				return Update();
			}
			return false;
		}
		bool Menu::Update() {
				printf("Parent:%x\n", (unsigned int)GetParentHWND());
				printf("menu:%x\n", (unsigned int)hwnd);
			return SetMenu(GetParentHWND(), (HMENU)hwnd);
		}
	}
}
