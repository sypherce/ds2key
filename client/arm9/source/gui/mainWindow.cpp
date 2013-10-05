#include <string>	//std::string
#include "mainWindow.h"
#include "keypadWindow.h"
#include "turboWindow.h"
#include "../ds2key.h"
#include "../config.h"
#include "../system.h"

namespace D2K {
	namespace GUI {
		namespace Main {
			//private
			extern void edit1Function();
			extern void edit2Function();
			extern void edit3Function();
			extern void button4Function();
			extern void button5Function();

			//public
			WindowClass *Window;
			Label *label1;
			Label *label2;
			Label *label3;
			Label *label4;
			Label *label5;
			Edit *edit1;
			Edit *edit2;
			Edit *edit3;
			Button *button4;
			Button *button5;

			WindowClass::WindowClass() : Window() {
				Color[colorBackground] = ARGB16(1, 19, 22, 25);

				Color[colorLabelText] = ARGB16(1, 0, 0, 0);
				Color[colorLabelBackground] = Color[colorBackground];

				Color[colorButtonText] = Color[colorLabelText];
				Color[colorButtonBackground] = ARGB16(1, 31, 30, 15);
				Color[colorButtonOutline] = Color[colorButtonText];

				Color[colorEditText] = Color[colorLabelText];
				Color[colorEditBackground] = ARGB16(1, 31, 31, 31);
				Color[colorEditOutline] = Color[colorEditText];

				Screen = 0;

				label1	= new Label(Screen, Rect(24,0,128,11), VERSION_STRING);
				label2	= new Label(Screen, Rect(160,0,70,11), System::getTime());
				label3	= new Label(Screen, Rect(64,24,40,11), "IP:");
				label4	= new Label(Screen, Rect(64,48,40,11), "Port:");
				label5	= new Label(Screen, Rect(64,72,40,11), "Profile:");

				edit1 = new Edit(Screen, Rect(96,24,95,11), DS2Key::GetIPString(), &edit1Function);
				edit2 = new Edit(Screen, Rect(112,48,79,11), DS2Key::GetPortString(), &edit2Function);
				edit3 = new Edit(Screen, Rect(136,72,55,11), DS2Key::GetProfileString(), &edit3Function);
				button5 = new Button(Screen, Rect(0,177,35,11), "Turbo", &button5Function);
				button4 = new Button(Screen, Rect(217,177,35,11), "Touch", &button4Function);

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
				List[i++] = button5;
			}
			WindowClass::~WindowClass() { }
			bool WindowClass::Update() {
				static char oldTime[13];

				static int c = 0;c++;if(c > 30)//counter
				if(label2->isVisible())
				{
					c = 0;
					char *newTime = System::getTime();
					if(strcmp(oldTime, newTime) != 0) {//if times differ
						strcpy(oldTime, newTime);
						label2->setText(oldTime);
					}
				}
				return Window::Update();
			}

			//private
			void edit1Function() {
				std::string entry = Keypad::Entry(label3, edit1, DS2Key::GetIPString(), System::LENGTH_IP);
				DS2Key::SetIP(entry);
				edit1->setText(DS2Key::GetIPString());
				Config::Save();
				DS2Key::DeInit();
				DS2Key::Init();
			}
			void edit2Function() {
				std::string entry = Keypad::Entry(label4, edit2, DS2Key::GetPortString(), System::LENGTH_PORT);
				DS2Key::SetPort(entry);
				edit2->setText(DS2Key::GetPortString());
				Config::Save();
				DS2Key::DeInit();
				DS2Key::Init();
			}
			void edit3Function() {
				std::string entry = Keypad::Entry(label5, edit3, DS2Key::GetProfileString(), System::LENGTH_PROFILE);
				DS2Key::SetProfile(entry);
				edit3->setText(DS2Key::GetProfileString());
				Config::Save();
				DS2Key::DeInit();
				DS2Key::Init();
			}
			void button4Function() {
				Main::Window->setVisible(false);
				button4->setVisible(true);
				Main::Window->Draw();

				while(true) {
					System::Update(true);
					if(keysDown()&KEY_TOUCH && button4->isClicked(System::stylusPos.px, System::stylusPos.py)) {
						break;//if pressed again, break
					}
					DS2Key::Update(keysHeld(), 0, guitarGripKeysHeld() * guitarGripIsInserted(), 0 * guitarGripIsInserted(), &System::stylusPos);
				}

				Main::Window->Draw();
				Main::Window->setVisible(true);
			}

			void button5Function() {
				Main::Window->setVisible(false);
				button5->setVisible(true);
				Turbo::Window->setVisible(true);
				Turbo::Window->Draw();
				button5->Draw();

				while(true) {
					System::Update(true);
					DS2Key::Update(keysHeld(), Turbo::GetKeys(), guitarGripKeysHeld() * guitarGripIsInserted(), 0 * guitarGripIsInserted(), (touchPosition*)NULL);
					Turbo::Window->Update();
					if(keysDown()&KEY_TOUCH && button5->isClicked(System::stylusPos.px, System::stylusPos.py)) {
						break;//if pressed again, break
					}
				}

				Main::Window->Draw();
				Main::Window->setVisible(true);
			}
		}
	}
}
