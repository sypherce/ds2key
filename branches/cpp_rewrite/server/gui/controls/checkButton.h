#ifndef __CHECKBUTTON_H__
#define __CHECKBUTTON_H__

#include <string>//std::string
#include "label.h"

namespace D2K {
	namespace GUI {
		class CheckButton : public Label {
			public:
				CheckButton(std::string text, int x, int y, int width, int height);
				~CheckButton();
				bool Attach(Object *parentObject);
				void SetChecked(bool checked);
		};
	}
}
#endif//__CHECKBUTTON_H__
