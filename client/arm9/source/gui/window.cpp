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
			for(unsigned int i = 0; i < ListMax; i++) {
				List[i] = NULL;
			}
		}
		Window::~Window() {
			for(unsigned int i = 0; i < ListMax; i++) {
				if(List[i]) {
					if(List[i]->getType() == ObjectLabel) {
						delete static_cast<GUI::Label*>(List[i]);
					}
					else if(List[i]->getType() == ObjectButton) {
						delete static_cast<GUI::Button*>(List[i]);
					}
					else if(List[i]->getType() == ObjectEdit) {
						delete static_cast<GUI::Edit*>(List[i]);
					}
					else
						delete List[i];
					//List[i] = NULL;
				}
			}
		}

		void Window::Draw() {
			if(GUI::isUpdated())
				GUI::ClearScreen(Screen, Color[colorBackground]);
			for(unsigned int i = 0; i < ListMax; i++) {
				if(List[i]) {
					if(List[i]->getType() == ObjectLabel) {
						if(static_cast<GUI::Label*>(List[i])->Draw())
							if(EMULATOR) printf("label draw %i: %s\n", i, static_cast<GUI::Label*>(List[i])->getText().c_str());
					}
					else if(List[i]->getType() == ObjectButton) {
						if(static_cast<GUI::Button*>(List[i])->Draw())
							if(EMULATOR) printf("button draw %i: %s\n", i, static_cast<GUI::Button*>(List[i])->getText().c_str());
					}
					else if(List[i]->getType() == ObjectCheckButton) {
						if(static_cast<GUI::CheckButton*>(List[i])->Draw())
							if(EMULATOR) printf("check button draw %i: %s\n", i, static_cast<GUI::CheckButton*>(List[i])->getText().c_str());
					}
					else if(List[i]->getType() == ObjectEdit) {
						if(static_cast<GUI::Edit*>(List[i])->Draw())
							if(EMULATOR) printf("button draw %i: %s\n", i, static_cast<GUI::Edit*>(List[i])->getText().c_str());
					}
				}
			}
			GUI::setUpdate(false);
		}
		void Window::setVisible(bool Visible) {
			for(unsigned int i = 0; i < ListMax; i++) {
				if(List[i])
					List[i]->setVisible(Visible);
			}
		}
		bool Window::isVisible() {	//possibly not accurate but we'll lookout for errors.
			if(List[0])
				return List[0]->isVisible();

			return false;
		}
		bool Window::CheckClick(Object *thisObject) {
			bool retVal = false;
			int status = -1;
			if(thisObject) {
				if(thisObject->isVisible()) {
					if(keysDown()&KEY_TOUCH) {
						if(thisObject->isClicked((uint8_t)System::stylusPos.px, (uint8_t)System::stylusPos.py)) {
							status = 1;//Pressed
						}
					}
					else if(keysHeld()&KEY_TOUCH) {
						if(thisObject->getStatus() > 0) {
							if(thisObject->isClicked((uint8_t)System::stylusPos.px, (uint8_t)System::stylusPos.py)) {
								status = 2;//if we pressed and we're still hovering set to Held
							}
							else {
								status = 1;//if we held but we're not hovering set to Pressed
							}
						}
					}
					else if(keysUp()&KEY_TOUCH) {
						if(thisObject->isClicked((uint8_t)System::stylusPos.px, (uint8_t)System::stylusPos.py)) {
							if(thisObject->getStatus() == 2) {//if we pressed and we're still hovering
								retVal = true;
							}
						}
						status = 0;//reset status
					}
					if(status >= 0) {
						if(thisObject->getType() == ObjectLabel) {
							static_cast<GUI::Label*>(thisObject)->setStatus(status);
						}
						else if(thisObject->getType() == ObjectButton) {
							static_cast<GUI::Button*>(thisObject)->setStatus(status);
						}
						else if(thisObject->getType() == ObjectCheckButton) {
							static_cast<GUI::CheckButton*>(thisObject)->setStatus(status);
						}
						else if(thisObject->getType() == ObjectEdit) {
							static_cast<GUI::Edit*>(thisObject)->setStatus(status);
						}
					}
				}
			}

			return retVal;
		}
		bool Window::Update() {
			bool retVal = false;
			Draw();
			for(unsigned int i = 0; i < ListMax; i++) {
				if(CheckClick(List[i])) {
					List[i]->function();//execute the function
					retVal = true;
				}
			}

			return retVal;
		}
	}
}
