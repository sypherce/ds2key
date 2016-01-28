#include <iostream>
#include "window.h"
#include "gui.h"
#include "common/udp.h"
#include "../core.h"

namespace D2K {namespace GUI {

Window::Window()
{
	Screen = 0;
	SetVisible(false);
}
Window::~Window()
{
	for(unsigned int i = 0; i < Objects.size(); i++)
	{
		delete Objects.at(i);
	}
	Objects.clear();
}
void Window::AppendObject(Object *object)
{
	Objects.push_back(object);
}
void Window::RemoveObject(Object *object)
{
	for(unsigned int i = 0; i < Objects.size(); i++)
	{
		if(Objects.at(i) == object)
		{
			Objects.erase(Objects.begin()+i);
			delete object;
		}
	}
}
//draws the window and it's children objects
void Window::Draw()
{
	if(GUI::IsUpdated())									//if gui was set to update
		GUI::ClearScreen(Screen, Color[colorBackground]);	//clear everything
	for(unsigned int i = 0; i < Objects.size(); i++)		//for each child
	{
		if(Objects.at(i)->Draw())							//draw if object AND/OR gui updated
			if(EMULATOR) std::cout << "button draw " << i << ": " << Objects.at(i)->GetText() << "\n";
	}
	GUI::SetUpdate(false);									//gui all updated
}
void Window::SetVisible(bool visible)
{
	Window::Visible = visible;								//window is now set
	for(unsigned int i = 0; i < Objects.size(); i++)		//for each child
	{
		Objects.at(i)->SetVisible(visible);					//set the same
	}
}
bool Window::IsVisible()
{
	return Visible;
}
bool Window::CheckClick(Object *object)
{
	if(object)
	{
		if(object->IsVisible())
		{
			if(keysDown()&KEY_TOUCH)
			{
				if(object->IsClicked((uint8_t)D2K::StylusPos.px, (uint8_t)D2K::StylusPos.py))
				{
					object->SetStatus(OBJECT_STATUS::PRESSED);
				}
			}
			else if(keysHeld()&KEY_TOUCH)
			{
				if(object->GetStatus() != OBJECT_STATUS::IDLE)
				{
					if(object->IsClicked((uint8_t)D2K::StylusPos.px, (uint8_t)D2K::StylusPos.py))
						object->SetStatus(OBJECT_STATUS::HELD);	//if we pressed and we're still hovering set to Held
					else
						object->SetStatus(OBJECT_STATUS::PRESSED);	//if we held but we're not hovering set to Pressed
				}
			}
			else if(keysUp()&KEY_TOUCH)
			{
				int status = object->GetStatus();			//save status value
				object->SetStatus(OBJECT_STATUS::IDLE);		//reset status
				if(object->IsClicked((uint8_t)D2K::StylusPos.px, (uint8_t)D2K::StylusPos.py))
				{
					if(status == OBJECT_STATUS::HELD)			//if we pressed and we're still hovering
					{
						return true;						//the object was clicked
					}
				}
			}
		}
	}

	return false;//the object wasn't clicked
}
bool Window::Update()
{
	Draw();
	for(unsigned int i = 0; i < Objects.size(); i++)
	{
		if(CheckClick(Objects.at(i)))
		{
			Objects.at(i)->Function();//execute the function
			return true;
		}
	}

	return false;
}

}}//namespace D2K::GUI
