#ifndef __TURBOWINDOW_H__
#define __TURBOWINDOW_H__

#include "window.h"
#include "edit.h"
#include "button.h"
#include "checkButton.h"
#include "label.h"

namespace D2K {
	namespace GUI {
		namespace Turbo {
			class WindowClass : public GUI::Window {
				public:
					WindowClass();
					~WindowClass();
			};
			extern uint16_t GetKeys();
			extern GUI::Turbo::WindowClass *Window;
		}
	}
}

#endif//__TURBOWINDOW_H__
