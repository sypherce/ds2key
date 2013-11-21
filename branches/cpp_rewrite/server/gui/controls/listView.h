#ifndef __LISTVIEW_H__
#define __LISTVIEW_H__

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "label.h"
using std::string;

namespace D2K {
	namespace GUI {
		class ListView : public Label {
			public:
				ListView(string text, int x, int y, int width, int height);
				~ListView();
				unsigned GetColumnCount();
				void SetHeaderVisible(bool visible);
				void InsertColumn(string Text, int i);
				string GetText(int row, int column);
				void SetText(string Text, int row, int column);
				void SetDoubleClick(void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam));
				void AutoSizeColumns();
				unsigned GetSelection();
				void SetSelection(unsigned row);
				void Append(string text);
				void Append(string text, string text2);
				bool Attach(Object *parentObject);
		};
	}
}
#endif//__LISTVIEW_H__
