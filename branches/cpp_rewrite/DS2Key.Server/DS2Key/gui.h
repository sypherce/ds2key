/*
DS2Key Server - An application to use your DS as a PC Gamepad
Copyright (C) 2008  Derrick (sypherce) Wirth

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
#ifndef __GUI_H__
#define __GUI_H__

#define WM_APP_TRAYMSG WM_APP
#define IDT_TIMER (WM_APP + 1)

#ifdef __GNUC__
#define MAPVK_VK_TO_VSC (0)
#define MAPVK_VSC_TO_VK (1)
#define MAPVK_VK_TO_CHAR (2)
#define MAPVK_VSC_TO_VK_EX (3)
#endif


extern HINSTANCE hInst;
extern HWND hwndPointer[3];
extern NOTIFYICONDATA niData;
extern unsigned int lastID;
extern char *keyText;
extern int dialogCounter;

extern ULONGLONG GetDllVersion(LPCTSTR lpszDllName);
extern void showNotifyIcon(HWND hwndDlg, HICON hIcon);
extern void hideNotifyIcon();
extern void ShowContextMenu(HWND hWnd);
extern BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

#endif //__GUI_H__
