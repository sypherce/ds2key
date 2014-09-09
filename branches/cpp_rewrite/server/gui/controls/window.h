#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <windows.h>
#include <string>//std::string
#include "object.h"

namespace D2K {
	namespace GUI {
		class Window : public Object {
			public:
				Window();
				~Window();
				void Append(Object *object, void (*function)(HWND, UINT, WPARAM, LPARAM));
				void Append(Object *object);
				bool Setup(HINSTANCE hThisInstance, std::string ClassName, std::string Title, int Width, int Height);
				void SetVisible(int visible);
				void SetVisible(bool visible = true);
				void CreateMainMenu();
				void SetText(std::string text);
			private:
				std::string Text;
		};
	}
}
#endif//__WINDOW_H__