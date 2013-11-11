#include <string>	//std::string
#include "label.h"

namespace D2K {
	namespace GUI {
		Label::Label(uint8_t Screen, GUI::Rect Rect, std::string String) : Object(Screen, Rect) {
			GUI::Rect thisRect = getRect();
			int width = String.length() * 6;
			if(thisRect.getW() < width)
				thisRect.setW(width);
			setRect(thisRect);
			setText(String);
			setVisible(true);
		}
		Label::~Label() {

		}

		bool Label::Draw() {
			if(!Object::Draw())
				return false;
			if(isVisible()) {
				Clear(Color[colorBackground]);

				DrawString(getScreen(), getText(), getRect().getX(), getRect().getY(), Color[colorLabelText]);
			}

			return true;
		}
		void Label::setText(std::string Text) {
			Label::setUpdate(true);
			Label::Text = Text;
		}
		std::string Label::getText() {
			return Text;
		}
	}
}
