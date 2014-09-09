#ifndef __STATUSBAR_H__
#define __STATUSBAR_H__

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "label.h"

namespace D2K {
	namespace GUI {
		class StatusBar : public Label {
			public:
				StatusBar(std::string text, int x, int y, int width, int height);
				~StatusBar();
				bool Attach(Object *parentObject);
				void SetText(std::string Text);
		};
	}
}
#endif//__STATUSBAR_H__