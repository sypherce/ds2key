#include <nds.h>	//u8, ARGB16
#include <string>	//std::string
#include "label.h"
using D2K::GUI::Label;

Label::Label(u8 Screen, GUI::Rect *Rect, std::string String) : Object(Screen, Rect) {
	setText(String);
	Object::Type = ObjectLabel;
	setVisible(true);
}
Label::~Label() {

}
bool Label::Draw() {
	if(!Object::Draw())
		return false;
	if(isVisible()) {//ifChanged()
		Clear(Color[colorBackground]);

		DrawString(Screen, getText(), Rect->getX()+3, Rect->getY()+3, Color[colorLabelText]);
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
