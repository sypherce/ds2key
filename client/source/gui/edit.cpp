#include <string>  // std::string
#include "edit.h"
#include "gui.h"

namespace D2K {namespace GUI{

Edit::Edit(uint8_t screen, GUI::Rect rect, std::string text, void (*function)()) : Button(screen, rect, text, function) { }

Edit::~Edit() { }

bool Edit::Draw()
{
	if(!Object::Draw())
		return false;
	if(IsVisible())
	{
		Clear(Color[COLOR_EDIT_BACKGROUND]);
		if(GetStatus() == OBJECT_STATUS::HELD)
			DrawRect(GetScreen(), GetRect(), Color[COLOR_EDIT_OUTLINE_ACTIVE]);
		else
			DrawRect(GetScreen(), GetRect(), Color[COLOR_EDIT_OUTLINE]);
		DrawString(GetScreen(), GetText(), GetRect().GetX()+3, GetRect().GetY()+3, Color[COLOR_EDIT_TEXT]);
	}

	return true;
}

}}//namespace D2K::GUI
