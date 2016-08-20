#include <string>//std::string
#include "keyboardWindow.h"
#include "configWindow.h"
#include "turboWindow.h"
#include "gui/gui.h"
#include "common/udp.h"
#include "config.h"
#include "core.h"

namespace D2K {namespace GUI {namespace Keyboard {

WindowClass g_window;

Edit* edit_entry;
Label* label_entry;

std::string Buffer;
size_t BufferLen = 0;
void append(char c)
{
	ConfigWindow::current_pressed_key = c;
	switch(c)
	{
	//TODO::UPDATE COMMMENTED SECTION
	/*case ';':
	{
		ConfigWindow::current_pressed_key = 0xba;
		break;
	}
	case '\'':
	{
		ConfigWindow::current_pressed_key = 0xde;
		break;
	}
	case '.':
	{
		ConfigWindow::current_pressed_key = 0xbe;
		break;
	}
	case ',':
	{
		ConfigWindow::current_pressed_key = 0xbc;
		break;
	}
	case '=':
	{
		ConfigWindow::current_pressed_key = 0xbb;
		break;
	}*/
	case WINKEY_BACK:
	{
		if(Buffer.length() > 0)
			Buffer.resize(Buffer.length() - 1);
		return;
	}
	}
	if(BufferLen > Buffer.size())
	{
		Buffer += c;
	}
}

void ButtonCloseFunction()
{
	Keyboard::g_window.SetVisible(false);
}

void ButtonEnterFunction()
{
	append(WINKEY_RETURN);
	ButtonCloseFunction();
}

void Button1Function()              { append(WINKEY_1); }
void Button2Function()              { append(WINKEY_2); }
void Button3Function()              { append(WINKEY_3); }
void Button4Function()              { append(WINKEY_4); }
void Button5Function()              { append(WINKEY_5); }
void Button6Function()              { append(WINKEY_6); }
void Button7Function()              { append(WINKEY_7); }
void Button8Function()              { append(WINKEY_8); }
void Button9Function()              { append(WINKEY_9); }
void Button0Function()              { append(WINKEY_0); }

void ButtonMinusFunction()          { append(WINKEY_OEM_MINUS); }
void ButtonEqualsFunction()         { append(WINKEY_OEM_PLUS); }

void ButtonEscapeFunction()         { append(WINKEY_ESCAPE); }
void ButtonF1Function()             { append(WINKEY_F1); }
void ButtonF2Function()             { append(WINKEY_F2); }
void ButtonF3Function()             { append(WINKEY_F3); }
void ButtonF4Function()             { append(WINKEY_F4); }
void ButtonF5Function()             { append(WINKEY_F5); }
void ButtonF6Function()             { append(WINKEY_F6); }
void ButtonF7Function()             { append(WINKEY_F7); }
void ButtonF8Function()             { append(WINKEY_F8); }
void ButtonF9Function()             { append(WINKEY_F9); }
void ButtonF10Function()            { append(WINKEY_F10); }
void ButtonF11Function()            { append(WINKEY_F11); }
void ButtonF12Function()            { append(WINKEY_F12); }

void ButtonBackQuoteFunction()      { append(WINKEY_OEM_3); }
void ButtonTabFunction()            { append(WINKEY_TAB); }
void ButtonLeftBracketFunction()    { append(WINKEY_OEM_4); }
void ButtonRightBracketFunction()   { append(WINKEY_OEM_6); }
void ButtonBackSlashFunction()      { append(WINKEY_OEM_5); }
void ButtonCapsLockFunction()       { append(WINKEY_CAPITAL);}
void ButtonSemiColonFunction()      { append(WINKEY_OEM_1); }
void ButtonQuoteFunction()          { append(WINKEY_OEM_7); }
void ButtonLeftShiftFunction()      { append(WINKEY_LSHIFT); }
void ButtonCommaFunction()          { append(WINKEY_OEM_COMMA); }
void ButtonPeriodFunction()         { append(WINKEY_OEM_PERIOD); }
void ButtonSlashFunction()          { append(WINKEY_OEM_2); }
void ButtonRightShiftFunction()     { append(WINKEY_RSHIFT); }
void ButtonLeftControlFunction()    { append(WINKEY_LCONTROL); }
void ButtonLeftWinFunction()        { append(WINKEY_LWIN); }
void ButtonLeftAltFunction()        { append(WINKEY_LMENU); }
void ButtonSpaceFunction()          { append(WINKEY_SPACE); }
void ButtonRightAltFunction()       { append(WINKEY_RMENU); }
void ButtonRightWinFunction()       { append(WINKEY_RWIN); }
void ButtonRightControlFunction()   { append(WINKEY_RCONTROL); }

void ButtonAFunction()              { append(WINKEY_A); }
void ButtonBFunction()              { append(WINKEY_B); }
void ButtonCFunction()              { append(WINKEY_C); }
void ButtonDFunction()              { append(WINKEY_D); }
void ButtonEFunction()              { append(WINKEY_E); }
void ButtonFFunction()              { append(WINKEY_F); }
void ButtonGFunction()              { append(WINKEY_G); }
void ButtonHFunction()              { append(WINKEY_H); }
void ButtonIFunction()              { append(WINKEY_I); }
void ButtonJFunction()              { append(WINKEY_J); }
void ButtonKFunction()              { append(WINKEY_K); }
void ButtonLFunction()              { append(WINKEY_L); }
void ButtonMFunction()              { append(WINKEY_M); }
void ButtonNFunction()              { append(WINKEY_N); }
void ButtonOFunction()              { append(WINKEY_O); }
void ButtonPFunction()              { append(WINKEY_P); }
void ButtonQFunction()              { append(WINKEY_Q); }
void ButtonRFunction()              { append(WINKEY_R); }
void ButtonSFunction()              { append(WINKEY_S); }
void ButtonTFunction()              { append(WINKEY_T); }
void ButtonUFunction()              { append(WINKEY_U); }
void ButtonVFunction()              { append(WINKEY_V); }
void ButtonWFunction()              { append(WINKEY_W); }
void ButtonXFunction()              { append(WINKEY_X); }
void ButtonYFunction()              { append(WINKEY_Y); }
void ButtonZFunction()              { append(WINKEY_Z); }

void ButtonPrintScreenFunction()    { append(WINKEY_PRINT); }
void ButtonScrollLockFunction()     { append(WINKEY_SCROLL); }
void ButtonPauseBreakFunction()     { append(WINKEY_PAUSE); }
void ButtonInsertFunction()         { append(WINKEY_INSERT); }
void ButtonHomeFunction()           { append(WINKEY_HOME); }
void ButtonPageUpFunction()         { append(WINKEY_PRIOR); }
void ButtonNumLockFunction()        { append(WINKEY_NUMLOCK); }
void ButtonNumPadSlashFunction()    { append(WINKEY_DIVIDE); }
void ButtonNumPadAsteriskFunction() { append(WINKEY_MULTIPLY); }
void ButtonDeleteFunction()         { append(WINKEY_DELETE); } // TODO: currently unsupported
void ButtonNumPadMinusFunction()    { append(WINKEY_SUBTRACT); }
void ButtonEndFunction()            { append(WINKEY_END); }
void ButtonPageDownFunction()       { append(WINKEY_NEXT); }
void ButtonNumPad7Function()        { append(WINKEY_NUMPAD7); }
void ButtonNumPad8Function()        { append(WINKEY_NUMPAD8); }
void ButtonNumPad9Function()        { append(WINKEY_NUMPAD9); }
void ButtonNumPadPlusFunction()     { append(WINKEY_ADD); }
void ButtonNumPad4Function()        { append(WINKEY_NUMPAD4); }
void ButtonNumPad5Function()        { append(WINKEY_NUMPAD5); }
void ButtonNumPad6Function()        { append(WINKEY_NUMPAD6); }
void ButtonNumPad1Function()        { append(WINKEY_NUMPAD1); }
void ButtonNumPad2Function()        { append(WINKEY_NUMPAD2); }
void ButtonNumPad3Function()        { append(WINKEY_NUMPAD3); }
void ButtonNumPadEnterFunction()    { append(WINKEY_RETURN); } // TODO: currently unsupported
void ButtonNumPad0Function()        { append(WINKEY_NUMPAD0); }
void ButtonNumPadPeriodFunction()   { append(WINKEY_DECIMAL); }
void ButtonUpFunction()             { append(WINKEY_UP); }
void ButtonDownFunction()           { append(WINKEY_DOWN); }
void ButtonLeftFunction()           { append(WINKEY_LEFT); }
void ButtonRightFunction()          { append(WINKEY_RIGHT); }
void ButtonBackspaceFunction()      { append(WINKEY_BACK); }

WindowClass::WindowClass() : Window()
{
	m_screen = 0;
	const int gap = 1;
	int x = gap;
	int y = gap;
	const int w = 15;
	const int h = 15;
	const int keyboard_base_x = gap;
	const int keyboard_base_y = gap;
	
	const int print_scroll_pause_base_x = keyboard_base_x;
	const int print_scroll_pause_base_y = gap + (h + gap) * 6;
	const int ins_home_pgup_base_x      = keyboard_base_x;
	const int ins_home_pgup_base_y      = gap + (h + gap) * 8;
	const int up_down_left_right_base_x = keyboard_base_x;
	const int up_down_left_right_base_y = gap + (h + gap) * 10;

	{
		AppendObject(new Button(m_screen, Rect(229, 1, 10, 10), "Close", ButtonCloseFunction));
		//Keyboard
		{
		//Function row
		{
		x = keyboard_base_x;
		y = keyboard_base_y;
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "Esc", ButtonEscapeFunction)); (x += w + 7 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "F1",  ButtonF1Function));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "F2",  ButtonF2Function));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "F3",  ButtonF3Function));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "F4",  ButtonF4Function));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "F5",  ButtonF5Function));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "F6",  ButtonF6Function));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "F7",  ButtonF7Function));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "F8",  ButtonF8Function));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "F9",  ButtonF9Function));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "F10", ButtonF10Function));    (x += w + 7 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "F11", ButtonF11Function));    (x += w + 7 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "F12", ButtonF12Function));    (x += w + gap);
		}
		//number row
		{
		x = keyboard_base_x;
		y += h + gap;
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "`",  ButtonBackQuoteFunction)); (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "1",  Button1Function));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "2",  Button2Function));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "3",  Button3Function));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "4",  Button4Function));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "5",  Button5Function));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "6",  Button6Function));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "7",  Button7Function));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "8",  Button8Function));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "9",  Button9Function));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "0",  Button0Function));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "-",  ButtonMinusFunction));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   "=",  ButtonEqualsFunction));    (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*2, h), "<-", ButtonBackspaceFunction)); (x += w*2 + gap);
		}
		//qwerty row
		{
		x = keyboard_base_x;
		y += h + gap;
		AppendObject(new Button(m_screen, Rect(x, y, w*1.6, h), "Tab", ButtonTabFunction));          (x += w*1.6 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "Q",   ButtonQFunction));            (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "W",   ButtonWFunction));            (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "E",   ButtonEFunction));            (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "R",   ButtonRFunction));            (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "T",   ButtonTFunction));            (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "Y",   ButtonYFunction));            (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "U",   ButtonUFunction));            (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "I",   ButtonIFunction));            (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "O",   ButtonOFunction));            (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "P",   ButtonPFunction));            (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "[",   ButtonLeftBracketFunction));  (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "]",   ButtonRightBracketFunction)); (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*1.4, h), "\\",  ButtonBackSlashFunction));    (x += w*1.4 + gap);
		}
		//asdfgh row
		{
		x = keyboard_base_x;
		y += h + gap;
		AppendObject(new Button(m_screen, Rect(x, y, w*1.8, h),  "Caps",  ButtonCapsLockFunction));  (x += w*1.8 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "A",     ButtonAFunction));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "S",     ButtonSFunction));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "D",     ButtonDFunction));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "F",     ButtonFFunction));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "G",     ButtonGFunction));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "H",     ButtonHFunction));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "J",     ButtonJFunction));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "K",     ButtonKFunction));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "L",     ButtonLFunction));         (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      ";",     ButtonSemiColonFunction)); (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "'",     ButtonQuoteFunction));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*2.3, h),  "Enter", ButtonEnterFunction));     (x += w*2.3 + gap);
		}
		//zxcvbn row
		{
		x = keyboard_base_x;
		y += h + gap;
		AppendObject(new Button(m_screen, Rect(x, y, w*2.6, h),   "Shift", ButtonLeftShiftFunction));  (x += w*2.6 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),       "Z",     ButtonZFunction));          (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),       "X",     ButtonXFunction));          (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),       "C",     ButtonCFunction));          (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),       "V",     ButtonVFunction));          (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),       "B",     ButtonBFunction));          (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),       "N",     ButtonNFunction));          (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),       "M",     ButtonMFunction));          (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),       ",",     ButtonCommaFunction));      (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),       ".",     ButtonPeriodFunction));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),       "/",     ButtonSlashFunction));      (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*2.5+1, h), "Shift", ButtonRightShiftFunction)); (x += w*2.5+1 + gap);
		}
		//ctrl row
		{
		x = keyboard_base_x;
		y += h + gap;
		AppendObject(new Button(m_screen, Rect(x, y, w*1.8, h), "Ctrl",  ButtonLeftControlFunction));  (x += w*1.8 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*1.5, h), "Win",   ButtonLeftWinFunction));      (x += w*1.5 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*1.5, h), "Alt",   ButtonLeftAltFunction));      (x += w*1.5 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*6, h),   "Space", ButtonSpaceFunction));        (x += w*6 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*1.5, h), "Alt",   ButtonRightAltFunction));     (x += w*1.5 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*1.5, h), "Win",   ButtonRightWinFunction));     (x += w*1.5 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*1.8, h), "Ctrl",  ButtonRightControlFunction)); (x += w*1.8 + gap);
		}
		}

		//printscroll pause
		{{
		x = print_scroll_pause_base_x;
		y = print_scroll_pause_base_y;
		AppendObject(new Button(m_screen, Rect(x, y, w*2, h), "PrSc", ButtonPrintScreenFunction)); (x += w*2 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*2, h), "ScLk", ButtonScrollLockFunction));  (x += w*2 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*2, h), "PsBk", ButtonPauseBreakFunction));  (x += w*2 + gap);
		}}

		//ins home pgup del end pgdown
		{
		//ins home pgup
		{
		x = ins_home_pgup_base_x;
		y = ins_home_pgup_base_y;
		AppendObject(new Button(m_screen, Rect(x, y, w*2, h), "Ins",  ButtonInsertFunction)); (x += w*2 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*2, h), "Home", ButtonHomeFunction));   (x += w*2 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*2, h), "PgUp", ButtonPageUpFunction)); (x += w*2 + gap);
		}
		//del end pgdown
		{
		x = ins_home_pgup_base_x;
		y += h + gap; // row 2
		AppendObject(new Button(m_screen, Rect(x, y, w*2, h), "Del",  ButtonDeleteFunction));   (x += w*2 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*2, h), "End",  ButtonEndFunction));      (x += w*2 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w*2, h), "PgDn", ButtonPageDownFunction)); (x += w*2 + gap);
		}
		}

		//numpad
		{
		const int numpad_base_x = gap + (w + gap) * 11;
		const int numpad_base_y = gap + (h + gap) * 7;
		//numlock row
		{
		x = numpad_base_x;
		y = numpad_base_y;
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "Nu", ButtonNumLockFunction));        (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "/",  ButtonNumPadSlashFunction));    (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "*",  ButtonNumPadAsteriskFunction)); (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "-",  ButtonNumPadMinusFunction));    (x += w + gap);
		}
		//7 8 9
		{
		x = numpad_base_x;
		y += h + gap;
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "7", ButtonNumPad7Function));    (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "8", ButtonNumPad8Function));    (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),     "9", ButtonNumPad9Function));    (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h * 2), "+", ButtonNumPadPlusFunction)); (x += w + gap);
		}
		//4 5 6
		{
		x = numpad_base_x;
		y += h + gap;
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "4", ButtonNumPad4Function)); (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "5", ButtonNumPad5Function)); (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "6", ButtonNumPad6Function)); (x += w + gap);
		}
		//1 2 3
		{
		x = numpad_base_x;
		y += h + gap;
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "1", ButtonNumPad1Function));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "2", ButtonNumPad2Function));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),      "3", ButtonNumPad3Function));     (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h * 2), "En", ButtonNumPadEnterFunction)); (x += w + gap);
		}
		//0, .
		{
		x = numpad_base_x;
		y += h + gap;
		AppendObject(new Button(m_screen, Rect(x, y, w*2, h), "0", ButtonNumPad0Function));      (x += w*2 + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h),   ".", ButtonNumPadPeriodFunction)); (x += w + gap);
		}
		}

		//up down left right
		{
		x = up_down_left_right_base_x + w + gap; // column 2
		y = up_down_left_right_base_y;           // row 1
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "^", ButtonUpFunction));    (x += w + gap);
		x = up_down_left_right_base_x;           // coloumn 1
		y = up_down_left_right_base_y + h + gap; // row 2
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "<", ButtonLeftFunction));  (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), "v", ButtonDownFunction));  (x += w + gap);
		AppendObject(new Button(m_screen, Rect(x, y, w, h), ">", ButtonRightFunction)); (x += w + gap);
		}
	}
}
WindowClass::~WindowClass() { }

