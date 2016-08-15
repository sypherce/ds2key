#ifdef _NDS
#include <nds/arm9/video.h>  // SCREEN_WIDTH
#include <nds/dma.h>  // dmaFillHalfWords
#endif
#include <algorithm>  // std::min, std::max
#include "gui.h"

namespace D2K {namespace GUI {

#if defined(_NDS)
const uint16_t MAX_X = SCREEN_WIDTH;
const uint16_t MAX_Y = SCREEN_HEIGHT;
#elif defined(_3DS)
const uint16_t MAX_X = _3DS_SCREEN_WIDTH;
const uint16_t MAX_Y = _3DS_SCREEN_HEIGHT;
#endif
uint16_t* g_screen[2];
bool Update = false;
uint16_t Color[colorMax];
void VoidFunction() { }

void SetUpdate(bool value)
{
	Update = value;
}
bool IsUpdated()
{
	return Update;
}
void DrawFastHorizontleLine(uint8_t screen, uint16_t x, uint16_t y, uint16_t w, uint16_t c)
{
	if(x >= MAX_X)
		return;
	if(y >= MAX_Y)
		return;
	if(x + w >= MAX_X)
		w = MAX_X - x;
	if(w == 0)
		return;
#if defined(_NDS)
	dmaFillHalfWords(c, &GUI::g_screen[screen][x + (y * SCREEN_WIDTH)], w * 2);
#elif defined(_3DS)
	for(int i = 0; i < w; i++)
		SetPixel(screen, x + i, y, c);
#endif
}

void SetPixel(uint8_t screen, uint16_t x, uint16_t y, uint16_t c)
{
	if((x < MAX_X) // if we're drawing on screen
	&& (y < MAX_Y))
#if defined(_NDS)
		GUI::g_screen[screen][x + (y * SCREEN_WIDTH)] = c;
#elif defined(_3DS)
	{
		uint8_t* screen_pointer = (uint8_t*)GUI::g_screen[screen];

		unsigned char blue   = (c & 0x7C00) >> 10 << 3;
		unsigned char green = (c & 0x3E0)  >> 5  << 3;
		unsigned char red  =  c & 0x001f        << 3;

		screen_pointer[((x * _3DS_SCREEN_HEIGHT + (_3DS_SCREEN_HEIGHT - 1 - y)) * 3) + 0] = blue;
		screen_pointer[((x * _3DS_SCREEN_HEIGHT + (_3DS_SCREEN_HEIGHT - 1 - y)) * 3) + 1] = green;
		screen_pointer[((x * _3DS_SCREEN_HEIGHT + (_3DS_SCREEN_HEIGHT - 1 - y)) * 3) + 2] = red;
	}
#endif
}

void ClearScreen(uint8_t screen, uint16_t c)
{
#if defined(_NDS)
	dmaFillHalfWords(c, GUI::g_screen[screen], SCREEN_WIDTH * SCREEN_HEIGHT * 2);
#elif defined(_3DS)
	for(int x = 0; x < _3DS_SCREEN_WIDTH; x++)
		for(int y = 0; y < _3DS_SCREEN_HEIGHT; y++)
			SetPixel(screen, x, y, c);
#endif
}
void DrawRect(uint8_t screen, GUI::Rect rect, uint16_t c)
{
	for(int y = rect.GetY(); y <= rect.GetY2(); y++)
	{
		SetPixel(screen, rect.GetX(), y, c);
		SetPixel(screen, rect.GetX2(), y, c);
	}

	DrawFastHorizontleLine(screen, rect.GetX(), rect.GetY(), rect.GetW(), c);
	DrawFastHorizontleLine(screen, rect.GetX(), rect.GetY2(), rect.GetW(), c);
}
void DrawFilledRect(uint8_t screen, GUI::Rect rect, uint16_t c)
{
	for(int y = rect.GetY(); y <= rect.GetY2(); y++)
		DrawFastHorizontleLine(screen, rect.GetX(), y, rect.GetW(), c);
}
void DrawLine(uint8_t screen, std::string text, uint16_t x, uint16_t y, uint16_t c)
{
	if (text == "X")
	{
		SetPixel(screen, x, y, c);
	}
	else if(text == "XX")
	{
		DrawFastHorizontleLine(screen, x, y, 2, c);
	}
	else if(text == " XX")
	{
		DrawFastHorizontleLine(screen, 1 + x, y, 2, c);
	}
	else if(text == "  XX")
	{
		DrawFastHorizontleLine(screen, 2 + x, y, 2, c);
	}
	else if(text == "   XX")
	{
		DrawFastHorizontleLine(screen, 3 + x, y, 2, c);
	}
	else if(text == "XXX")
	{
		DrawFastHorizontleLine(screen, x, y, 3, c);
	}
	else if(text == " XXX")
	{
		DrawFastHorizontleLine(screen, 1 + x, y, 3, c);
	}
	else if(text == "  XXX")
	{
		DrawFastHorizontleLine(screen, 2 + x, y, 3, c);
	}
	else if(text == "XXXX")
	{
		DrawFastHorizontleLine(screen, x, y, 4, c);
	}
	else if(text == " XXXX")
	{
		DrawFastHorizontleLine(screen, 1 + x, y, 4, c);
	}
	else if(text == "XXXXX")
	{
		DrawFastHorizontleLine(screen, x, y, 5, c);
	}
	else
	{
		for (unsigned int i = 0; i < text.length(); i++)
		{
			if (text.at(i) != ' ')
				SetPixel(screen, x + i, y, c);
		}
	}
}
void DrawLetter(uint8_t screen, char letter, uint16_t x, uint16_t y, uint16_t c)
{
	switch(letter)
	{
	case 'A':
	case 'a':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "XXXXX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, "X   X",	x, y+4, c);
		break;
	}
	case 'B':
	case 'b':
	{
		DrawLine(screen, "XXXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "XXXX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, "XXXX",  	x, y+4, c);
		break;
	}
	case 'C':
	case 'c':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "X",		x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case 'D':
	case 'd':
	{
		DrawLine(screen, "XXXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "X   X",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, "XXXX",	x, y+4, c);
		break;
	}
	case 'E':
	case 'e':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "X",		x, y+1, c);
		DrawLine(screen, "XXXXX",	x, y+2, c);
		DrawLine(screen, "X",		x, y+3, c);
		DrawLine(screen, "XXXXX",	x, y+4, c);
		break;
	}
	case 'F':
	case 'f':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "X",		x, y+1, c);
		DrawLine(screen, "XXX",		x, y+2, c);
		DrawLine(screen, "X",		x, y+3, c);
		DrawLine(screen, "X",		x, y+4, c);
		break;
	}
	case 'G':
	case 'g':
	{
		DrawLine(screen, " XXXX",	x, y+0, c);
		DrawLine(screen, "X",		x, y+1, c);
		DrawLine(screen, "X  XX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case 'H':
	case 'h':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "XXXXX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, "X   X",	x, y+4, c);
		break;
	}
	case 'I':
	case 'i':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "  X",		x, y+1, c);
		DrawLine(screen, "  X",		x, y+2, c);
		DrawLine(screen, "  X",		x, y+3, c);
		DrawLine(screen, "XXXXX",	x, y+4, c);
		break;
	}
	case 'J':
	case 'j':
	{
		DrawLine(screen, "    X",	x, y+0, c);
		DrawLine(screen, "    X",	x, y+1, c);
		DrawLine(screen, "    X",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case 'K':
	case 'k':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, "X  X",	x, y+1, c);
		DrawLine(screen, "XXX",		x, y+2, c);
		DrawLine(screen, "X  X",	x, y+3, c);
		DrawLine(screen, "X   X",	x, y+4, c);
		break;
	}
	case 'L':
	case 'l':
	{
		DrawLine(screen, "X",		x, y+0, c);
		DrawLine(screen, "X",		x, y+1, c);
		DrawLine(screen, "X",		x, y+2, c);
		DrawLine(screen, "X",		x, y+3, c);
		DrawLine(screen, "XXXXX",	x, y+4, c);
		break;
	}
	case 'M':
	case 'm':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, "XX XX",	x, y+1, c);
		DrawLine(screen, "X X X",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, "X   X",	x, y+4, c);
		break;
	}
	case 'N':
	case 'n':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, "XX  X",	x, y+1, c);
		DrawLine(screen, "X X X",	x, y+2, c);
		DrawLine(screen, "X  XX",	x, y+3, c);
		DrawLine(screen, "X   X",	x, y+4, c);
		break;
	}
	case 'O':
	case 'o':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "X   X",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case 'P':
	case 'p':
	{
		DrawLine(screen, "XXXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "XXXX",	x, y+2, c);
		DrawLine(screen, "X",		x, y+3, c);
		DrawLine(screen, "X",		x, y+4, c);
		break;
	}
	case 'Q':
	case 'q':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "X X X",	x, y+2, c);
		DrawLine(screen, "X  X",	x, y+3, c);
		DrawLine(screen, " XX X",	x, y+4, c);
		break;
	}
	case 'R':
	case 'r':
	{
		DrawLine(screen, "XXXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "XXXX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, "X   X",	x, y+4, c);
		break;
	}
	case 'S':
	case 's':
	{
		DrawLine(screen, " XXXX",	x, y+0, c);
		DrawLine(screen, "X",	x, y+1, c);
		DrawLine(screen, " XXX",	x, y+2, c);
		DrawLine(screen, "    X",	x, y+3, c);
		DrawLine(screen, "XXXX",	x, y+4, c);
		break;
	}
	case 'T':
	case 't':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "  X",		x, y+1, c);
		DrawLine(screen, "  X",		x, y+2, c);
		DrawLine(screen, "  X",		x, y+3, c);
		DrawLine(screen, "  X",		x, y+4, c);
		break;
	}
	case 'U':
	case 'u':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, "X   X",		x, y+1, c);
		DrawLine(screen, "X   X",		x, y+2, c);
		DrawLine(screen, "X   X",		x, y+3, c);
		DrawLine(screen, " XXX",		x, y+4, c);
		break;
	}
	case 'V':
	case 'v':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, "X   X",		x, y+1, c);
		DrawLine(screen, " X X",		x, y+2, c);
		DrawLine(screen, " X X",		x, y+3, c);
		DrawLine(screen, "  X",		x, y+4, c);
		break;
	}
	case 'W':
	case 'w':
	{
		DrawLine(screen, "X X X",	x, y+0, c);
		DrawLine(screen, "X X X",		x, y+1, c);
		DrawLine(screen, "X X X",		x, y+2, c);
		DrawLine(screen, "X X X",		x, y+3, c);
		DrawLine(screen, " X X",		x, y+4, c);
		break;
	}
	case 'X':
	case 'x':
	{
		DrawLine(screen, "X   X",		x, y+0, c);
		DrawLine(screen, " X X",		x, y+1, c);
		DrawLine(screen, "  X",			x, y+2, c);
		DrawLine(screen, " X X",		x, y+3, c);
		DrawLine(screen, "X   X",		x, y+4, c);
		break;
	}
	case 'Y':
	case 'y':
	{
		DrawLine(screen, "X   X",	x, y+0, c);
		DrawLine(screen, " X X",	x, y+1, c);
		DrawLine(screen, "  X",		x, y+2, c);
		DrawLine(screen, "  X",		x, y+3, c);
		DrawLine(screen, "  X",		x, y+4, c);
		break;
	}
	case 'Z':
	case 'z':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "   X",	x, y+1, c);
		DrawLine(screen, "  X",		x, y+2, c);
		DrawLine(screen, " X",		x, y+3, c);
		DrawLine(screen, "XXXXX",		x, y+4, c);
		break;
	}
	case '0':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X  XX",	x, y+1, c);
		DrawLine(screen, "X X X",		x, y+2, c);
		DrawLine(screen, "XX  X",		x, y+3, c);
		DrawLine(screen, " XXX",		x, y+4, c);
		break;
	}
	case '1':
	{
		DrawLine(screen, "  X",		x, y+0, c);
		DrawLine(screen, " XX",		x, y+1, c);
		DrawLine(screen, "  X",		x, y+2, c);
		DrawLine(screen, "  X",		x, y+3, c);
		DrawLine(screen, "XXXXX",	x, y+4, c);
		break;
	}
	case '2':
	{
		DrawLine(screen, " XXX",		x, y+0, c);
		DrawLine(screen, "X   X",		x, y+1, c);
		DrawLine(screen, "  XX",		x, y+2, c);
		DrawLine(screen, " X",		x, y+3, c);
		DrawLine(screen, "XXXXX",	x, y+4, c);
		break;
	}
	case '3':
	{
		DrawLine(screen, " XXX",		x, y+0, c);
		DrawLine(screen, "X   X",		x, y+1, c);
		DrawLine(screen, "  XX",		x, y+2, c);
		DrawLine(screen, "X   X",		x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case '4':
	{
		DrawLine(screen, "X   X",		x, y+0, c);
		DrawLine(screen, "X   X",		x, y+1, c);
		DrawLine(screen, "XXXXX",		x, y+2, c);
		DrawLine(screen, "    X",		x, y+3, c);
		DrawLine(screen, "    X",	x, y+4, c);
		break;
	}
	case '5':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "X",	x, y+1, c);
		DrawLine(screen, "XXXX",	x, y+2, c);
		DrawLine(screen, "    X",	x, y+3, c);
		DrawLine(screen, "XXXX",	x, y+4, c);
		break;
	}
	case '6':
	{
		DrawLine(screen, " XXXX",	x, y+0, c);
		DrawLine(screen, "X",	x, y+1, c);
		DrawLine(screen, "XXXX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case '7':
	{
		DrawLine(screen, "XXXXX",	x, y+0, c);
		DrawLine(screen, "    X",	x, y+1, c);
		DrawLine(screen, "   X",	x, y+2, c);
		DrawLine(screen, "  X",	x, y+3, c);
		DrawLine(screen, "  X",	x, y+4, c);
		break;
	}
	case '8':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, " XXX",	x, y+2, c);
		DrawLine(screen, "X   X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case '9':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, " XXXX",	x, y+2, c);
		DrawLine(screen, "    X",	x, y+3, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case '.':
	{
		DrawLine(screen, "   X",	x, y+3, c);
		break;
	}
	case ',':
	{
		DrawLine(screen, "   X",	x, y+3, c);
		DrawLine(screen, "  X",	x, y+4, c);
		break;
	}
	case '?':
	{
		DrawLine(screen, " XXX",	x, y+0, c);
		DrawLine(screen, "X   X",	x, y+1, c);
		DrawLine(screen, "  XX",	x, y+2, c);
		DrawLine(screen, "  X",	x, y+4, c);
		break;
	}
	case ':':
	{
		DrawLine(screen, "  X",	x, y+1, c);
		DrawLine(screen, "  X",	x, y+3, c);
		break;
	}
	case '<':
	{
		DrawLine(screen, "  X",	x, y+0, c);
		DrawLine(screen, " X",	x, y+1, c);
		DrawLine(screen, "X",	x, y+2, c);
		DrawLine(screen, " X",	x, y+3, c);
		DrawLine(screen, "  X",	x, y+4, c);
		break;
	}
	case '>':
	{
		DrawLine(screen, "X",	x, y+0, c);
		DrawLine(screen, " X",	x, y+1, c);
		DrawLine(screen, "  X",	x, y+2, c);
		DrawLine(screen, " X",	x, y+3, c);
		DrawLine(screen, "X",	x, y+4, c);
		break;
	}
	case '*':
	{
		DrawLine(screen, "  X",	x, y+0, c);
		DrawLine(screen, "X X X",	x, y+1, c);
		DrawLine(screen, " XXX",	x, y+2, c);
		DrawLine(screen, "X X X",	x, y+3, c);
		DrawLine(screen, "  X",	x, y+4, c);
		break;
	}
	case '+':
	{
		DrawLine(screen, "  X",	x, y+1, c);
		DrawLine(screen, " XXX",	x, y+2, c);
		DrawLine(screen, "  X",	x, y+3, c);
		break;
	}
	case '-':
	{
		DrawLine(screen, " XXX",	x, y+2, c);
		break;
	}
	case '=':
	{
		DrawLine(screen, " XXX",	x, y+1, c);
		DrawLine(screen, " XXX",	x, y+4, c);
		break;
	}
	case '\'':
	{
		DrawLine(screen, "  X",	x, y+1, c);
		DrawLine(screen, "  X",	x, y+2, c);
		DrawLine(screen, "  X",	x, y+3, c);
		break;
	}
	case '`':
	{
		DrawLine(screen, " X",	x, y+1, c);
		DrawLine(screen, "  X",	x, y+2, c);
		break;
	}
	case '"':
	{
		DrawLine(screen, " X X",	x, y+1, c);
		DrawLine(screen, " X X",	x, y+2, c);
		DrawLine(screen, " X X",	x, y+3, c);
		break;
	}
	case ';':
	{
		DrawLine(screen, "  X",	x, y+1, c);
		DrawLine(screen, "  X",	x, y+3, c);
		DrawLine(screen, " X",	x, y+4, c);
		break;
	}
	case '^':
	{
		DrawLine(screen, "  X",	x, y+0, c);
		DrawLine(screen, " X X",	x, y+1, c);
		DrawLine(screen, "X   X",	x, y+2, c);
		break;
	}
	case '/':
	{
		DrawLine(screen, "    X",x, y+0, c);
		DrawLine(screen, "   X", x, y+1, c);
		DrawLine(screen, "  X",  x, y+2, c);
		DrawLine(screen, " X",   x, y+3, c);
		DrawLine(screen, "X",    x, y+4, c);
		break;
	}
	case '\\':
	{
		DrawLine(screen, "X",    x, y+0, c);
		DrawLine(screen, " X",   x, y+1, c);
		DrawLine(screen, "  X",  x, y+2, c);
		DrawLine(screen, "   X", x, y+3, c);
		DrawLine(screen, "    X",x, y+4, c);
		break;
	}
	case '[':
	{
		DrawLine(screen, "XX",x, y+0, c);
		DrawLine(screen, "X", x, y+1, c);
		DrawLine(screen, "X", x, y+2, c);
		DrawLine(screen, "X", x, y+3, c);
		DrawLine(screen, "XX",x, y+4, c);
		break;
	}
	case ']':
	{
		DrawLine(screen, "XX",x, y+0, c);
		DrawLine(screen, " X",x, y+1, c);
		DrawLine(screen, " X",x, y+2, c);
		DrawLine(screen, " X",x, y+3, c);
		DrawLine(screen, "XX",x, y+4, c);
		break;
	}
	}
}
void DrawString(uint8_t ccreen, std::string text, uint16_t x, uint16_t y, uint16_t c)
{
	for(unsigned int i = 0; i < text.length(); i++)
	{
		DrawLetter(ccreen, text.at(i), x + (i * 6), y, c);
	}
}

}}//namespace D2K::GUI
