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
	if(!GUI::IsUpdated())
		GUI::DrawBackgroundImage(m_screen, m_rect, color);
	GUI::DrawFilledRect(m_screen, m_rect, color);
}
// TODO: This function still doesn't work how it should I don't think...
void Object::Clear()
{
	if(!GUI::IsUpdated())
	{
		if(GetColor() != UINT16_MAX)
			Clear(GetColor());
		else
			GUI::DrawBackgroundImage(m_screen, m_rect, Color[COLOR_BACKGROUND]);
	}
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
		//Draw(); // I think this line needs removed
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
// TODO: value should be the same as Status, you could update parts of the object by status
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
uint16_t Object::GetColor()
{
	return m_color;
}
void Object::SetColor(uint16_t color)
{
	m_color = color;
}

}} // namespace D2K::GUI
