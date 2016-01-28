#include <nds/arm9/video.h>	//SCREEN_WIDTH
#include <nds/dma.h>		//dmaFillHalfWords
#include <algorithm>
//std::min, std::max
#include "gui.h"

namespace D2K {namespace GUI {

Rect::Rect()
{
	Rect::X =
	Rect::Y =
	Rect::W =
	Rect::H = 0;
}
Rect::Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	int X2 = x + w;
	int Y2 = y + h;
	if(X2 >= SCREEN_WIDTH - 1)		//if X2 is offscreen
		w = SCREEN_WIDTH - 1 - x;
	if(Y2 >= SCREEN_HEIGHT - 1)		//if Y2 is offscreen
		h = SCREEN_HEIGHT - 1 - y;
	Rect::X = x;
	Rect::Y = y;
	Rect::W = w;
	Rect::H = h;
}
uint8_t Rect::GetX()
{
	return X;
}
void Rect::SetX(uint8_t x)
{
	Rect::X = x;
}
uint8_t Rect::GetY()
{
	return Y;
}
void Rect::SetY(uint8_t y)
{
	Rect::Y = y;
}
uint8_t Rect::GetW()
{
	return W;
}
void Rect::SetW(uint8_t w)
{
	int tempW = GetX() + w;
	if(tempW >= SCREEN_WIDTH - 1)//if tempW is offscreen
		Rect::W = SCREEN_WIDTH - 1 - GetX();
	else
		Rect::W = w;
}
uint8_t Rect::GetH()
{
	return H;
}
void Rect::SetH(uint8_t h)
{
	int tempH = GetY() + h;
	if(tempH >= SCREEN_HEIGHT - 1)//if tempH is offscreen
		Rect::H = SCREEN_HEIGHT - 1 - GetY();
	else
		Rect::H = h;
}
uint8_t Rect::GetX2()
{
	return X + W;
}
uint8_t Rect::GetY2()
{
	return Y + H;
}
bool Rect::PointIntersect(uint8_t x, uint8_t y)
{
	return (x >= std::min((uint16_t)GetX(), (uint16_t)GetX2())
			&& x <= std::max((uint16_t)GetX(), (uint16_t)GetX2())
			&& y >= std::min((uint16_t)GetY(), (uint16_t)GetY2())
			&& y <= std::max((uint16_t)GetY(), (uint16_t)GetY2()));
}
Rect::~Rect() { }

}}//namespace D2K::GUI
