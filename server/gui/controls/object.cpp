/*
Windows GUI Object base control
*/

#include <windows.h>
#include <commctrl.h>
#include <iostream>//std::cout, std::clog
#include "gui/gui.h"
#include "gui/resource.h"

namespace D2K
{
	namespace GUI
	{
		Object::Object(int x, int y, int width, int height)
		{
			X = x;
			Y = y;
			Width = width;
			Height = height;
			ID = -1;
			parent = NULL_VALUE;
			hwnd = NULL_VALUE;
		}
		Object::~Object() { }
		bool Object::Attach(Object *parentObject)
		{
			if(D2K::GUI::eventCounter >= eventMax)
			{
				std::clog << "Error (Object::Attach): eventCounter maxed out\n";
				return false;
			}
			if(parentObject == NULL_VALUE)
			{
				std::clog << "Error (Object::Attach): parentObject does not exist\n";
				return false;
			}
			ID = eventCounter++;
			parent = parentObject;
			return true;
		}

		int Object::GetID()
		{					
			return ID;
		}
		HWND Object::GetParentHWND()
		{
			if(GetParent() == NULL_VALUE)			//object not attached
			{
				std::clog << "Error (GetParentHWND): Object #" << GetID() << " not attached to parent\n";
				return NULL_VALUE;
			}
			else if(GetParent()->hwnd == NULL_VALUE)//parent not initialized
			{
				std::clog << "Error (GetParentHWND): Parent #" << GetID() << " not attached to parent\n";
				return NULL_VALUE;
			}

			return GetParent()->hwnd;
		}
		Object *Object::GetParent()
		{
			return parent;
		}
		void Object::SetVisible(bool visible)
		{
			if(hwnd)
				ShowWindow(hwnd, visible);
		}
	}
}
