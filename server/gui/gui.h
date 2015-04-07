#ifndef __GUI_H__
#define __GUI_H__

#include <windows.h>
#include <string>//std::string
#include "controls/button.h"
#include "controls/checkButton.h"
#include "controls/comboButton.h"
#include "controls/edit.h"
#include "controls/label.h"
#include "controls/listView.h"
#include "controls/menu.h"
#include "controls/menuItem.h"
#include "controls/menuPopUp.h"
#include "controls/object.h"
#include "controls/radioButton.h"
#include "controls/statusBar.h"
#include "controls/trayIcon.h"
#include "controls/window.h"

namespace D2K {
	namespace GUI {
		extern void voidFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		extern HINSTANCE hThisInstance;
		extern LPSTR lpszArgument;
		extern int eventCounter;
		const int eventMax = 200;
		extern bool locked;

		void SetFont(HWND hwnd);

		struct Event {
			D2K::GUI::Object *object;
			void (*function)(HWND, UINT, WPARAM, LPARAM);
			void (*function2)(HWND, UINT, WPARAM, LPARAM);
		};
		extern Event events[D2K::GUI::eventMax];

		void Setup();

		/*  Make the class name into a global variable  */

		extern MSG messages;            /* Here messages to the application are saved */

		/*  This function is called by the Windows function DispatchMessage()  */
		WPARAM GetMessages();

		LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	}
}
#endif//__GUI_H__
