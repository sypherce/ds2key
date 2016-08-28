#include "object.h"
#include "gui.h"

namespace D2K {namespace GUI {

Object::Object(uint8_t screen, GUI::Rect rect)
{
	m_screen = screen;
	m_rect = rect;
	m_status = OBJECT_STATUS::IDLE;
	SetVisible(false);
	Function = &GUI::VoidFunction;
}
Object::~Object()
{

}

bool Object::Draw()
{
	if(!Object::IsUpdated() && !GUI::IsUpdated())
		return false;

	Object::SetUpdate(false);
	return true;
}
void Object::SetText(std::string text)
{

}
std::string Object::GetText()
{
	return "";
}
void Object::Clear(uint16_t color)
{
	GUI::DrawBackgroundImage(m_screen, m_rect, color);
	GUI::DrawFilledRect(m_screen, m_rect, color);
}
void Object::Clear()
{
	GUI::DrawBackgroundImage(m_screen, m_rect, Color[COLOR_BACKGROUND]);
}
void Object::SetVisible(bool visible)
{
	GUI::SetUpdate(true);
	Object::SetUpdate(true);
	m_visible = visible;
}
void Object::SetStatus(uint8_t value)
{
	if(GetStatus() != value)
	{
		m_status = value;
		SetUpdate(true);
		Draw();///I think this line needs removed
	}
}
uint8_t Object::GetStatus()
{
	return m_status;
}
bool Object::IsVisible()
{
	return m_visible;
}
bool Object::IsClicked(uint16_t x, uint16_t y)
{
	return m_rect.PointIntersect(x, y);
}
void Object::SetUpdate(bool value)
{
	m_update = value;
}
bool Object::IsUpdated()
{
	return m_update;
}
bool Object::GetScreen()
{
	return m_screen;
}
D2K::GUI::Rect Object::GetRect()
{
	return m_rect;
}
void Object::SetRect(GUI::Rect rect)
{
	m_rect = rect;
}

}}//namespace D2K::GUI
