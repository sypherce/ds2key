/*
DS2Key Server - An application to use your DS as a PC Gamepad
Copyright (C) 2008, 2009  Derrick (sypherce) Wirth

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef GUI
#define WIN32_LEAN_AND_MEAN
#define _WIN32_IE 0x0600

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <shlwapi.h>
#include <shellapi.h>

#include "main.h"
#include "gui.h"
#include "config.h"
#include "key.h"
#include "resource.h"

HINSTANCE hInst;
HWND hwndPointer[3];
NOTIFYICONDATA niData;
unsigned int lastID;
char *keyText;
int dialogCounter = 0;
int currentGUIProfile = 0;

ULONGLONG GetDllVersion(LPCTSTR lpszDllName)
{
	ULONGLONG ullVersion = 0;
	HINSTANCE hinstDll;
	hinstDll = LoadLibrary(lpszDllName);

	if(hinstDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");

		if(pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			HRESULT hr;
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			hr = (*pDllGetVersion)(&dvi);

			if(SUCCEEDED(hr))
				ullVersion = MAKEDLLVERULL(dvi.dwMajorVersion, dvi.dwMinorVersion, 0, 0);
		}

		FreeLibrary(hinstDll);
	}

	return ullVersion;
}

void addNotifyIcon(HWND hwndDlg, HICON hIcon)
{
	ULONGLONG ullVersion = GetDllVersion("Shell32.dll");
	ZeroMemory(&niData, sizeof(NOTIFYICONDATA));

	if(ullVersion >= MAKEDLLVERULL(5, 0, 0, 0))
	{
		niData.cbSize = sizeof(NOTIFYICONDATA);
	}
	else
	{
		niData.cbSize = NOTIFYICONDATA_V1_SIZE;
	}

	niData.uID = ID_TRAY;
	niData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	strcpy(niData.szTip, "DS2Key");
	niData.hIcon = hIcon;
	niData.hWnd = hwndDlg;
	niData.uCallbackMessage = WM_APP_TRAYMSG;
	Shell_NotifyIcon(NIM_ADD, &niData);
}

void deleteNotifyIcon()
{
	Shell_NotifyIcon(NIM_DELETE, &niData);
}

void ShowContextMenu(HWND hWnd)
{
	POINT pt;
	HMENU hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU_MAIN));
	GetCursorPos(&pt);

	if(hMenu)
	{
		HMENU hmenuPopup = GetSubMenu(hMenu, 0);
		int checked;
		if(connectedDS2Key == TRUE)
		{
            checked = IDM_CONNECT;
		}
		else
		{
            checked = IDM_DISCONNECT;
		}
		CheckMenuRadioItem(hMenu, IDM_CONNECT, IDM_DISCONNECT, checked, MF_BYCOMMAND);
		SetForegroundWindow(hWnd);

		TrackPopupMenu(hmenuPopup, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
		DestroyMenu(hMenu);
	}
}

unsigned int buttonControlLookup[pEND] =
{
	0,
	IDC_BTN_UP,
	IDC_BTN_DOWN,
	IDC_BTN_LEFT,
	IDC_BTN_RIGHT,
	IDC_BTN_A,
	IDC_BTN_B,
	IDC_BTN_X,
	IDC_BTN_Y,
	IDC_BTN_L,
	IDC_BTN_R,
	IDC_BTN_START,
	IDC_BTN_SELECT,
	IDC_BTN_BLUE,
	IDC_BTN_YELLOW,
	IDC_BTN_RED,
	IDC_BTN_GREEN,
	IDC_BTN_BUTTON1,
	IDC_BTN_BUTTON2,
	IDC_BTN_BUTTON3,
	IDC_BTN_BUTTON4,
	IDC_BTN_BUTTON5,
	IDC_BTN_BUTTON6,
	IDC_BTN_BUTTON7,
	IDC_BTN_BUTTON8,
	IDC_BTN_BUTTON9,
	IDC_BTN_BUTTON10,
	IDC_BTN_BUTTON11,
	IDC_BTN_BUTTON12
};

bool loadGUIProfile(unsigned char profileNumber)
{
	int returnVal = readProfileConfig(profileNumber);
	int i;
	currentGUIProfile = profileNumber;

	for(i = 1; i < pEND; i++)
	{
		SetDlgItemText(hwndPointer[0], buttonControlLookup[i], getKeyString(profile[profileNumber][i]) + 4);
	}

	return returnVal;
}

void applyGUIProfile()
{
	int i;
	char text[256];
	for(i = 1; i < pEND; i++)
	{
		char KEY_text[260];
		unsigned int key = 0;
		GetDlgItemText(hwndPointer[0], buttonControlLookup[i], text, 256);
		sprintf(KEY_text, "KEY_%s", text);
		key = getKeyNumber(KEY_text);

		if(key <= 65535)
		{
			profile[currentGUIProfile][i] = key;
		}
		else
		{
			profile[currentGUIProfile][i] = 0;
		}
	}
    GetDlgItemText(hwndPointer[0], IDC_EDT_PORT, text, 256);
    serverPort = atoi(text);
    writeConfig();
    if(connectedDS2Key)
    {
        ds2keyDisconnect();
        ds2keyConnect(serverPort);
    }
}

bool saveGUIProfile()
{
	applyGUIProfile();

	return writeProfileConfig(currentGUIProfile);
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(dialogCounter == 3)
	{
		BYTE keys[256];
		int i;
		GetKeyboardState((PBYTE)keys);
		for(i = 0; i < 255; i++)
		{
			if(keys[i] > 1)
			{
				if(i != VK_LBUTTON && i != VK_RBUTTON && i != VK_MBUTTON)
				{
					int key;
					for(key = 0; key < pEND; key++)
					{
						if(buttonControlLookup[key] == lastID)
						{
							break;
						}
					}

					keyText = getKeyString(i) + 4; //(char*)malloc(3*sizeof(char));

					PostMessage(hwndPointer[2], WM_CLOSE, (WPARAM)0, (LPARAM)0);
					break;
				}
			}
		}
	}

	switch(uMsg)
	{
		case WM_TIMER:
		{
			switch(wParam)
			{
				case IDT_TIMER:
				{
					serverLoop();
					break;
				}
			}

			break;
		}
		case WM_INITDIALOG:
		{
			hwndPointer[dialogCounter] = hwndDlg;

			if(dialogCounter == 0)
			{
				HICON hIcon = (HICON)NULL;
				char profileComboText[256][4];
				hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MAIN_ICON));
				SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)hIcon);
				SendMessage(GetDlgItem(hwndDlg, IDC_EDT_PORT), EM_SETLIMITTEXT, (WPARAM)5, (LPARAM)NULL);
				{
					int i;
					for(i = 0; i <= 255; i++)
					{
						sprintf(profileComboText[i], "%i", i);
						SendDlgItemMessage(hwndDlg, IDC_CBO_PROFILE, CB_ADDSTRING, 0, (LPARAM)profileComboText[i]);
					}
				}
				SendDlgItemMessage(hwndDlg, IDC_CBO_PROFILE, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
				SendDlgItemMessage(hwndDlg, IDC_CBO_LOG, CB_ADDSTRING, 0, (LPARAM)"Disabled");
				SendDlgItemMessage(hwndDlg, IDC_CBO_LOG, CB_ADDSTRING, 0, (LPARAM)"Default");
				SendDlgItemMessage(hwndDlg, IDC_CBO_LOG, CB_ADDSTRING, 0, (LPARAM)"Warnings");
				SendDlgItemMessage(hwndDlg, IDC_CBO_LOG, CB_ADDSTRING, 0, (LPARAM)"All Messages");
				SendDlgItemMessage(hwndDlg, IDC_CBO_LOG, CB_SETCURSEL, (WPARAM)printDebugLevel, (LPARAM)0);
				SetDlgItemInt(hwndDlg, IDC_EDT_PORT, serverPort, FALSE);
				loadGUIProfile(0);

				addNotifyIcon(hwndDlg, hIcon);

				if(hIcon && DestroyIcon(hIcon))
				{
					hIcon = (HICON)NULL;
				}

				{//check menu
                    HMENU hMenu = GetMenu(hwndPointer[0]);
                    int checked;

                    if(connectedDS2Key == TRUE)
                    {
                        checked = IDM_CONNECT;
                    }
                    else
                    {
                        checked = IDM_DISCONNECT;
                    }
                    CheckMenuRadioItem(hMenu, IDM_CONNECT, IDM_DISCONNECT, checked, MF_BYCOMMAND);
				}

				SetTimer(hwndDlg, IDT_TIMER, 10, (TIMERPROC)NULL);
			}
			else if(dialogCounter == 1)
			{
				//SendMessage(hwndDlg, WM_KILLFOCUS,(WPARAM)hwndDlg, 0);
			}

			dialogCounter++;

			break;
		}
		case WM_CLOSE:
		{
			if(dialogCounter == 1)
			{
				ShowWindow(hwndDlg, SW_HIDE);
			}
			else
			{
				EndDialog(hwndDlg, 0);
			}

			dialogCounter--;
			break;
		}

		case WM_APP_TRAYMSG:
		{
			switch(lParam)
			{
				case WM_LBUTTONDOWN:
				{
					ShowWindow(hwndDlg, SW_RESTORE);
					break;
				}
				case WM_RBUTTONDOWN:
				case WM_CONTEXTMENU:
				{
					ShowContextMenu(hwndDlg);
					break;
				}
			}

			break;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDC_BTN_A:
				case IDC_BTN_B:
				case IDC_BTN_X:
				case IDC_BTN_Y:
				case IDC_BTN_START:
				case IDC_BTN_SELECT:
				case IDC_BTN_L:
				case IDC_BTN_R:
				case IDC_BTN_UP:
				case IDC_BTN_DOWN:
				case IDC_BTN_LEFT:
				case IDC_BTN_RIGHT:
				case IDC_BTN_BLUE:
				case IDC_BTN_YELLOW:
				case IDC_BTN_RED:
				case IDC_BTN_GREEN:
				case IDC_BTN_BUTTON1:
				case IDC_BTN_BUTTON2:
				case IDC_BTN_BUTTON3:
				case IDC_BTN_BUTTON4:
				case IDC_BTN_BUTTON5:
				case IDC_BTN_BUTTON6:
				case IDC_BTN_BUTTON7:
				case IDC_BTN_BUTTON8:
				case IDC_BTN_BUTTON9:
				case IDC_BTN_BUTTON10:
				case IDC_BTN_BUTTON11:
				case IDC_BTN_BUTTON12:
				{
					lastID = LOWORD(wParam);
					DialogBox(NULL, MAKEINTRESOURCE(IDD_DLG_KEY_MAPPING), hwndDlg, DialogProc);
					break;
				}
				case IDC_BTN_SAVE:
				{
					saveGUIProfile();

					break;
				}
				case IDC_BTN_APPLY:
				{
					applyGUIProfile();

					break;
				}
				case IDC_BTN_CAPTURE_KEY:
				case IDC_BTN_MOUSE_LEFT_CLICK:
				case IDC_BTN_MOUSE_RIGHT_CLICK:
				case IDC_BTN_MOUSE_MIDDLE_CLICK:
				case IDC_BTN_NONE:
				{
					keyText = "None";

					if(LOWORD(wParam) == IDC_BTN_CAPTURE_KEY)
					{
						char *lastText;
						lastText = keyText;
						DialogBox(NULL, MAKEINTRESOURCE(IDD_DLG_KEY_CAPTURE), hwndDlg, DialogProc);

						if(lastText == keyText)
						{
							break;
						}

						//check if it closed by [x] or if we caputred something
					}
					else if(LOWORD(wParam) == IDC_BTN_MOUSE_LEFT_CLICK)
					{
						keyText = "LBUTTON"; //Get Key Input
					}
					else if(LOWORD(wParam) == IDC_BTN_MOUSE_RIGHT_CLICK)
					{
						keyText = "RBUTTON"; //Get Key Input
					}
					else if(LOWORD(wParam) == IDC_BTN_MOUSE_MIDDLE_CLICK)
					{
						keyText = "MBUTTON"; //Get Key Input
					}

					SetDlgItemText(hwndPointer[0], lastID, (char *)keyText);
					SendMessage(hwndDlg, WM_CLOSE, (WPARAM)0, (LPARAM)0);
					//EndDialog(hwndDlg, 0);
					break;
				}
				case IDC_CBO_PROFILE:
				{
					switch(HIWORD(wParam))
					{
						case CBN_SELCHANGE:
						{
							//check edited stuff!!!!
							int selected = SendDlgItemMessage(hwndDlg, IDC_CBO_PROFILE, CB_GETCURSEL, 0, (LPARAM)0);

							if(selected != currentGUIProfile && selected != CB_ERR)
							{
								loadGUIProfile(selected);
							}

							break;
						}
					}
				}
				case IDC_CBO_LOG:
				{
					switch(HIWORD(wParam))
					{
						case CBN_SELCHANGE:
						{
							//check edited stuff!!!!
							printDebugLevel = (char)SendDlgItemMessage(hwndDlg, IDC_CBO_LOG, CB_GETCURSEL, 0, (LPARAM)0);

							break;
						}
					}
				}
				case IDM_CONNECT:
				{
                    HMENU hMenu = GetMenu(hwndPointer[0]);
                    int checked;

					ds2keyConnect(serverPort);
                    if(connectedDS2Key == TRUE)
                    {
                        checked = IDM_CONNECT;
                    }
                    else
                    {
                        checked = IDM_DISCONNECT;
                    }
                    CheckMenuRadioItem(hMenu, IDM_CONNECT, IDM_DISCONNECT, checked, MF_BYCOMMAND);

					break;
				}
				case IDM_DISCONNECT:
				{
                    HMENU hMenu = GetMenu(hwndPointer[0]);
                    int checked;

					ds2keyDisconnect();
                    if(connectedDS2Key == TRUE)
                    {
                        checked = IDM_CONNECT;
                    }
                    else
                    {
                        checked = IDM_DISCONNECT;
                    }
                    CheckMenuRadioItem(hMenu, IDM_CONNECT, IDM_DISCONNECT, checked, MF_BYCOMMAND);

					break;
				}
				case IDM_EXIT:
				{
					deleteNotifyIcon();
					EndDialog(hwndDlg, 0);

					break;
				}
				case IDM_ABOUT:
				{
					MessageBox(hwndDlg, "DS2Key 1.02\nCopyright (C) 2008, 2009 Derrick (sypherce) Wirth\nhttp://ds2key.1emulation.com/", "About DS2Key...", MB_ICONINFORMATION);

					break;
				}
				case IDC_EDT_PORT:
				{
					BOOL translated;
					unsigned int port = GetDlgItemInt(hwndDlg, LOWORD(wParam), &translated, FALSE);
					unsigned int lastPort = port;

					if(port > 65535)
					{
						port = 65535;
					}
					else if(port < 1)
					{
						port = 1;
					}

					if(port != lastPort)
					{
						SetDlgItemInt(hwndDlg, LOWORD(wParam), port, FALSE);
					}

					break;
				}
			}

			break;
		}
		default:
		{
			//return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
		}
	}

	return 0;
}
#endif//GUI

