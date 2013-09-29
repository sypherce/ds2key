#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <nds/ndstypes.h>	//u8
#include <string>	//std::string
#include "gui.h"	//Rect
#include "label.h"

namespace D2K {
	namespace GUI {
		class Button : public Label {
			public:
				Button(u8 Screen, GUI::Rect *Rect, std::string String, void (*function)());
				//Button(u8 Screen, GUI::Rect *Rect, std::string String);
				~Button();
				bool Draw();
		};
	}
}

#endif//__BUTTON_H__
