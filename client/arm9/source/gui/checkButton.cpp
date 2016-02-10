#include <string>  // std::string
#include "checkButton.h"
#include "gui.h"

namespace D2K {namespace GUI {

CheckButton::CheckButton(uint8_t screen, GUI::Rect rect, std::string text, void (*function)()) : Button(screen, rect, text, function)
{
	SetChecked(false);
}

CheckButton::~CheckButton() { }

bool CheckButton::Draw()
{
	if(!Object::Draw())
		return false;

	if(IsVisible())//ifChanged()
	{
		Clear(Color[COLOR_BUTTON_BACKGROUND]);
		if(GetStatus() == OBJECT_STATUS::HELD)
			DrawRect(GetScreen(), GetRect(), Color[COLOR_BUTTON_OUTLINE_ACTIVE]);
		else if(GetChecked())
			DrawRect(GetScreen(), GetRect(), Color[COLOR_BUTTON_OUTLINE]);
		else
			DrawRect(GetScreen(), GetRect(), Color[COLOR_BUTTON_BACKGROUND]);//draws a flat button
		DrawString(GetScreen(), GetText(), GetRect().GetX()+3, GetRect().GetY()+3, Color[COLOR_BUTTON_TEXT]);
	}

	return true;
}
bool CheckButton::GetChecked()
{
	return m_checked;
}
void CheckButton::SetChecked(bool checked)
{
	SetUpdate(true);
	m_checked = checked;
}

}}//namespace D2K::GUI
