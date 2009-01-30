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
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef WIN32
#ifdef WINVER
#undef WINVER
#endif //WINVER
#define WINVER 0x0500
#define WIN32_LEAN_AND_MEAN
#ifdef _WIN32_IE
#undef _WIN32_IE
#endif //_WIN32_IE
#define _WIN32_IE 0x0600
#include <windows.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <winsock.h>
#define sockaddr_in__address(sockaddr_in) sockaddr_in.sin_addr.S_un.S_addr
#else //WIN32
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#define sockaddr_in__address(sockaddr_in) sockaddr_in.sin_addr.s_addr
#endif //WIN32

#ifdef GUI
#include "gui.h"
#include "resource.h"
#endif //GUI

#include "config.h"
#include "key.h"

//Variables
#ifdef WIN32
HINSTANCE hInst;
INPUT input;
#ifdef GUI
char *logText = (char *)NULL;
#endif //GUI
#else //WIN32
Display *display;
#endif //WIN32
bool mouseKeys[13];
bool mouseKeysLast[13];
bool newTouch = 0;
int screenBorder = 8;
bool relativeTouch = 0;
int mouseXLast = -1;
int mouseYLast = -1;
struct sockaddr_in servAddr;
int sd;
bool connectedDS2Key;
#include "main.h"

int _printf(char debugLevel, const char *format, ...)
{
	int returnVal = 0;
	if(debugLevel <= printDebugLevel)
	{
		va_list argList;
#ifdef GUI
		HWND hwndLog;
		int position;
		int lines;
		int logTextLen;
		char displayText[1024];
		//char *position = IupGetAttribute(ml, IUP_CARET);
#endif //GUI
		va_start(argList, format);

#ifdef GUI
		returnVal = vsprintf(displayText, format, argList);

		if(logText != (char *)NULL)
		{
			logTextLen = strlen(logText);
		}
		else
		{
			logTextLen = 0;
		}

		logTextLen += strlen(displayText) + strlen("\x0d\x0a") + 1;
		if(logText == (char*)NULL)
		{
		    logText = (char*)malloc(logTextLen);
		    logText[0] = 0;
		}
		else
		{
            logText = (char *)realloc(logText, logTextLen);
		}
		strcat(logText, displayText);
		strcat(logText, "\x0d\x0a");
		hwndLog = GetDlgItem(hwndPointer[0], IDC_EDT_LOG);
		position = SendMessage(hwndLog, EM_GETFIRSTVISIBLELINE, (WPARAM)NULL, (LPARAM)NULL);
		lines = SendMessage(hwndLog, EM_GETLINECOUNT, (WPARAM)NULL, (LPARAM)NULL);
		SetDlgItemText(hwndPointer[0], IDC_EDT_LOG, logText);

		if(lines - 12 < position)
		{
			SendMessage(hwndLog, EM_LINESCROLL, (WPARAM)NULL, (LPARAM)lines - 10);
		}
		else
		{
			SendMessage(hwndLog, EM_LINESCROLL, (WPARAM)NULL, (LPARAM)position);
		}

#else //GUI
		returnVal = vprintf(format, argList);
		printf("\n");
#endif //GUI

		va_end(argList);
	}

	return returnVal;
}

