#include <string>	//std::string
#include "checkButton.h"

namespace D2K {
	namespace GUI {
		CheckButton::CheckButton(uint8_t Screen, GUI::Rect Rect, std::string String, void (*function)()) : Label(Screen, Rect, String) {
			CheckButton::function = function;
			Object::Type = ObjectCheckButton;
			setVisible(true);
			setChecked(false);
		}

		CheckButton::~CheckButton() { }

		bool CheckButton::Draw() {
			if(!Object::Draw())
				return false;
			if(isVisible()) {//ifChanged()
				Clear(Color[colorButtonBackground]);
				if(getChecked())
					DrawRect(getScreen(), getRect(), Color[colorButtonOutline]);
				DrawString(getScreen(), getText(), getRect().getX()+3, getRect().getY()+3, Color[colorButtonText]);
			}

			return true;
		}
		bool CheckButton::getChecked() {
			return Checked;
		}
		void CheckButton::setChecked(bool Checked) {
			setUpdate(true);
			CheckButton::Checked = Checked;
		}
	}
}
