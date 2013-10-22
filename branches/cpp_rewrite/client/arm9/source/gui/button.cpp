#include <string>	//std::string
#include "button.h"

namespace D2K {
	namespace GUI {
		Button::Button(uint8_t Screen, GUI::Rect Rect, std::string String, void (*function)()) : Label(Screen, Rect, String) {
			Button::function = function;
			Object::Type = ObjectButton;
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
		void Button::setStatus(uint8_t Value) {
			Status = Value;
			setUpdate(true);
			Draw();
		}
	}
}
