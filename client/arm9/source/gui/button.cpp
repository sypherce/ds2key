#include <string>	//std::string
#include "button.h"
#include "gui.h"

namespace D2K {
	namespace GUI {
		Button::Button(uint8_t screen, GUI::Rect rect, std::string text, void (*function)()) : Label(screen, rect, text) {
			GUI::Rect thisRect = GetRect();
			int width = text.length() * 6 + 4;	//calculate width of "std::string text"

			if(thisRect.GetW() < width)			//if button isn't big enough
				thisRect.SetW(width);			//adjust it to text's length
			SetRect(thisRect);					//and then reassign the Rect

			Button::Function = function;
		}

		Button::~Button() { }

		bool Button::Draw() {
			if(!Object::Draw())
				return false;

			if(IsVisible()) {
				Clear(Color[colorButtonBackground]);
				if(GetStatus() == ObjectStatusHeld)	//if we're active/clicked
					DrawRect(GetScreen(), GetRect(), Color[colorButtonOutlineActive]);
				else								//if we're idle
					DrawRect(GetScreen(), GetRect(), Color[colorButtonOutline]);
				DrawString(GetScreen(), GetText(), GetRect().GetX()+3, GetRect().GetY()+3, Color[colorButtonText]);
			}

			return true;
		}
	}
}
