#include <iostream>
#include "window.h"
#include "gui.h"
#include "common/udp.h"
#include "../core.h"

namespace D2K {namespace GUI {

Window::Window()
{
	m_screen = 0;
	SetVisible(false);
}
Window::~Window()
{
	for(unsigned int i = 0; i < m_objects.size(); i++)
	{
		delete m_objects.at(i);
	}
	m_objects.clear();
}
void Window::AppendObject(Object* object)
{
	m_objects.push_back(object);
}
void Window::RemoveObject(Object* object)
{
	for(unsigned int i = 0; i < m_objects.size(); i++)
	{
		if(m_objects.at(i) == object)
		{
			m_objects.erase(m_objects.begin()+i);
			delete object;
		}
	}
}
//draws the window and it's children objects
void Window::Draw()
{
	if(GUI::IsUpdated())						//if gui was set to update
		GUI::ClearScreen(m_screen, Color[COLOR_BACKGROUND]);	//clear everything
	for(unsigned int i = 0; i < m_objects.size(); i++)		//for each child
	{
		if(m_objects.at(i)->Draw()
		&& EMULATOR)						//draw if object AND/OR gui updated
			std::cout << "button draw " << i << ": " << m_objects.at(i)->GetText() << "\n";
	}
	GUI::SetUpdate(false);						//gui all updated
}
void Window::SetVisible(bool visible)
{
	m_visible = visible;						//window is now set
	for(unsigned int i = 0; i < m_objects.size(); i++)		//for each child
	{
		m_objects.at(i)->SetVisible(visible);			//set the same
	}
}
bool Window::IsVisible()
{
	return m_visible;
}
bool Window::CheckClick(Object* object)
{
	if(object != nullptr
	&& object->IsVisible())
	{
		if(keysDown()&KEY_TOUCH
		&& object->IsClicked((uint8_t)D2K::g_stylus_position.px, (uint8_t)D2K::g_stylus_position.py))
		{
			object->SetStatus(OBJECT_STATUS::PRESSED);
		}
		else if(keysHeld()&KEY_TOUCH
		&& object->GetStatus() != OBJECT_STATUS::IDLE)
		{
			if(object->IsClicked((uint8_t)D2K::g_stylus_position.px, (uint8_t)D2K::g_stylus_position.py))
				object->SetStatus(OBJECT_STATUS::HELD);		//if we pressed and we're still hovering set to Held
			else
				object->SetStatus(OBJECT_STATUS::PRESSED);	//if we held but we're not hovering set to Pressed
		}
		else if(keysUp()&KEY_TOUCH)
		{
			int status = object->GetStatus();			//save status value
			object->SetStatus(OBJECT_STATUS::IDLE);			//reset status
			if(object->IsClicked((uint8_t)D2K::g_stylus_position.px, (uint8_t)D2K::g_stylus_position.py)
			&& status == OBJECT_STATUS::HELD)			//if we pressed and we're still hovering
			{
				return true;					//the object was clicked
			}
		}
	}

	return false;//the object wasn't clicked
}
bool Window::Update()
{
	Draw();
	for(unsigned int i = 0; i < m_objects.size(); i++)
	{
		if(CheckClick(m_objects.at(i)))
		{
			m_objects.at(i)->Function();//execute the function
			return true;
		}
	}

	return false;
}

}}//namespace D2K::GUI
