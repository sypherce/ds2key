#ifndef __TURBOWINDOW_H__
#define __TURBOWINDOW_H__

#include "gui/window.h"

namespace D2K {
	namespace GUI {
		namespace Turbo {
			class WindowClass : public GUI::Window {
				public:
					WindowClass();
					~WindowClass();
			};
			extern uint16_t GetKeys();
			extern uint8_t GHGetKeys();
			extern GUI::Turbo::WindowClass *Window;
		}
	}
}

#endif//__TURBOWINDOW_H__
