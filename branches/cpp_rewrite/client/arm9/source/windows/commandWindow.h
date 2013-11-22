#ifndef __COMMANDINDOW_H__
#define __COMMANDWINDOW_H__

#include "gui/window.h"

namespace D2K {
	namespace GUI {
		namespace Command {
			class WindowClass : public GUI::Window {
				public:
					WindowClass();
					~WindowClass();
			};
			extern GUI::Command::WindowClass *Window;
		}
	}
}

#endif//__COMMANDWINDOW_H__
