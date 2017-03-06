#ifdef _NDS
#include <nds/arm9/video.h> // SCREEN_WIDTH
#include <nds/dma.h>        // dmaFillHalfWords
#endif
#include <iostream>
#include <algorithm> // std::min, std::max
#include "gui.h"

namespace D2K {namespace GUI {

#if defined(_NDS)
const float SCREEN_SCALE = 1.0;
#elif defined(_3DS)
//const float SCREEN_SCALE = 1.25;
const float SCREEN_SCALE = 1.0;
#endif

Rect::Rect()
{
	m_x =
	m_y =
	m_w =
	m_h = { };
}
Rect::Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	int x_2 = x + w;
	int y_2 = y + h;
	if(x_2 > SCREEN_WIDTH - 1)  // If x_2 is offscreen
		w = SCREEN_WIDTH - 1 - x;
	if(y_2 > SCREEN_HEIGHT - 1) // If y_2 is offscreen
		h = SCREEN_HEIGHT - 1 - y;
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
}
uint16_t Rect::GetX()
{
	return (uint16_t)(m_x * SCREEN_SCALE);
}
void Rect::SetX(uint16_t x)
{
	m_x = x;
}
uint16_t Rect::GetY()
{
	return (uint16_t)(m_y * SCREEN_SCALE);
}
void Rect::SetY(uint16_t y)
{
	m_y = y;
}
uint16_t Rect::GetW()
{
	return (uint16_t)(m_w * SCREEN_SCALE);
}
void Rect::SetW(uint16_t w)
{
	int tempW = m_x + w;
	if(tempW > SCREEN_WIDTH - 1) // If tempW is offscreen
		m_w = SCREEN_WIDTH - 1 - m_x;
	else
		m_w = w;
}
uint16_t Rect::GetH()
{
	return (uint16_t)(m_h * SCREEN_SCALE);
}
void Rect::SetH(uint16_t h)
{
	int tempH = m_y + h;
	if(tempH > SCREEN_HEIGHT - 1) // If tempH is offscreen
		m_h = SCREEN_HEIGHT - 1 - m_y;
	else
		m_h = h;
}
uint16_t Rect::GetX2()
{
	return GetX() + GetW();
}
uint16_t Rect::GetY2()
{
	return GetY() + GetH();
}
bool Rect::PointIntersect(uint16_t x, uint16_t y)
{
	return (   x >= std::min((uint16_t)GetX(), (uint16_t)GetX2())
		&& x <= std::max((uint16_t)GetX(), (uint16_t)GetX2())
		&& y >= std::min((uint16_t)GetY(), (uint16_t)GetY2())
		&& y <= std::max((uint16_t)GetY(), (uint16_t)GetY2()));
}
Rect::~Rect()
{

}

}} // namespace D2K::GUI
