/*
Windows GUI Object base control
*/

#include <windows.h>
#include <commctrl.h>
#include <iostream>//std::cout, std::clog
#include "gui/gui.h"
#include "gui/resource.h"

namespace D2K {
	namespace GUI {
		Object::Object(int x, int y, int width, int height) {
			X = x;
			Y = y;
			Width = width;
			Height = height;
			ID = -1;
			parent = NULL;
			hwnd = NULL;
		}
		Object::~Object() {}
		bool Object::Attach(Object *parentObject) {
			if(D2K::GUI::eventCounter >= eventMax) {
				std::clog << "Error (Object::Attach): eventCounter maxed out\n";
				return false;
			}
			if(parentObject == NULL) {
				std::clog << "Error (Object::Attach): parentObject does not exist\n";
				return NULL;
			}
			ID = eventCounter++;
			parent = parentObject;
			return true;
		}

		int Object::GetID() {					
			return ID;
		}
		HWND Object::GetParentHWND() {
			if(GetParent() == NULL) {			//object not attached
				std::clog << "Error (GetParentHWND): Object #" << GetID() << " not attached to parent\n";
				return NULL;
			}
			else if(GetParent()->hwnd == NULL) {//parent not initialized
				std::clog << "Error (GetParentHWND): Parent #" << GetID() << " not attached to parent\n";
				return NULL;
			}

			return GetParent()->hwnd;
		}
		Object *Object::GetParent() {
			return parent;
		}
		void Object::SetVisible(bool visible) {
			if(hwnd)
				ShowWindow(hwnd, visible);
		}
	}
}
