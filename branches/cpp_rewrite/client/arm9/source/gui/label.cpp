#include "label.h"
#include "gui.h"

namespace D2K {
	namespace GUI {
		Label::Label(uint8_t screen, GUI::Rect rect, std::string text) : Object(screen, rect) {
			GUI::Rect thisRect = GetRect();
			int width = text.length() * 6;
			if(thisRect.GetW() < width)
				thisRect.SetW(width);
			SetRect(thisRect);
			SetText(text);
			SetVisible(true);
		}
		Label::~Label() {

		}

		bool Label::Draw() {
			if(!Object::Draw())
				return false;
			if(IsVisible()) {
				Clear(Color[colorBackground]);

				DrawString(GetScreen(), GetText(), GetRect().GetX(), GetRect().GetY(), Color[colorLabelText]);
			}

			return true;
		}
		void Label::SetText(std::string text) {
			Label::SetUpdate(true);
			Label::Text = text;
		}
		std::string Label::GetText() {
			return Text;
		}
	}
}
