#include <string>	//std::string
#include "turboWindow.h"
#include "checkButton.h"
#include "label.h"
#include "../core.h"

namespace D2K {
	namespace GUI {
		namespace Turbo {
			//private
			extern void checkButton1Function();
			extern void checkButton2Function();
			extern void checkButton3Function();
			extern void checkButton4Function();
			extern void checkButton5Function();
			extern void checkButton6Function();
			extern void checkButton7Function();
			extern void checkButton8Function();
			extern void checkButton9Function();
			extern void checkButton10Function();
			extern void checkButton11Function();
			extern void checkButton12Function();
			extern void checkButton13Function();

			//public
			WindowClass *Window;
			Label *label1;
			CheckButton *checkButton1;
			CheckButton *checkButton2;
			CheckButton *checkButton3;
			CheckButton *checkButton4;
			CheckButton *checkButton5;
			CheckButton *checkButton6;
			CheckButton *checkButton7;
			CheckButton *checkButton8;
			CheckButton *checkButton9;
			CheckButton *checkButton10;
			CheckButton *checkButton11;
			CheckButton *checkButton12;
			CheckButton *checkButton13;

			WindowClass::WindowClass() : Window() {
				Screen = 0;

				AppendObject(label1	= new Label(Screen, Rect(24,0+3,128,10), "Turbo Settings"));

				AppendObject(checkButton1 = new CheckButton(Screen, Rect(39,39,10,10), "L", &checkButton1Function));
				AppendObject(checkButton2 = new CheckButton(Screen, Rect(205,40,10,10), "R", &checkButton2Function));

				AppendObject(checkButton3 = new CheckButton(Screen, Rect(9,95,10,10), "Left", &checkButton3Function));
				AppendObject(checkButton4 = new CheckButton(Screen, Rect(69,95,10,10), "Right", &checkButton4Function));
				AppendObject(checkButton5 = new CheckButton(Screen, Rect(39,80,10,10), "Up", &checkButton5Function));
				AppendObject(checkButton6 = new CheckButton(Screen, Rect(39,110,10,10), "Down", &checkButton6Function));

				AppendObject(checkButton7 = new CheckButton(Screen, Rect(235,95,10,10), "A", &checkButton7Function));
				AppendObject(checkButton8 = new CheckButton(Screen, Rect(205,110,10,10), "B", &checkButton8Function));
				AppendObject(checkButton9 = new CheckButton(Screen, Rect(205,80,10,10), "X", &checkButton9Function));
				AppendObject(checkButton10 = new CheckButton(Screen, Rect(175,95,10,10), "Y", &checkButton10Function));

				AppendObject(checkButton11 = new CheckButton(Screen, Rect(145,115,10,10), "Start", &checkButton11Function));
				AppendObject(checkButton12 = new CheckButton(Screen, Rect(85,115,10,10), "Select", &checkButton12Function));
				AppendObject(checkButton13 = new CheckButton(Screen, Rect(100,35,10,10), "Enable", &checkButton13Function));
			}
			WindowClass::~WindowClass() { }
			uint16_t currentTurboKeys = 0;
			void UpdateTurboKeys() {
				currentTurboKeys = 0;
				if(checkButton1->GetChecked())
					currentTurboKeys |= KEY_L;
				if(checkButton2->GetChecked())
					currentTurboKeys |= KEY_R;
				if(checkButton3->GetChecked())
					currentTurboKeys |= KEY_LEFT;
				if(checkButton4->GetChecked())
					currentTurboKeys |= KEY_RIGHT;
				if(checkButton5->GetChecked())
					currentTurboKeys |= KEY_UP;
				if(checkButton6->GetChecked())
					currentTurboKeys |= KEY_DOWN;
				if(checkButton7->GetChecked())
					currentTurboKeys |= KEY_A;
				if(checkButton8->GetChecked())
					currentTurboKeys |= KEY_B;
				if(checkButton9->GetChecked())
					currentTurboKeys |= KEY_X;
				if(checkButton10->GetChecked())
					currentTurboKeys |= KEY_Y;
				if(checkButton11->GetChecked())
					currentTurboKeys |= KEY_START;
				if(checkButton12->GetChecked())
					currentTurboKeys |= KEY_SELECT;
			}
			uint16_t CheckTurboKey(uint16_t a) {
				if(currentTurboKeys&a)
					if(keysHeld()&a)
						return a;
				return 0;
			}
			uint16_t GetKeys() {
				uint16_t keys = 0;
				if(checkButton13->GetChecked() == false)
					return 0;
				UpdateTurboKeys();
				keys |= CheckTurboKey(KEY_A);
				keys |= CheckTurboKey(KEY_B);
				keys |= CheckTurboKey(KEY_X);
				keys |= CheckTurboKey(KEY_Y);
				keys |= CheckTurboKey(KEY_L);
				keys |= CheckTurboKey(KEY_R);
				keys |= CheckTurboKey(KEY_UP);
				keys |= CheckTurboKey(KEY_DOWN);
				keys |= CheckTurboKey(KEY_LEFT);
				keys |= CheckTurboKey(KEY_RIGHT);
				keys |= CheckTurboKey(KEY_START);
				keys |= CheckTurboKey(KEY_SELECT);
				return keys;
			}

			void checkButton1Function() {
				checkButton1->SetChecked(!checkButton1->GetChecked());
			}
			void checkButton2Function() {
				checkButton2->SetChecked(!checkButton2->GetChecked());
			}
			void checkButton3Function() {
				checkButton3->SetChecked(!checkButton3->GetChecked());
			}
			void checkButton4Function() {
				checkButton4->SetChecked(!checkButton4->GetChecked());
			}
			void checkButton5Function() {
				checkButton5->SetChecked(!checkButton5->GetChecked());
			}
			void checkButton6Function() {
				checkButton6->SetChecked(!checkButton6->GetChecked());
			}
			void checkButton7Function() {
				checkButton7->SetChecked(!checkButton7->GetChecked());
			}
			void checkButton8Function() {
				checkButton8->SetChecked(!checkButton8->GetChecked());
			}
			void checkButton9Function() {
				checkButton9->SetChecked(!checkButton9->GetChecked());
			}
			void checkButton10Function() {
				checkButton10->SetChecked(!checkButton10->GetChecked());
			}
			void checkButton11Function() {
				checkButton11->SetChecked(!checkButton11->GetChecked());
			}
			void checkButton12Function() {
				checkButton12->SetChecked(!checkButton12->GetChecked());
			}
			void checkButton13Function() {
				checkButton13->SetChecked(!checkButton13->GetChecked());
			}
		}
	}
}
