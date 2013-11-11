#include <nds/arm9/video.h>	//SCREEN_WIDTH
#include <nds/dma.h>		//dmaFillHalfWords
#include <algorithm>		//std::min, std::max
#include "gui.h"

namespace D2K {
	namespace GUI {
		u16 *Screen[2];
		bool Update = false;
		uint16_t Color[colorMax];
		Rect::Rect() {
			Rect::X =
			Rect::Y =
			Rect::W =
			Rect::H = 0;
		}
		Rect::Rect(uint8_t X, uint8_t Y, uint8_t W, uint8_t H) {
			Rect::X = X;
			Rect::Y = Y;
			Rect::W = W;//check if X + W > 255 ?
			Rect::H = H;//check if Y + H > 191 ?
		}
		uint8_t Rect::getX() {
			return X;
		}
		void Rect::setX(uint8_t X) {
			Rect::X = X;
		}
		uint8_t Rect::getY() {
			return Y;
		}
		void Rect::setY(uint8_t Y) {
			Rect::Y = Y;
		}
		uint8_t Rect::getW() {
			return W;
		}
		void Rect::setW(uint8_t W) {
			Rect::W = W;
		}
		uint8_t Rect::getH() {
			return H;
		}
		void Rect::setH(uint8_t H) {
			Rect::H = H;
		}
		uint16_t Rect::getX2() {
			return X + W;
		}
		uint16_t Rect::getY2() {
			return Y + H;
		}
		bool Rect::PointIntersect(uint8_t X, uint8_t Y) {
			return (X >= std::min((uint16_t)getX(), getX2())
				 && X <= std::max((uint16_t)getX(), getX2())
				 && Y >= std::min((uint16_t)getY(), getY2())
				 && Y <= std::max((uint16_t)getY(), getY2()));
		}
		Rect::~Rect() { }

		void VoidFunction() { }

		void setUpdate(bool Value) {
			Update = Value;
		}
		bool isUpdated() {
			return Update;
		}

