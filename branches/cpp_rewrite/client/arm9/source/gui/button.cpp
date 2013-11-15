#include <string>	//std::string
#include "button.h"
#include "gui.h"

namespace D2K {
	namespace GUI {
		Button::Button(uint8_t screen, GUI::Rect rect, std::string text, void (*function)()) : Label(screen, rect, text) {
			GUI::Rect thisRect = GetRect();
			int width = text.length() * 6 + 4;
			if(thisRect.GetW() < width)
				thisRect.SetW(width);
			SetRect(thisRect);
			Button::Function = function;
			SetVisible(true);
		}

		Button::~Button() { }

		bool Button::Draw() {
			if(!Object::Draw())
				return false;

			if(IsVisible()) {//ifChanged()
				Clear(Color[colorButtonBackground]);
				if(GetStatus() == 2)
					DrawRect(GetScreen(), GetRect(), Color[colorButtonOutlineActive]);
				else
					DrawRect(GetScreen(), GetRect(), Color[colorButtonOutline]);
				DrawString(GetScreen(), GetText(), GetRect().GetX()+3, GetRect().GetY()+3, Color[colorButtonText]);
			}

			return true;
		}
	}
}
