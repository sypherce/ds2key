#include <string>	//std::string
#include "button.h"

namespace D2K {
	namespace GUI {
		Button::Button(uint8_t Screen, GUI::Rect Rect, std::string String, void (*Function)()) : Label(Screen, Rect, String) {
			GUI::Rect thisRect = getRect();
			int width = String.length() * 6 + 4;
			if(thisRect.getW() < width)
				thisRect.setW(width);
			setRect(thisRect);
			Button::Function = Function;
			setVisible(true);
		}

		Button::~Button() { }

		bool Button::Draw() {
			if(!Object::Draw())
				return false;

			if(isVisible()) {//ifChanged()
				Clear(Color[colorButtonBackground]);
				if(getStatus() == 2)
					DrawRect(getScreen(), getRect(), Color[colorButtonOutlineActive]);
				else
					DrawRect(getScreen(), getRect(), Color[colorButtonOutline]);
				DrawString(getScreen(), getText(), getRect().getX()+3, getRect().getY()+3, Color[colorButtonText]);
			}

			return true;
		}
	}
}
