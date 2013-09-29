#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "window.h"	//Rect
#include "edit.h"	//Rect
#include "button.h"	//Rect
#include "label.h"	//Rect

namespace D2K {
	namespace GUI {
		class MainWindow : public Window {
			public:
				MainWindow();
				~MainWindow();
				bool Update();
		};
		extern GUI::MainWindow *mainWindow;
	}
}

#endif//__MAINWINDOW_H__
