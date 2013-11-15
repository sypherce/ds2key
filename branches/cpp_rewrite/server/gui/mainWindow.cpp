/*
	Main window for DS2Key
*/

#include <windows.h>
#include <commctrl.h>//LPNMLISTVIEW
#include <sstream>//std::stringstream
#include <stdio.h>//printf
#include "core/core.h"
#include "core/key.h"
#include "mainWindow.h"

using std::string;
template <class T>
inline std::string itos (const T& t) {
	std::stringstream ss;
	ss << t;

	return ss.str();
}

template <typename T>
long stoi(const std::basic_string<T> &str) {
	std::basic_stringstream<T> stream(str);
	long res;
	return !(stream >>res)?0:res;
}

namespace D2K {
	namespace GUI {
		namespace MainWindow {
			int width = 580;
			int height = 360;
			int x = 100;
			int border = 10;
			GUI::Window window;
			GUI::ListView *categoryListView;
			GUI::StatusBar *StatusBar;
			GUI::TrayIcon *TrayIcon;
			namespace MainMenu {
				GUI::Menu *menu = new GUI::Menu();
				GUI::MenuPopUp *file;
				GUI::MenuItem *fileConnect;
				GUI::MenuItem *fileDisconnect;
				GUI::MenuItem *fileMinimize;
				GUI::MenuItem *fileExit;

