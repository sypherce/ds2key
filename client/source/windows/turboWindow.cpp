#include <string> // std::string

// windows
#include "turboWindow.h"
#include "barWindow.h"

// controls
#include "gui/checkButton.h"
#include "gui/label.h"

// core
#include "core.h"
#include "common/udp.h"

#ifdef _3DS
#include "gui/gui.h"
#endif

namespace D2K {namespace GUI {namespace Turbo {

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

WindowClass g_window;
CheckButton* check_button_l;
CheckButton* check_button_r;
CheckButton* check_button_left;
CheckButton* check_button_right;
CheckButton* check_button_up;
CheckButton* check_button_down;
CheckButton* check_button_a;
CheckButton* check_button_b;
CheckButton* check_button_x;
CheckButton* check_button_y;
CheckButton* check_button_start;
CheckButton* check_button_select;
CheckButton* check_button_enable;
CheckButton* check_button_blue;
CheckButton* check_button_yellow;
CheckButton* check_button_red;
CheckButton* check_button_green;

void ButtonCloseFunction()
{
	Turbo::g_window.SetVisible(false);
}

WindowClass::WindowClass() : Window()
{
	m_screen = 0;

	AppendObject(new Button(m_screen, Rect(220, 1, 10, 10), "Close", ButtonCloseFunction));
				
	AppendObject(check_button_l             = new CheckButton(m_screen, Rect(39,39,10,10), "L", &CheckButtonLFunction));
	AppendObject(check_button_r             = new CheckButton(m_screen, Rect(205,40,10,10), "R", &CheckButtonRFunction));

	AppendObject(check_button_left          = new CheckButton(m_screen, Rect(9,95,10,10), "Left", &CheckButtonLeftFunction));
	AppendObject(check_button_right         = new CheckButton(m_screen, Rect(69,95,10,10), "Right", &CheckButtonRightFunction));
	AppendObject(check_button_up            = new CheckButton(m_screen, Rect(39,80,10,10), "Up", &CheckButtonUpFunction));
	AppendObject(check_button_down          = new CheckButton(m_screen, Rect(39,110,10,10), "Down", &CheckButtonDownFunction));

	AppendObject(check_button_a             = new CheckButton(m_screen, Rect(235,95,10,10), "A", &CheckButtonAFunction));
	AppendObject(check_button_b             = new CheckButton(m_screen, Rect(205,110,10,10), "B", &CheckButtonBFunction));
	AppendObject(check_button_x             = new CheckButton(m_screen, Rect(205,80,10,10), "X", &CheckButtonXFunction));
	AppendObject(check_button_y             = new CheckButton(m_screen, Rect(175,95,10,10), "Y", &CheckButtonYFunction));

	AppendObject(check_button_start         = new CheckButton(m_screen, Rect(145,115,10,10), "Start", &CheckButtonStartFunction));
	AppendObject(check_button_select        = new CheckButton(m_screen, Rect(85,115,10,10), "Select", &CheckButtonSelectFunction));
	AppendObject(check_button_enable        = new CheckButton(m_screen, Rect(100,35,10,10), "Enable", &CheckButtonEnableFunction));

#ifdef _NDS
	AppendObject(check_button_blue          = new CheckButton(m_screen, Rect(40,130,10,10), "Blue", &CheckButtonBlueFunction));
	AppendObject(check_button_yellow        = new CheckButton(m_screen, Rect(90,130,10,10), "Yellow", &CheckButtonYellowFunction));
	AppendObject(check_button_red           = new CheckButton(m_screen, Rect(140,130,10,10), "Red", &CheckButtonRedFunction));
	AppendObject(check_button_green         = new CheckButton(m_screen, Rect(190,130,10,10), "Green", &CheckButtonGreenFunction));
#endif
}
WindowClass::~WindowClass() { }

void WindowClass::SetVisible(bool visible)
{
	Window::SetVisible(visible);

	if(!visible)
		return;

	Bar::SetText("[Turbo Settings]");
	Bar::g_window.SetVisible(true);

	ForceBacklightsOn(true);          // Lock backlights on
}
bool WindowClass::Update()
{
	UDP::Update(D2K::g_keys_held, Turbo::GetKeys(), nullptr,
	            &D2K::g_circle_position, &D2K::g_cstick_position,
	            &D2K::g_accel_status, &D2K::g_gyro_status,
	            &D2K::g_slider_volume_status, &D2K::g_slider_3d_status, 0);

	return Window::Update()
	    || Bar::g_window.Update();
}

uint32_t currentTurboKeys = 0;
void UpdateTurboKeys()
{
	currentTurboKeys = 0;
	if(check_button_a->GetChecked())
		currentTurboKeys |= KEY_A;
	if(check_button_b->GetChecked())
		currentTurboKeys |= KEY_B;
	if(check_button_x->GetChecked())
		currentTurboKeys |= KEY_X;
	if(check_button_y->GetChecked())
		currentTurboKeys |= KEY_Y;
	if(check_button_l->GetChecked())
		currentTurboKeys |= KEY_L;
	if(check_button_r->GetChecked())
		currentTurboKeys |= KEY_R;
	if(check_button_up->GetChecked())
		currentTurboKeys |= KEY_UP;
	if(check_button_down->GetChecked())
		currentTurboKeys |= KEY_DOWN;
	if(check_button_left->GetChecked())
		currentTurboKeys |= KEY_LEFT;
	if(check_button_right->GetChecked())
		currentTurboKeys |= KEY_RIGHT;
	if(check_button_start->GetChecked())
		currentTurboKeys |= KEY_START;
	if(check_button_select->GetChecked())
		currentTurboKeys |= KEY_SELECT;
}
uint32_t CheckTurboKey(uint32_t a)
{
	if((currentTurboKeys&a)
	&& (g_keys_held&a))
		return a;

	return 0;
}
uint32_t GetKeys()
{
	if(check_button_enable->GetChecked() == false)
		return 0;

	UpdateTurboKeys();

	uint32_t keys = 0;
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

void CheckButtonLFunction()
{
	check_button_l->ToggleChecked();
}
void CheckButtonRFunction()
{
	check_button_r->ToggleChecked();
}
void CheckButtonLeftFunction()
{
	check_button_left->ToggleChecked();
}
void CheckButtonRightFunction()
{
	check_button_right->ToggleChecked();
}
void CheckButtonUpFunction()
{
	check_button_up->ToggleChecked();
}
void CheckButtonDownFunction()
{
	check_button_down->ToggleChecked();
}
void CheckButtonAFunction()
{
	check_button_a->ToggleChecked();
}
void CheckButtonBFunction()
{
	check_button_b->ToggleChecked();
}
void CheckButtonXFunction()
{
	check_button_x->ToggleChecked();
}
void CheckButtonYFunction()
{
	check_button_y->ToggleChecked();
}
void CheckButtonStartFunction()
{
	check_button_start->ToggleChecked();
}
void CheckButtonSelectFunction()
{
	check_button_select->ToggleChecked();
}
void CheckButtonEnableFunction()
{
	check_button_enable->ToggleChecked();
}
void CheckButtonBlueFunction()
{
	check_button_blue->ToggleChecked();
}
void CheckButtonYellowFunction()
{
	check_button_yellow->ToggleChecked();
}
void CheckButtonGreenFunction()
{
	check_button_green->ToggleChecked();
}
void CheckButtonRedFunction()
{
	check_button_red->ToggleChecked();
}
}}} // namespace D2K::GUI::Turbo
