#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <vector>
#include "object.h"

namespace D2K {
	namespace GUI {
		class Window {
			private:
				std::vector<GUI::Object*> Objects;
				bool Visible;
			public:
				Window();
				~Window();
				void AppendObject(Object *Object);
				void RemoveObject(Object *Object);
				bool Screen;
				void Draw();
				void setVisible(bool Visible);
				bool isVisible();
				bool CheckClick(Object *thisObject);//needs relocated?
				bool Update();
		};
	}
}

#endif//__WINDOW_H__