		void setPixel(uint8_t Screen, uint8_t X, uint8_t Y, uint16_t C) {
			//dmaFillHalfWords(C, &GUI::Screen[Screen][X + (Y * SCREEN_WIDTH)], 2);
			GUI::Screen[Screen][X + (Y * SCREEN_WIDTH)] = C;
		}
		void ClearScreen(uint8_t Screen, uint16_t C) {
			dmaFillHalfWords(C, GUI::Screen[Screen], 256*192*2);
			//dmaFillWords(C | (C << 16), GUI::Screen[Screen], 256*192*2);
			//GUI::Screen[Screen][X + (Y * SCREEN_WIDTH)] = C;
		}
		void DrawRect(uint8_t Screen, GUI::Rect Rect, uint16_t C) {
			for(int iy = Rect.getY(); iy <= Rect.getY2(); iy++) {
				setPixel(Screen, Rect.getX(), iy, C);
				setPixel(Screen, Rect.getX2(), iy, C);
			}

			dmaFillHalfWords(C, &GUI::Screen[Screen][Rect.getX() + (Rect.getY() * SCREEN_WIDTH)], Rect.getW()*2);
			dmaFillHalfWords(C, &GUI::Screen[Screen][Rect.getX() + (Rect.getY2() * SCREEN_WIDTH)], Rect.getW()*2);
			/*slow?
			for(int ix = Rect.getX(); ix <= Rect.getX2(); ix++) {
				setPixel(Screen, ix, Rect.getY(), C);
				setPixel(Screen, ix, Rect.getY2(), C);
			}*/
		}
		void DrawFilledRect(uint8_t Screen, GUI::Rect Rect, uint16_t C) {
			//for(int ix = Rect.getX(); ix <= Rect.getX2(); ix++)
				for(int iy = Rect.getY(); iy <= Rect.getY2(); iy++)
					dmaFillHalfWords(C, &GUI::Screen[Screen][Rect.getX() + (iy * SCREEN_WIDTH)], Rect.getW()*2);
					//setPixel(Screen,ix, iy, C);
		}
		void DrawLine(uint8_t Screen, std::string String, uint8_t X, uint8_t Y, uint16_t C) {
			if(String == "X")
				setPixel(Screen, X, Y, C);
			else if(String == "XX")
				dmaFillHalfWords(C, &GUI::Screen[Screen][X + (Y * SCREEN_WIDTH)], 2*2);
			else if(String == " XX")
				dmaFillHalfWords(C, &GUI::Screen[Screen][1 + X + (Y * SCREEN_WIDTH)], 2*2);
			else if(String == "  XX")
				dmaFillHalfWords(C, &GUI::Screen[Screen][2 + X + (Y * SCREEN_WIDTH)], 2*2);
			else if(String == "   XX")
				dmaFillHalfWords(C, &GUI::Screen[Screen][3 + X + (Y * SCREEN_WIDTH)], 2*2);
			else if(String == "XXX")
				dmaFillHalfWords(C, &GUI::Screen[Screen][X + (Y * SCREEN_WIDTH)], 3*2);
			else if(String == " XXX")
				dmaFillHalfWords(C, &GUI::Screen[Screen][1 + X + (Y * SCREEN_WIDTH)], 3*2);
			else if(String == "  XXX")
				dmaFillHalfWords(C, &GUI::Screen[Screen][2 + X + (Y * SCREEN_WIDTH)], 3*2);
			else if(String == "XXXX")
				dmaFillHalfWords(C, &GUI::Screen[Screen][X + (Y * SCREEN_WIDTH)], 4*2);
			else if(String == " XXXX")
				dmaFillHalfWords(C, &GUI::Screen[Screen][1 + X + (Y * SCREEN_WIDTH)], 4*2);
			else if(String == "XXXXX")
				dmaFillHalfWords(C, &GUI::Screen[Screen][X + (Y * SCREEN_WIDTH)], 5*2);
			else {
				for(unsigned int i = 0; i < String.length(); i++)
					if(String.at(i) != ' ')
						setPixel(Screen, X + i, Y, C);
			}
		}
		void DrawLetter(uint8_t Screen, char Letter, uint8_t X, uint8_t Y, uint16_t C) {
			switch(Letter) {
				case 'A':
				case 'a': {
					DrawLine(Screen, " XXX",	X, Y+0, C);
					DrawLine(Screen, "X   X",	X, Y+1, C);
					DrawLine(Screen, "XXXXX",	X, Y+2, C);
					DrawLine(Screen, "X   X",	X, Y+3, C);
					DrawLine(Screen, "X   X",	X, Y+4, C);
					break;
				}
				case 'B':
				case 'b': {
					DrawLine(Screen, "XXXX",	X, Y+0, C);
					DrawLine(Screen, "X   X",	X, Y+1, C);
					DrawLine(Screen, "XXXX",	X, Y+2, C);
					DrawLine(Screen, "X   X",	X, Y+3, C);
					DrawLine(Screen, "XXXX",  	X, Y+4, C);
					break;
				}
				case 'C':
				case 'c': {
					DrawLine(Screen, " XXX",	X, Y+0, C);
					DrawLine(Screen, "X   X",	X, Y+1, C);
					DrawLine(Screen, "X",		X, Y+2, C);
					DrawLine(Screen, "X   X",	X, Y+3, C);
					DrawLine(Screen, " XXX",	X, Y+4, C);
					break;
				}
				case 'D':
				case 'd': {
					DrawLine(Screen, "XXXX",	X, Y+0, C);
					DrawLine(Screen, "X   X",	X, Y+1, C);
					DrawLine(Screen, "X   X",	X, Y+2, C);
					DrawLine(Screen, "X   X",	X, Y+3, C);
					DrawLine(Screen, "XXXX",	X, Y+4, C);
					break;
				}
				case 'E':
				case 'e': {
					DrawLine(Screen, "XXXXX",	X, Y+0, C);
					DrawLine(Screen, "X",		X, Y+1, C);
					DrawLine(Screen, "XXXXX",	X, Y+2, C);
					DrawLine(Screen, "X",		X, Y+3, C);
					DrawLine(Screen, "XXXXX",	X, Y+4, C);
					break;
				}
				case 'F':
				case 'f': {
					DrawLine(Screen, "XXXXX",	X, Y+0, C);
					DrawLine(Screen, "X",		X, Y+1, C);
					DrawLine(Screen, "XXX",		X, Y+2, C);
					DrawLine(Screen, "X",		X, Y+3, C);
					DrawLine(Screen, "X",		X, Y+4, C);
					break;
				}
				case 'G':
				case 'g': {
					DrawLine(Screen, " XXXX",	X, Y+0, C);
					DrawLine(Screen, "X",		X, Y+1, C);
					DrawLine(Screen, "X  XX",	X, Y+2, C);
					DrawLine(Screen, "X   X",	X, Y+3, C);
					DrawLine(Screen, " XXX",	X, Y+4, C);
					break;
				}
				case 'H':
				case 'h': {
					DrawLine(Screen, "X   X",	X, Y+0, C);
					DrawLine(Screen, "X   X",	X, Y+1, C);
					DrawLine(Screen, "XXXXX",	X, Y+2, C);
					DrawLine(Screen, "X   X",	X, Y+3, C);
					DrawLine(Screen, "X   X",	X, Y+4, C);
					break;
				}
				case 'I':
				case 'i': {
					DrawLine(Screen, "XXXXX",	X, Y+0, C);
					DrawLine(Screen, "  X",		X, Y+1, C);
					DrawLine(Screen, "  X",		X, Y+2, C);
					DrawLine(Screen, "  X",		X, Y+3, C);
					DrawLine(Screen, "XXXXX",	X, Y+4, C);
					break;
				}
				case 'J':
				case 'j': {
					DrawLine(Screen, "    X",	X, Y+0, C);
					DrawLine(Screen, "    X",	X, Y+1, C);
					DrawLine(Screen, "    X",	X, Y+2, C);
					DrawLine(Screen, "X   X",	X, Y+3, C);
					DrawLine(Screen, " XXX",	X, Y+4, C);
					break;
				}
				case 'K':
				case 'k': {
					DrawLine(Screen, "X   X",	X, Y+0, C);
					DrawLine(Screen, "X  X",	X, Y+1, C);
					DrawLine(Screen, "XXX",		X, Y+2, C);
					DrawLine(Screen, "X  X",	X, Y+3, C);
					DrawLine(Screen, "X   X",	X, Y+4, C);
					break;
				}
				case 'L':
				case 'l': {
					DrawLine(Screen, "X",		X, Y+0, C);
					DrawLine(Screen, "X",		X, Y+1, C);
					DrawLine(Screen, "X",		X, Y+2, C);
					DrawLine(Screen, "X",		X, Y+3, C);
					DrawLine(Screen, "XXXXX",	X, Y+4, C);
					break;
				}
				case 'M':
				case 'm': {
					DrawLine(Screen, "X   X",	X, Y+0, C);
					DrawLine(Screen, "XX XX",	X, Y+1, C);
					DrawLine(Screen, "X X X",	X, Y+2, C);
					DrawLine(Screen, "X   X",	X, Y+3, C);
					DrawLine(Screen, "X   X",	X, Y+4, C);
					break;
				}
				case 'N':
				case 'n': {
					DrawLine(Screen, "X   X",	X, Y+0, C);
					DrawLine(Screen, "XX  X",	X, Y+1, C);
					DrawLine(Screen, "X X X",	X, Y+2, C);
					DrawLine(Screen, "X  XX",	X, Y+3, C);
					DrawLine(Screen, "X   X",	X, Y+4, C);
					break;
				}
				case 'O':
				case 'o': {
					DrawLine(Screen, " XXX",	X, Y+0, C);
					DrawLine(Screen, "X   X",	X, Y+1, C);
					DrawLine(Screen, "X   X",	X, Y+2, C);
					DrawLine(Screen, "X   X",	X, Y+3, C);
					DrawLine(Screen, " XXX",	X, Y+4, C);
					break;
				}
				case 'P':
				case 'p': {
					DrawLine(Screen, "XXXX",	X, Y+0, C);
					DrawLine(Screen, "X   X",	X, Y+1, C);
					DrawLine(Screen, "XXXX",	X, Y+2, C);
					DrawLine(Screen, "X",		X, Y+3, C);
					DrawLine(Screen, "X",		X, Y+4, C);
					break;
				}
				case 'Q':
				case 'q': {
					DrawLine(Screen, " XXX",	X, Y+0, C);
					DrawLine(Screen, "X   X",	X, Y+1, C);
					DrawLine(Screen, "X X X",	X, Y+2, C);
					DrawLine(Screen, "X  X",	X, Y+3, C);
					DrawLine(Screen, " XX X",	X, Y+4, C);
					break;
				}
				case 'R':
				case 'r': {
					DrawLine(Screen, "XXXX",	X, Y+0, C);
					DrawLine(Screen, "X   X",	X, Y+1, C);
					DrawLine(Screen, "XXXX",	X, Y+2, C);
					DrawLine(Screen, "X   X",	X, Y+3, C);
					DrawLine(Screen, "X   X",	X, Y+4, C);
					break;
				}
				case 'S':
				case 's': {
					DrawLine(Screen, " XXXX",	X, Y+0, C);
					DrawLine(Screen, "X",	X, Y+1, C);
					DrawLine(Screen, " XXX",	X, Y+2, C);
					DrawLine(Screen, "    X",	X, Y+3, C);
					DrawLine(Screen, "XXXX",	X, Y+4, C);
					break;
				}
				case 'T':
				case 't': {
					DrawLine(Screen, "XXXXX",	X, Y+0, C);
					DrawLine(Screen, "  X",		X, Y+1, C);
					DrawLine(Screen, "  X",		X, Y+2, C);
					DrawLine(Screen, "  X",		X, Y+3, C);
					DrawLine(Screen, "  X",		X, Y+4, C);
					break;
				}
				case 'U':
				case 'u': {
					DrawLine(Screen, "X   X",	X, Y+0, C);
					DrawLine(Screen, "X   X",		X, Y+1, C);
					DrawLine(Screen, "X   X",		X, Y+2, C);
					DrawLine(Screen, "X   X",		X, Y+3, C);
					DrawLine(Screen, " XXX",		X, Y+4, C);
					break;
				}
				case 'V':
				case 'v': {
					DrawLine(Screen, "X   X",	X, Y+0, C);
					DrawLine(Screen, "X   X",		X, Y+1, C);
					DrawLine(Screen, " X X",		X, Y+2, C);
					DrawLine(Screen, " X X",		X, Y+3, C);
					DrawLine(Screen, "  X",		X, Y+4, C);
					break;
				}
				case 'W':
				case 'w': {
					DrawLine(Screen, "X X X",	X, Y+0, C);
					DrawLine(Screen, "X X X",		X, Y+1, C);
					DrawLine(Screen, "X X X",		X, Y+2, C);
					DrawLine(Screen, "X X X",		X, Y+3, C);
					DrawLine(Screen, " X X",		X, Y+4, C);
					break;
				}
				case 'X':
				case 'x': {
					DrawLine(Screen, "X   X",		X, Y+0, C);
					DrawLine(Screen, " X X",		X, Y+1, C);
					DrawLine(Screen, "  X",			X, Y+2, C);
					DrawLine(Screen, " X X",		X, Y+3, C);
					DrawLine(Screen, "X   X",		X, Y+4, C);
					break;
				}
				case 'Y':
				case 'y': {
					DrawLine(Screen, "X   X",	X, Y+0, C);
					DrawLine(Screen, " X X",	X, Y+1, C);
					DrawLine(Screen, "  X",		X, Y+2, C);
					DrawLine(Screen, "  X",		X, Y+3, C);
					DrawLine(Screen, "  X",		X, Y+4, C);
					break;
				}
				case 'Z':
				case 'z': {
					DrawLine(Screen, "XXXXX",	X, Y+0, C);
					DrawLine(Screen, "   X",	X, Y+1, C);
					DrawLine(Screen, "  X",		X, Y+2, C);
					DrawLine(Screen, " X",		X, Y+3, C);
					DrawLine(Screen, "XXXXX",		X, Y+4, C);
					break;
				}
				case '0': {
					DrawLine(Screen, " XXX",	X, Y+0, C);
					DrawLine(Screen, "X  XX",	X, Y+1, C);
					DrawLine(Screen, "X X X",		X, Y+2, C);
					DrawLine(Screen, "XX  X",		X, Y+3, C);
					DrawLine(Screen, " XXX",		X, Y+4, C);
					break;
				}
				case '1': {
					DrawLine(Screen, "  X",		X, Y+0, C);
					DrawLine(Screen, " XX",		X, Y+1, C);
					DrawLine(Screen, "  X",		X, Y+2, C);
					DrawLine(Screen, "  X",		X, Y+3, C);
					DrawLine(Screen, "XXXXX",	X, Y+4, C);
					break;
				}
				case '2': {
					DrawLine(Screen, " XXX",		X, Y+0, C);
					DrawLine(Screen, "X   X",		X, Y+1, C);
					DrawLine(Screen, "  XX",		X, Y+2, C);
					DrawLine(Screen, " X",		X, Y+3, C);
					DrawLine(Screen, "XXXXX",	X, Y+4, C);
					break;
				}
				case '3': {
					DrawLine(Screen, " XXX",		X, Y+0, C);
					DrawLine(Screen, "X   X",		X, Y+1, C);
					DrawLine(Screen, "  XX",		X, Y+2, C);
					DrawLine(Screen, "X   X",		X, Y+3, C);
					DrawLine(Screen, " XXX",	X, Y+4, C);
					break;
				}
				case '4': {
					DrawLine(Screen, "X   X",		X, Y+0, C);
					DrawLine(Screen, "X   X",		X, Y+1, C);
					DrawLine(Screen, "XXXXX",		X, Y+2, C);
					DrawLine(Screen, "    X",		X, Y+3, C);
					DrawLine(Screen, "    X",	X, Y+4, C);
					break;
				}
				case '5': {
					DrawLine(Screen, "XXXXX",	X, Y+0, C);
					DrawLine(Screen, "X",	X, Y+1, C);
					DrawLine(Screen, "XXXX",	X, Y+2, C);
					DrawLine(Screen, "    X",	X, Y+3, C);
					DrawLine(Screen, "XXXX",	X, Y+4, C);
					break;
				}
				case '6': {
					DrawLine(Screen, " XXXX",	X, Y+0, C);
					DrawLine(Screen, "X",	X, Y+1, C);
					DrawLine(Screen, "XXXX",	X, Y+2, C);
					DrawLine(Screen, "X   X",	X, Y+3, C);
					DrawLine(Screen, " XXX",	X, Y+4, C);
					break;
				}
				case '7': {
					DrawLine(Screen, "XXXXX",	X, Y+0, C);
					DrawLine(Screen, "    X",	X, Y+1, C);
					DrawLine(Screen, "   X",	X, Y+2, C);
					DrawLine(Screen, "  X",	X, Y+3, C);
					DrawLine(Screen, "  X",	X, Y+4, C);
					break;
				}
				case '8': {
					DrawLine(Screen, " XXX",	X, Y+0, C);
					DrawLine(Screen, "X   X",	X, Y+1, C);
					DrawLine(Screen, " XXX",	X, Y+2, C);
					DrawLine(Screen, "X   X",	X, Y+3, C);
					DrawLine(Screen, " XXX",	X, Y+4, C);
					break;
				}
				case '9': {
					DrawLine(Screen, " XXX",	X, Y+0, C);
					DrawLine(Screen, "X   X",	X, Y+1, C);
					DrawLine(Screen, " XXXX",	X, Y+2, C);
					DrawLine(Screen, "    X",	X, Y+3, C);
					DrawLine(Screen, " XXX",	X, Y+4, C);
					break;
				}
				case '.': {
				  //DrawLine(Screen, ""	X, Y+0, C);
					DrawLine(Screen, "",	X, Y+1, C);
				  //DrawLine(Screen, ""	X, Y+2, C);
					DrawLine(Screen, "   X",	X, Y+3, C);
				  //DrawLine(Screen, ""	X, Y+4, C);
					break;
				}
				case ',': {
				  //DrawLine(Screen, ""	X, Y+0, C);
				  //DrawLine(Screen, ""	X, Y+1, C);
				  //DrawLine(Screen, ""	X, Y+2, C);
					DrawLine(Screen, "   X",	X, Y+3, C);
					DrawLine(Screen, "  X",	X, Y+4, C);
					break;
				}
				case '?': {
					DrawLine(Screen, " XXX",	X, Y+0, C);
					DrawLine(Screen, "X   X",	X, Y+1, C);
					DrawLine(Screen, "  XX",	X, Y+2, C);
				  //DrawLine(Screen, ""	X, Y+3, C);
					DrawLine(Screen, "  X",	X, Y+4, C);
					break;
				}
				case ':': {
				  //DrawLine(Screen, ""	X, Y+0, C);
					DrawLine(Screen, "  X",	X, Y+1, C);
				  //DrawLine(Screen, ""	X, Y+2, C);
					DrawLine(Screen, "  X",	X, Y+3, C);
				  //DrawLine(Screen, ""	X, Y+4, C);
					break;
				}
				case '<': {
					DrawLine(Screen, "  X",	X, Y+0, C);
					DrawLine(Screen, " X",	X, Y+1, C);
					DrawLine(Screen, "X",	X, Y+2, C);
					DrawLine(Screen, " X",	X, Y+3, C);
					DrawLine(Screen, "  X",	X, Y+4, C);
					break;
				}
			}
		}
		void DrawString(uint8_t Screen, std::string String, uint8_t x, uint8_t y, uint16_t c) {
			for(unsigned int i = 0; i < String.length(); i++) {
				DrawLetter(Screen, String.at(i), x + (i * 6), y, c);
			}
		}
	}
}
