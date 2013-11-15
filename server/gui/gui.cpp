/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>//printf
#include "gui/gui.h"
#include "gui/resource.h"
#include "core/core.h"//Print

namespace D2K {
	namespace GUI {
		void voidFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
			D2K::Core::Print(D2K::Core::dAll, "voidFunction()");
		}
		HINSTANCE hThisInstance;
		int eventCounter;
		bool locked = true;
		Event events[D2K::GUI::eventMax] = {{NULL}};
		MSG messages;            /* Here messages to the application are saved */
		void setFont(HWND hwnd) {
			NONCLIENTMETRICS ncm;
			ncm.cbSize = sizeof(NONCLIENTMETRICS);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
			HFONT hFont = ::CreateFontIndirect(&ncm.lfMessageFont);
			SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
		}
		Object::Object(int x, int y, int width, int height) {
			X = x;
			Y = y;
			Width = width;
			Height = height;
			ID = -1;
		}
		Object::~Object() {}
		bool Object::Attach(Object *parentObject) {
			if(D2K::GUI::eventCounter >= eventMax)
				return false;
			ID = eventCounter++;
			parent = parentObject;
			return true;
		}
		int Object::GetID() {
			return ID;
		}
		HWND Object::GetParentHWND() {
			//printf("this:%x parent:%x\n", (unsigned int)this, (unsigned int)parent->hwnd);
			return parent->hwnd;
		}
		Object *Object::GetParent() {
			return parent;
		}
		void Object::SetVisible(bool visible) {
			if(hwnd)
				ShowWindow(hwnd, visible);
		}

