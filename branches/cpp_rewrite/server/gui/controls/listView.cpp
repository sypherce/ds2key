/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>//printf
#include "gui/gui.h"
#include "gui/resource.h"
#include "core/core.h"//Print
#include "listView.h"

namespace D2K {
	namespace GUI {
		ListView::ListView(std::string text, int x, int y, int width, int height) : Label(text, x, y, width, height) {}
		ListView::~ListView() {}
		void ListView::SetHeaderVisible(bool visible) {
			SetWindowLong(hwnd, GWL_STYLE,
						  (GetWindowLong(hwnd, GWL_STYLE) & ~LVS_NOCOLUMNHEADER) |
						  (visible ? 0 : LVS_NOCOLUMNHEADER));
		}
		void ListView::InsertColumn(std::string Text, int i) {
			LVCOLUMN column;
			column.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
			column.fmt = LVCFMT_LEFT;
			column.iSubItem = i;
			column.pszText = (LPSTR)Text.c_str();
			ListView_InsertColumn(hwnd, i, &column);
			AutoSizeColumns();
		}
		std::string ListView::GetText(int row, int column) {
			char *buffer = new char[256]; // Create temp buffer
			ListView_GetItemText(hwnd, row, column, buffer, 256);
			std::string text = buffer;
			delete buffer;

			return text;
		}
		void ListView::SetText(std::string Text, int row, int column) {
			ListView_SetItemText(hwnd, row, column, (LPSTR)Text.c_str());//set second item text

			return;
		}
		void ListView::SetDoubleClick(void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)) {
			events[GetID()].function2 = function;
		}

		void ListView::AutoSizeColumns() {
			unsigned columns = GetColumnCount();
			for(unsigned n = 0; n < columns; n++) {
				ListView_SetColumnWidth(hwnd, n, LVSCW_AUTOSIZE_USEHEADER);
			}
		}
		unsigned ListView::GetColumnCount() {
			unsigned count = 0;
			LVCOLUMN column;
			column.mask = LVCF_WIDTH;
			while(ListView_GetColumn(hwnd, count++, &column));
			return --count;
		}
		unsigned ListView::GetSelection() {
			return SendMessage(hwnd, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
		}
		void ListView::SetSelection(unsigned row) {
			ListView_SetItemState(hwnd, row, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		}

		bool ListView::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				hwnd = CreateWindowEx(
						   WS_EX_CLIENTEDGE,
						   WC_LISTVIEW, Text.c_str(),
						   WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER,
						   X, Y, Width, Height,
						   GetParentHWND(),
						   (HMENU)ID,
						   GetModuleHandle(NULL),
						   0);
				SetFont(hwnd);
				InsertColumn(Text, 0);
				AutoSizeColumns();
				return true;
			}
			return false;
		}
		void ListView::Append(std::string text) {
			unsigned row = ListView_GetItemCount(hwnd);
			LVITEM item;
			item.mask = LVIF_TEXT;
			item.iItem = row;
			item.iSubItem = 0;
			item.pszText = (LPSTR)text.c_str();

			locked = true;
			ListView_InsertItem(hwnd, &item);//set item text
			locked = false;
		}
		void ListView::Append(std::string text, std::string text2) {
			unsigned row = ListView_GetItemCount(hwnd);
			LVITEM item;
			item.mask = LVIF_TEXT;
			item.iItem = row;
			item.iSubItem = 0;
			item.pszText = (LPSTR)text.c_str();

			locked = true;
			ListView_InsertItem(hwnd, &item);//set first item text
			ListView_SetItemText(hwnd, row, 1, (LPSTR)text2.c_str());//set second item text
			locked = false;
		}
	}
}
