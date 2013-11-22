#include <stdio.h>
#include "window.h"
#include "gui.h"
#include "common/udp.h"
#include "../core.h"

namespace D2K {
	namespace GUI {
		Window::Window() {
			Screen = 0;
			Visible = false;
		}
		Window::~Window() {
			for(unsigned int i = 0; i < Objects.size(); i++) {
				delete Objects.at(i);
			}
			Objects.clear();
		}
		void Window::AppendObject(Object *object) {
			Objects.push_back(object);
		}
		void Window::RemoveObject(Object *object) {
			for(unsigned int i = 0; i < Objects.size(); i++) {
				if(Objects.at(i) == object) {
					Objects.erase(Objects.begin()+i);
					delete object;
				}
			}
		}

		void Window::Draw() {
			if(GUI::IsUpdated())
				GUI::ClearScreen(Screen, Color[colorBackground]);
			for(unsigned int i = 0; i < Objects.size(); i++) {
				if(Objects.at(i)->Draw())
					if(EMULATOR) printf("button draw %i: %s\n", i, Objects.at(i)->GetText().c_str());
			}
			GUI::SetUpdate(false);
		}
		void Window::SetVisible(bool visible) {
			Window::Visible = visible;
			for(unsigned int i = 0; i < Objects.size(); i++) {
				Objects.at(i)->SetVisible(visible);
			}
		}
		bool Window::IsVisible() {
			return Visible;
		}
		bool Window::CheckClick(Object *object) {
			if(object) {
				if(object->IsVisible()) {
					if(keysDown()&KEY_TOUCH) {
						if(object->IsClicked((uint8_t)Core::StylusPos.px, (uint8_t)Core::StylusPos.py)) {
							object->SetStatus(1);//Pressed
						}
					}
					else if(keysHeld()&KEY_TOUCH) {
						if(object->GetStatus() > 0) {
							if(object->IsClicked((uint8_t)Core::StylusPos.px, (uint8_t)Core::StylusPos.py)) {
								object->SetStatus(2);//if we pressed and we're still hovering set to Held
							}
							else {
								object->SetStatus(1);//if we held but we're not hovering set to Pressed
							}
						}
					}
					else if(keysUp()&KEY_TOUCH) {
						if(object->IsClicked((uint8_t)Core::StylusPos.px, (uint8_t)Core::StylusPos.py)) {
							int status = object->GetStatus();//save status value
							object->SetStatus(0);//reset status
							if(status == 2) {//if we pressed and we're still hovering
								return true;//the object was clicked
							}
						}
					}
				}
			}

			return false;//the object wasn't clicked
		}
		bool Window::Update() {
			Draw();
			for(unsigned int i = 0; i < Objects.size(); i++) {
				if(CheckClick(Objects.at(i))) {
					Objects.at(i)->Function();//execute the function
					return true;
				}
			}

			return false;
		}
	}
}
