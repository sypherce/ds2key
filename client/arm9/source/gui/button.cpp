#include <nds.h>	//u8, ARGB16
#include <string>	//std::string
#include "button.h"
using D2K::GUI::Button;

Button::Button(u8 Screen, GUI::Rect *Rect, std::string String, void (*function)()) : Label(Screen, Rect, String) {
	Button::function = function;
	Object::Type = ObjectButton;
	setVisible(true);
}
Button::~Button() {

}
bool Button::Draw() {
	if(!Object::Draw())
		return false;
	if(isVisible()) {//ifChanged()
		Clear(Color[colorButtonBackground]);
		DrawRect(Screen, Rect, Color[colorButtonOutline]);
		DrawString(Screen, getText(), Rect->getX()+3, Rect->getY()+3, Color[colorButtonText]);
	}

	return true;
}
