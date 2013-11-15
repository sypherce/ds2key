#include <string>	//std::string
#include "edit.h"
#include "gui.h"

namespace D2K {
	namespace GUI {
		Edit::Edit(uint8_t screen, GUI::Rect rect, std::string text, void (*function)()) : Button(screen, rect, text, function) {
			SetVisible(true);
		}

		Edit::~Edit() { }

		bool Edit::Draw() {
			if(!Object::Draw())
				return false;
			if(IsVisible()) {//ifChanged()
				Clear(Color[colorEditBackground]);
				if(GetStatus() == 2)
					DrawRect(GetScreen(), GetRect(), Color[colorEditOutlineActive]);
				else
					DrawRect(GetScreen(), GetRect(), Color[colorEditOutline]);
				DrawString(GetScreen(), GetText(), GetRect().GetX()+3, GetRect().GetY()+3, Color[colorEditText]);
			}

			return true;
		}
	}
}
