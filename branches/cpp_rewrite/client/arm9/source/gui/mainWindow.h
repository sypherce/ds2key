#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "window.h"
#include "edit.h"
#include "button.h"
#include "label.h"

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
