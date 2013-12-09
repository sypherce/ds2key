/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include "gui/gui.h"
#include "gui/resource.h"
#include "comboButton.h"

namespace D2K {
	namespace GUI {
		ComboButton::ComboButton(int x, int y, int width, int height) : Label("", x, y, width, height) {}
		ComboButton::~ComboButton() {}
		bool ComboButton::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				hwnd = CreateWindowEx(
						   WS_EX_CLIENTEDGE,
						   "COMBOBOX", "",
						   WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
						   X, Y, Width, Height,
						   GetParentHWND(),
						   (HMENU)ID,
						   GetModuleHandle(NULL),
						   0);
				SetFont(hwnd);
				return true;
			}
			return false;
		}
		void ComboButton::Append(std::string text) {
			SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)text.c_str());
			if(SendMessage(hwnd, CB_GETCOUNT, 0, 0) == 1) SetSelection(0);
		}
		void ComboButton::SetSelection(unsigned row) {
			SendMessage(hwnd, CB_SETCURSEL, row, 0);
		}
		int ComboButton::GetSelection() {
			int message = SendMessage(hwnd, CB_GETCURSEL, 0, 0);
			if(message == CB_ERR)//why is CB_ERR different?
				return 0;
			return message;
		}
	}
}
