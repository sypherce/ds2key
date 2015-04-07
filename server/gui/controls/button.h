#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <windows.h>
#include <string>//std::string
#include "label.h"

namespace D2K {
	namespace GUI {
		class Button : public Label {
			public:
				Button(std::string text, int x, int y, int width, int height);
				~Button();
				bool Attach(Object *parentObject);
		};
	}
}
#endif//__BUTTON_H__
