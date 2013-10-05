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
				Label(uint8_t Screen, GUI::Rect Rect, std::string String);
				~Label();
				void setText(std::string Text);
				std::string getText();
				bool Draw();
		};
	}
}

#endif//__LABEL_H__