				void fileConnectFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("fileConnectFunction()\n");
					if(!Core::UDP->IsConnected()) {
						Core::UDP->Connect();
					}
					CheckMenuRadioItem(	(HMENU)GUI::MainWindow::MainMenu::file->GetParentHWND(),
										GUI::MainWindow::MainMenu::fileConnect->GetID(),
										GUI::MainWindow::MainMenu::fileDisconnect->GetID(),
										Core::UDP->IsConnected() ? GUI::MainWindow::MainMenu::fileConnect->GetID() : GUI::MainWindow::MainMenu::fileDisconnect->GetID(),
										MF_BYCOMMAND);
					//check if connected properly
					string status = "Connected on Port #" + itos(Core::Config->GetPort());
					GUI::MainWindow::StatusBar->SetText(Core::UDP->IsConnected() ? status : "Disconnected");
				}
				void fileDisconnectFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("fileConnectFunction()\n");
					if(Core::UDP->IsConnected()) {
						Core::UDP->Disconnect();
					}
					CheckMenuRadioItem(	(HMENU)GUI::MainWindow::MainMenu::file->GetParentHWND(),
										GUI::MainWindow::MainMenu::fileConnect->GetID(),
										GUI::MainWindow::MainMenu::fileDisconnect->GetID(),
										Core::UDP->IsConnected() ? GUI::MainWindow::MainMenu::fileConnect->GetID() : GUI::MainWindow::MainMenu::fileDisconnect->GetID(),
										MF_BYCOMMAND);
					string status = "Connected on Port #" + itos(Core::Config->GetPort());
					GUI::MainWindow::StatusBar->SetText(Core::UDP->IsConnected() ? status : "Disconnected");
				}
				void fileMinimizeFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("fileMinimizeFunction()\n");
					menu->GetParent()->SetVisible(false);
				}
				void fileExitFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("fileExitFunction()\n");
					//delete GUI::MainWindow::TrayIcon;
					//ExitProcess(0);
					Core::Running = false;
				}

				void Setup() {
					window.Append(menu);
						menu->Append(
							file = new GUI::MenuPopUp("&File"));
							file->Append(
								fileConnect = new GUI::MenuItem("&Connect"), &fileConnectFunction);
							file->Append(
								fileDisconnect = new GUI::MenuItem("&Disconnect"), &fileDisconnectFunction);
							file->Append(
								fileMinimize = new GUI::MenuItem("&Minimize"), &fileMinimizeFunction);
							file->Append(
								fileExit = new GUI::MenuItem("-"));
							file->Append(
								fileExit = new GUI::MenuItem("E&xit"), &fileExitFunction);
						CheckMenuRadioItem(	(HMENU)GUI::MainWindow::MainMenu::file->GetParentHWND(),
											GUI::MainWindow::MainMenu::fileConnect->GetID(),
											GUI::MainWindow::MainMenu::fileDisconnect->GetID(),
											Core::UDP->IsConnected() ? GUI::MainWindow::MainMenu::fileConnect->GetID() : GUI::MainWindow::MainMenu::fileDisconnect->GetID(),
											MF_BYCOMMAND);
						menu->Update();
				}
			}

			namespace Profile {
				Label *List[listMax] = {NULL};

				Edit *edit1;
				RadioButton *radioButton1;
				RadioButton *radioButton2;
				RadioButton *radioButton3;
				CheckButton *checkButton1;
				ComboButton *comboButton1;
				ListView *listView1;
				Button *button1;
				Button *button2;
				Button *button3;
				Button *button4;
				Button *button5;
				Button *button6;
				Button *button7;

				bool waiting;

				void radioButton1Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("profileRadioButton1Function()\n");
					checkButton1->SetEnabled(true);
					Core::Client[GUI::MainWindow::Profile::comboButton1->GetSelection()]->SetButton(Core::kMouse, Core::mRelative);
				}

				void radioButton2Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("profileRadioButton2Function()\n");
					checkButton1->SetEnabled(true);
					Core::Client[GUI::MainWindow::Profile::comboButton1->GetSelection()]->SetButton(Core::kMouse, Core::mAbsolute);
				}

				void radioButton3Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("profileRadioButton3Function()\n");
					Core::Client[GUI::MainWindow::Profile::comboButton1->GetSelection()]->SetButton(Core::kMouse, Core::mButtons);
					checkButton1->SetEnabled(false);
				}

				void checkButton1Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("checkButton1Function()\n");
				}
				void comboButton1Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					int Profile = comboButton1->GetSelection();
					if(Core::Client[Profile] == (Core::C::Client*)NULL)							//if profile not active,
						Core::Client[Profile] = new Core::C::Client();							//create it
					Core::Config->LoadProfile(Core::Client[Profile]->GetSettings(), Profile);		//then load it
					int row = 0;
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kUp), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kDown), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kLeft), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kRight), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kA), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kB), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kX), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kY), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kL), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kR), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kStart), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kSelect), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kLid), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kBlue), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kYellow), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kRed), row++, 1);
					listView1->SetText(Core::Client[Profile]->GetButtonString(Core::kGreen), row++, 1);

						radioButton1->SetChecked(false);
						radioButton2->SetChecked(false);
						radioButton3->SetChecked(false);
						checkButton1->SetEnabled(true);

					if(Core::Client[Profile]->GetButton(Core::kMouse) == Core::mAbsolute)
						radioButton2->SetChecked(true);
					else if(Core::Client[Profile]->GetButton(Core::kMouse) == Core::mButtons) {
						radioButton3->SetChecked(true);
						checkButton1->SetEnabled(false);
					}
					else
						radioButton1->SetChecked(true);
				}

				void listView1Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					int selected = listView1->GetSelection();
					string oldStatus = GUI::MainWindow::StatusBar->GetText();
					GUI::MainWindow::StatusBar->SetText("Assign a Key [" + listView1->GetText(selected, 0) + "]");
					//disable
					GUI::MainWindow::categoryListView->SetEnabled(false);
					GUI::MainWindow::Profile::radioButton1->SetEnabled(false);
					GUI::MainWindow::Profile::radioButton2->SetEnabled(false);
					GUI::MainWindow::Profile::radioButton3->SetEnabled(false);
					GUI::MainWindow::Profile::checkButton1->SetEnabled(false);
					GUI::MainWindow::Profile::comboButton1->SetEnabled(false);
					GUI::MainWindow::Profile::listView1->SetEnabled(false);
					GUI::MainWindow::Profile::button1->SetEnabled(false);
					GUI::MainWindow::Profile::button2->SetEnabled(false);
					GUI::MainWindow::Profile::edit1->SetEnabled(false);
					//show
					GUI::MainWindow::Profile::edit1->SetVisible(true);
					GUI::MainWindow::Profile::button7->SetVisible(true);
					GUI::MainWindow::Profile::button3->SetVisible(true);
					GUI::MainWindow::Profile::button4->SetVisible(true);
					GUI::MainWindow::Profile::button5->SetVisible(true);
					GUI::MainWindow::Profile::button6->SetVisible(true);

					BYTE keys[256];
					int i = 0;
					waiting = true;
					while(waiting) {
						GetKeyboardState((PBYTE)keys);
						for(i = 0; i < 255; i++) {
							if(!waiting)
								break;
							if(keys[i] > 1) {
								if(i != VK_LBUTTON && i != VK_RBUTTON && i != VK_MBUTTON) {
									listView1->SetText(Core::Key::GetString(i), selected, 1);
									Core::Client[GUI::MainWindow::Profile::comboButton1->GetSelection()]->SetButton(listView1->GetSelection() + 3, i);

									waiting = false;
								}
							}
						}
						GUI::GetMessages();//Application::processEvents();
					}
					GUI::MainWindow::StatusBar->SetText(oldStatus);
					//enable
					GUI::MainWindow::categoryListView->SetEnabled(true);
					GUI::MainWindow::Profile::radioButton1->SetEnabled(true);
					GUI::MainWindow::Profile::radioButton2->SetEnabled(true);
					GUI::MainWindow::Profile::radioButton3->SetEnabled(true);
					GUI::MainWindow::Profile::checkButton1->SetEnabled(true);
					GUI::MainWindow::Profile::comboButton1->SetEnabled(true);
					GUI::MainWindow::Profile::listView1->SetEnabled(true);
					GUI::MainWindow::Profile::button1->SetEnabled(true);
					GUI::MainWindow::Profile::button2->SetEnabled(true);
					//hide
					GUI::MainWindow::Profile::edit1->SetVisible(false);
					GUI::MainWindow::Profile::button7->SetVisible(false);
					GUI::MainWindow::Profile::button3->SetVisible(false);
					GUI::MainWindow::Profile::button4->SetVisible(false);
					GUI::MainWindow::Profile::button5->SetVisible(false);
					GUI::MainWindow::Profile::button6->SetVisible(false);
				}

				void button1Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					Core::Config->SaveProfile(
							Core::Client[GUI::MainWindow::Profile::comboButton1->GetSelection()]->GetSettings(),
							GUI::MainWindow::Profile::comboButton1->GetSelection());
					comboButton1Function(hwnd, message, wParam, lParam);
				}

				void button2Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					comboButton1Function(hwnd, message, wParam, lParam);
				}

				void button3Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					waiting = false;
				}

				void button4Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					listView1->SetText(Core::Key::GetString(VK_LBUTTON), listView1->GetSelection(), 1);
					Core::Client[GUI::MainWindow::Profile::comboButton1->GetSelection()]->SetButton(listView1->GetSelection() + 3, VK_LBUTTON);
					waiting = false;
				}

				void button5Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					listView1->SetText(Core::Key::GetString(VK_MBUTTON), listView1->GetSelection(), 1);
					Core::Client[GUI::MainWindow::Profile::comboButton1->GetSelection()]->SetButton(listView1->GetSelection() + 3, VK_MBUTTON);
					waiting = false;
				}

				void button6Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					listView1->SetText(Core::Key::GetString(VK_RBUTTON), listView1->GetSelection(), 1);
					Core::Client[GUI::MainWindow::Profile::comboButton1->GetSelection()]->SetButton(listView1->GetSelection() + 3, VK_RBUTTON);
					waiting = false;
				}

				void button7Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					uint32_t Value = 0x100 + atoi(edit1->GetText().c_str());
					edit1->SetEnabled(true);
					listView1->SetText(Core::Key::GetString(Value), listView1->GetSelection(), 1);
					Core::Client[GUI::MainWindow::Profile::comboButton1->GetSelection()]->SetButton(listView1->GetSelection() + 3, Value);
					waiting = false;
				}

				void Setup() {
					waiting = false;
					window.Append(
						radioButton1 = new RadioButton("Touchpad (Relative)", x, 10, 120, 16), &radioButton1Function);
					window.Append(
						radioButton2 = new RadioButton("Touchpad (Absolute)", x + 120 + border, border, 120, 16), &radioButton2Function);
					window.Append(
						radioButton3 = new RadioButton("Touch Buttons", x + (120 * 2) + border, border, 120, 16), &radioButton3Function);
					window.Append(
						checkButton1 = new CheckButton("Double tap click", x + (120 * 3) + border, border, 120, 16), &checkButton1Function);
					window.Append(
						comboButton1 = new ComboButton(x, (border * 2) + 16, width - (border * 2) - x, 250), &comboButton1Function);

						for(int i = 0; i <= 255; i++) {
							char *text = new char[15];//too big?
							sprintf(text, "Profile #%i", i);
							comboButton1->Append(text);
							delete text;
						}

					window.Append(listView1 = new ListView("Name", x, (border * 2) + 16 + 24, 450, 200));
						listView1->SetDoubleClick(&listView1Function);
						listView1->InsertColumn("Mapping", 1);
						listView1->Append("Up");
						listView1->Append("Down");
						listView1->Append("Left");
						listView1->Append("Right");
						listView1->Append("A");
						listView1->Append("B");
						listView1->Append("X");
						listView1->Append("Y");
						listView1->Append("L");
						listView1->Append("R");
						listView1->Append("Start");
						listView1->Append("Select");
						listView1->Append("Lid");
						listView1->Append("Blue");
						listView1->Append("Yellow");
						listView1->Append("Red");
						listView1->Append("Green");
						listView1->SetHeaderVisible(true);
						comboButton1Function((HWND)NULL, 0, (WPARAM)NULL, (LPARAM)NULL);

					window.Append(
						button1 = new Button("Apply", width - (80 * 2) - (border * 3), 268, 80, 25), &button1Function);
					window.Append(
						button2 = new Button("Revert", width - 80 - (border * 2), 268, 80, 25), &button2Function);

					window.Append(
						button3 = new Button("Cancel", x, 268, 80, 25), &button3Function);
						button3->SetVisible(false);
					window.Append(
						button7 = new Button("Joy Button", x + 80 + border, 268, 80 - border, 25), &button7Function);
						button7->SetVisible(false);
					window.Append(
						edit1 = new Edit("", x + (80 * 2) + (border ), 268, 20, 25), &GUI::voidFunction);
						edit1->SetStyle(edit1->GetStyle()|ES_NUMBER);
						edit1->SetVisible(false);
					window.Append(
						button4 = new Button("Left Click", width - (80 * 3) - (border * 4), 268, 80, 25), &button4Function);
						button4->SetVisible(false);
					window.Append(
						button5 = new Button("Middle Click", width - (80 * 2) - (border * 3), 268, 80, 25), &button5Function);
						button5->SetVisible(false);
					window.Append(
						button6 = new Button("Right Click", width - 80 - (border * 2), 268, 80, 25), &button6Function);
						button6->SetVisible(false);


					List[0] = radioButton1;
					List[1] = radioButton2;
					List[2] = radioButton3;
					List[3] = checkButton1;
					List[4] = comboButton1;
					List[5] = listView1;
					List[6] = button1;
					List[7] = button2;
				}
				void SetVisible(bool visible) {
					for(int i = 0; i < listMax; i++) {
						if(List[i] != NULL)
							List[i]->SetVisible(visible);
					}
				}
			}
			namespace Settings {
				Label *List[listMax] = {NULL};

				Label *label;
				Edit *edit;

				string Port = "9501";

				void editFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					string portString = edit->GetText();
					int portInt = atoi(portString.c_str());
					if(portInt > 0 && portInt < 0xFFFF)
						Port = portString;
					else
						edit->SetText(Port);
					Core::Config->SetPort(atoi(Port.c_str()));
					Core::Config->Save();
					Core::UDP->Connect(Core::Config->GetPort());

					string status = "Connected on Port #" + itos(Core::Config->GetPort());
					GUI::MainWindow::StatusBar->SetText(Core::UDP->IsConnected() ? status : "Disconnected");
					CheckMenuRadioItem(	(HMENU)GUI::MainWindow::MainMenu::file->GetParentHWND(),
										GUI::MainWindow::MainMenu::fileConnect->GetID(),
										GUI::MainWindow::MainMenu::fileDisconnect->GetID(),
										Core::UDP->IsConnected() ? GUI::MainWindow::MainMenu::fileConnect->GetID() : GUI::MainWindow::MainMenu::fileDisconnect->GetID(),
										MF_BYCOMMAND);

					printf("%i:%s\n", portInt, Port.c_str());
				}

				void Setup() {
					Port = itos(Core::Config->GetPort());
					window.Append(label = new Label("Port", x, 15, 20, 16));

					window.Append(
						edit = new Edit(Port, x + 30, 10, width - x - 30 - (border * 2), 25), &editFunction);
					edit->SetStyle(edit->GetStyle()|ES_NUMBER);

					List[0] = label;
					List[1] = edit;
				}
				void SetVisible(bool visible) {
					for(int i = 0; i < listMax; i++) {
						if(List[i] != NULL)
							List[i]->SetVisible(visible);
					}
				}
			}
			namespace Log {
				Label *List[listMax] = {NULL};

				void listViewFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("logListViewFunction()\n");
				}

				void comboButtonFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("logComboButtonFunction()\n");
				}

				ListView *listView;
				ComboButton * comboButton;

				void Setup() {
					window.Append(
						listView = new ListView("", 100, 10, 450, 252), &listViewFunction);
						listView->Append("Port set to 9501");
						listView->Append("Connected");
						listView->Append("Exiting...");

					window.Append(
						comboButton = new ComboButton(100, 272, 450, 200), &comboButtonFunction);
						comboButton->Append("Disabled");
						comboButton->Append("Default");
						comboButton->Append("Warnings");
						comboButton->Append("All Messages");
						comboButton->SetSelection(Core::Config->GetDebugLevel());

					List[0] = listView;
					List[1] = comboButton;
				}
				void SetVisible(bool visible) {
					for(int i = 0; i < listMax; i++) {
						if(List[i] != NULL)
							List[i]->SetVisible(visible);
					}
				}
			}
			namespace About {
				Label *List[listMax] = {NULL};

				Label *label1;
				Label *label2;
				Label *label3;

				void Setup() {
					window.Append(label1 = new Label(VERSION_STRING, 100, 10, 250, 16));

					window.Append(label2 = new Label("Copyright (c) 2006-2013 Derrick (sypherce) Wirth", 100, 30, 250, 16));

					window.Append(label3 = new Label("http://ds2key.1emulation.com/", 100, 50, 250, 16));

					List[0] = label1;
					List[1] = label2;
					List[2] = label3;
				}
				void SetVisible(bool visible) {
					for(int i = 0; i < listMax; i++) {
						if(About::List[i] != NULL)
							List[i]->SetVisible(visible);
					}
				}
			}

			void categoryListViewFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
				printf("categoryListViewFunction()\n");
				int selection = ((LPNMLISTVIEW)lParam)->iItem;

				Profile::SetVisible(selection == 0);
				Settings::SetVisible(selection == 1);
				#ifdef LOG_IMPLEMENTED
				Log::SetVisible(selection == 2);
				About::SetVisible(selection == 3);
				#else//log not implemented
				Log::SetVisible(false);
				About::SetVisible(selection == 2);
				#endif//LOG_IMPLEMENTED
			}

			int Setup(HINSTANCE hThisInstance, int nCmdShow) {
				if(!window.Setup(hThisInstance, "DS2Key", VERSION_STRING, GUI::MainWindow::width, GUI::MainWindow::height))
					return 1;

				window.Append(
					categoryListView = new GUI::ListView("", 10, 10, 80, 268 + 25 - border), &categoryListViewFunction);
					categoryListView->Append("Profile");
					categoryListView->Append("Settings");
				#ifdef LOG_IMPLEMENTED
					categoryListView->Append("Log");
				#endif//LOG_IMPLEMENTED
					categoryListView->Append("About");

				MainMenu::Setup();
				Profile::Setup();
				Settings::Setup();
				Log::Setup();
				About::Setup();
				string status = "Connected on Port #" + itos(Core::Config->GetPort());
				window.Append(GUI::MainWindow::StatusBar = new GUI::StatusBar(Core::UDP->IsConnected() ? status : "Disconnected", 0, 0, 120, 120));
				window.Append(GUI::MainWindow::TrayIcon = new GUI::TrayIcon("DS2Key"));

				window.SetVisible(nCmdShow);
				categoryListView->SetSelection(0);
				GUI::locked = false;

				return 0;
			}
		}
	}
}
