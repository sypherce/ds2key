#include <stdio.h>
#include "../ds2key.h"
#include "../system.h"
#include "window.h"
#include "label.h"
#include "edit.h"
#include "checkButton.h"
#include "button.h"

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
		void Window::AppendObject(Object *Object) {
			Objects.push_back(Object);
		}
		void Window::RemoveObject(Object *Object) {
			for(unsigned int i = 0; i < Objects.size(); i++) {
				if(Objects.at(i) == Object) {
					Objects.erase(Objects.begin()+i);
					delete Object;
				}
			}
		}

		void Window::Draw() {
			if(GUI::isUpdated())
				GUI::ClearScreen(Screen, Color[colorBackground]);
			for(unsigned int i = 0; i < Objects.size(); i++) {
				if(Objects.at(i)->Draw())
					if(EMULATOR) printf("button draw %i: %s\n", i, Objects.at(i)->getText().c_str());
			}
			GUI::setUpdate(false);
		}
		void Window::setVisible(bool Visible) {
			Window::Visible = Visible;
			for(unsigned int i = 0; i < Objects.size(); i++) {
				Objects.at(i)->setVisible(Visible);
			}
		}
		bool Window::isVisible() {
			return Visible;
		}
		bool Window::CheckClick(Object *thisObject) {
			if(thisObject) {
				if(thisObject->isVisible()) {
					if(keysDown()&KEY_TOUCH) {
						if(thisObject->isClicked((uint8_t)System::stylusPos.px, (uint8_t)System::stylusPos.py)) {
							thisObject->setStatus(1);//Pressed
						}
					}
					else if(keysHeld()&KEY_TOUCH) {
						if(thisObject->getStatus() > 0) {
							if(thisObject->isClicked((uint8_t)System::stylusPos.px, (uint8_t)System::stylusPos.py)) {
								thisObject->setStatus(2);//if we pressed and we're still hovering set to Held
							}
							else {
								thisObject->setStatus(1);//if we held but we're not hovering set to Pressed
							}
						}
					}
					else if(keysUp()&KEY_TOUCH) {
						if(thisObject->isClicked((uint8_t)System::stylusPos.px, (uint8_t)System::stylusPos.py)) {
							int status = thisObject->getStatus();//save status value
							thisObject->setStatus(0);//reset status
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
