#include <string> // std::string
#include "canvas.h"
#include "gui.h"

namespace D2K {namespace GUI {

extern const uint8_t SCREEN_BYTES;

Canvas::Canvas(uint8_t screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h) : Object(screen, GUI::Rect(x, y, w, h))
{
	m_w = w;
	m_h = h;
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

		for(int x = 0; x < m_w; x++)
		{
			for(int y = 0; y < m_h; y++)
			{
				int buffer_memory_position = x + (y * m_w);

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
