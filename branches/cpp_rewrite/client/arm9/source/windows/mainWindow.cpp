#include <string>	//std::string

//windows
#include "mainWindow.h"
#include "keypadWindow.h"
#include "turboWindow.h"
#include "commandWindow.h"

//controls
#include "gui/gui.h"
#include "gui/edit.h"
#include "gui/button.h"
#include "gui/label.h"

//core
#include "common/udp.h"
#include "common/misc.h"
#include "config.h"
#include "core.h"

const int LENGTH_IP = 15;
const int LENGTH_PORT = 5;
const int LENGTH_PROFILE = 3;

namespace D2K {
	namespace GUI {
		namespace Main {
			//private
			extern void EditIPFunction();
			extern void EditPortFunction();
			extern void EditProfileFunction();
			extern void ButtonTouchFunction();
			extern void ButtonTurboFunction();
			extern void ButtonIPFunction();
			extern void ButtonCommandsFunction();

			WindowClass *Window;
			Label *LabelTitle;
			Label *LabelClock;
			Label *LabelIP;
			Label *LabelPort;
			Label *LabelProfile;
			Edit *EditIP;
			Edit *EditPort;
			Edit *EditProfile;
			Button *ButtonTouch;
			Button *ButtonTurbo;
			Button *ButtonIP;
			Button *ButtonCommands;

			//public
			WindowClass::WindowClass() : Window() {
				Color[colorBackground] 			= ARGB16(1, 19, 22, 25);

				Color[colorLabelText] 			= ARGB16(1, 0, 0, 0);
				Color[colorLabelBackground]		= Color[colorBackground];

				Color[colorButtonText]			= Color[colorLabelText];
				Color[colorButtonBackground]	= ARGB16(1, 31, 30, 15);
				Color[colorButtonOutline]		= Color[colorButtonText];
				Color[colorButtonOutlineActive]	= ARGB16(1, 31, 0, 0);

				Color[colorEditText]			= Color[colorLabelText];
				Color[colorEditBackground]		= ARGB16(1, 31, 31, 31);
				Color[colorEditOutline]			= Color[colorEditText];
				Color[colorEditOutlineActive]	= Color[colorButtonOutlineActive];

				Screen = 0;

				AppendObject(LabelTitle		= new Label(Screen, Rect(24,0+3,128,10), Core::VERSION_STRING));
				AppendObject(LabelClock		= new Label(Screen, Rect(160,0+3,70,10), Core::GetTime()));
				AppendObject(LabelIP		= new Label(Screen, Rect(64,24+3,10,10), "IP:"));
				AppendObject(LabelPort		= new Label(Screen, Rect(64,48+3,10,10), "Port:"));
				AppendObject(LabelProfile	= new Label(Screen, Rect(64,72+3,10,10), "Profile:"));

				AppendObject(EditIP			= new Edit(Screen, Rect(96,24,95,10), Core::UDP->GetRemoteIPString(), &EditIPFunction));
				AppendObject(ButtonIP		= new Button(Screen, Rect(204,24,35,10), "Find IP", &ButtonIPFunction));
				AppendObject(EditPort		= new Edit(Screen, Rect(112,48,79,10), Core::UDP->GetPortString(), &EditPortFunction));
				AppendObject(EditProfile	= new Edit(Screen, Rect(136,72,55,10), Core::UDP->GetProfileString(), &EditProfileFunction));
				AppendObject(ButtonTurbo	= new Button(Screen, Rect(0,177,35,10), "Turbo", &ButtonTurboFunction));
				AppendObject(ButtonCommands = new Button(Screen, Rect(95,177,35,10), "Commands", &ButtonCommandsFunction));
				AppendObject(ButtonTouch	= new Button(Screen, Rect(217,177,35,10), "Touch", &ButtonTouchFunction));
			}
			WindowClass::~WindowClass() { }
			bool WindowClass::Update() {
				static char seconds = 0;

				static int c = 0;c++;if(c > 30)//counter
				if(LabelClock->IsVisible()) {
					c = 0;
					char *timePointer = Core::GetTime();
					if(seconds != timePointer[7]) {	//if seconds differ
						seconds = timePointer[7];
						LabelClock->SetText(timePointer);
					}
				}
				return Window::Update();
			}

