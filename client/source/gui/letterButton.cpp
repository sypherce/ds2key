#include <string> // std::string
#include "letterButton.h"
#include "gui.h"

namespace D2K {namespace GUI {

LetterButton::LetterButton(uint8_t screen, GUI::Rect rect, char letter, std::string text, std::string sub_text, void (*function)()) : Label(screen, rect, text)
{
	GUI::Rect thisRect = GetRect();
	int width = text.length() * 6 + 4; // Calculate width of "std::string text"

	if(thisRect.GetW() < width)        // If button isn't big enough
		thisRect.SetW(width);      // Adjust it to text's length
	SetRect(thisRect);                 // Then reassign the Rect
	
	LetterButton::m_sub_text = sub_text;
	LetterButton::m_letter = letter;
	LetterButton::Function = function;
}

LetterButton::~LetterButton() { }

bool LetterButton::Draw()
{
	if(!Object::Draw())
		return false;

	if(IsVisible())
	{
		Clear(Color[COLOR_BUTTON_BACKGROUND]);

		if(GetStatus() == OBJECT_STATUS::HELD) // If we're active/clicked
			DrawRect(GetScreen(), GetRect(), Color[COLOR_BUTTON_OUTLINE_ACTIVE]);
		else                                   // If we're idle
			DrawRect(GetScreen(), GetRect(), Color[COLOR_BUTTON_OUTLINE]);

		Rect button_image_rect = GetRect();
		button_image_rect.SetX(button_image_rect.GetX() + (10));
		button_image_rect.SetY(button_image_rect.GetY() + (10));

		char letter[2]{};
		letter[0] = LetterButton::m_letter;
		DrawButtonImage(GetScreen(), (char*)letter, button_image_rect);
		
		DrawString(GetScreen(), GetText(), TTF::FONT_SIZE_TITLE, TTF::FONT_BOLD,
			GetRect().GetX() + 3, GetRect().GetY() + 3, Color[COLOR_BUTTON_TEXT]);
		DrawString(GetScreen(), GetSubText(), TTF::FONT_SIZE_REGULAR, TTF::FONT_REGULAR,
			GetRect().GetX() + 15, GetRect().GetY() + 10, Color[COLOR_BUTTON_TEXT]);
	}

	return true;
}
void LetterButton::SetSubText(std::string sub_text)
{
	LetterButton::SetUpdate(true);
	m_sub_text = sub_text;
}
std::string LetterButton::GetSubText()
{
	return m_sub_text;
}

}} // namespace D2K::GUI
