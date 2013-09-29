#ifndef __EDIT_H__
#define __EDIT_H__

#include <nds/ndstypes.h>	//u8
#include <string>	//std::string
#include "gui.h"	//Rect
#include "button.h"

namespace D2K {
	namespace GUI {
		class Edit : public Button {
			public:
				Edit(u8 Screen, GUI::Rect *Rect, std::string String, void (*function)());
				//Button(u8 Screen, GUI::Rect *Rect, std::string String);
				~Edit();
				bool Draw();
		};
	}
}

#endif//__EDIT_H__
