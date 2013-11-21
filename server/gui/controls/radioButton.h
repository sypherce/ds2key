#ifndef __RADIOBUTTON_H__
#define __RADIOBUTTON_H__

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "checkButton.h"
using std::string;

namespace D2K {
	namespace GUI {
		class RadioButton : public CheckButton {
			public:
				RadioButton(string text, int x, int y, int width, int height);
				~RadioButton();
				bool Attach(Object *parentObject);
		};
	}
}
#endif//__RADIOBUTTON_H__
