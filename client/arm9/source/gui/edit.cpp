#include <string>	//std::string
#include "edit.h"

namespace D2K {
	namespace GUI {
		Edit::Edit(uint8_t Screen, GUI::Rect Rect, std::string String, void (*Function)()) : Button(Screen, Rect, String, Function) {
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
	}
}
