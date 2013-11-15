#ifndef __CHECKBUTTON_H__
#define __CHECKBUTTON_H__

#include <string>	//std::string
#include "rect.h"
#include "button.h"

namespace D2K {
	namespace GUI {
		class CheckButton : public Button {
			private:
				bool Checked;
			public:
				CheckButton(uint8_t screen, GUI::Rect rect, std::string text, void (*function)());
				~CheckButton();
				bool Draw();
				bool GetChecked();
				void SetChecked(bool checked);
		};
	}
}

#endif//__CHECKBUTTON_H__
