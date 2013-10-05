#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "object.h"

namespace D2K {
	namespace GUI {
		class Window {
			private:
				const static unsigned ListMax = 50;
			public:
				Window();
				~Window();
				bool Screen;
				GUI::Object *List[ListMax];
				void Draw();
				void setVisible(bool Visible);
				bool isVisible();
				bool Update();
		};
	}
}

#endif//__WINDOW_H__
