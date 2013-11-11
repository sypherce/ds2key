#ifndef __CHECKBUTTON_H__
#define __CHECKBUTTON_H__

#include <string>	//std::string
#include "gui.h"	//Rect
#include "label.h"

namespace D2K {
	namespace GUI {
		class CheckButton : public Label {
			private:
				bool Checked;
			public:
				CheckButton(uint8_t Screen, GUI::Rect Rect, std::string String, void (*Function)());
				~CheckButton();
				bool Draw();
				bool getChecked();
				void setChecked(bool Checked);
		};
	}
}

#endif//__CHECKBUTTON_H__
