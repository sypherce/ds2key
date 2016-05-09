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
	if(GUI::IsUpdated())                                         //if gui was set to update
		GUI::ClearScreen(m_screen, Color[COLOR_BACKGROUND]); //clear everything
	for(unsigned int i = 0; i < m_objects.size(); i++)           //for each child
	{
		if(m_objects.at(i)->Draw()                           //draw if object AND/OR gui updated
		&& EMULATOR)
			std::cout << "button draw " << i << ": " << m_objects.at(i)->GetText() << "\n";
	}
	GUI::SetUpdate(false);                                       //gui all updated
}
void Window::SetVisible(bool visible)
{
	m_visible = visible;                                          //window is now set
	for(unsigned int i = 0; i < m_objects.size(); i++)           //for each child
	{
		m_objects.at(i)->SetVisible(visible);                //set the same
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
		uint16_t x = D2K::g_stylus_position.px,
			y = D2K::g_stylus_position.py;

		// clamp x value
		if(D2K::g_stylus_position.px >= MAX_X)
			x = MAX_X - 1;
		else if(D2K::g_stylus_position.px < 0)
			x = 0;
			
		// clamp y value
		if(D2K::g_stylus_position.py > MAX_Y)
			x = MAX_Y - 1;
		else if(D2K::g_stylus_position.py < 0)
			y = 0;

		if(g_keys_down&KEY_TOUCH
		&& object->IsClicked(x, y))
		{
			object->SetStatus(OBJECT_STATUS::PRESSED);
		}
		else if(g_keys_held&KEY_TOUCH
		&& object->GetStatus() != OBJECT_STATUS::IDLE)
		{
			if(object->IsClicked(x, y))
				object->SetStatus(OBJECT_STATUS::HELD);    //if we pressed and we're still hovering set to Held
			else
				object->SetStatus(OBJECT_STATUS::PRESSED); //if we held but we're not hovering set to Pressed
		}
		else if(g_keys_up&KEY_TOUCH)
		{
			int status = object->GetStatus();                  //save status value
			object->SetStatus(OBJECT_STATUS::IDLE);            //reset status
			if(object->IsClicked(x, y)
			&& status == OBJECT_STATUS::HELD)                  //if we pressed and we're still hovering
			{
				return true;                               //the object was clicked
			}
		}
	}

	return false; // the object wasn't clicked
}
bool Window::Update()
{
	Draw();
	for(unsigned int i = 0; i < m_objects.size(); i++)
	{
		if(CheckClick(m_objects.at(i)))
		{
			m_objects.at(i)->Function(); // execute the function
			return true;
		}
	}

	return false;
}

}}//namespace D2K::GUI
