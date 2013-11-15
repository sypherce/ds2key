#include "object.h"
#include "gui.h"

namespace D2K {
	namespace GUI {
		Object::Object(uint8_t screen, GUI::Rect rect) {
			Object::Screen = screen;
			Object::Rect = rect;
			Object::Status = 0;
			SetVisible(false);
			Function = &GUI::VoidFunction;
		}
		Object::~Object() {

		}

		bool Object::Draw() {
			if(!Object::IsUpdated() && !GUI::IsUpdated())
				return false;

			Object::SetUpdate(false);
			return true;
		}
		void Object::SetText(std::string text) {

		};
		std::string Object::GetText() {
			return "";
		};
		void Object::Clear(uint16_t c) {
			GUI::DrawFilledRect(Screen, Rect, c);
		}
		void Object::SetVisible(bool visible) {
			GUI::SetUpdate(true);
			Object::SetUpdate(true);
			Object::Visible = visible;
		}
		void Object::SetStatus(uint8_t value) {
			if(GetStatus() != value) {
				Status = value;
				SetUpdate(true);
				Draw();
			}
		}
		uint8_t Object::GetStatus() {
			return Status;
		}
		bool Object::IsVisible() {
			return Visible;
		}
		bool Object::IsClicked(uint8_t x, uint8_t y) {
			return Rect.PointIntersect(x, y);
		}
		void Object::SetUpdate(bool value) {
			Update = value;
		}
		bool Object::IsUpdated() {
			return Update;
		}
		bool Object::GetScreen() {
			return Screen;
		}
		D2K::GUI::Rect Object::GetRect() {
			return Object::Rect;
		}
		void Object::SetRect(GUI::Rect rect) {
			Object::Rect = rect;
		}
	}
}
