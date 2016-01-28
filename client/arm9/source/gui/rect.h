#pragma once

#include <stdint.h>

namespace D2K {namespace GUI {class Rect {

public:
	Rect();
	Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
	uint8_t GetX();
	void SetX(uint8_t x);
	uint8_t GetY();
	void SetY(uint8_t y);
	uint8_t GetW();
	void SetW(uint8_t w);
	uint8_t GetH();
	void SetH(uint8_t h);
	uint8_t GetX2();
	uint8_t GetY2();
	//@return true if (x), (y) Intersects, false if not
	bool PointIntersect(uint8_t x, uint8_t y);
	~Rect();

private:
	uint8_t X;
	uint8_t Y;
	uint8_t W;
	uint8_t H;
};

}}//namespace D2K::GUI::Rect
