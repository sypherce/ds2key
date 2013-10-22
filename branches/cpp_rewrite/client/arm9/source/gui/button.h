#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <string>	//std::string
#include "gui.h"	//Rect
#include "label.h"

namespace D2K {
	namespace GUI {
		class Button : public Label {
			public:
				Button(uint8_t Screen, GUI::Rect Rect, std::string String, void (*function)());
				~Button();
				bool Draw();
				void setStatus(uint8_t Value);
		};
	}
}

#endif//__BUTTON_H__
