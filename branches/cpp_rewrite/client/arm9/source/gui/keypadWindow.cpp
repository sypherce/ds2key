#include <nds.h>	//u8, ARGB16
#include <string>	//std::string
#include "keypadWindow.h"

#include <stdio.h>
#include "../ds2key.h"
#include "../config.h"
#include "../system.h"
#include "mainWindow.h"
using namespace D2K;
using namespace D2K::GUI;
using D2K::GUI::KeypadWindow;

D2K::GUI::KeypadWindow *GUI::keypadWindow;
D2K::GUI::Button *button1;
D2K::GUI::Button *button2;
D2K::GUI::Button *button3;
D2K::GUI::Button *button4x;
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

void setBuffer(char *thisBuffer, int thisBufferLen) {
	Buffer = thisBuffer;
	BufferLen = thisBufferLen;
}

char *KeyPadEdit(Label *thisLabel, Edit *thisEdit, std::string String, int maxLength) {
	static Rect *labelRect = new Rect();
		labelRect->setX(thisLabel->getRect()->getX());
		labelRect->setY(label->getRect()->getY());
		labelRect->setW(thisLabel->getRect()->getW());
		labelRect->setH(label->getRect()->getH());
		label->setRect(labelRect);
		label->setText(thisLabel->getText());
	static Rect *editRect = new Rect();
		editRect->setX(thisEdit->getRect()->getX());
		editRect->setY(edit->getRect()->getY());
		editRect->setW(thisEdit->getRect()->getW());
		editRect->setH(edit->getRect()->getH());
		edit->setRect(editRect);
		edit->setText(thisEdit->getText());

	GUI::mainWindow->setVisible(false);
	GUI::keypadWindow->setVisible(true);

	GUI::keypadWindow->Draw();
	char *keyBuffer = new char[maxLength + 1];
	strcpy(keyBuffer, String.c_str());
	setBuffer(keyBuffer, maxLength + 1);
	while(GUI::keypadWindow->List[0]->isVisible()) {
		if(GUI::keypadWindow->Update()) {	//if pressed
			edit->setText(keyBuffer);		//set text
		}
		System::Update(true);
	}
	GUI::keypadWindow->setVisible(false);

	GUI::mainWindow->setVisible(true);
	return keyBuffer;
}

void button1Function()			{ append('1'); }
void button2Function()			{ append('2'); }
void button3Function()			{ append('3'); }
void button4xFunction()			{ append('4'); }
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
	D2K::GUI::keypadWindow->setVisible(false);
	D2K::GUI::mainWindow->setVisible(true);
}
KeypadWindow::KeypadWindow() : Window() {
	Screen = 0;
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
	button4x 		= new D2K::GUI::Button(Screen, new D2K::GUI::Rect(x + 0, y, w, h), "4", &button4xFunction);
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
	List[i++] = button4x;
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
KeypadWindow::~KeypadWindow() { }

