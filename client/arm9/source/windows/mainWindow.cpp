#include <string>	//std::string

//windows
#include "mainWindow.h"
#include "keypadWindow.h"
#include "turboWindow.h"
#include "commandWindow.h"

//controls
#include "gui.h"
#include "edit.h"
#include "button.h"
#include "label.h"

//core
#include "../udp.h"
#include "../config.h"
#include "../core.h"

const int LENGTH_IP = 15;
const int LENGTH_PORT = 5;
const int LENGTH_PROFILE = 3;

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
				AppendObject(label2	= new Label(Screen, Rect(160,0+3,70,10), Core::GetTime()));
				AppendObject(label3	= new Label(Screen, Rect(64,24+3,10,10), "IP:"));
				AppendObject(label4	= new Label(Screen, Rect(64,48+3,10,10), "Port:"));
				AppendObject(label5	= new Label(Screen, Rect(64,72+3,10,10), "Profile:"));

				AppendObject(edit1 = new Edit(Screen, Rect(96,24,95,10), Core::UDP->GetRemoteIPString(), &edit1Function));
				AppendObject(button6 = new Button(Screen, Rect(204,24,35,10), "Find IP", &button6Function));
				AppendObject(edit2 = new Edit(Screen, Rect(112,48,79,10), Core::UDP->GetPortString(), &edit2Function));
				AppendObject(edit3 = new Edit(Screen, Rect(136,72,55,10), Core::UDP->GetProfileString(), &edit3Function));
				AppendObject(button5 = new Button(Screen, Rect(0,177,35,10), "Turbo", &button5Function));
				AppendObject(button7 = new Button(Screen, Rect(95,177,35,10), "Commands", &button7Function));
				AppendObject(button4 = new Button(Screen, Rect(217,177,35,10), "Touch", &button4Function));
			}
			WindowClass::~WindowClass() { }
			bool WindowClass::Update() {
				static char seconds = 0;

				static int c = 0;c++;if(c > 30)//counter
				if(label2->IsVisible()) {
					c = 0;
					char *timePointer = Core::GetTime();
					if(seconds != timePointer[7]) {//if seconds differ
						seconds = timePointer[7];
						label2->SetText(timePointer);
					}
				}
				return Window::Update();
			}

			//private
			void edit1Function() {
				std::string entry = Keypad::Entry(label3, edit1, Core::UDP->GetRemoteIPString(), LENGTH_IP);
				Core::UDP->SetRemoteIP(entry);
				edit1->SetText(Core::UDP->GetRemoteIPString());
				Core::Config->Save();
				Core::UDP->Connect();
			}
			void edit2Function() {
				std::string entry = Keypad::Entry(label4, edit2, Core::UDP->GetPortString(), LENGTH_PORT);
				Core::UDP->SetPort(entry);
				edit2->SetText(Core::UDP->GetPortString());
				Core::Config->Save();
				Core::UDP->Connect();
			}
			void edit3Function() {
				std::string entry = Keypad::Entry(label5, edit3, Core::UDP->GetProfileString(), LENGTH_PROFILE);
				Core::UDP->SetProfile(entry);
				edit3->SetText(Core::UDP->GetProfileString());
				Core::Config->Save();
				Core::UDP->Connect();
			}
			void button4Function() {
				Main::Window->SetVisible(false);
				label1->SetText("Touch Mode");
				label1->SetVisible(true);
				button4->SetVisible(true);
				Main::Window->Draw();

				while(true) {
					Core::Loop();
					if(Main::Window->CheckClick(button4)) {
						break;//if pressed again, break
					}
					Core::UDP->Update(keysHeld(), 0, guitarGripKeysHeld() * guitarGripIsInserted(), 0 * guitarGripIsInserted(), &Core::StylusPos);
				}

				label1->SetText(VERSION_STRING);
				Main::Window->Draw();
				Main::Window->SetVisible(true);
			}

			void button5Function() {
				Main::Window->SetVisible(false);
				button5->SetVisible(true);
				Turbo::Window->SetVisible(true);
				Turbo::Window->Draw();
				button5->Draw();

				while(true) {
					Core::Loop();
					Core::UDP->Update(keysHeld(), Turbo::GetKeys(), guitarGripKeysHeld() * guitarGripIsInserted(), 0 * guitarGripIsInserted(), (touchPosition*)NULL);
					Turbo::Window->Update();
					if(Main::Window->CheckClick(button5)) {
						break;//if pressed again, break
					}
				}

				Main::Window->Draw();
				Main::Window->SetVisible(true);
			}
			void button6Function() {
				Core::UDP->ServerLookup();
				edit1->SetText(Core::UDP->GetRemoteIPString());
			}
			void button7Function() {
				Main::Window->SetVisible(false);
				button7->SetVisible(true);
				Command::Window->SetVisible(true);
				Command::Window->Draw();
				button7->Draw();

				while(true) {
					Core::Loop();
					Command::Window->Update();
					if(Main::Window->CheckClick(button7)) {
						break;//if pressed again, break
					}
				}

				Main::Window->Draw();
				Main::Window->SetVisible(true);
			}
		}
	}
}
