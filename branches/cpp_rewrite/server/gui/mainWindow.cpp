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
string itos(int i) // convert int to string
{
    std::stringstream s;
    s << i;
    return s.str();
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
					if(!Core::udp->IsConnected())
					{
						Core::udp->Connect();
					}
					CheckMenuRadioItem(	(HMENU)GUI::MainWindow::MainMenu::file->getParentHWND(),
										GUI::MainWindow::MainMenu::fileConnect->getID(),
										GUI::MainWindow::MainMenu::fileDisconnect->getID(),
										Core::udp->IsConnected() ? GUI::MainWindow::MainMenu::fileConnect->getID() : GUI::MainWindow::MainMenu::fileDisconnect->getID(),
										MF_BYCOMMAND);
					//check if connected properly
					string status = "Connected on Port #" + itos(Core::config->GetPort());
					GUI::MainWindow::StatusBar->setText(Core::udp->IsConnected() ? status : "Disconnected");
				}
				void fileDisconnectFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("fileConnectFunction()\n");
					if(Core::udp->IsConnected())
					{
						Core::udp->Disconnect();
					}
					CheckMenuRadioItem(	(HMENU)GUI::MainWindow::MainMenu::file->getParentHWND(),
										GUI::MainWindow::MainMenu::fileConnect->getID(),
										GUI::MainWindow::MainMenu::fileDisconnect->getID(),
										Core::udp->IsConnected() ? GUI::MainWindow::MainMenu::fileConnect->getID() : GUI::MainWindow::MainMenu::fileDisconnect->getID(),
										MF_BYCOMMAND);
					string status = "Connected on Port #" + itos(Core::config->GetPort());
					GUI::MainWindow::StatusBar->setText(Core::udp->IsConnected() ? status : "Disconnected");
				}
				void fileMinimizeFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("fileMinimizeFunction()\n");
					menu->getParent()->setVisible(false);
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
						CheckMenuRadioItem(	(HMENU)GUI::MainWindow::MainMenu::file->getParentHWND(),
											GUI::MainWindow::MainMenu::fileConnect->getID(),
											GUI::MainWindow::MainMenu::fileDisconnect->getID(),
											Core::udp->IsConnected() ? GUI::MainWindow::MainMenu::fileConnect->getID() : GUI::MainWindow::MainMenu::fileDisconnect->getID(),
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
					checkButton1->setEnabled(true);
					Core::ClientArray[GUI::MainWindow::Profile::comboButton1->getSelection()]->SetButton(Core::pMouse, Core::mRelative);
				}

				void radioButton2Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("profileRadioButton2Function()\n");
					checkButton1->setEnabled(true);
					Core::ClientArray[GUI::MainWindow::Profile::comboButton1->getSelection()]->SetButton(Core::pMouse, Core::mAbsolute);
				}

				void radioButton3Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("profileRadioButton3Function()\n");
					Core::ClientArray[GUI::MainWindow::Profile::comboButton1->getSelection()]->SetButton(Core::pMouse, Core::mButtons);
					checkButton1->setEnabled(false);
				}

				void checkButton1Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					printf("checkButton1Function()\n");
				}
				void comboButton1Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					int Profile = comboButton1->getSelection();
					if(Core::ClientArray[Profile] == (Core::Client*)NULL)		//if profile not active, load it, then send data
					{
						Core::ClientArray[Profile] = new Core::Client();
					}
					Core::config->ReadProfile(Profile);
					int row = 0;
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pUp), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pDown), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pLeft), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pRight), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pA), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pB), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pX), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pY), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pL), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pR), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pStart), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pSelect), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pLid), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pBlue), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pYellow), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pRed), row++, 1);
					listView1->SetText(Core::ClientArray[Profile]->GetButtonString(Core::pGreen), row++, 1);

						radioButton1->setChecked(false);
						radioButton2->setChecked(false);
						radioButton3->setChecked(false);
						checkButton1->setEnabled(true);

					if(Core::ClientArray[Profile]->GetButton(Core::pMouse) == Core::mAbsolute)
						radioButton2->setChecked(true);
					else if(Core::ClientArray[Profile]->GetButton(Core::pMouse) == Core::mButtons) {
						radioButton3->setChecked(true);
						checkButton1->setEnabled(false);
					}
					else
						radioButton1->setChecked(true);
				}

				void listView1Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					int selected = listView1->getSelection();
					string oldStatus = GUI::MainWindow::StatusBar->getText();
					GUI::MainWindow::StatusBar->setText("Assign a Key [" + listView1->GetText(selected, 0) + "]");
					//disable
					GUI::MainWindow::categoryListView->setEnabled(false);
					GUI::MainWindow::Profile::radioButton1->setEnabled(false);
					GUI::MainWindow::Profile::radioButton2->setEnabled(false);
					GUI::MainWindow::Profile::radioButton3->setEnabled(false);
					GUI::MainWindow::Profile::checkButton1->setEnabled(false);
					GUI::MainWindow::Profile::comboButton1->setEnabled(false);
					GUI::MainWindow::Profile::listView1->setEnabled(false);
					GUI::MainWindow::Profile::button1->setEnabled(false);
					GUI::MainWindow::Profile::button2->setEnabled(false);
					GUI::MainWindow::Profile::edit1->setEnabled(false);
					//show
					GUI::MainWindow::Profile::edit1->setVisible(true);
					GUI::MainWindow::Profile::button7->setVisible(true);
					GUI::MainWindow::Profile::button3->setVisible(true);
					GUI::MainWindow::Profile::button4->setVisible(true);
					GUI::MainWindow::Profile::button5->setVisible(true);
					GUI::MainWindow::Profile::button6->setVisible(true);

					{
						BYTE keys[256];
						int i = 0;
						waiting = true;
						while(waiting)
						{
							GetKeyboardState((PBYTE)keys);
							for(i = 0; i < 255; i++)
							{
								if(!waiting)
									break;
								if(keys[i] > 1)
								{
									if(i != VK_LBUTTON && i != VK_RBUTTON && i != VK_MBUTTON)
									{
										listView1->SetText(Core::Key::GetString(i), selected, 1);
										Core::ClientArray[GUI::MainWindow::Profile::comboButton1->getSelection()]->SetButton(listView1->getSelection() + 3, i);

										waiting = false;
									}
								}
							}
							GUI::GetMessages();//Application::processEvents();
						}
						GUI::MainWindow::StatusBar->setText(oldStatus);
						//enable
						GUI::MainWindow::categoryListView->setEnabled(true);
						GUI::MainWindow::Profile::radioButton1->setEnabled(true);
						GUI::MainWindow::Profile::radioButton2->setEnabled(true);
						GUI::MainWindow::Profile::radioButton3->setEnabled(true);
						GUI::MainWindow::Profile::checkButton1->setEnabled(true);
						GUI::MainWindow::Profile::comboButton1->setEnabled(true);
						GUI::MainWindow::Profile::listView1->setEnabled(true);
						GUI::MainWindow::Profile::button1->setEnabled(true);
						GUI::MainWindow::Profile::button2->setEnabled(true);
						//hide
						GUI::MainWindow::Profile::edit1->setVisible(false);
						GUI::MainWindow::Profile::button7->setVisible(false);
						GUI::MainWindow::Profile::button3->setVisible(false);
						GUI::MainWindow::Profile::button4->setVisible(false);
						GUI::MainWindow::Profile::button5->setVisible(false);
						GUI::MainWindow::Profile::button6->setVisible(false);
					}
				}

				void button1Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					Core::config->SaveProfile(
							GUI::MainWindow::Profile::comboButton1->getSelection());
					comboButton1Function(hwnd, message, wParam, lParam);
				}

				void button2Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					comboButton1Function(hwnd, message, wParam, lParam);
				}

				void button3Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					waiting = false;
				}

				void button4Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					listView1->SetText(Core::Key::GetString(VK_LBUTTON), listView1->getSelection(), 1);
					Core::ClientArray[GUI::MainWindow::Profile::comboButton1->getSelection()]->SetButton(listView1->getSelection() + 3, VK_LBUTTON);
					waiting = false;
				}

				void button5Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					listView1->SetText(Core::Key::GetString(VK_MBUTTON), listView1->getSelection(), 1);
					Core::ClientArray[GUI::MainWindow::Profile::comboButton1->getSelection()]->SetButton(listView1->getSelection() + 3, VK_MBUTTON);
					waiting = false;
				}

				void button6Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					listView1->SetText(Core::Key::GetString(VK_RBUTTON), listView1->getSelection(), 1);
					Core::ClientArray[GUI::MainWindow::Profile::comboButton1->getSelection()]->SetButton(listView1->getSelection() + 3, VK_RBUTTON);
					waiting = false;
				}

				void button7Function(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					uint32_t Value = 0x100 + atoi(edit1->getText().c_str());
					edit1->setEnabled(true);
					listView1->SetText(Core::Key::GetString(Value), listView1->getSelection(), 1);
					Core::ClientArray[GUI::MainWindow::Profile::comboButton1->getSelection()]->SetButton(listView1->getSelection() + 3, Value);
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
						listView1->setHeaderVisible(true);
						comboButton1Function((HWND)NULL, 0, (WPARAM)NULL, (LPARAM)NULL);

					window.Append(
						button1 = new Button("Apply", width - (80 * 2) - (border * 3), 268, 80, 25), &button1Function);
					window.Append(
						button2 = new Button("Revert", width - 80 - (border * 2), 268, 80, 25), &button2Function);

					window.Append(
						button3 = new Button("Cancel", x, 268, 80, 25), &button3Function);
						button3->setVisible(false);
					window.Append(
						button7 = new Button("Joy Button", x + 80 + border, 268, 80 - border, 25), &button7Function);
						button7->setVisible(false);
					window.Append(
						edit1 = new Edit("", x + (80 * 2) + (border ), 268, 20, 25), &GUI::voidFunction);
						edit1->setStyle(edit1->getStyle()|ES_NUMBER);
						edit1->setVisible(false);
					window.Append(
						button4 = new Button("Left Click", width - (80 * 3) - (border * 4), 268, 80, 25), &button4Function);
						button4->setVisible(false);
					window.Append(
						button5 = new Button("Middle Click", width - (80 * 2) - (border * 3), 268, 80, 25), &button5Function);
						button5->setVisible(false);
					window.Append(
						button6 = new Button("Right Click", width - 80 - (border * 2), 268, 80, 25), &button6Function);
						button6->setVisible(false);


					List[0] = radioButton1;
					List[1] = radioButton2;
					List[2] = radioButton3;
					List[3] = checkButton1;
					List[4] = comboButton1;
					List[5] = listView1;
					List[6] = button1;
					List[7] = button2;
				}
				void setVisible(bool visible) {
					for(int i = 0; i < listMax; i++) {
						if(List[i] != NULL)
							List[i]->setVisible(visible);
					}
				}
			}
			namespace Settings {
				Label *List[listMax] = {NULL};

				Label *label;
				Edit *edit;

				string Port = "9501";

				void editFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
					string portString = edit->getText();
					int portInt = atoi(portString.c_str());
					if(portInt > 0 && portInt < 0xFFFF)
						Port = portString;
					else
						edit->setText(Port);
					Core::config->SetPort(atoi(Port.c_str()));
					Core::config->Save();
					Core::udp->Connect(Core::config->GetPort());

					string status = "Connected on Port #" + itos(Core::config->GetPort());
					GUI::MainWindow::StatusBar->setText(Core::udp->IsConnected() ? status : "Disconnected");
					CheckMenuRadioItem(	(HMENU)GUI::MainWindow::MainMenu::file->getParentHWND(),
										GUI::MainWindow::MainMenu::fileConnect->getID(),
										GUI::MainWindow::MainMenu::fileDisconnect->getID(),
										Core::udp->IsConnected() ? GUI::MainWindow::MainMenu::fileConnect->getID() : GUI::MainWindow::MainMenu::fileDisconnect->getID(),
										MF_BYCOMMAND);

					printf("%i:%s\n", portInt, Port.c_str());
				}

				void Setup() {
					Port = itos(Core::config->GetPort());
					window.Append(label = new Label("Port", x, 15, 20, 16));

					window.Append(
						edit = new Edit(Port, x + 30, 10, width - x - 30 - (border * 2), 25), &editFunction);
					edit->setStyle(edit->getStyle()|ES_NUMBER);

					List[0] = label;
					List[1] = edit;
				}
				void setVisible(bool visible) {
					for(int i = 0; i < listMax; i++) {
						if(List[i] != NULL)
							List[i]->setVisible(visible);
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
						comboButton->setSelection(Core::config->GetDebugLevel());

					List[0] = listView;
					List[1] = comboButton;
				}
				void setVisible(bool visible) {
					for(int i = 0; i < listMax; i++) {
						if(List[i] != NULL)
							List[i]->setVisible(visible);
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
				void setVisible(bool visible) {
					for(int i = 0; i < listMax; i++) {
						if(About::List[i] != NULL)
							List[i]->setVisible(visible);
					}
				}
			}

			void categoryListViewFunction(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
				printf("categoryListViewFunction()\n");
				int selection = ((LPNMLISTVIEW)lParam)->iItem;

				Profile::setVisible(selection == 0);
				Settings::setVisible(selection == 1);
				#ifdef LOG_IMPLEMENTED
				Log::setVisible(selection == 2);
				About::setVisible(selection == 3);
				#else
				Log::setVisible(false);
				About::setVisible(selection == 2);
				#endif//LOG_IMPLEMENTED
			}

			int Setup(HINSTANCE hThisInstance, int nCmdShow)
			{
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
				string status = "Connected on Port #" + itos(Core::config->GetPort());
				window.Append(GUI::MainWindow::StatusBar = new GUI::StatusBar(Core::udp->IsConnected() ? status : "Disconnected", 0, 0, 120, 120));
				window.Append(GUI::MainWindow::TrayIcon = new GUI::TrayIcon("DS2Key"));

				window.setVisible(nCmdShow);
				categoryListView->setSelection(0);
				GUI::locked = false;

				return 0;
			}
		}
	}
}
