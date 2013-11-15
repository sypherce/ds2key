#ifndef __EDIT_H__
#define __EDIT_H__

#include <string>	//std::string
#include "rect.h"
#include "button.h"

namespace D2K {
	namespace GUI {
		class Edit : public Button {
			public:
				Edit(uint8_t screen, GUI::Rect rect, std::string text, void (*function)());
				~Edit();
				bool Draw();
		};
	}
}

#endif//__EDIT_H__
