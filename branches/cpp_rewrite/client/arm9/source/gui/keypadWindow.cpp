#include <string>	//std::string
#include "keypadWindow.h"
#include "mainWindow.h"
#include "../ds2key.h"
#include "../config.h"
#include "../system.h"

namespace D2K {
	namespace GUI {
		namespace Keypad {
			//private
			extern void button1Function();
			extern void button2Function();
			extern void button3Function();
			extern void button4Function();
			extern void button5Function();
			extern void button6Function();
			extern void button7Function();
			extern void button8Function();
			extern void button9Function();
			extern void buttonPeriodFunction();
			extern void button0Function();
			extern void buttonBackspaceFunction();
			extern void buttonEnterFunction();

			//public
			WindowClass *Window = (WindowClass *)NULL;
			Button *button1;
			Button *button2;
			Button *button3;
			Button *button4;
			Button *button5;
			Button *button6;
			Button *button7;
			Button *button8;
			Button *button9;
			Button *buttonPeriod;
			Button *button0;
			Button *buttonBackspace;
			Button *buttonEnter;

			Edit *edit;
			Label *label;

			WindowClass::WindowClass() : Window() {
				Screen = 0;
				int x = 97;
				int y = 90;
				int w = 15;
				int h = 15;
				int gap = 6;

				label			= new Label(Screen, Rect(64,24,40,11), "");
				edit			= new Edit(Screen, Rect(96,24,95,11), "", &VoidFunction);

				button1 		= new Button(Screen, Rect(x + 0, y, w, h), "1", &button1Function);
				button2 		= new Button(Screen, Rect(x + w + gap, y, w, h), "2", &button2Function);
				button3 		= new Button(Screen, Rect(x + (w * 2) + (gap * 2), y, w, h), "3", &button3Function);
				y += h + gap;
				button4 		= new Button(Screen, Rect(x + 0, y, w, h), "4", &button4Function);
				button5 		= new Button(Screen, Rect(x + w + gap, y, w, h), "5", &button5Function);
				button6 		= new Button(Screen, Rect(x + (w * 2) + (gap * 2), y, w, h), "6", &button6Function);
				y += h + gap;
				button7 		= new Button(Screen, Rect(x + 0, y, w, h), "7", &button7Function);
				button8 		= new Button(Screen, Rect(x + w + gap, y, w, h), "8", &button8Function);
				button9 		= new Button(Screen, Rect(x + (w * 2) + (gap * 2), y, w, h), "9", &button9Function);
				y += h + gap;
				buttonPeriod 	= new Button(Screen, Rect(x + 0, y, w, h), ".", &buttonPeriodFunction);
				button0 		= new Button(Screen, Rect(x + w + gap, y, w, h), "0", &button0Function);
				buttonBackspace	= new Button(Screen, Rect(x + (w * 2) + (gap * 2), y, w, h), "<", &buttonBackspaceFunction);
				y += h + gap;
				buttonEnter 	= new Button(Screen, Rect(x + 0, y, 42, h), "Enter", &buttonEnterFunction);

				int i = 0;
				List[i++] = label;
				List[i++] = edit;
				List[i++] = button1;
				List[i++] = button2;
				List[i++] = button3;
				List[i++] = button4;
				List[i++] = button5;
				List[i++] = button6;
				List[i++] = button7;
				List[i++] = button8;
				List[i++] = button9;
				List[i++] = buttonPeriod;
				List[i++] = button0;
				List[i++] = buttonBackspace;
				List[i++] = buttonEnter;
			}
			WindowClass::~WindowClass() { }

			//private
			std::string Buffer;
			size_t BufferLen = 0;
			void append(char c) {
				if(BufferLen > Buffer.size())
					Buffer += c;
			}

			void buttonEnterFunction() {
				Keypad::Window->setVisible(false);
				Main::Window->setVisible(true);
			}

			void button1Function()			{ append('1'); }
			void button2Function()			{ append('2'); }
			void button3Function()			{ append('3'); }
			void button4Function()			{ append('4'); }
			void button5Function()			{ append('5'); }
			void button6Function()			{ append('6'); }
			void button7Function()			{ append('7'); }
			void button8Function()			{ append('8'); }
			void button9Function()			{ append('9'); }
			void buttonPeriodFunction()		{ append('.'); }
			void button0Function()			{ append('0'); }
			void buttonBackspaceFunction()	{
				if(Buffer.length() > 0)
					Buffer.resize(Buffer.length() - 1);
			}

			//public
			std::string Entry(Label *label, Edit *edit, std::string String, int maxLength) {
				if(Keypad::Window != (WindowClass*)NULL) {
					Keypad::label->setRect(label->getRect());
					Keypad::label->setText(label->getText());

					Keypad::edit->setRect(edit->getRect());
					Keypad::edit->setText(edit->getText());

					Buffer = String;
					BufferLen = maxLength;

					Main::Window->setVisible(false);			//hide main window
					Keypad::Window->setVisible(true);			//show keypad

					Keypad::Window->Draw();						//and actually draw it
					while(Keypad::Window->isVisible()) {
						if(Keypad::Window->Update()) {			//if pressed
							Keypad::edit->setText(Buffer);		//set text
						}
						System::Update(true);
					}
					Keypad::Window->setVisible(false);			//hide keypad

					Main::Window->setVisible(true);				//go back to main window
					return Buffer;
				}
				return "Error (Keypad::Entry): Keypad == NULL";//this shouldn't happen
			}
		}
	}
}


