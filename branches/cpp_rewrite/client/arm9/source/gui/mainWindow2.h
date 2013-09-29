#ifndef __MainWindow2_H__
#define __MainWindow2_H__

#include "window.h"	//Rect
#include "edit.h"	//Rect
#include "button.h"	//Rect
#include "label.h"	//Rect

namespace D2K {
	namespace GUI {
		class MainWindow2 : public Window {
			public:
				MainWindow2();
				~MainWindow2();
				bool Update();
		};
	}
}

#endif//__MainWindow2_H__
