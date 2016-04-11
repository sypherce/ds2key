/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include "gui/gui.h"
#include "gui/resource.h"
#include "edit.h"

namespace D2K {namespace GUI {

Edit::Edit(std::string text, int x, int y, int width, int height) : Label(text, x, y, width, height) { }
Edit::~Edit() { }
bool Edit::Attach(Object *parentObject)
{
	if(Object::Attach(parentObject))
	{
		hwnd = CreateWindowEx(
					WS_EX_CLIENTEDGE,
					"EDIT", Text.c_str(),
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
					X, Y, Width, Height,
					GetParentHWND(),
					(HMENU)ID,
					GetModuleHandle(NULL_VALUE),
					0);
		SetFont(hwnd);
		return true;
	}
	return false;
}

}}//namespace D2K::GUI

