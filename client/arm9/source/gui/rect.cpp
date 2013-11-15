#include <nds/arm9/video.h>	//SCREEN_WIDTH
#include <nds/dma.h>		//dmaFillHalfWords
#include <algorithm>		//std::min, std::max
#include "gui.h"

namespace D2K {
	namespace GUI {
		Rect::Rect() {
			Rect::X =
			Rect::Y =
			Rect::W =
			Rect::H = 0;
		}
		Rect::Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
			Rect::X = x;
			Rect::Y = y;
			Rect::W = w;//check if X + W > 255 ?
			Rect::H = h;//check if Y + H > 191 ?
		}
		uint8_t Rect::GetX() {
			return X;
		}
		void Rect::SetX(uint8_t x) {
			Rect::X = x;
		}
		uint8_t Rect::GetY() {
			return Y;
		}
		void Rect::SetY(uint8_t y) {
			Rect::Y = y;
		}
		uint8_t Rect::GetW() {
			return W;
		}
		void Rect::SetW(uint8_t w) {
			Rect::W = w;
		}
		uint8_t Rect::GetH() {
			return H;
		}
		void Rect::SetH(uint8_t h) {
			Rect::H = h;
		}
		uint16_t Rect::GetX2() {
			return X + W;
		}
		uint16_t Rect::GetY2() {
			return Y + H;
		}
		bool Rect::PointIntersect(uint8_t x, uint8_t y) {
			return (x >= std::min((uint16_t)GetX(), GetX2())
				 && x <= std::max((uint16_t)GetX(), GetX2())
				 && y >= std::min((uint16_t)GetY(), GetY2())
				 && y <= std::max((uint16_t)GetY(), GetY2()));
		}
		Rect::~Rect() { }
	}
}
