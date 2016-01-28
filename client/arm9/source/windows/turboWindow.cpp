#include <string>	//std::string
#include "turboWindow.h"
#include "gui/checkButton.h"
#include "gui/label.h"
#include "core.h"

namespace D2K {namespace GUI {namespace Turbo {
//private
extern void CheckButtonLFunction();
extern void CheckButtonRFunction();
extern void CheckButtonLeftFunction();
extern void CheckButtonRightFunction();
extern void CheckButtonUpFunction();
extern void CheckButtonDownFunction();
extern void CheckButtonAFunction();
extern void CheckButtonBFunction();
extern void CheckButtonXFunction();
extern void CheckButtonYFunction();
extern void CheckButtonStartFunction();
extern void CheckButtonSelectFunction();
extern void CheckButtonEnableFunction();
extern void CheckButtonBlueFunction();
extern void CheckButtonYellowFunction();
extern void CheckButtonRedFunction();
extern void CheckButtonGreenFunction();

WindowClass *Window;
Label *LabelTitle;
CheckButton *CheckButtonL;
CheckButton *CheckButtonR;
CheckButton *CheckButtonLeft;
CheckButton *CheckButtonRight;
CheckButton *CheckButtonUp;
CheckButton *CheckButtonDown;
CheckButton *CheckButtonA;
CheckButton *CheckButtonB;
CheckButton *CheckButtonX;
CheckButton *CheckButtonY;
CheckButton *CheckButtonStart;
CheckButton *CheckButtonSelect;
CheckButton *CheckButtonEnable;
CheckButton *CheckButtonBlue;
CheckButton *CheckButtonYellow;
CheckButton *CheckButtonRed;
CheckButton *CheckButtonGreen;

//public
WindowClass::WindowClass() : Window()
{
	Screen = 0;

	AppendObject(LabelTitle	= new Label(Screen, Rect(24,0+3,128,10), "Turbo Settings"));
				
	AppendObject(CheckButtonL = new CheckButton(Screen, Rect(39,39,10,10), "L", &CheckButtonLFunction));
	AppendObject(CheckButtonR = new CheckButton(Screen, Rect(205,40,10,10), "R", &CheckButtonRFunction));

	AppendObject(CheckButtonLeft = new CheckButton(Screen, Rect(9,95,10,10), "Left", &CheckButtonLeftFunction));
	AppendObject(CheckButtonRight = new CheckButton(Screen, Rect(69,95,10,10), "Right", &CheckButtonRightFunction));
	AppendObject(CheckButtonUp = new CheckButton(Screen, Rect(39,80,10,10), "Up", &CheckButtonUpFunction));
	AppendObject(CheckButtonDown = new CheckButton(Screen, Rect(39,110,10,10), "Down", &CheckButtonDownFunction));

	AppendObject(CheckButtonA = new CheckButton(Screen, Rect(235,95,10,10), "A", &CheckButtonAFunction));
	AppendObject(CheckButtonB = new CheckButton(Screen, Rect(205,110,10,10), "B", &CheckButtonBFunction));
	AppendObject(CheckButtonX = new CheckButton(Screen, Rect(205,80,10,10), "X", &CheckButtonXFunction));
	AppendObject(CheckButtonY = new CheckButton(Screen, Rect(175,95,10,10), "Y", &CheckButtonYFunction));

	AppendObject(CheckButtonStart = new CheckButton(Screen, Rect(145,115,10,10), "Start", &CheckButtonStartFunction));
	AppendObject(CheckButtonSelect = new CheckButton(Screen, Rect(85,115,10,10), "Select", &CheckButtonSelectFunction));
	AppendObject(CheckButtonEnable = new CheckButton(Screen, Rect(100,35,10,10), "Enable", &CheckButtonEnableFunction));

				
	AppendObject(CheckButtonBlue = new CheckButton(Screen, Rect(40,130,10,10), "Blue", &CheckButtonBlueFunction));
	AppendObject(CheckButtonYellow = new CheckButton(Screen, Rect(90,130,10,10), "Yellow", &CheckButtonYellowFunction));
	AppendObject(CheckButtonRed = new CheckButton(Screen, Rect(140,130,10,10), "Red", &CheckButtonRedFunction));
	AppendObject(CheckButtonGreen = new CheckButton(Screen, Rect(190,130,10,10), "Green", &CheckButtonGreenFunction));
}
WindowClass::~WindowClass() { }
uint16_t currentTurboKeys = 0;
void UpdateTurboKeys()
{
	currentTurboKeys = 0;
	if(CheckButtonL->GetChecked())
		currentTurboKeys |= KEY_L;
	if(CheckButtonR->GetChecked())
		currentTurboKeys |= KEY_R;
	if(CheckButtonLeft->GetChecked())
		currentTurboKeys |= KEY_LEFT;
	if(CheckButtonRight->GetChecked())
		currentTurboKeys |= KEY_RIGHT;
	if(CheckButtonUp->GetChecked())
		currentTurboKeys |= KEY_UP;
	if(CheckButtonDown->GetChecked())
		currentTurboKeys |= KEY_DOWN;
	if(CheckButtonA->GetChecked())
		currentTurboKeys |= KEY_A;
	if(CheckButtonB->GetChecked())
		currentTurboKeys |= KEY_B;
	if(CheckButtonX->GetChecked())
		currentTurboKeys |= KEY_X;
	if(CheckButtonY->GetChecked())
		currentTurboKeys |= KEY_Y;
	if(CheckButtonStart->GetChecked())
		currentTurboKeys |= KEY_START;
	if(CheckButtonSelect->GetChecked())
		currentTurboKeys |= KEY_SELECT;
}
uint16_t CheckTurboKey(uint16_t a)
{
	if(currentTurboKeys&a)
		if(keysHeld()&a)
			return a;
	return 0;
}
uint16_t GetKeys()
{
	uint16_t keys = 0;
	if(CheckButtonEnable->GetChecked() == false)
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
			
//gh
uint8_t currentGHTurboKeys = 0;
void GHUpdateTurboKeys()
{
	currentGHTurboKeys = 0;
	if(CheckButtonBlue->GetChecked())
		currentGHTurboKeys |= GUITARGRIP_BLUE;
	if(CheckButtonYellow->GetChecked())
		currentGHTurboKeys |= GUITARGRIP_YELLOW;
	if(CheckButtonRed->GetChecked())
		currentGHTurboKeys |= GUITARGRIP_RED;
	if(CheckButtonGreen->GetChecked())
		currentGHTurboKeys |= GUITARGRIP_GREEN;
}
uint8_t GHCheckTurboKey(uint16_t a)
{
	if(currentGHTurboKeys&a)
		if(guitarGripKeysHeld()&a)
			return a;
	return 0;
}
uint8_t GHGetKeys()
{
	uint8_t keys = 0;
	if(CheckButtonEnable->GetChecked() == false)
		return 0;
	GHUpdateTurboKeys();
	keys |= GHCheckTurboKey(GUITARGRIP_BLUE);
	keys |= GHCheckTurboKey(GUITARGRIP_YELLOW);
	keys |= GHCheckTurboKey(GUITARGRIP_RED);
	keys |= GHCheckTurboKey(GUITARGRIP_GREEN);
	return keys;
}

void CheckButtonLFunction()
{
	CheckButtonL->SetChecked(!CheckButtonL->GetChecked());
}
void CheckButtonRFunction()
{
	CheckButtonR->SetChecked(!CheckButtonR->GetChecked());
}
void CheckButtonLeftFunction()
{
	CheckButtonLeft->SetChecked(!CheckButtonLeft->GetChecked());
}
void CheckButtonRightFunction()
{
	CheckButtonRight->SetChecked(!CheckButtonRight->GetChecked());
}
void CheckButtonUpFunction()
{
	CheckButtonUp->SetChecked(!CheckButtonUp->GetChecked());
}
void CheckButtonDownFunction()
{
	CheckButtonDown->SetChecked(!CheckButtonDown->GetChecked());
}
void CheckButtonAFunction()
{
	CheckButtonA->SetChecked(!CheckButtonA->GetChecked());
}
void CheckButtonBFunction()
{
	CheckButtonB->SetChecked(!CheckButtonB->GetChecked());
}
void CheckButtonXFunction()
{
	CheckButtonX->SetChecked(!CheckButtonX->GetChecked());
}
void CheckButtonYFunction()
{
	CheckButtonY->SetChecked(!CheckButtonY->GetChecked());
}
void CheckButtonStartFunction()
{
	CheckButtonStart->SetChecked(!CheckButtonStart->GetChecked());
}
void CheckButtonSelectFunction()
{
	CheckButtonSelect->SetChecked(!CheckButtonSelect->GetChecked());
}
void CheckButtonEnableFunction()
{
	CheckButtonEnable->SetChecked(!CheckButtonEnable->GetChecked());
}
void CheckButtonBlueFunction()
{
	CheckButtonBlue->SetChecked(!CheckButtonBlue->GetChecked());
}
void CheckButtonYellowFunction()
{
	CheckButtonYellow->SetChecked(!CheckButtonYellow->GetChecked());
}
void CheckButtonGreenFunction()
{
	CheckButtonGreen->SetChecked(!CheckButtonGreen->GetChecked());
}
void CheckButtonRedFunction()
{
	CheckButtonRed->SetChecked(!CheckButtonRed->GetChecked());
}
}}}//namespace D2K::GUI::Turbo
