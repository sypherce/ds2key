#ifndef __LISTVIEW_H__
#define __LISTVIEW_H__

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "label.h"

namespace D2K {
	namespace GUI {
		class ListView : public Label {
			public:
				ListView(std::string text, int x, int y, int width, int height);
				~ListView();
				unsigned GetColumnCount();
				void SetHeaderVisible(bool visible);
				void InsertColumn(std::string Text, int i);
				std::string GetText(int row, int column);
				void SetText(std::string Text, int row, int column);
				void SetDoubleClick(void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam));
				void AutoSizeColumns();
				unsigned GetSelection();
				void SetSelection(unsigned row);
				void Append(std::string text);
				void Append(std::string text, std::string text2);
				bool Attach(Object *parentObject);
		};
	}
}
#endif//__LISTVIEW_H__
