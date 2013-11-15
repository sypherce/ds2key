#ifndef __LABEL_H__
#define __LABEL_H__

#include <string>//std::string
#include "object.h"

namespace D2K {
	namespace GUI {
		class Label : public Object {
			protected:
				std::string Text;
			public:
				Label(uint8_t screen, GUI::Rect rect, std::string text);
				~Label();
				void SetText(std::string text);
				std::string GetText();
				bool Draw();
		};
	}
}

#endif//__LABEL_H__
