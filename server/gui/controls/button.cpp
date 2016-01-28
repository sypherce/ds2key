#include "button.h"
#include "../gui.h"

namespace D2K
{
namespace GUI
{
Button::Button(std::string text, int x, int y, int width, int height) : Label(text, x, y, width, height) { }
Button::~Button() { }
bool Button::Attach(Object *parentObject)
{
	if(Object::Attach(parentObject))
	{
		hwnd = CreateWindow (
					"BUTTON", Text.c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
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
}

}
