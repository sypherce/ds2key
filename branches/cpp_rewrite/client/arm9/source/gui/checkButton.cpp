#include <string>	//std::string
#include "checkButton.h"

namespace D2K {
	namespace GUI {
		CheckButton::CheckButton(uint8_t Screen, GUI::Rect Rect, std::string String, void (*function)()) : Label(Screen, Rect, String) {
			GUI::Rect thisRect = getRect();
			int width = String.length() * 6 + 4;
			if(thisRect.getW() < width)
				thisRect.setW(String.length() * 6 + 4);
			setRect(thisRect);
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
				if(getStatus() == 2)
					DrawRect(getScreen(), getRect(), Color[colorButtonOutlineActive]);
				else if(getChecked())
					DrawRect(getScreen(), getRect(), Color[colorButtonOutline]);
				else
					DrawRect(getScreen(), getRect(), Color[colorButtonBackground]);
				DrawString(getScreen(), getText(), getRect().getX()+3, getRect().getY()+3, Color[colorButtonText]);
			}

			return true;
		}
		void CheckButton::setStatus(uint8_t Value) {
			Status = Value;
			setUpdate(true);
			Draw();
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
