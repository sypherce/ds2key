#ifndef __LABEL_H__
#define __LABEL_H__

#include <windows.h>
#include <string>//std::string
#include "object.h"
using std::string;

namespace D2K {
	namespace GUI {
		class Label : public Object {
			public:
				Label(string text, int x, int y, int width, int height);
				virtual ~Label();
				bool Attach(Object *parentObject);
				string GetText();
				void SetText(string text);
				void SetEnabled(bool enabled);
				long GetStyle();
				long SetStyle(long style);
			protected:
				string Text;
		};
	}
}
#endif//_LABEL_H__
