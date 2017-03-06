#include <string> // std::string
#include "canvas.h"
#include "gui.h"

namespace D2K {namespace GUI {

extern const uint8_t SCREEN_BYTES;

Canvas::Canvas(uint8_t screen, GUI::Rect rect) : Object(screen, rect)
{
}

Canvas::~Canvas()
{
	if(m_buffer)
		free(m_buffer);
}

bool Canvas::Draw()
{
	if(!Object::Draw())
		return false;

	if(IsVisible())
	{
		//Clear();

		if(!m_buffer)
			return false;

		for(int x = 0; x < GetRect().GetW(); x++)
		{
			for(int y = 0; y < GetRect().GetH(); y++)
			{
				int buffer_memory_position = x + (y * GetRect().GetW());

				// ignore pure black for transparency
				if(m_buffer[buffer_memory_position] != 0x0000)
					SetPixel(GetScreen(), x + GetRect().GetX(), y + GetRect().GetY(), m_buffer[buffer_memory_position]);
			}
		}
	}

	return true;
}

bool Canvas::IsClicked(uint16_t x, uint16_t y)
{
	return false;
}

const uint16_t* Canvas::GetBuffer()
{
	return m_buffer;
}

bool Canvas::SetBuffer(const uint16_t* buffer)
{
	int buffer_size = GetRect().GetH() * GetRect().GetW() * sizeof(uint16_t);
	m_buffer = (uint16_t*)malloc(buffer_size);
	if(!m_buffer)
		return false;
	memcpy(m_buffer, buffer, buffer_size);
	SetUpdate(true);

	return true;
}

}} // namespace D2K::GUI
