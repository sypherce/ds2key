/*
Windows GUI Object base control
*/

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>//printf
#include "gui/gui.h"
#include "gui/resource.h"
#include "core/core.h"//Print

namespace D2K {
	namespace GUI {
		Object::Object(int x, int y, int width, int height) {
			X = x;
			Y = y;
			Width = width;
			Height = height;
			ID = -1;
		}
		Object::~Object() {}
		bool Object::Attach(Object *parentObject) {
			if(D2K::GUI::eventCounter >= eventMax)
				return false;
			ID = eventCounter++;
			parent = parentObject;
			return true;
		}
		int Object::GetID() {
			return ID;
		}
		HWND Object::GetParentHWND() {
			//printf("this:%x parent:%x\n", (unsigned int)this, (unsigned int)parent->hwnd);
			return parent->hwnd;
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
