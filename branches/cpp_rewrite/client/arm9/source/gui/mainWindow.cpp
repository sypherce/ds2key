#include <string>	//std::string
#include "mainWindow.h"
#include "keypadWindow.h"
#include "turboWindow.h"
#include "commandWindow.h"
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
			extern void button6Function();
			extern void button7Function();

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
			Button *button6;
			Button *button7;

			WindowClass::WindowClass() : Window() {
				Color[colorBackground] = ARGB16(1, 19, 22, 25);

				Color[colorLabelText] = ARGB16(1, 0, 0, 0);
				Color[colorLabelBackground] = Color[colorBackground];

				Color[colorButtonText] = Color[colorLabelText];
				Color[colorButtonBackground] = ARGB16(1, 31, 30, 15);
				Color[colorButtonOutline] = Color[colorButtonText];
				Color[colorButtonOutlineActive] = ARGB16(1, 31, 0, 0);

				Color[colorEditText] = Color[colorLabelText];
				Color[colorEditBackground] = ARGB16(1, 31, 31, 31);
				Color[colorEditOutline] = Color[colorEditText];
				Color[colorEditOutlineActive] = Color[colorButtonOutlineActive];

				Screen = 0;

				AppendObject(label1	= new Label(Screen, Rect(24,0+3,128,10), VERSION_STRING));
				AppendObject(label2	= new Label(Screen, Rect(160,0+3,70,10), System::getTime()));
				AppendObject(label3	= new Label(Screen, Rect(64,24+3,10,10), "IP:"));
				AppendObject(label4	= new Label(Screen, Rect(64,48+3,10,10), "Port:"));
				AppendObject(label5	= new Label(Screen, Rect(64,72+3,10,10), "Profile:"));

				AppendObject(edit1 = new Edit(Screen, Rect(96,24,95,10), DS2Key::GetIPString(), &edit1Function));
				AppendObject(button6 = new Button(Screen, Rect(204,24,35,10), "Find IP", &button6Function));
				AppendObject(edit2 = new Edit(Screen, Rect(112,48,79,10), DS2Key::GetPortString(), &edit2Function));
				AppendObject(edit3 = new Edit(Screen, Rect(136,72,55,10), DS2Key::GetProfileString(), &edit3Function));
				AppendObject(button5 = new Button(Screen, Rect(0,177,35,10), "Turbo", &button5Function));
				AppendObject(button7 = new Button(Screen, Rect(95,177,35,10), "Commands", &button7Function));
				AppendObject(button4 = new Button(Screen, Rect(217,177,35,10), "Touch", &button4Function));
			}
			WindowClass::~WindowClass() { }
			bool WindowClass::Update() {
				static char seconds = 0;

				static int c = 0;c++;if(c > 30)//counter
				if(label2->isVisible()) {
					c = 0;
					char *timePointer = System::getTime();
					if(seconds != timePointer[7]) {//if seconds differ
						seconds = timePointer[7];
						label2->setText(timePointer);
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
					if(Main::Window->CheckClick(button4)) {
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
					if(Main::Window->CheckClick(button5)) {
						break;//if pressed again, break
					}
				}

				Main::Window->Draw();
				Main::Window->setVisible(true);
			}
			void button6Function() {
				DS2Key::ServerLookup();
				edit1->setText(DS2Key::GetIPString());
			}
			void button7Function() {
				Main::Window->setVisible(false);
				button7->setVisible(true);
				Command::Window->setVisible(true);
				Command::Window->Draw();
				button7->Draw();

				while(true) {
					System::Update(true);
					Command::Window->Update();
					if(Main::Window->CheckClick(button7)) {
						break;//if pressed again, break
					}
				}

				Main::Window->Draw();
				Main::Window->setVisible(true);
			}
		}
	}
}
