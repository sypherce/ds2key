#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "gui.h"
#include "button.h"
#include "label.h"
#include "edit.h"

namespace D2K {
	namespace GUI {
		namespace MainWindow {
			enum {Default, EditIP, EditPort, EditProfile};
			extern void Setup();
			extern void Draw();
			extern bool Update();
			extern void setVisible(bool Visible);
		}
	}
}
#endif//__MAINWINDOW_H__
