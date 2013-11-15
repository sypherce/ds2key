#ifndef __IPINDOW_H__
#define __IPWINDOW_H__

#include "window.h"

namespace D2K {
	namespace GUI {
		namespace IP {
			class WindowClass : public GUI::Window {
				public:
					WindowClass();
					~WindowClass();
					bool Update();
			};
			extern GUI::IP::WindowClass *Window;
		}
	}
}

#endif//__IPWINDOW_H__
