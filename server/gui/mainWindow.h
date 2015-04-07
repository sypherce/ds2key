#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <windef.h>
#include <string>//std::string
#include "gui/gui.h"

namespace D2K {
	namespace GUI {
		namespace MainWindow {
			const int width = 580;
			const int height = 360;
			const int x = 100;
			const int border = 10;
			const int listMax = 8;
			extern GUI::Window window;
			extern GUI::ListView *categoryListView;
			extern GUI::TrayIcon *TrayIcon;
			namespace MainMenu {
				extern GUI::Menu *menu;
				extern GUI::MenuPopUp *file;
				extern GUI::MenuItem *fileConnect;
				extern GUI::MenuItem *fileMinimize;
				extern GUI::MenuItem *fileExit;

				void fileConnectFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
				void fileMinimizeFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
				void fileExitFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

				void Setup();
			}

			namespace Profile {
				extern Label *List[listMax];

				extern RadioButton *radioButton1;
				extern RadioButton *radioButton2;
				extern RadioButton *radioButton3;
				extern CheckButton *checkButton1;
				extern ComboButton *comboButton1;
				extern ListView *listView1;
				extern Button *button1;
				extern Button *button2;

				void radioButton1Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

				void radioButton2Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

				void radioButton3Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

				void comboButton1Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

				void Setup();
				void SetVisible(bool visible);
			}
			namespace Settings {
				extern Label *List[listMax];

				extern Label *label;
				extern Edit *edit;

				extern std::string Port;

				void editFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

				void Setup();
				void SetVisible(bool visible);
			}
			namespace About {
				extern Label *List[listMax];

				extern Label *label1;
				extern Label *label2;
				extern Label *label3;

				void Setup();
				void SetVisible(bool visible);
			}

			void categoryListViewFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

			int Setup(HINSTANCE hThisInstance, int nCmdShow);
		}
	}
}
#endif//__MAINWINDOW_H__
