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
				void AppendObject(Object *object);
				void RemoveObject(Object *object);
				bool Screen;
				void Draw();
				void SetVisible(bool visible);
				bool IsVisible();
				bool CheckClick(Object *object);//needs relocated?
				bool Update();
		};
	}
}

#endif//__WINDOW_H__
