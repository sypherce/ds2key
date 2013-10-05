#include <string>	//std::string
#include "turboWindow.h"
#include "../ds2key.h"
#include "../config.h"
#include "../system.h"

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

				label1	= new Label(Screen, Rect(24,0,128,11), "Turbo Settings");

				checkButton1 = new CheckButton(Screen, Rect(39,39,11,11), "L", &checkButton1Function);
				checkButton2 = new CheckButton(Screen, Rect(205,40,11,11), "R", &checkButton2Function);

				checkButton3 = new CheckButton(Screen, Rect(9,95,11,11), "Left", &checkButton3Function);
				checkButton4 = new CheckButton(Screen, Rect(69,95,11,11), "Right", &checkButton4Function);
				checkButton5 = new CheckButton(Screen, Rect(39,80,11,11), "Up", &checkButton5Function);
				checkButton6 = new CheckButton(Screen, Rect(39,110,11,11), "Down", &checkButton6Function);

				checkButton7 = new CheckButton(Screen, Rect(235,95,11,11), "A", &checkButton7Function);
				checkButton8 = new CheckButton(Screen, Rect(205,110,11,11), "B", &checkButton8Function);
				checkButton9 = new CheckButton(Screen, Rect(205,80,11,11), "X", &checkButton9Function);
				checkButton10 = new CheckButton(Screen, Rect(175,95,11,11), "Y", &checkButton10Function);

				checkButton11 = new CheckButton(Screen, Rect(145,115,50,11), "Start", &checkButton11Function);
				checkButton12 = new CheckButton(Screen, Rect(85,115,50,11), "Select", &checkButton12Function);
				checkButton13 = new CheckButton(Screen, Rect(100,35,50,11), "Enable", &checkButton13Function);

				int i = 0;
				List[i++] = label1;

				List[i++] = checkButton1;
				List[i++] = checkButton2;
				List[i++] = checkButton3;
				List[i++] = checkButton4;
				List[i++] = checkButton5;
				List[i++] = checkButton6;
				List[i++] = checkButton7;
				List[i++] = checkButton8;
				List[i++] = checkButton9;
				List[i++] = checkButton10;
				List[i++] = checkButton11;
				List[i++] = checkButton12;
				List[i++] = checkButton13;

			}
			WindowClass::~WindowClass() { }
			uint16_t currentTurboKeys = 0;
			void UpdateTurboKeys()
			{
				currentTurboKeys = 0;
				if(checkButton1->getChecked())
					currentTurboKeys |= KEY_L;
				if(checkButton2->getChecked())
					currentTurboKeys |= KEY_R;
				if(checkButton3->getChecked())
					currentTurboKeys |= KEY_LEFT;
				if(checkButton4->getChecked())
					currentTurboKeys |= KEY_RIGHT;
				if(checkButton5->getChecked())
					currentTurboKeys |= KEY_UP;
				if(checkButton6->getChecked())
					currentTurboKeys |= KEY_DOWN;
				if(checkButton7->getChecked())
					currentTurboKeys |= KEY_A;
				if(checkButton8->getChecked())
					currentTurboKeys |= KEY_B;
				if(checkButton9->getChecked())
					currentTurboKeys |= KEY_X;
				if(checkButton10->getChecked())
					currentTurboKeys |= KEY_Y;
				if(checkButton11->getChecked())
					currentTurboKeys |= KEY_START;
				if(checkButton12->getChecked())
					currentTurboKeys |= KEY_SELECT;
			}
			uint16_t CheckTurboKey(uint16_t a)
			{
				if(currentTurboKeys&a)
					if(keysHeld()&a)
						return a;
				return 0;
			}
			uint16_t GetKeys(){
				uint16_t keys = 0;
				if(checkButton13->getChecked() == false)
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
				checkButton1->setChecked(!checkButton1->getChecked());
			}
			void checkButton2Function() {
				checkButton2->setChecked(!checkButton2->getChecked());
			}
			void checkButton3Function() {
				checkButton3->setChecked(!checkButton3->getChecked());
			}
			void checkButton4Function() {
				checkButton4->setChecked(!checkButton4->getChecked());
			}
			void checkButton5Function() {
				checkButton5->setChecked(!checkButton5->getChecked());
			}
			void checkButton6Function() {
				checkButton6->setChecked(!checkButton6->getChecked());
			}
			void checkButton7Function() {
				checkButton7->setChecked(!checkButton7->getChecked());
			}
			void checkButton8Function() {
				checkButton8->setChecked(!checkButton8->getChecked());
			}
			void checkButton9Function() {
				checkButton9->setChecked(!checkButton9->getChecked());
			}
			void checkButton10Function() {
				checkButton10->setChecked(!checkButton10->getChecked());
			}
			void checkButton11Function() {
				checkButton11->setChecked(!checkButton11->getChecked());
			}
			void checkButton12Function() {
				checkButton12->setChecked(!checkButton12->getChecked());
			}
			void checkButton13Function() {
				checkButton13->setChecked(!checkButton13->getChecked());
			}
		}
	}
}
