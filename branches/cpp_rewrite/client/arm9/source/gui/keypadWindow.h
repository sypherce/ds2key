#ifndef __KEYPADWINDOW_H__
#define __KEYPADWINDOW_H__

#include "gui.h"
#include "button.h"
#include "label.h"
#include "edit.h"

namespace D2K {
	namespace GUI {
		namespace KeypadWindow {
			extern D2K::GUI::Label *label;
			extern D2K::GUI::Edit *edit;
			extern D2K::GUI::Button *button1;
			extern D2K::GUI::Button *button2;
			extern D2K::GUI::Button *button3;
			extern D2K::GUI::Button *button4;
			extern D2K::GUI::Button *button5;
			extern D2K::GUI::Button *button6;
			extern D2K::GUI::Button *button7;
			extern D2K::GUI::Button *button8;
			extern D2K::GUI::Button *button9;
			extern D2K::GUI::Button *buttonPeriod;
			extern D2K::GUI::Button *button0;
			extern D2K::GUI::Button *buttonBackspace;
			extern char *Buffer;
			extern int BufferLen;
			extern char *KeyPadEdit(Label *label, Edit *edit, std::string String, int maxLength);

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
			extern void Setup();
			extern void setBuffer(char *Buffer, int BufferLen);
			extern void Draw();
			extern bool Update();
			extern void setVisible(bool Visible);
		}
	}
}
#endif//__KEYPADWINDOW_H__
