/*
Windows GUI
*/

#include <windows.h>
#include <commctrl.h>
#include <iostream>//std::cout, std::clog
#include "gui/gui.h"
#include "gui/resource.h"
#include "window.h"

namespace D2K {namespace GUI {

Window::Window() : Object(0, 0, 0, 0)
{
	hwnd = NULL_VALUE;
}
Window::~Window()
{
	if(hwnd)
	{
		DestroyWindow(hwnd);
		hwnd = NULL_VALUE;
	}
}
void Window::Append(Object *object, void (*function)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam))
{
	if(this && object->Attach(this))
	{
		events[object->GetID()].object = object;
		events[object->GetID()].function = function;
		events[object->GetID()].function2 = function;
	}
}
void Window::Append(Object *object)
{
	Window::Append(object, &voidFunction);
}
bool Window::Setup(HINSTANCE hThisInstance, std::string ClassName, std::string Title, int Width, int Height)
{
	if(!hwnd)
	{
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
		wincl.hCursor = LoadCursor (NULL_VALUE, IDC_ARROW);
		wincl.lpszMenuName = NULL_VALUE;                 /* No menu */
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
					NULL_VALUE,                /* No menu */
					hThisInstance,       /* Program Instance handler */
					NULL_VALUE                 /* No Window Creation data */
				);
		std::cout << "Window hwnd:" << (unsigned int)hwnd << "\n";
		SetFont(hwnd);
		//CreateMainMenu();
		return true;
	}
	return false;
}

void Window::CreateMainMenu()
{
	enum { MenuFileConnect, MenuFileMinimize, MenuFileExit };
	HMENU mainMenu = CreateMenu();
	HMENU fileMenu = CreatePopupMenu();
	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)fileMenu, "&File");
	AppendMenu(fileMenu, MF_STRING, MenuFileConnect, "&Connect");
	AppendMenu(fileMenu, MF_STRING, MenuFileMinimize, "&Minimize");
	AppendMenu(fileMenu, MF_STRING, MenuFileExit, "&Exit");
	SetMenu(hwnd, mainMenu);
}

void Window::SetVisible(int visible)
{
	if(hwnd)
		ShowWindow(hwnd, visible);
}

void Window::SetVisible(bool visible)
{
	if(hwnd)
		ShowWindow(hwnd, visible);
}
void Window::SetText(std::string text)
{
	Text = text;
	locked = true;
	SetWindowText(hwnd, Text.c_str());
	locked = false;
}

}}//namespace D2K::GUI
