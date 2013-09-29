#ifndef __KEYPADWINDOW2_H__
#define __KEYPADWINDOW2_H__

#include "window.h"	//Rect
#include "edit.h"	//Rect
#include "button.h"	//Rect
#include "label.h"	//Rect

			extern char *KeyPadEdit(D2K::GUI::Label *label, D2K::GUI::Edit *edit, std::string String, int maxLength);
namespace D2K {
	namespace GUI {
		class KeypadWindow : public Window {
			public:
				KeypadWindow();
				~KeypadWindow();
		};
		extern GUI::KeypadWindow *keypadWindow;
	}
}

#endif//__KEYPADWINDOW2_H__