void doInput(unsigned int type, unsigned int key, bool state)
{
	if(type == INPUT_KEYBOARD)
	{
		if(key == KEY_LBUTTON || key == KEY_RBUTTON || key == KEY_MBUTTON)
		{
#ifdef WIN32
			input.type = INPUT_MOUSE;
			input.mi.dx = 0;
			input.mi.dy = 0;
			input.mi.dwExtraInfo = 0;
			input.mi.mouseData = 0;
			input.mi.time = 0;

			if(key == KEY_LBUTTON)
			{
				if(state)
				{
					input.mi.dwFlags = MOUSEEVENTF_LEFTUP | MOUSEEVENTF_MOVE;
				}
				else
				{
					input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_MOVE;
				}
			}
			else if(key == KEY_RBUTTON)
			{
				if(state)
				{
					input.mi.dwFlags = MOUSEEVENTF_RIGHTUP | MOUSEEVENTF_MOVE;
				}
				else
				{
					input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_MOVE;
				}
			}
			else if(key == KEY_MBUTTON)
			{
				if(state)
				{
					input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP | MOUSEEVENTF_MOVE;
				}
				else
				{
					input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MOVE;
				}
			}

#else//WIN32
            XTestFakeButtonEvent(display, key - KEY_LBUTTON + 1, !state, 0);
#endif//WIN32
		}
		else
		{
#ifdef WIN32
            input.type = type;
			input.ki.wVk = key;
			input.ki.dwFlags = KEYEVENTF_SCANCODE;

			if(isExtended(key))
			{
				input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
			}

			if(state)
			{
				input.ki.dwFlags |= KEYEVENTF_KEYUP;
			}

			input.ki.wScan = MapVirtualKey(key, 0);
			input.ki.time = 0;
			input.ki.dwExtraInfo = 0;
#else//WIN32
            int code = XKeysymToKeycode(display, key);

            XTestFakeKeyEvent(display, code, !state, CurrentTime);
#endif//WIN32
		}
	}
	else if(type == INPUT_MOUSE)
	{
		unsigned char keyX = key & 0xff;
		unsigned char keyY = (key >> 8) & 0xff;
		if(relativeTouch)
		{
			screenBorder = 0;
		}
		else
		{
			screenBorder = 8;
		}
		if(keyX < screenBorder)
		{
			keyX = screenBorder;
		}
		else if(keyX > 256 - screenBorder)
		{
			keyX = 256 - screenBorder;
		}

		if(keyY < screenBorder)
		{
			keyY = screenBorder;
		}
		else if(keyY > 192 - screenBorder)
		{
			keyY = 192 - screenBorder;
		}
		keyX -= screenBorder;
		keyY -= screenBorder;
		//end border stuff

#ifdef WIN32
	    input.type = type;
		if(relativeTouch)
		{
			input.mi.dx = (keyX - mouseXLast) * 3;//-16 border
			input.mi.dy = (keyY - mouseYLast) * 3;//-16 border
			mouseXLast = keyX;
			mouseYLast = keyY;
			input.mi.dwFlags = MOUSEEVENTF_MOVE;
		}
		else
		{
			input.mi.dx = 65535 * keyX / (256 - (screenBorder * 2));//-16 border
			input.mi.dy = 65535 * keyY / (192 - (screenBorder * 2));//-16 border
			input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		}
		input.mi.dwExtraInfo = 0;
		input.mi.mouseData = 0;
		input.mi.time = 0;
#else//WIN32
        Window dummyWin;
        int dummyX, dummyY;
        unsigned int width, height, dummyBorder, dummyDepth;

        int screen = DefaultScreen(display);
        Window rootwindow = RootWindow(display, screen);

        if(XGetGeometry(display, rootwindow, &dummyWin, &dummyX, &dummyY, &width, &height, &dummyBorder, &dummyDepth))
        {
			int x, y;
			if(relativeTouch)
			{
				x = (keyX - mouseXLast) * 3;//-16 border
				y = (keyY - mouseYLast) * 3;//-16 border
				mouseXLast = keyX;
				mouseYLast = keyY;
	            XTestFakeRelativeMotionEvent(display, x, y, 0);
			}
			else
			{
	            x = width * keyX / (256 - (screenBorder * 2));//-16 border
	            y = height * keyY / (192 - (screenBorder * 2));//-16 border
	            XTestFakeMotionEvent(display, screen, x, y, 0);
			}
        }
#endif//WIN32
	}

#ifdef WIN32
	SendInput(1, (LPINPUT)&input, sizeof(INPUT));
#else//WIN32
    XFlush(display);
#endif//WIN32
}

