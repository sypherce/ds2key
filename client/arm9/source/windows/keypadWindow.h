#ifndef __KEYPADWINDOW_H__
#define __KEYPADWINDOW_H__

#include <string>//std::string
#include "gui/window.h"
#include "gui/edit.h"
#include "gui/label.h"

namespace D2K {
	namespace GUI {
		namespace Keypad {
			class WindowClass : public GUI::Window {
				public:
					WindowClass();
					~WindowClass();
			};
			extern GUI::Keypad::WindowClass *Window;
			extern std::string Entry(GUI::Label *label, GUI::Edit *edit, std::string text, int maxLength);
		}
	}
}

#endif//__KEYPADWINDOW_H__
