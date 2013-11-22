/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>//printf
#include "gui/gui.h"
#include "gui/resource.h"
#include "core/core.h"//Print
#include "statusBar.h"

namespace D2K {
	namespace GUI {
		StatusBar::StatusBar(std::string text, int x, int y, int width, int height) : Label(text, x, y, width, height) {}
		StatusBar::~StatusBar() {}
		bool StatusBar::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				hwnd = CreateWindow (
						   STATUSCLASSNAME, "",
						   WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
						   X, Y, Width, Height,
						   GetParentHWND(),
						   (HMENU)ID,
						   GetModuleHandle(NULL),
						   0);
				SetFont(hwnd);
				SetText(Text);
				return true;
			}
			return false;
		}
		void StatusBar::SetText(std::string Text) {
			SendMessage(hwnd, SB_SETTEXT, 0, (LPARAM)Text.c_str());

			return;
		}
	}
}
