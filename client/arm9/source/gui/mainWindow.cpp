#include <stdio.h>
#include "../ds2key.h"
#include "../config.h"
#include "../system.h"
#include "mainWindow.h"
#include "keypadWindow.h"

namespace D2K {
	namespace GUI {
		namespace MainWindow {
			bool Screen = 0;
			const unsigned ListMax = 50;
			D2K::GUI::Object *List[ListMax];
			D2K::GUI::Label *label1;
			D2K::GUI::Label *label2;
			D2K::GUI::Label *label3;
			D2K::GUI::Label *label4;
			D2K::GUI::Label *label5;
			D2K::GUI::Edit *edit1;
			D2K::GUI::Edit *edit2;
			D2K::GUI::Edit *edit3;
			D2K::GUI::Button *button4;

			void edit1Function() {
				char *ipString = KeypadWindow::KeyPadEdit(label3, edit1, DS2Key::GetIPString(), D2K::System::LENGTH_IP);
				DS2Key::SetIP(ipString);
				delete ipString;
				edit1->setText(DS2Key::GetIPString());
				Config::Save();
				DS2Key::DeInit();
				DS2Key::Init();
			}

			void edit2Function() {
				char *portString = KeypadWindow::KeyPadEdit(label4, edit2, DS2Key::GetPortString(), D2K::System::LENGTH_PORT);
				DS2Key::SetPort(portString);
				delete portString;
				edit2->setText(DS2Key::GetPortString());
				Config::Save();
				DS2Key::DeInit();
				DS2Key::Init();
			}

			void edit3Function() {
				char *profileString = KeypadWindow::KeyPadEdit(label5, edit3, DS2Key::GetProfileString(), D2K::System::LENGTH_PROFILE);
				DS2Key::SetProfile(profileString);
				delete profileString;
				edit3->setText(DS2Key::GetProfileString());
				Config::Save();
				DS2Key::DeInit();
				DS2Key::Init();
			}
			void button4Function() {
				MainWindow::setVisible(false);
				button4->setVisible(true);

				Draw();

				while(true) {
					System::Update(true);
					if(keysDown()&KEY_TOUCH && button4->isClicked(System::stylusPos.px, System::stylusPos.py)) {
						break;
					}
					DS2Key::Update(keysHeld(), guitarGripKeysHeld() * guitarGripIsInserted(), &System::stylusPos);
				}

				Draw();

				setVisible(true);
			}
			void Setup() {
				GUI::Color[colorBackground] = ARGB16(1, 19, 22, 25);

				GUI::Color[colorLabelText] = ARGB16(1, 0, 0, 0);
				GUI::Color[colorLabelBackground] = Color[colorBackground];

				GUI::Color[colorButtonText] = Color[colorLabelText];
				GUI::Color[colorButtonBackground] = ARGB16(1, 31, 30, 15);
				GUI::Color[colorButtonOutline] = Color[colorButtonText];

				GUI::Color[colorEditText] = Color[colorLabelText];
				GUI::Color[colorEditBackground] = ARGB16(1, 31, 31, 31);
				GUI::Color[colorEditOutline] = Color[colorEditText];

				for(unsigned int i = 0; i < ListMax; i++) {
					List[i] = NULL;
				}
				label1	= new D2K::GUI::Label(Screen, new D2K::GUI::Rect(24,0,65,11), "DS2Key 2.0");
				label2	= new D2K::GUI::Label(Screen, new D2K::GUI::Rect(160,0,70,11), D2K::System::getTime());
				label3	= new D2K::GUI::Label(Screen, new D2K::GUI::Rect(64,24,40,11), "IP:");
				label4	= new D2K::GUI::Label(Screen, new D2K::GUI::Rect(64,48,40,11), "Port:");
				label5	= new D2K::GUI::Label(Screen, new D2K::GUI::Rect(64,72,40,11), "Profile:");

				edit1 = new D2K::GUI::Edit(Screen, new D2K::GUI::Rect(96,24,95,11), D2K::DS2Key::GetIPString(), &edit1Function);
				edit2 = new D2K::GUI::Edit(Screen, new D2K::GUI::Rect(112,48,79,11), D2K::DS2Key::GetPortString(), &edit2Function);
				edit3 = new D2K::GUI::Edit(Screen, new D2K::GUI::Rect(136,72,55,11), D2K::DS2Key::GetProfileString(), &edit3Function);
				button4 = new D2K::GUI::Button(Screen, new D2K::GUI::Rect(217,177,35,11), "Touch", &button4Function);

				int i = 0;
				List[i++] = label1;
				List[i++] = label2;
				List[i++] = label3;
				List[i++] = label4;
				List[i++] = label5;

				List[i++] = edit1;
				List[i++] = edit2;
				List[i++] = edit3;
				List[i++] = button4;

				KeypadWindow::Setup();
			}
			void Draw() {
				if(GUI::isUpdated()) {
					D2K::GUI::ClearScreen(Screen, Color[colorBackground]);
				}
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
						else if(List[i]->getType() == ObjectEdit) {
							if(static_cast<GUI::Edit*>(List[i])->Draw())
								if(EMULATOR) printf("button draw %i: %s\n", i, static_cast<GUI::Edit*>(List[i])->getText().c_str());
						}
					}
				}
				if(GUI::isUpdated()) {
					GUI::setUpdate(false);
				}
			}
			void setVisible(bool Visible) {
				for(unsigned int i = 0; i < ListMax; i++) {
					if(List[i])
						List[i]->setVisible(Visible);
				}
			}
			bool Update() {
				static char oldTime[13];
				if(label2->isVisible())
				{
					char *newTime = D2K::System::getTime();
					if(strcmp(oldTime, newTime) != 0) {//if times differ
						strcpy(oldTime, newTime);
						label2->setText(oldTime);
					}
				}
				MainWindow::Draw();
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
}
