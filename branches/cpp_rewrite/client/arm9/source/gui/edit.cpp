#include <string>	//std::string
#include "edit.h"

namespace D2K {
	namespace GUI {
		Edit::Edit(uint8_t Screen, GUI::Rect Rect, std::string String, void (*function)()) : Button(Screen, Rect, String, function) {

			Object::Type = ObjectEdit;
			setVisible(true);
		}

		Edit::~Edit() { }

		bool Edit::Draw() {
			if(!Object::Draw())
				return false;
			if(isVisible()) {//ifChanged()
				Clear(Color[colorEditBackground]);
				if(getStatus() == 2)
					DrawRect(getScreen(), getRect(), Color[colorEditOutlineActive]);
				else
					DrawRect(getScreen(), getRect(), Color[colorEditOutline]);
				DrawString(getScreen(), getText(), getRect().getX()+3, getRect().getY()+3, Color[colorEditText]);
			}

			return true;
		}
		void Edit::setStatus(uint8_t Value) {
			if(getStatus() != Value)
			{
				Object::setStatus(Value);
				setUpdate(true);
				Draw();
			}
		}
	}
}
