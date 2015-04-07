/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include <iostream>//std::cout, std::clog
#include "gui/gui.h"
#include "gui/resource.h"

namespace D2K {
	namespace GUI {
		void voidFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
			std::clog << "voidFunction()\n";
		}
		HINSTANCE hThisInstance;
		int eventCounter;
		bool locked = true; /*Event locker. By setting this to true, it stops the
							 *gui from updating when we don't need it to,
							 *later we set it to false again.*/
		Event events[D2K::GUI::eventMax] = {{NULL}};
		MSG messages;            //Here messages to the application are saved
		void SetFont(HWND hwnd) {
			NONCLIENTMETRICS ncm;
			ncm.cbSize = sizeof(NONCLIENTMETRICS);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
			HFONT hFont = ::CreateFontIndirect(&ncm.lfMessageFont);
			SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
		}

		void Setup() {
			D2K::GUI::eventCounter = 1;//0 = null so no event can have the number 0
		}

		WPARAM GetMessages() {

			//Run the message loop. It will run until GetMessage() returns 0
			WPARAM message = PeekMessage(&messages, NULL, 0, 0, PM_REMOVE);
			if(message == TRUE) {
				//Translate virtual-key messages into character messages
				TranslateMessage(&messages);
				//Send message to WindowProcedure
				DispatchMessage(&messages);
			}

			//The program return-value is 0 - The value that PostQuitMessage() gave
			return 1;
		}

		LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
			switch(message) {
				case WM_TRAYMSG: {                              //tray icon messages
					switch(lParam) {
						case WM_LBUTTONDOWN: {                  //Left Button
							if(IsWindowVisible(hwnd))
								ShowWindow(hwnd, SW_HIDE);
							else
								ShowWindow(hwnd, SW_SHOW);
							break;
						}
						case WM_RBUTTONDOWN:                    //Right Button
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
						case LVN_ITEMACTIVATE: {                                            //listview item activated
							events[event].function2(hwnd, message, wParam, lParam);			//perform the event's function2
							break;
						}
						case LVN_ITEMCHANGED: {                                             //listview item selection changed
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
					if(event >= 0 && event <= eventMax) {								//if it's an event
						if(HIWORD(wParam) == BN_CLICKED) {								//and if a button is clicked
							events[event].function(hwnd, message, wParam, lParam);		//perform the event's function
						}
						else if(HIWORD(wParam) == EN_CHANGE) {							//and if a edit box is changed
							if(!locked)													//and if not locked
								events[event].function(hwnd, message, wParam, lParam);	//perform the event's function
						}
						else if(HIWORD(wParam) == LBN_SELCHANGE) {						//and if a listbox is clicked
							if(!locked)													//and if not locked
								events[event].function(hwnd, message, wParam, lParam);	//perform the event's function
						}
					}
					else if(event > eventMax)
						std::clog << "Error (event) Increase eventMax value to " << event << " or higher\n";
					break;
				}
				case WM_DESTROY: {
					PostQuitMessage (0);       //send a WM_QUIT to the message queue
					break;
				}
				case WM_CLOSE: {
					ShowWindow(hwnd, false);
					break;
				}
				default:                      //for messages that we don't deal with
					return DefWindowProc (hwnd, message, wParam, lParam);
			}

			return 0;
		}
	}
}
