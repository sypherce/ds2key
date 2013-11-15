#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <string>	//std::string
#include "rect.h"
#include "label.h"

namespace D2K {
	namespace GUI {
		class Button : public Label {
			public:
				Button(uint8_t screen, GUI::Rect rect, std::string text, void (*function)());
				~Button();
				bool Draw();
		};
	}
}

#endif//__BUTTON_H__