			//private
			void EditIPFunction() {
				std::string entry = Keypad::Entry(LabelIP, EditIP, Core::UDP->GetRemoteIPString(), LENGTH_IP);
				Core::UDP->SetRemoteIP(entry);
				EditIP->SetText(Core::UDP->GetRemoteIPString());
				Core::Config->Save();
				Core::UDP->Connect();
			}
			void EditPortFunction() {
				std::string entry = Keypad::Entry(LabelPort, EditPort, Core::UDP->GetPortString(), LENGTH_PORT);
				Core::UDP->SetPort(entry);
				EditPort->SetText(Core::UDP->GetPortString());
				Core::Config->Save();
				Core::UDP->Connect();
			}
			void EditProfileFunction() {
				std::string entry = Keypad::Entry(LabelProfile, EditProfile, Core::UDP->GetProfileString(), LENGTH_PROFILE);
				Core::UDP->SetProfile(entry);
				EditProfile->SetText(Core::UDP->GetProfileString());
				Core::Config->Save();
				Core::UDP->Connect();
			}
			void ButtonTouchFunction() {
				Main::Window->SetVisible(false);	//hide main window
				LabelTitle->SetText("Touch Mode");
				LabelTitle->SetVisible(true);
				ButtonTouch->SetVisible(true);
				Main::Window->Draw();

				while(true) {
					Core::Loop();
					if(Main::Window->CheckClick(ButtonTouch)) {
						break;						//if pressed again, break
					}
					Core::UDP->Update(keysHeld(), 0, guitarGripKeysHeld() * guitarGripIsInserted(), Turbo::GHGetKeys() * guitarGripIsInserted(), &Core::StylusPos);
				}

				LabelTitle->SetText(Core::VERSION_STRING);
				Main::Window->SetVisible(true);
			}

			void ButtonTurboFunction() {
				Main::Window->SetVisible(false);	//hide main window
				ButtonTurbo->SetVisible(true);
				Turbo::Window->SetVisible(true);

				while(true) {
					Core::Loop();
					Core::UDP->Update(keysHeld(), Turbo::GetKeys(), guitarGripKeysHeld() * guitarGripIsInserted(), Turbo::GHGetKeys() * guitarGripIsInserted(), (touchPosition*)NULL);
					Turbo::Window->Update();
					ButtonTurbo->Draw();
					if(Main::Window->CheckClick(ButtonTurbo)) {
						break;						//if pressed again, break
					}
				}

				Turbo::Window->SetVisible(false);
				Main::Window->SetVisible(true);
			}
			void ButtonIPFunction() {
				Core::UDP->ServerLookup();
				EditIP->SetText(Core::UDP->GetRemoteIPString());
			}
			void ButtonCommandsFunction() {
				Main::Window->SetVisible(false);	//hide main window
				ButtonCommands->SetVisible(true);	//keep [Commands] button visible
				Command::Window->SetVisible(true);	//show command window

				while(true) {
					Core::Loop();
					Core::UDP->Update(keysHeld(), Turbo::GetKeys(), guitarGripKeysHeld() * guitarGripIsInserted(), Turbo::GHGetKeys() * guitarGripIsInserted(), (touchPosition*)NULL);
					Command::Window->Update();		//update and draw command window
					ButtonCommands->Draw();			//draw [Commands] button
					if(Main::Window->CheckClick(ButtonCommands)) {
						break;						//if pressed again, break
					}
				}

				Command::Window->SetVisible(false);
				Main::Window->SetVisible(true);		//show main window
			}
		}
	}
}
