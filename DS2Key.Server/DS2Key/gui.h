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
