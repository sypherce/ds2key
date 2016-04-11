#pragma once

#include <stdint.h>

namespace D2K {namespace GUI {
	
class Rect {
public:
	Rect();
	Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
	~Rect();

	uint8_t GetX();
	uint8_t GetY();
	uint8_t GetW();
	uint8_t GetH();
	uint8_t GetX2();
	uint8_t GetY2();

	void SetX(uint8_t x);
	void SetY(uint8_t y);
	void SetW(uint8_t w);
	void SetH(uint8_t h);

	//@return true if (x), (y) Intersects, false if not
	bool PointIntersect(uint8_t x, uint8_t y);

private:
	uint8_t m_x;
	uint8_t m_y;
	uint8_t m_w;
	uint8_t m_h;
};

}}//namespace D2K::GUI
