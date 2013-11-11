#include "object.h"

namespace D2K {
	namespace GUI {
		Object::Object(uint8_t Screen, GUI::Rect Rect) {
			Object::Screen = Screen;
			Object::Rect = Rect;
			Object::Status = 0;
			setVisible(false);
			Function = &GUI::VoidFunction;
		}
		Object::~Object() {

		}

		bool Object::Draw() {
			if(!Object::isUpdated() && !GUI::isUpdated())
				return false;

			Object::setUpdate(false);
			return true;
		}
		void Object::setText(std::string Text) {

		};
		std::string Object::getText() {
			return "";
		};
		void Object::Clear(uint16_t C) {
			GUI::DrawFilledRect(Screen, Rect, C);
		}
		void Object::setVisible(bool Visible) {
			GUI::setUpdate(true);
			Object::setUpdate(true);
			Object::Visible = Visible;
		}
		void Object::setStatus(uint8_t Value) {
			if(getStatus() != Value) {
				Status = Value;
				setUpdate(true);
				Draw();
			}
		}
		uint8_t Object::getStatus() {
			return Status;
		}
		bool Object::isVisible() {
			return Visible;
		}
		bool Object::isClicked(uint8_t X, uint8_t Y) {
			return Rect.PointIntersect(X, Y);
		}
		void Object::setUpdate(bool value) {
			Update = value;
		}
		bool Object::isUpdated() {
			return Update;
		}
		bool Object::getScreen() {
			return Screen;
		}
		D2K::GUI::Rect Object::getRect() {
			return Object::Rect;
		}
		void Object::setRect(GUI::Rect Rect) {
			Object::Rect = Rect;
		}
	}
}
