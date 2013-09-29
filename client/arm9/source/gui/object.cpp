#include "object.h"
using D2K::GUI::Object;

Object::Object(u8 Screen, GUI::Rect *Rect) {
	Object::Screen = Screen;
	Object::Rect = Rect;
	Object::Type = ObjectObject;
	setVisible(false);
	function = &GUI::VoidFunction;
}
Object::~Object() {

}
bool Object::Draw() {
	if(!Object::isUpdated() && !GUI::isUpdated())
		return false;

	Object::setUpdate(false);
	return true;
}
void Object::Clear(uint16_t C) {
	GUI::DrawFilledRect(Screen, Rect, C);
}
void Object::setVisible(bool Visible) {
	GUI::setUpdate(true);
	Object::setUpdate(true);
	Object::Visible = Visible;
}
bool Object::isVisible() {
	return Visible;
}
bool Object::isClicked(uint8_t X, uint8_t Y) {
	return Rect->PointIntersect(X, Y);
}
void Object::setUpdate(bool value)
{
	Update = value;
}
bool Object::isUpdated()
{
	return Update;
}
u8 Object::getType() {
	return Type;
}
D2K::GUI::Rect *Object::getRect() {
	return Object::Rect;
}
void Object::setRect(GUI::Rect *Rect) {
	Object::Rect = Rect;
}
