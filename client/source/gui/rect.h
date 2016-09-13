#pragma once

#include <stdint.h>

namespace D2K {namespace GUI {
	
class Rect {
public:
	Rect();
	Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
	~Rect();

	uint16_t GetX();
	uint16_t GetY();
	uint16_t GetW();
	uint16_t GetH();
	uint16_t GetX2();
	uint16_t GetY2();

	void SetX(uint16_t x);
	void SetY(uint16_t y);
	void SetW(uint16_t w);
	void SetH(uint16_t h);

	// @return true if (x), (y) Intersects, false if not
	bool PointIntersect(uint16_t x, uint16_t y);

private:
	uint16_t m_x;
	uint16_t m_y;
	uint16_t m_w;
	uint16_t m_h;
};

}} // namespace D2K::GUI
