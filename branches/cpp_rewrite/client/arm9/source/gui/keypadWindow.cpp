#include <stdio.h>
#include "../ds2key.h"
#include "../config.h"
#include "../system.h"
#include "mainWindow.h"
#include "keypadWindow.h"

namespace D2K {
	namespace GUI {
		namespace KeypadWindow {
			bool Screen = 0;
			const unsigned ListMax = 50;
			D2K::GUI::Object *List[ListMax];
			D2K::GUI::Button *button1;
			D2K::GUI::Button *button2;
			D2K::GUI::Button *button3;
			D2K::GUI::Button *button4;
			D2K::GUI::Button *button5;
			D2K::GUI::Button *button6;
			D2K::GUI::Button *button7;
			D2K::GUI::Button *button8;
			D2K::GUI::Button *button9;
			D2K::GUI::Button *buttonPeriod;
			D2K::GUI::Button *button0;
			D2K::GUI::Button *buttonBackspace;
			D2K::GUI::Button *buttonEnter;

			D2K::GUI::Edit *edit;
			D2K::GUI::Label *label;

			char *Buffer = NULL;
			int BufferLen = 0;

			void append(char c) {
				int length = strlen(Buffer);
				if(length < BufferLen - 1) {
					Buffer[length] = c;
					Buffer[length+1] = 0;
				}
			}

			char *KeyPadEdit(Label *label, Edit *edit, std::string String, int maxLength) {
				static Rect *labelRect = new Rect();
					labelRect->setX(label->getRect()->getX());
					labelRect->setY(KeypadWindow::label->getRect()->getY());
					labelRect->setW(label->getRect()->getW());
					labelRect->setH(KeypadWindow::label->getRect()->getH());
					KeypadWindow::label->setRect(labelRect);
					KeypadWindow::label->setText(label->getText());
				static Rect *editRect = new Rect();
					editRect->setX(edit->getRect()->getX());
					editRect->setY(KeypadWindow::edit->getRect()->getY());
					editRect->setW(edit->getRect()->getW());
					editRect->setH(KeypadWindow::edit->getRect()->getH());
					KeypadWindow::edit->setRect(editRect);
					KeypadWindow::edit->setText(edit->getText());

				GUI::MainWindow::setVisible(false);
				KeypadWindow::setVisible(true);

				Draw();
				char *keyBuffer = new char[maxLength + 1];
				strcpy(keyBuffer, String.c_str());
				KeypadWindow::setBuffer(keyBuffer, maxLength + 1);
				while(KeypadWindow::List[0]->isVisible()) {
					if(KeypadWindow::Update()) {	//if pressed
						KeypadWindow::edit->setText(keyBuffer);		//set text
					}
					System::Update(true);
				}
				KeypadWindow::setVisible(false);

				GUI::MainWindow::setVisible(true);
				return keyBuffer;
			}

			void setBuffer(char *Buffer, int BufferLen) {
				D2K::GUI::KeypadWindow::Buffer = Buffer;
				D2K::GUI::KeypadWindow::BufferLen = BufferLen;
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
				int length = strlen(Buffer);
					Buffer[length-1] = 0;
			}
			void buttonEnterFunction() {
				D2K::GUI::KeypadWindow::setVisible(false);
				D2K::GUI::MainWindow::setVisible(true);
			}
			void Setup() {
				for(unsigned int i = 0; i < ListMax; i++) {
					List[i] = NULL;
				}
				int x = 107;
				int y = 116;
				int w = 10;
				int h = 10;
				int gap = 6;

				label			= new D2K::GUI::Label(Screen, new D2K::GUI::Rect(64,24,40,11), "");
				edit			= new D2K::GUI::Edit(Screen, new D2K::GUI::Rect(96,24,95,11), "", &GUI::VoidFunction);

				button1 		= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + 0, y, w, h), "1", &button1Function);
				button2 		= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + w + gap, y, w, h), "2", &button2Function);
				button3 		= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + (w * 2) + (gap * 2), y, w, h), "3", &button3Function);
				y += h + gap;
				button4 		= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + 0, y, w, h), "4", &button4Function);
				button5 		= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + w + gap, y, w, h), "5", &button5Function);
				button6 		= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + (w * 2) + (gap * 2), y, w, h), "6", &button6Function);
				y += h + gap;
				button7 		= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + 0, y, w, h), "7", &button7Function);
				button8 		= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + w + gap, y, w, h), "8", &button8Function);
				button9 		= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + (w * 2) + (gap * 2), y, w, h), "9", &button9Function);
				y += h + gap;
				buttonPeriod 	= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + 0, y, w, h), ".", &buttonPeriodFunction);
				button0 		= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + w + gap, y, w, h), "0", &button0Function);
				buttonBackspace	= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + (w * 2) + (gap * 2), y, w, h), "<", &buttonBackspaceFunction);
				y += h + gap;
				buttonEnter 	= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + 0, y, 42, h), "Enter", &buttonEnterFunction);

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
			void Draw() {
				if(GUI::isUpdated()) {
					D2K::GUI::ClearScreen(Screen, Color[colorBackground]);
				}
				for(unsigned int i = 0; i < ListMax; i++) {
					if(List[i]) {
						if(List[i]->getType() == ObjectLabel) {
							if(static_cast<GUI::Label*>(List[i])->Draw())
								if(EMULATOR) printf("label draw %i: %s\n", i, static_cast<GUI::Label*>(List[i])->getText().c_str());
						}
						else if(List[i]->getType() == ObjectButton) {
							if(static_cast<GUI::Button*>(List[i])->Draw())
								if(EMULATOR) printf("button draw %i: %s\n", i, static_cast<GUI::Button*>(List[i])->getText().c_str());
						}
						else if(List[i]->getType() == ObjectEdit) {
							if(static_cast<GUI::Edit*>(List[i])->Draw())
								if(EMULATOR) printf("button draw %i: %s\n", i, static_cast<GUI::Edit*>(List[i])->getText().c_str());
						}
					}
				}
				if(GUI::isUpdated()) {
					GUI::setUpdate(false);
				}
			}
			void setVisible(bool Visible) {
				for(unsigned int i = 0; i < ListMax; i++) {
					if(List[i])
						List[i]->setVisible(Visible);
				}
			}
			bool Update() {
				KeypadWindow::Draw();
				for(unsigned int i = 0; i < ListMax; i++) {
					if(keysDown()&KEY_TOUCH)
						if(List[i])
							if(List[i]->isVisible())
								if(List[i]->isClicked((uint8_t)System::stylusPos.px, (uint8_t)System::stylusPos.py)) {
									List[i]->function();
									return 1;
								}
				}

				return 0;
			}
		}
	}
}
