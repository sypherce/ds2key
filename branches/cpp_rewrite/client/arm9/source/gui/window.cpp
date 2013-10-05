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
		bool Window::Update() {
			Draw();
			for(unsigned int i = 0; i < ListMax; i++) {
				if(keysDown()&KEY_TOUCH)
					if(List[i])
						if(List[i]->isVisible())
							if(List[i]->isClicked((uint8_t)System::stylusPos.px, (uint8_t)System::stylusPos.py)) {
								List[i]->function();
								return 1;
							}
			}

			return 0;
		}
	}
}