void connectDS2Key()
{   //setup connections
    if(connectedDS2Key == 0)
    {
#ifdef WIN32
        unsigned long ioctlsocketCommand = 1;
        WSADATA wsaData;
        WSAStartup(0x0202, &wsaData);   //windows socket startup
#endif //WIN32

        //socket creation
        sd = socket(AF_INET, SOCK_DGRAM, 0);

        if(sd < 0)
        {
            fprintf(stderr, "Cannot open socket\n");
            return;
        }

        //bind local server port
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servAddr.sin_port = htons((unsigned short)serverPort);

#ifdef WIN32

        if(ioctlsocket(sd, FIONBIO, &ioctlsocketCommand) == -1)
#else //WIN32

        if(fcntl(sd, F_SETFL, O_NONBLOCK) == -1)
#endif //WIN32
        {
            fprintf(stderr, "Setting to non-blocking failed\n");
            return;
        }

        if(bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
        {
            fprintf(stderr, "Cannot bind port number %d\n", serverPort);
            return;
        }

        connectedDS2Key = 1;

#ifdef GUI
        //sprintf(mlDefaultTxt, "Waiting for data on port UDP %u", serverPort);
#else
        printf("Waiting for data on port UDP %u\n", serverPort);
#endif //GUI
    }
}   //setup connections

void disconnectDS2Key()
{
    if(connectedDS2Key == 1)
    {
#ifdef WIN32
        closesocket(sd);
        WSACleanup();
#else//WIN32
        close(sd);
#endif//WIN32
        connectedDS2Key = 0;
    }
}

void serverLoop()
{
	#ifndef GUI
	_sleep(10);
	#endif//GUI
    if(connectedDS2Key)
    {
        int n;
        #ifndef WIN32
        socklen_t cliLen;
        #else//WIN32
        int cliLen;
        #endif//WIN32
        struct sockaddr_in cliAddr;
        char msg[MAX_MSG];
        char *ip;
        unsigned int currentProfile;

        //init buffer
        memset(msg, 0x0, MAX_MSG);

        //receive message
        cliLen = sizeof(cliAddr);
        n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *)&cliAddr, &cliLen);
        ip = (char *)inet_ntoa(cliAddr.sin_addr);

        if(n < 0)
        {
            //_printf(lDefault, "Cannot receive data");
            return;
        }

        if(!strnicmp(msg, "/p", 2))
        {
            for(currentProfile = 0; currentProfile <= 255; currentProfile++)
            {
                if(sockaddr_in__address(cliAddr) == ((unsigned long *)&profile[currentProfile])[pIP])
                {
                    ((unsigned long *)&profile[currentProfile])[pIP] = 0;
                }
            }

            readProfileConfig(atoi(&msg[2]));

            _printf(lDefault, "%s: [%s] profile set to %s", ip, msg, &msg[2]);
            ((unsigned long *)&profile[atoi(&msg[2])])[pIP] = sockaddr_in__address(cliAddr);
        }
        else
        {
            bool noProfile = 0;
            for(currentProfile = 0; currentProfile <= 255; currentProfile++)
            {
                if(sockaddr_in__address(cliAddr) == ((unsigned long *)&profile[currentProfile])[pIP])
                {
                    break;
                }
                else if(currentProfile == 255)
                {
                    char *command = "/p?";

                    if(sendto(sd, command, strlen(command), 0, (struct sockaddr *)&cliAddr, sizeof(cliAddr)) >= 0)
                    {
                        _printf(lDefault, "Sent: %s", command);
                    }
                    else
                    {
                        _printf(lDefault, "Failed to send: %s", command);
                    }

                    noProfile = 1;

                    break;
                }
            }

            if(noProfile)
            {
            }
            else if(!stricmp(msg, "/dl0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pLeft], 0);
                _printf(lCommand, "%s: [%s] left button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/dl1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pLeft], 1);
                _printf(lCommand, "%s: [%s] left button released", ip, msg);
            }
            else if(!stricmp(msg, "/dr0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pRight], 0);
                _printf(lCommand, "%s: [%s] right button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/dr1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pRight], 1);
                _printf(lCommand, "%s: [%s] right button released", ip, msg);
            }
            else if(!stricmp(msg, "/du0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pUp], 0);
                _printf(lCommand, "%s: [%s] up button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/du1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pUp], 1);
                _printf(lCommand, "%s: [%s] up button released", ip, msg);
            }
            else if(!stricmp(msg, "/dd0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pDown], 0);
                _printf(lCommand, "%s: [%s] down button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/dd1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pDown], 1);
                _printf(lCommand, "%s: [%s] down button released", ip, msg);
            }
            else if(!stricmp(msg, "/ba0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pA], 0);
                _printf(lCommand, "%s: [%s] a button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/ba1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pA], 1);
                _printf(lCommand, "%s: [%s] a button released", ip, msg);
            }
            else if(!stricmp(msg, "/bb0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pB], 0);
                _printf(lCommand, "%s: [%s] b button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/bb1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pB], 1);
                _printf(lCommand, "%s: [%s] b button released", ip, msg);
            }
            else if(!stricmp(msg, "/bx0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pX], 0);
                _printf(lCommand, "%s: [%s] x button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/bx1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pX], 1);
                _printf(lCommand, "%s: [%s] x button released", ip, msg);
            }
            else if(!stricmp(msg, "/by0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pY], 0);
                _printf(lCommand, "%s: [%s] y button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/by1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pY], 1);
                _printf(lCommand, "%s: [%s] y button released", ip, msg);
            }
            else if(!stricmp(msg, "/bl0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pL], 0);
                _printf(lCommand, "%s: [%s] l button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/bl1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pL], 1);
                _printf(lCommand, "%s: [%s] l button released", ip, msg);
            }
            else if(!stricmp(msg, "/br0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pR], 0);
                _printf(lCommand, "%s: [%s] r button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/br1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pR], 1);
                _printf(lCommand, "%s: [%s] r button released", ip, msg);
            }
            else if(!stricmp(msg, "/bt0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pStart], 0);
                _printf(lCommand, "%s: [%s] start button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/bt1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pStart], 1);
                _printf(lCommand, "%s: [%s] start button released", ip, msg);
            }
            else if(!stricmp(msg, "/be0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pSelect], 0);
                _printf(lCommand, "%s: [%s] select button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/be1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pSelect], 1);
                _printf(lCommand, "%s: [%s] select button released", ip, msg);
            }
            else if(!stricmp(msg, "/gb0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pBlue], 0);
                _printf(lCommand, "%s: [%s] blue button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/gb1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pBlue], 1);
                _printf(lCommand, "%s: [%s] blue button released", ip, msg);
            }
            else if(!stricmp(msg, "/gy0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pYellow], 0);
                _printf(lCommand, "%s: [%s] yellow button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/gy1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pYellow], 1);
                _printf(lCommand, "%s: [%s] yellow button released", ip, msg);
            }
            else if(!stricmp(msg, "/gr0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pRed], 0);
                _printf(lCommand, "%s: [%s] red button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/gr1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pRed], 1);
                _printf(lCommand, "%s: [%s] red button released", ip, msg);
            }
            else if(!stricmp(msg, "/gg0"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pGreen], 0);
                _printf(lCommand, "%s: [%s] green button pressed", ip, msg);
            }
            else if(!stricmp(msg, "/gg1"))
            {
                doInput(INPUT_KEYBOARD, profile[currentProfile][pGreen], 1);
                _printf(lCommand, "%s: [%s] green button released", ip, msg);
            }
            else if(!strnicmp(msg, "/m", 2))
            {
                char *xc = &msg[2];
                char *yc = strchr(xc, '.');
                char *zc = strchr(&yc[1], '.');
                unsigned char x;
                unsigned char y;
                unsigned char buttonX;
                unsigned char buttonY;
                bool z;
                bool commandSent = 0;
                x = (unsigned char)atoi(xc);
                y = (unsigned char)atoi(&yc[1]);
                z = (bool)atoi(&zc[1]);
                buttonX = (x / 64);
                buttonY = (y / 64);

                if(buttonX == 0)
                {
                    if(buttonY == 0)
                    {
                        if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch0X0Y])))
                        {
                            mouseKeys[0] = 1; //doInput(INPUT_KEYBOARD, profile[currentProfile][pTouch0X0Y], 0);
                            commandSent = 1;
                        }
                    }
                    else if(buttonY == 1)
                    {
                        if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch0X1Y])))
                        {
                            mouseKeys[4] = 1;
                            commandSent = 1;
                        }
                    }
                    else if(buttonY == 2)
                    {
                        if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch0X2Y])))
                        {
                            mouseKeys[8] = 1;
                            commandSent = 1;
                        }
                    }
                }
                else if(buttonX == 1)
                {
                    if(buttonY == 0)
                    {
                        if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch1X0Y])))
                        {
                            mouseKeys[1] = 1;
                            commandSent = 1;
                        }
                    }
                    else if(buttonY == 1)
                    {
                        if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch1X1Y])))
                        {
                            mouseKeys[5] = 1;
                            commandSent = 1;
                        }
                    }
                    else if(buttonY == 2)
                    {
                        if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch1X2Y])))
                        {
                            mouseKeys[9] = 1;
                            commandSent = 1;
                        }
                    }
                }
                else if(buttonX == 2)
                {
                    if(buttonY == 0)
                    {
                        if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch2X0Y])))
                        {
                            mouseKeys[2] = 1;
                            commandSent = 1;
                        }
                    }
                    else if(buttonY == 1)
                    {
                        if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch2X1Y])))
                        {
                            mouseKeys[6] = 1;
                            commandSent = 1;
                        }
                    }
                    else if(buttonY == 2)
                    {
                        if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch2X2Y])))
                        {
                            mouseKeys[10] = 1;
                            commandSent = 1;
                        }
                    }
                }
                else if(buttonX == 3)
                {
                    if(buttonY == 0)
                    {
                        if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch3X0Y])))
                        {
                            mouseKeys[3] = 1;
                            commandSent = 1;
                        }
                    }
                    else if(buttonY == 1)
                    {
                        if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch3X1Y])))
                        {
                            mouseKeys[7] = 1;
                            commandSent = 1;
                        }
                    }
                    else if(buttonY == 2)
                    {
                        if(strcmp("KEY_NONE", getKeyString(profile[currentProfile][pTouch3X2Y])))
                        {
                            mouseKeys[11] = 1;
                            commandSent = 1;
                        }
                    }
                }

                if(commandSent == 0)
                {
					bool status = 0;
					if(mouseXLast != x || mouseYLast != y)
					{
						status = 1;
					}
					if(z && newTouch)
					{
						mouseXLast = x;
						mouseYLast = y;
						newTouch = 0;
					}
                    doInput(INPUT_MOUSE, (y << 8) | x, z);
					
					if(!z && !newTouch)
					{
						newTouch = 1;
					}
					if(status)
					{
		                if(z)
		                {
		                    _printf(lCommand, "%s: [%s] touch screen pressed at %i, %i", ip, msg, x, y);
		                }
		                else
		                {
		                    _printf(lCommand, "%s: [%s] touch screen released at %i, %i", ip, msg, x, y);
		                }
					}
                }
                else
                {
                    int key;
                    for(key = 0; key < 12; key++)
                    {
                        int status = 0;

                        if(mouseKeys[key] && !z)
                        {
                            mouseKeys[key] = 0;
                            doInput(INPUT_KEYBOARD, profile[currentProfile][pTouch + key], !mouseKeys[key]);
                            mouseKeysLast[key] = 0;
                            status = 1;
                        }
                        else if(mouseKeys[key] != mouseKeysLast[key])
                        {
                            doInput(INPUT_KEYBOARD, profile[currentProfile][pTouch + key], !mouseKeys[key]);
                            mouseKeysLast[key] = mouseKeys[key];
                            status = mouseKeys[key] + 1;
                        }

                        if(status == 1)
                        {
                            _printf(lCommand, "%s: [%s] released touch screen button %i", ip, msg, key);
                        }
                        else if(status == 2)
                        {
                            _printf(lCommand, "%s: [%s] pressed touch screen button %i", ip, msg, key);
                        }

                        mouseKeys[key] = 0;
                    }
                }
            }
            else
            {
                _printf(lMissingCommand, "%s: [%s] undefined command", ip, msg);
            }
			#undef _printf
			#define _printf real_printf
        }
    }
}

#ifdef WIN32
#ifdef GUI
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	hInst = hInstance;
#else //GUI
int main(int argc, char *argv[])
{
#endif //GUI
#else //WIN32
int main(int argc, char *argv[])
{
	display = XOpenDisplay(NULL);

	if(display == NULL)
	{
		fprintf(stderr, "Unable to connect to display: %s\n", XDisplayName(NULL));
		exit(1);
	}

#endif //WIN32

	printDebugLevel = lDefault;

	initKeyTable();

	memset(mouseKeys, 0, 12);
	memset(mouseKeysLast, 0, 12);

    readConfig();
    connectDS2Key();

	//infinite loop
	#ifdef GUI
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DLG_MAIN), NULL, DialogProc);
	#else
	while(1)
	{
		serverLoop();
	}

	#endif
	//end of infinite loop

	return 0;
}
