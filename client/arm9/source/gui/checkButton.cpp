#include <string>	//std::string
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
		Clear(Color[colorButtonBackground]);
		if(GetStatus() == OBJECT_STATUS::HELD)
			DrawRect(GetScreen(), GetRect(), Color[colorButtonOutlineActive]);
		else if(GetChecked())
			DrawRect(GetScreen(), GetRect(), Color[colorButtonOutline]);
		else
			DrawRect(GetScreen(), GetRect(), Color[colorButtonBackground]);//draws a flat button
		DrawString(GetScreen(), GetText(), GetRect().GetX()+3, GetRect().GetY()+3, Color[colorButtonText]);
	}

	return true;
}
bool CheckButton::GetChecked()
{
	return Checked;
}
void CheckButton::SetChecked(bool checked)
{
	SetUpdate(true);
	CheckButton::Checked = checked;
}

}}//namespace D2K::GUI
