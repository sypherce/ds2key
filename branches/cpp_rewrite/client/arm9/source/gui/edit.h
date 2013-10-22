#ifndef __EDIT_H__
#define __EDIT_H__

#include <string>	//std::string
#include "gui.h"	//Rect
#include "button.h"

namespace D2K {
	namespace GUI {
		class Edit : public Button {
			public:
				Edit(uint8_t Screen, GUI::Rect Rect, std::string String, void (*function)());
				~Edit();
				bool Draw();
				void setStatus(uint8_t Value);
		};
	}
}

#endif//__EDIT_H__
