#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "gui/window.h"

namespace D2K {
	namespace GUI {
		namespace Main {
			class WindowClass : public GUI::Window {
				public:
					WindowClass();
					~WindowClass();
					bool Update();
			};
			extern GUI::Main::WindowClass *Window;
		}
	}
}

#endif//__MAINWINDOW_H__
