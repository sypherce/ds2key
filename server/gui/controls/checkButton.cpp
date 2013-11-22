#include "checkButton.h"
#include "../gui.h"

namespace D2K {
	namespace GUI {
		CheckButton::CheckButton(std::string text, int x, int y, int width, int height) : Label(text, x, y, width, height) {}
		CheckButton::~CheckButton() {}
		bool CheckButton::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				hwnd = CreateWindow (
						   "BUTTON", Text.c_str(),
						   WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
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
		void CheckButton::SetChecked(bool checked) {
			SendMessage(hwnd, BM_SETCHECK, (WPARAM)checked, 0);
		}
	}
}
