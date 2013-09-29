#include <nds.h>	//u8, ARGB16
#include <string>	//std::string
#include "edit.h"
using D2K::GUI::Edit;

Edit::Edit(u8 Screen, GUI::Rect *Rect, std::string String, void (*function)()) : Button(Screen, Rect, String, function) {

	Object::Type = ObjectEdit;
	setVisible(true);
}
Edit::~Edit() {

}
bool Edit::Draw() {
	if(!Object::Draw())
		return false;
	if(isVisible()) {//ifChanged()
		Clear(Color[colorEditBackground]);
		DrawRect(Screen, Rect, Color[colorEditOutline]);
		DrawString(Screen, getText(), Rect->getX()+3, Rect->getY()+3, Color[colorEditText]);
	}

	return true;
}
