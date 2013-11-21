#ifndef __EDIT_H__
#define __EDIT_H__

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "label.h"
using std::string;

namespace D2K {
	namespace GUI {
		class Edit : public Label {
			public:
				Edit(string text, int x, int y, int width, int height);
				~Edit();
				bool Attach(Object *parentObject);
		};
	}
}
#endif//__EDIT_H__
