#ifndef __MENUPOPUP_H__
#define __MENUPOPUP_H__

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "label.h"
using std::string;

namespace D2K {
	namespace GUI {
		class MenuPopUp : public Label {
			public:
				MenuPopUp(string text);
				~MenuPopUp();
				void Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam));
				void Append(Object *object);
				bool Attach(Object *parentObject);
		};
	}
}
#endif//__MENUPOPUP_H__