//configWindow function
std::string GetString(Label* label, Edit* edit, std::string text, int maxLength)
{
	Keyboard::label_entry->SetRect(label->GetRect());
	Keyboard::label_entry->SetText(label->GetText());

	Keyboard::edit_entry->SetRect(edit->GetRect());
	Keyboard::edit_entry->SetText(edit->GetText());

	Buffer = text;
	BufferLen = maxLength;

	ConfigWindow::g_window.SetVisible(false);                       // Hide config window
	Keyboard::g_window.SetVisible(true);                            // Show keyboard

	Keyboard::g_window.Draw();                                      // And actually draw it
	while(D2K::Loop()
	   && Keyboard::g_window.IsVisible())
	{
		UDP::Update(g_keys_held, Turbo::GetKeys(), nullptr, 
			        &D2K::g_circle_position, &D2K::g_cstick_position,
			        &D2K::g_accel_status, &D2K::g_gyro_status,
			        &D2K::g_slider_volume_status, &D2K::g_slider_3d_status,
			        ConfigWindow::current_pressed_key); // Update keys and press active gamepad key
		if(Keyboard::g_window.Update())                         // If pressed
		{
			ConfigWindow::current_pressed_key = 0;
			Keyboard::edit_entry->SetText(Buffer);          // Set text
		}
	}
	Keyboard::g_window.SetVisible(false);                           // Hide keyboard

	ConfigWindow::g_window.SetVisible(true);                        // Go back to config window
	return Buffer;
}

//configWindow function
uint16_t GetKey()
{
	Buffer = "";
	BufferLen = 0;

	ConfigWindow::g_window.SetVisible(false); // Hide config window
	Keyboard::g_window.SetVisible(true);      // Show keyboard

	Keyboard::g_window.Draw();                // And actually draw it
	while(D2K::Loop()
	   && Keyboard::g_window.IsVisible())
	{
		if(Keyboard::g_window.Update())   // If pressed
		{
			break;
		}
	}
	Keyboard::g_window.SetVisible(false);     // Hide keyboard

	ConfigWindow::g_window.SetVisible(true);  // Go back to config window
	return ConfigWindow::current_pressed_key;
}

}}}//namespace D2K::GUI::Keyboard
