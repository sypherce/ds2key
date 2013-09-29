#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "object.h"

const static unsigned ListMax = 50;
namespace D2K {
	namespace GUI {
		class Window {
			private:
			public:
				Window();
				~Window();
				bool Screen;
				D2K::GUI::Object *List[ListMax];
				void Draw();
				void setVisible(bool Visible);
				bool Update();
		};
	}
}

#endif//__WINDOW_H__
