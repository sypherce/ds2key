#include <string> // std::string
#include "frame.h"
#include "gui.h"

namespace D2K {namespace GUI {

Frame::Frame(uint8_t screen, GUI::Rect rect, uint16_t color) : Object(screen, rect)
{
	SetColor(color);
}

Frame::~Frame()
{
}

bool Frame::Draw()
{
	if(!Object::Draw())
		return false;

	if(IsVisible())
		Clear(GetColor());

	return true;
}

bool Frame::IsClicked(uint16_t x, uint16_t y)
{
	return false;
}

}} // namespace D2K::GUI
