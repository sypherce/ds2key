#ifndef __COMBOBUTTON_H__
#define __COMBOBUTTON_H__

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "label.h"
using std::string;

namespace D2K {
	namespace GUI {
		class ComboButton : public Label {
			public:
				ComboButton(int x, int y, int width, int height);
				~ComboButton();
				bool Attach(Object *parentObject);
				void Append(string text);
				void SetSelection(unsigned row);
				int GetSelection();
		};
	}
}
#endif//__COMBOBUTTON_H__
