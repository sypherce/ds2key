#include <string>//std::string
#include "keypadWindow.h"
#include "mainWindow.h"
#include "gui/gui.h"
#include "common/udp.h"
#include "config.h"
#include "core.h"

namespace D2K {
	namespace GUI {
		namespace Keypad {
			//private
			extern void Button1Function();
			extern void Button2Function();
			extern void Button3Function();
			extern void Button4Function();
			extern void Button5Function();
			extern void Button6Function();
			extern void Button7Function();
			extern void Button8Function();
			extern void Button9Function();
			extern void ButtonPeriodFunction();
			extern void Button0Function();
			extern void ButtonBackspaceFunction();
			extern void ButtonEnterFunction();

			WindowClass *Window = (WindowClass *)NULL;
			Button *Button1;
			Button *Button2;
			Button *Button3;
			Button *Button4;
			Button *Button5;
			Button *Button6;
			Button *Button7;
			Button *Button8;
			Button *Button9;
			Button *ButtonPeriod;
			Button *Button0;
			Button *ButtonBackspace;
			Button *ButtonEnter;

			Edit *EditEntry;
			Label *LabelEntry;

			//public
			WindowClass::WindowClass() : Window() {
				Screen = 0;
				int x = 97;
				int y = 90;
				int w = 15;
				int h = 15;
				int gap = 6;

				AppendObject(LabelEntry = new Label(Screen, Rect(64,24+3,40,10), ""));
				AppendObject(EditEntry = new Edit(Screen, Rect(96,24,95,10), "", &VoidFunction));

				AppendObject(Button1 = new Button(Screen, Rect(x + 0, y, w, h), "1", &Button1Function));
				AppendObject(Button2 = new Button(Screen, Rect(x + w + gap, y, w, h), "2", &Button2Function));
				AppendObject(Button3 = new Button(Screen, Rect(x + (w * 2) + (gap * 2), y, w, h), "3", &Button3Function));
				y += h + gap;
				AppendObject(Button4 = new Button(Screen, Rect(x + 0, y, w, h), "4", &Button4Function));
				AppendObject(Button5 = new Button(Screen, Rect(x + w + gap, y, w, h), "5", &Button5Function));
				AppendObject(Button6 = new Button(Screen, Rect(x + (w * 2) + (gap * 2), y, w, h), "6", &Button6Function));
				y += h + gap;
				AppendObject(Button7 = new Button(Screen, Rect(x + 0, y, w, h), "7", &Button7Function));
				AppendObject(Button8 = new Button(Screen, Rect(x + w + gap, y, w, h), "8", &Button8Function));
				AppendObject(Button9 = new Button(Screen, Rect(x + (w * 2) + (gap * 2), y, w, h), "9", &Button9Function));
				y += h + gap;
				AppendObject(ButtonPeriod = new Button(Screen, Rect(x + 0, y, w, h), ".", &ButtonPeriodFunction));
				AppendObject(Button0 = new Button(Screen, Rect(x + w + gap, y, w, h), "0", &Button0Function));
				AppendObject(ButtonBackspace = new Button(Screen, Rect(x + (w * 2) + (gap * 2), y, w, h), "<", &ButtonBackspaceFunction));
				y += h + gap;
				AppendObject(ButtonEnter = new Button(Screen, Rect(x + 0, y, 42, h), "Enter", &ButtonEnterFunction));
			}
			WindowClass::~WindowClass() { }

			//private
			std::string Buffer;
			size_t BufferLen = 0;
			void append(char c) {
				if(BufferLen > Buffer.size())
					Buffer += c;
			}

			void ButtonEnterFunction() {
				Keypad::Window->SetVisible(false);
				Main::Window->SetVisible(true);
			}

			void Button1Function()			{ append('1'); }
			void Button2Function()			{ append('2'); }
			void Button3Function()			{ append('3'); }
			void Button4Function()			{ append('4'); }
			void Button5Function()			{ append('5'); }
			void Button6Function()			{ append('6'); }
			void Button7Function()			{ append('7'); }
			void Button8Function()			{ append('8'); }
			void Button9Function()			{ append('9'); }
			void ButtonPeriodFunction()		{ append('.'); }
			void Button0Function()			{ append('0'); }
			void ButtonBackspaceFunction()	{
				if(Buffer.length() > 0)
					Buffer.resize(Buffer.length() - 1);
			}

			//public
			std::string Entry(Label *label, Edit *edit, std::string text, int maxLength) {
				if(Keypad::Window != (WindowClass*)NULL) {
					Keypad::LabelEntry->SetRect(label->GetRect());
					Keypad::LabelEntry->SetText(label->GetText());

					Keypad::EditEntry->SetRect(edit->GetRect());
					Keypad::EditEntry->SetText(edit->GetText());

					Buffer = text;
					BufferLen = maxLength;

					Main::Window->SetVisible(false);			//hide main window
					Keypad::Window->SetVisible(true);			//show keypad

					Keypad::Window->Draw();						//and actually draw it
					while(Keypad::Window->IsVisible()) {
						if(Keypad::Window->Update()) {			//if pressed
							Keypad::EditEntry->SetText(Buffer);	//set text
						}
						Core::Loop();
					}
					Keypad::Window->SetVisible(false);			//hide keypad

					Main::Window->SetVisible(true);				//go back to main window
					return Buffer;
				}
				return "Error (Keypad::Entry): Keypad == NULL";	//this shouldn't happen
			}
		}
	}
}