		Label::Label(string text, int x, int y, int width, int height) : Object(x, y, width, height) {
			Text = text;
		}
		Label::~Label() {}
		bool Label::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				hwnd = CreateWindow (
						   "STATIC", Text.c_str(),
						   WS_CHILD | WS_VISIBLE | BS_TEXT,
						   X, Y, Width, Height,
						   GetParentHWND(),
						   (HMENU)ID,
						   GetModuleHandle(NULL),
						   0);
				setFont(hwnd);
				return true;
			}
			return false;
		}
		string Label::GetText() {
			int length = GetWindowTextLength(hwnd);
			char *windowText = new char[length+1];
			GetWindowText(hwnd, windowText, length+1);
			windowText[length] = 0;
			Text = windowText;
			delete windowText;
			return Text;
		}
		void Label::SetText(string text) {
			Text = text;
			locked = true;
			SetWindowText(hwnd, Text.c_str());
			locked = false;
		}
		void Label::SetEnabled(bool enabled) {
			EnableWindow(hwnd, enabled);
		}
		long Label::GetStyle() {
			return GetWindowLong(hwnd, GWL_STYLE);
		}
		long Label::SetStyle(long style) {
			return SetWindowLong(hwnd, GWL_STYLE, style);
		}
		Button::Button(string text, int x, int y, int width, int height) : Label(text, x, y, width, height) {}
		Button::~Button() {}
		bool Button::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				hwnd = CreateWindow (
						   "BUTTON", Text.c_str(),
						   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
						   X, Y, Width, Height,
						   GetParentHWND(),
						   (HMENU)ID,
						   GetModuleHandle(NULL),
						   0);
				setFont(hwnd);
				return true;
			}
			return false;
		}
		StatusBar::StatusBar(string text, int x, int y, int width, int height) : Label(text, x, y, width, height) {}
		StatusBar::~StatusBar() {}
		bool StatusBar::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				hwnd = CreateWindow (
						   STATUSCLASSNAME, "",
						   WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
						   X, Y, Width, Height,
						   GetParentHWND(),
						   (HMENU)ID,
						   GetModuleHandle(NULL),
						   0);
				setFont(hwnd);
				SetText(Text);
				return true;
			}
			return false;
		}
		void StatusBar::SetText(string Text) {
			SendMessage(hwnd, SB_SETTEXT, 0, (LPARAM)Text.c_str());

			return;
		}
		TrayIcon::TrayIcon(string text) : Label(text, 0, 0, 0, 0) {
			ZeroMemory(&niData, sizeof(NOTIFYICONDATA));
			niData.cbSize = NOTIFYICONDATA_V1_SIZE;
			niData.uID = ID;//tray icon identifier
			niData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
			SetText(text.c_str());
			niData.hIcon = LoadIcon(GUI::hThisInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
			niData.uCallbackMessage = WM_TRAYMSG;
		}
		TrayIcon::~TrayIcon() {
			Delete();
		}
		bool TrayIcon::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				niData.hWnd = GetParentHWND();
				Shell_NotifyIcon(NIM_ADD, &niData);
				return true;
			}
			return false;
		}
		void TrayIcon::SetText(string Text) {
			this->Text = Text;
			ZeroMemory(&niData.szTip, 64);
			strncpy(niData.szTip, this->Text.c_str(), 63);

			return;
		}
		void TrayIcon::Delete() {
			Shell_NotifyIcon(NIM_DELETE, &niData);
		}
		Menu::Menu() : Label("", 0, 0, 0, 0) {
				hwnd = (HWND)CreateMenu();}
		Menu::~Menu() {}
		void Menu::Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)) {
			if(this && object->Attach(this)) {
				events[object->GetID()].object = object;
				events[object->GetID()].function = function;
				events[object->GetID()].function2 = &GUI::voidFunction;
			}
		}
		void Menu::Append(Object *object) {
			Menu::Append(object, &voidFunction);
		}
		bool Menu::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				printf("Parent:%x\n", (unsigned int)GetParentHWND());
				printf("menu:%x\n", (unsigned int)hwnd);
				return Update();
			}
			return false;
		}
		bool Menu::Update() {
				printf("Parent:%x\n", (unsigned int)GetParentHWND());
				printf("menu:%x\n", (unsigned int)hwnd);
			return SetMenu(GetParentHWND(), (HMENU)hwnd);
		}
		MenuPopUp::MenuPopUp(string text) : Label(text, 0, 0, 0, 0) {
				hwnd = (HWND)CreatePopupMenu();}
		MenuPopUp::~MenuPopUp() {}
		void MenuPopUp::Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)) {
			if(this && object->Attach(this)) {
				events[object->GetID()].object = object;
				events[object->GetID()].function = function;
				events[object->GetID()].function2 = &GUI::voidFunction;
			}
		}
		void MenuPopUp::Append(Object *object) {
			MenuPopUp::Append(object, &voidFunction);
		}
		bool MenuPopUp::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				printf("Parent:%x\n", (unsigned int)GetParentHWND());
				printf("menupopup:%x\n", (unsigned int)hwnd);
				printf("text:%s\n", Text.c_str());
				return AppendMenu((HMENU)GetParentHWND(), MF_STRING | MF_POPUP, (UINT)hwnd, Text.c_str());
			}
			return false;
		}
		MenuItem::MenuItem(string text) : Label(text, 0, 0, 0, 0) {
				hwnd = (HWND)NULL;}
		MenuItem::~MenuItem() {}
		void MenuItem::Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)) {
			if(this && object->Attach(this)) {
				events[object->GetID()].object = object;
				events[object->GetID()].function = function;
				events[object->GetID()].function2 = &GUI::voidFunction;
			}
		}
		void MenuItem::Append(Object *object) {
			MenuItem::Append(object, &voidFunction);
		}
		bool MenuItem::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				printf("Parent:%x\n", (unsigned int)GetParentHWND());
				printf("menuitem:%x\n", (unsigned int)hwnd);
				printf("text:%s\n", Text.c_str());
				if(Text == "-")//if it's a seperator
					return AppendMenu((HMENU)GetParentHWND(), MF_SEPARATOR, ID, Text.c_str());

				return AppendMenu((HMENU)GetParentHWND(), MF_STRING, ID, Text.c_str());
			}
			return false;
		}
		CheckButton::CheckButton(string text, int x, int y, int width, int height) : Label(text, x, y, width, height) {}
		CheckButton::~CheckButton() {}
		bool CheckButton::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				hwnd = CreateWindow (
						   "BUTTON", Text.c_str(),
						   WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
						   X, Y, Width, Height,
						   GetParentHWND(),
						   (HMENU)ID,
						   GetModuleHandle(NULL),
						   0);
				setFont(hwnd);
				return true;
			}
			return false;
		}
		void CheckButton::SetChecked(bool checked) {
			SendMessage(hwnd, BM_SETCHECK, (WPARAM)checked, 0);
		}
		RadioButton::RadioButton(string text, int x, int y, int width, int height) : CheckButton(text, x, y, width, height) {}
		RadioButton::~RadioButton() {}
		bool RadioButton::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				hwnd = CreateWindow (
						   "BUTTON", Text.c_str(),
						   WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
						   X, Y, Width, Height,
						   GetParentHWND(),
						   (HMENU)ID,
						   GetModuleHandle(NULL),
						   0);
				setFont(hwnd);
				return true;
			}
			return false;
		}
		Edit::Edit(string text, int x, int y, int width, int height) : Label(text, x, y, width, height) {}
		Edit::~Edit() {}
		bool Edit::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				hwnd = CreateWindowEx(
						   WS_EX_CLIENTEDGE,
						   "EDIT", Text.c_str(),
						   WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
						   X, Y, Width, Height,
						   GetParentHWND(),
						   (HMENU)ID,
						   GetModuleHandle(NULL),
						   0);
				setFont(hwnd);
				return true;
			}
			return false;
		}
		ComboButton::ComboButton(int x, int y, int width, int height) : Label("", x, y, width, height) {}
		ComboButton::~ComboButton() {}
		bool ComboButton::Attach(Object *parentObject) {
			if(Object::Attach(parentObject)) {
				hwnd = CreateWindowEx(
						   WS_EX_CLIENTEDGE,
						   "COMBOBOX", "",
						   WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
						   X, Y, Width, Height,
						   GetParentHWND(),
						   (HMENU)ID,
						   GetModuleHandle(NULL),
						   0);
				setFont(hwnd);
				return true;
			}
			return false;
		}
		void ComboButton::Append(string text) {
			SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)text.c_str());
			if(SendMessage(hwnd, CB_GETCOUNT, 0, 0) == 1) SetSelection(0);
		}
		void ComboButton::SetSelection(unsigned row) {
			SendMessage(hwnd, CB_SETCURSEL, row, 0);
		}
		int ComboButton::GetSelection() {
			int message = SendMessage(hwnd, CB_GETCURSEL, 0, 0);
			if(message == CB_ERR)//why is CB_ERR different?
				return 0;
			return message;
		}
		ListView::ListView(string text, int x, int y, int width, int height) : Label(text, x, y, width, height) {}
		ListView::~ListView() {}
		void ListView::SetHeaderVisible(bool visible) {
			SetWindowLong(hwnd, GWL_STYLE,
						  (GetWindowLong(hwnd, GWL_STYLE) & ~LVS_NOCOLUMNHEADER) |
						  (visible ? 0 : LVS_NOCOLUMNHEADER));
		}
		void ListView::InsertColumn(string Text, int i) {
			LVCOLUMN column;
			column.mask = LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
			column.fmt = LVCFMT_LEFT;
			column.iSubItem = i;
			column.pszText = (LPSTR)Text.c_str();
			ListView_InsertColumn(hwnd, i, &column);
			AutoSizeColumns();
		}
		string ListView::GetText(int row, int column) {
			char *buffer = new char[256]; // Create temp buffer
			ListView_GetItemText(hwnd, row, column, buffer, 256);
			string text = buffer;
			delete buffer;

			return text;
		}
		void ListView::SetText(string Text, int row, int column) {
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
				setFont(hwnd);
				InsertColumn(Text, 0);
				AutoSizeColumns();
				return true;
			}
			return false;
		}
		void ListView::Append(string text) {
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
		void ListView::Append(string text, string text2) {
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

		void Setup() {
			D2K::GUI::eventCounter = 1;//0 = null so no event can have the number 0
		}

		WPARAM GetMessages() {

			/* Run the message loop. It will run until GetMessage() returns 0 */
			WPARAM message = PeekMessage(&messages, NULL, 0, 0, PM_REMOVE);
			if(message == TRUE) {
				/* Translate virtual-key messages into character messages */
				TranslateMessage(&messages);
				/* Send message to WindowProcedure */
				DispatchMessage(&messages);
			}

			/* The program return-value is 0 - The value that PostQuitMessage() gave */
			return 1;
		}

		LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
			switch(message) {                /* handle the messages */
				case WM_TRAYMSG: {
					switch(lParam) {
						case WM_LBUTTONDOWN: {
							if(IsWindowVisible(hwnd))
								ShowWindow(hwnd, SW_HIDE);
							else
								ShowWindow(hwnd, SW_SHOW);
							break;
						}
						case WM_RBUTTONDOWN:
						case WM_CONTEXTMENU: {
							HMENU hMenu = GetMenu(hwnd);

							if(hMenu) {
								HMENU hmenuPopup = GetSubMenu(hMenu, 0);
								if(hmenuPopup) {
									POINT pt;
									GetCursorPos(&pt);
									SetForegroundWindow(hwnd);
									TrackPopupMenu(hmenuPopup, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hwnd, NULL);
								}
							}
							break;
						}
					}

					break;
				}
				case WM_NOTIFY: {
					int event = LOWORD(wParam);
					if(events[event].object == NULL)
						break;
					switch(((LPNMHDR) lParam)->code) {
						case LVN_ITEMACTIVATE: {
							events[event].function2(hwnd, message, wParam, lParam);	//perform the event's function2
							break;
						}
						case LVN_ITEMCHANGED: {
							if(((LPNMLISTVIEW)lParam)->uChanged & LVIF_STATE)
								if(((LPNMLISTVIEW)lParam)->uNewState & LVIS_SELECTED)
									events[event].function(hwnd, message, wParam, lParam);	//perform the event's function
							break;
						}
					}
					break;
				}
				case WM_COMMAND: {
					int event = LOWORD(wParam);
					if(events[event].object == NULL)
						break;
					if(event >= 0 && event <= eventMax) {//if it's an event
						if(HIWORD(wParam) == BN_CLICKED) {//and if a button is clicked
							events[event].function(hwnd, message, wParam, lParam);	//perform the event's function
						}
						else if(HIWORD(wParam) == EN_CHANGE) {//and if a edit box is changed
							if(!locked)						//and if not locked
								events[event].function(hwnd, message, wParam, lParam);	//perform the event's function
						}
						else if(HIWORD(wParam) == LBN_SELCHANGE) {//and if a listbox is clicked
							if(!locked)							//and if not locked
								events[event].function(hwnd, message, wParam, lParam);		//perform the event's function
						}
					}
					else if(event > eventMax)
						printf("Error: Increase eventMax Value to %i or higher!", event);
					break;
				}
				case WM_DESTROY: {
					PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
					break;
				}
				case WM_CLOSE: {
					ShowWindow(hwnd, false);
					break;
				}
				default:                      /* for messages that we don't deal with */
					return DefWindowProc (hwnd, message, wParam, lParam);
			}

			return 0;
		}

		Window::Window() : Object(0, 0, 0, 0) {
			hwnd = NULL;
		}
		Window::~Window() {
			if(hwnd) {
				DestroyWindow(hwnd);
				hwnd = NULL;
			}
		}
		void Window::Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)) {
			if(this && object->Attach(this)) {
				events[object->GetID()].object = object;
				events[object->GetID()].function = function;
				events[object->GetID()].function2 = function;
			}
		}
		void Window::Append(Object *object) {
			Window::Append(object, &voidFunction);
		}
		HWND hwnd;               /* This is the handle for our window */
		bool Window::Setup(HINSTANCE hThisInstance, string ClassName, string Title, int Width, int Height) {
			if(!hwnd) {
				GUI::hThisInstance = hThisInstance;
				WNDCLASSEX wincl;        /* Data structure for the windowclass */
				/* The Window structure */
				wincl.hInstance = GUI::hThisInstance;
				wincl.lpszClassName = ClassName.c_str();
				wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
				wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
				wincl.cbSize = sizeof (WNDCLASSEX);

				/* Use default icon and mouse-pointer */
				wincl.hIcon = LoadIcon(GUI::hThisInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
				wincl.hIconSm = wincl.hIcon;
				wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
				wincl.lpszMenuName = NULL;                 /* No menu */
				wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
				wincl.cbWndExtra = 0;                      /* structure or the window instance */
				/* Use Windows's default colour as the background of the window */
				wincl.hbrBackground = (HBRUSH) COLOR_WINDOW ;

				/* Register the window class, and if it fails quit the program */
				if (!RegisterClassEx (&wincl))
					return 1;
				parent = this;
				/* The class is registered, let's create the program*/
				hwnd = CreateWindowEx(
						   0,                   /* Extended possibilites for variation */
						   ClassName.c_str(),         /* Classname */
						   Title.c_str(),       /* Title Text */
						   WS_OVERLAPPEDWINDOW, /* default window */
						   CW_USEDEFAULT,       /* Windows decides the position */
						   CW_USEDEFAULT,       /* where the window ends up on the screen */
						   Width,                 /* The programs width */
						   Height,                 /* and height in pixels */
						   HWND_DESKTOP,        /* The window is a child-window to desktop */
						   NULL,                /* No menu */
						   hThisInstance,       /* Program Instance handler */
						   NULL                 /* No Window Creation data */
					   );
					   printf("window hwnd:%x\n", (unsigned int)hwnd);
				setFont(hwnd);
				//CreateMainMenu();
				return true;
			}
			return false;
		}

		void Window::CreateMainMenu() {
			enum { MenuFileConnect, MenuFileMinimize, MenuFileExit };
			HMENU mainMenu = CreateMenu();
			HMENU fileMenu = CreatePopupMenu();
			AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)fileMenu, "&File");
			AppendMenu(fileMenu, MF_STRING, MenuFileConnect, "&Connect");
			AppendMenu(fileMenu, MF_STRING, MenuFileMinimize, "&Minimize");
			AppendMenu(fileMenu, MF_STRING, MenuFileExit, "&Exit");
			SetMenu(hwnd, mainMenu);
		}

		void Window::SetVisible(int visible) {
			if(hwnd)
				ShowWindow(hwnd, visible);
		}

		void Window::SetVisible(bool visible) {
			if(hwnd)
				ShowWindow(hwnd, visible);
		}
		void Window::SetText(string text) {
			Text = text;
			locked = true;
			SetWindowText(hwnd, Text.c_str());
			locked = false;
		}
	}
}
