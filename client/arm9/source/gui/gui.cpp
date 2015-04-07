#include <nds/arm9/video.h>	//SCREEN_WIDTH
#include <nds/dma.h>		//dmaFillHalfWords
#include <algorithm>		//std::min, std::max
#include "gui.h"

namespace D2K {
	namespace GUI {
		u16 *Screen[2];
		bool Update = false;
		uint16_t Color[colorMax];
		void VoidFunction() { }

		void SetUpdate(bool value) {
			Update = value;
		}
		bool IsUpdated() {
			return Update;
		}
		void DrawFastHorizontleLine(uint8_t screen, uint8_t x, uint8_t y, uint8_t w, uint16_t c) {
			if(x >= SCREEN_WIDTH)
				return;
			if(y >= SCREEN_HEIGHT)
				return;
			if(x + w >= SCREEN_WIDTH)
				w = SCREEN_WIDTH - x;
			if(w == 0)
				return;
			dmaFillHalfWords(c, &GUI::Screen[screen][x + (y * SCREEN_WIDTH)], w * 2);
		}

		void SetPixel(uint8_t screen, uint8_t x, uint8_t y, uint16_t c) {
			if(y < SCREEN_HEIGHT)//if we're drawing on screen
				GUI::Screen[screen][x + (y * SCREEN_WIDTH)] = c;
		}
		void ClearScreen(uint8_t screen, uint16_t c) {
			dmaFillHalfWords(c, GUI::Screen[screen], SCREEN_WIDTH * SCREEN_HEIGHT * 2);
		}
		void DrawRect(uint8_t screen, GUI::Rect rect, uint16_t c) {
			for(int y = rect.GetY(); y <= rect.GetY2(); y++) {
				SetPixel(screen, rect.GetX(), y, c);
				SetPixel(screen, rect.GetX2(), y, c);
			}

			DrawFastHorizontleLine(screen, rect.GetX(), rect.GetY(), rect.GetW(), c);
			DrawFastHorizontleLine(screen, rect.GetX(), rect.GetY2(), rect.GetW(), c);
		}
		void DrawFilledRect(uint8_t screen, GUI::Rect rect, uint16_t c) {
			for(int y = rect.GetY(); y <= rect.GetY2(); y++)
				DrawFastHorizontleLine(screen, rect.GetX(), y, rect.GetW(), c);
		}
		void DrawLine(uint8_t screen, std::string text, uint8_t x, uint8_t y, uint16_t c) {
			if(text == "X")
				SetPixel(screen, x, y, c);
			else if(text == "XX")
				DrawFastHorizontleLine(screen, x, y, 2, c);
			else if(text == " XX")
				DrawFastHorizontleLine(screen, 1 + x, y, 2, c);
			else if(text == "  XX")
				DrawFastHorizontleLine(screen, 2 + x, y, 2, c);
			else if(text == "   XX")
				DrawFastHorizontleLine(screen, 3 + x, y, 2, c);
			else if(text == "XXX")
				DrawFastHorizontleLine(screen, x, y, 3, c);
			else if(text == " XXX")
				DrawFastHorizontleLine(screen, 1 + x, y, 3, c);
			else if(text == "  XXX")
				DrawFastHorizontleLine(screen, 2 + x, y, 3, c);
			else if(text == "XXXX")
				DrawFastHorizontleLine(screen, x, y, 4, c);
			else if(text == " XXXX")
				DrawFastHorizontleLine(screen, 1 + x, y, 4, c);
			else if(text == "XXXXX")
				DrawFastHorizontleLine(screen, x, y, 5, c);
			else {
				for(unsigned int i = 0; i < text.length(); i++)
					if(text.at(i) != ' ')
						SetPixel(screen, x + i, y, c);
			}
		}
		void DrawLetter(uint8_t screen, char letter, uint8_t x, uint8_t y, uint16_t c) {
			switch(letter) {
				case 'A':
				case 'a': {
					DrawLine(screen, " XXX",	x, y+0, c);
					DrawLine(screen, "X   X",	x, y+1, c);
					DrawLine(screen, "XXXXX",	x, y+2, c);
					DrawLine(screen, "X   X",	x, y+3, c);
					DrawLine(screen, "X   X",	x, y+4, c);
					break;
				}
				case 'B':
				case 'b': {
					DrawLine(screen, "XXXX",	x, y+0, c);
					DrawLine(screen, "X   X",	x, y+1, c);
					DrawLine(screen, "XXXX",	x, y+2, c);
					DrawLine(screen, "X   X",	x, y+3, c);
					DrawLine(screen, "XXXX",  	x, y+4, c);
					break;
				}
				case 'C':
				case 'c': {
					DrawLine(screen, " XXX",	x, y+0, c);
					DrawLine(screen, "X   X",	x, y+1, c);
					DrawLine(screen, "X",		x, y+2, c);
					DrawLine(screen, "X   X",	x, y+3, c);
					DrawLine(screen, " XXX",	x, y+4, c);
					break;
				}
				case 'D':
				case 'd': {
					DrawLine(screen, "XXXX",	x, y+0, c);
					DrawLine(screen, "X   X",	x, y+1, c);
					DrawLine(screen, "X   X",	x, y+2, c);
					DrawLine(screen, "X   X",	x, y+3, c);
					DrawLine(screen, "XXXX",	x, y+4, c);
					break;
				}
				case 'E':
				case 'e': {
					DrawLine(screen, "XXXXX",	x, y+0, c);
					DrawLine(screen, "X",		x, y+1, c);
					DrawLine(screen, "XXXXX",	x, y+2, c);
					DrawLine(screen, "X",		x, y+3, c);
					DrawLine(screen, "XXXXX",	x, y+4, c);
					break;
				}
				case 'F':
				case 'f': {
					DrawLine(screen, "XXXXX",	x, y+0, c);
					DrawLine(screen, "X",		x, y+1, c);
					DrawLine(screen, "XXX",		x, y+2, c);
					DrawLine(screen, "X",		x, y+3, c);
					DrawLine(screen, "X",		x, y+4, c);
					break;
				}
				case 'G':
				case 'g': {
					DrawLine(screen, " XXXX",	x, y+0, c);
					DrawLine(screen, "X",		x, y+1, c);
					DrawLine(screen, "X  XX",	x, y+2, c);
					DrawLine(screen, "X   X",	x, y+3, c);
					DrawLine(screen, " XXX",	x, y+4, c);
					break;
				}
				case 'H':
				case 'h': {
					DrawLine(screen, "X   X",	x, y+0, c);
					DrawLine(screen, "X   X",	x, y+1, c);
					DrawLine(screen, "XXXXX",	x, y+2, c);
					DrawLine(screen, "X   X",	x, y+3, c);
					DrawLine(screen, "X   X",	x, y+4, c);
					break;
				}
				case 'I':
				case 'i': {
					DrawLine(screen, "XXXXX",	x, y+0, c);
					DrawLine(screen, "  X",		x, y+1, c);
					DrawLine(screen, "  X",		x, y+2, c);
					DrawLine(screen, "  X",		x, y+3, c);
					DrawLine(screen, "XXXXX",	x, y+4, c);
					break;
				}
				case 'J':
				case 'j': {
					DrawLine(screen, "    X",	x, y+0, c);
					DrawLine(screen, "    X",	x, y+1, c);
					DrawLine(screen, "    X",	x, y+2, c);
					DrawLine(screen, "X   X",	x, y+3, c);
					DrawLine(screen, " XXX",	x, y+4, c);
					break;
				}
				case 'K':
				case 'k': {
					DrawLine(screen, "X   X",	x, y+0, c);
					DrawLine(screen, "X  X",	x, y+1, c);
					DrawLine(screen, "XXX",		x, y+2, c);
					DrawLine(screen, "X  X",	x, y+3, c);
					DrawLine(screen, "X   X",	x, y+4, c);
					break;
				}
				case 'L':
				case 'l': {
					DrawLine(screen, "X",		x, y+0, c);
					DrawLine(screen, "X",		x, y+1, c);
					DrawLine(screen, "X",		x, y+2, c);
					DrawLine(screen, "X",		x, y+3, c);
					DrawLine(screen, "XXXXX",	x, y+4, c);
					break;
				}
				case 'M':
				case 'm': {
					DrawLine(screen, "X   X",	x, y+0, c);
					DrawLine(screen, "XX XX",	x, y+1, c);
					DrawLine(screen, "X X X",	x, y+2, c);
					DrawLine(screen, "X   X",	x, y+3, c);
					DrawLine(screen, "X   X",	x, y+4, c);
					break;
				}
				case 'N':
				case 'n': {
					DrawLine(screen, "X   X",	x, y+0, c);
					DrawLine(screen, "XX  X",	x, y+1, c);
					DrawLine(screen, "X X X",	x, y+2, c);
					DrawLine(screen, "X  XX",	x, y+3, c);
					DrawLine(screen, "X   X",	x, y+4, c);
					break;
				}
				case 'O':
				case 'o': {
					DrawLine(screen, " XXX",	x, y+0, c);
					DrawLine(screen, "X   X",	x, y+1, c);
					DrawLine(screen, "X   X",	x, y+2, c);
					DrawLine(screen, "X   X",	x, y+3, c);
					DrawLine(screen, " XXX",	x, y+4, c);
					break;
				}
				case 'P':
				case 'p': {
					DrawLine(screen, "XXXX",	x, y+0, c);
					DrawLine(screen, "X   X",	x, y+1, c);
					DrawLine(screen, "XXXX",	x, y+2, c);
					DrawLine(screen, "X",		x, y+3, c);
					DrawLine(screen, "X",		x, y+4, c);
					break;
				}
				case 'Q':
				case 'q': {
					DrawLine(screen, " XXX",	x, y+0, c);
					DrawLine(screen, "X   X",	x, y+1, c);
					DrawLine(screen, "X X X",	x, y+2, c);
					DrawLine(screen, "X  X",	x, y+3, c);
					DrawLine(screen, " XX X",	x, y+4, c);
					break;
				}
				case 'R':
				case 'r': {
					DrawLine(screen, "XXXX",	x, y+0, c);
					DrawLine(screen, "X   X",	x, y+1, c);
					DrawLine(screen, "XXXX",	x, y+2, c);
					DrawLine(screen, "X   X",	x, y+3, c);
					DrawLine(screen, "X   X",	x, y+4, c);
					break;
				}
				case 'S':
				case 's': {
					DrawLine(screen, " XXXX",	x, y+0, c);
					DrawLine(screen, "X",	x, y+1, c);
					DrawLine(screen, " XXX",	x, y+2, c);
					DrawLine(screen, "    X",	x, y+3, c);
					DrawLine(screen, "XXXX",	x, y+4, c);
					break;
				}
				case 'T':
				case 't': {
					DrawLine(screen, "XXXXX",	x, y+0, c);
					DrawLine(screen, "  X",		x, y+1, c);
					DrawLine(screen, "  X",		x, y+2, c);
					DrawLine(screen, "  X",		x, y+3, c);
					DrawLine(screen, "  X",		x, y+4, c);
					break;
				}
				case 'U':
				case 'u': {
					DrawLine(screen, "X   X",	x, y+0, c);
					DrawLine(screen, "X   X",		x, y+1, c);
					DrawLine(screen, "X   X",		x, y+2, c);
					DrawLine(screen, "X   X",		x, y+3, c);
					DrawLine(screen, " XXX",		x, y+4, c);
					break;
				}
				case 'V':
				case 'v': {
					DrawLine(screen, "X   X",	x, y+0, c);
					DrawLine(screen, "X   X",		x, y+1, c);
					DrawLine(screen, " X X",		x, y+2, c);
					DrawLine(screen, " X X",		x, y+3, c);
					DrawLine(screen, "  X",		x, y+4, c);
					break;
				}
				case 'W':
				case 'w': {
					DrawLine(screen, "X X X",	x, y+0, c);
					DrawLine(screen, "X X X",		x, y+1, c);
					DrawLine(screen, "X X X",		x, y+2, c);
					DrawLine(screen, "X X X",		x, y+3, c);
					DrawLine(screen, " X X",		x, y+4, c);
					break;
				}
				case 'X':
				case 'x': {
					DrawLine(screen, "X   X",		x, y+0, c);
					DrawLine(screen, " X X",		x, y+1, c);
					DrawLine(screen, "  X",			x, y+2, c);
					DrawLine(screen, " X X",		x, y+3, c);
					DrawLine(screen, "X   X",		x, y+4, c);
					break;
				}
				case 'Y':
				case 'y': {
					DrawLine(screen, "X   X",	x, y+0, c);
					DrawLine(screen, " X X",	x, y+1, c);
					DrawLine(screen, "  X",		x, y+2, c);
					DrawLine(screen, "  X",		x, y+3, c);
					DrawLine(screen, "  X",		x, y+4, c);
					break;
				}
				case 'Z':
				case 'z': {
					DrawLine(screen, "XXXXX",	x, y+0, c);
					DrawLine(screen, "   X",	x, y+1, c);
					DrawLine(screen, "  X",		x, y+2, c);
					DrawLine(screen, " X",		x, y+3, c);
					DrawLine(screen, "XXXXX",		x, y+4, c);
					break;
				}
				case '0': {
					DrawLine(screen, " XXX",	x, y+0, c);
					DrawLine(screen, "X  XX",	x, y+1, c);
					DrawLine(screen, "X X X",		x, y+2, c);
					DrawLine(screen, "XX  X",		x, y+3, c);
					DrawLine(screen, " XXX",		x, y+4, c);
					break;
				}
				case '1': {
					DrawLine(screen, "  X",		x, y+0, c);
					DrawLine(screen, " XX",		x, y+1, c);
					DrawLine(screen, "  X",		x, y+2, c);
					DrawLine(screen, "  X",		x, y+3, c);
					DrawLine(screen, "XXXXX",	x, y+4, c);
					break;
				}
				case '2': {
					DrawLine(screen, " XXX",		x, y+0, c);
					DrawLine(screen, "X   X",		x, y+1, c);
					DrawLine(screen, "  XX",		x, y+2, c);
					DrawLine(screen, " X",		x, y+3, c);
					DrawLine(screen, "XXXXX",	x, y+4, c);
					break;
				}
				case '3': {
					DrawLine(screen, " XXX",		x, y+0, c);
					DrawLine(screen, "X   X",		x, y+1, c);
					DrawLine(screen, "  XX",		x, y+2, c);
					DrawLine(screen, "X   X",		x, y+3, c);
					DrawLine(screen, " XXX",	x, y+4, c);
					break;
				}
				case '4': {
					DrawLine(screen, "X   X",		x, y+0, c);
					DrawLine(screen, "X   X",		x, y+1, c);
					DrawLine(screen, "XXXXX",		x, y+2, c);
					DrawLine(screen, "    X",		x, y+3, c);
					DrawLine(screen, "    X",	x, y+4, c);
					break;
				}
				case '5': {
					DrawLine(screen, "XXXXX",	x, y+0, c);
					DrawLine(screen, "X",	x, y+1, c);
					DrawLine(screen, "XXXX",	x, y+2, c);
					DrawLine(screen, "    X",	x, y+3, c);
					DrawLine(screen, "XXXX",	x, y+4, c);
					break;
				}
				case '6': {
					DrawLine(screen, " XXXX",	x, y+0, c);
					DrawLine(screen, "X",	x, y+1, c);
					DrawLine(screen, "XXXX",	x, y+2, c);
					DrawLine(screen, "X   X",	x, y+3, c);
					DrawLine(screen, " XXX",	x, y+4, c);
					break;
				}
				case '7': {
					DrawLine(screen, "XXXXX",	x, y+0, c);
					DrawLine(screen, "    X",	x, y+1, c);
					DrawLine(screen, "   X",	x, y+2, c);
					DrawLine(screen, "  X",	x, y+3, c);
					DrawLine(screen, "  X",	x, y+4, c);
					break;
				}
				case '8': {
					DrawLine(screen, " XXX",	x, y+0, c);
					DrawLine(screen, "X   X",	x, y+1, c);
					DrawLine(screen, " XXX",	x, y+2, c);
					DrawLine(screen, "X   X",	x, y+3, c);
					DrawLine(screen, " XXX",	x, y+4, c);
					break;
				}
				case '9': {
					DrawLine(screen, " XXX",	x, y+0, c);
					DrawLine(screen, "X   X",	x, y+1, c);
					DrawLine(screen, " XXXX",	x, y+2, c);
					DrawLine(screen, "    X",	x, y+3, c);
					DrawLine(screen, " XXX",	x, y+4, c);
					break;
				}
				case '.': {
					DrawLine(screen, "",	x, y+1, c);
					DrawLine(screen, "   X",	x, y+3, c);
					break;
				}
				case ',': {
					DrawLine(screen, "   X",	x, y+3, c);
					DrawLine(screen, "  X",	x, y+4, c);
					break;
				}
				case '?': {
					DrawLine(screen, " XXX",	x, y+0, c);
					DrawLine(screen, "X   X",	x, y+1, c);
					DrawLine(screen, "  XX",	x, y+2, c);
					DrawLine(screen, "  X",	x, y+4, c);
					break;
				}
				case ':': {
					DrawLine(screen, "  X",	x, y+1, c);
					DrawLine(screen, "  X",	x, y+3, c);
					break;
				}
				case '<': {
					DrawLine(screen, "  X",	x, y+0, c);
					DrawLine(screen, " X",	x, y+1, c);
					DrawLine(screen, "X",	x, y+2, c);
					DrawLine(screen, " X",	x, y+3, c);
					DrawLine(screen, "  X",	x, y+4, c);
					break;
				}
			}
		}
		void DrawString(uint8_t ccreen, std::string text, uint8_t x, uint8_t y, uint16_t c) {
			for(unsigned int i = 0; i < text.length(); i++) {
				DrawLetter(ccreen, text.at(i), x + (i * 6), y, c);
			}
		}
	}
}
