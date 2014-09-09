#ifndef __MENU_H__
#define __MENU_H__

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "label.h"

namespace D2K {
	namespace GUI {
		class Menu : public Label {
			public:
				Menu();
				~Menu();
				void Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam));
				void Append(Object *object);
				bool Attach(Object *parentObject);
				bool Update();
		};
	}
}
#endif//__MENU_H__