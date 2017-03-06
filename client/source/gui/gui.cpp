#if defined(_NDS)
#include <nds/arm9/video.h> // SCREEN_WIDTH
#include <nds/dma.h>        // dmaFillHalfWords
#endif
#include <algorithm>        // std::min, std::max
#include <cstring>
#include "common/easylogging++Wrapper.h"
#include "gui.h"
#include "png_format.h"
#include "ttf_format.h"

namespace D2K {namespace GUI {

#if defined(_NDS)
const uint16_t MAX_X = SCREEN_WIDTH;
const uint16_t MAX_Y = SCREEN_HEIGHT;
const uint8_t SCREEN_BYTES = 2;
#elif defined(_3DS)
const uint16_t MAX_X = _3DS_SCREEN_WIDTH;
const uint16_t MAX_Y = _3DS_SCREEN_HEIGHT;
const uint8_t SCREEN_BYTES = 3;
#endif
const uint8_t IMAGE_BYTES = 3;
const uint8_t ALPHA_IMAGE_BYTES = 4;
uint16_t* g_screen[2];
bool Update = false;
uint16_t Color[colorMax];
uint8_t alpha_setting = 140;
void VoidFunction() { }

std::string background_filename{};
void SetBackground(const std::string& text)
{
	background_filename = text;
}
std::string GetBackground()
{
	return background_filename;
}
std::string font_filename[TTF::FACE_TYPE_MAX]{};
void SetFont(uint8_t face_type, const std::string& text)
{
	font_filename[face_type] = text;
}
std::string GetFont(uint8_t face_type)
{
	return font_filename[face_type];
}

// orientation false = normal, true = rotated -90degrees
inline uint32_t GetPixelPosition(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t byte_depth, bool orientation)
{
	if(x >= w)
	{
		LOG_N_TIMES(15, ERROR) << "GetPixelPosition X:" << x << ".";
		x = w;
		//return 0;
	}
	else if(y >= h)
	{
		LOG_N_TIMES(15, ERROR) << "GetPixelPosition Y:" << y << ".";
		y = h;
		//return 0;
	}
	if(orientation == false)
	{
		return (x + (y * w)) * byte_depth;
	}
	else
	{
		return (x * h + (h - 1 - y)) * byte_depth;
	}
}

inline uint8_t* GetScreenPointer(uint8_t screen, uint16_t x, uint16_t y)
{
	uint8_t* screen_pointer = (uint8_t*)GUI::g_screen[screen];
#if defined(_NDS)
	screen_pointer += GetPixelPosition(x, y, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BYTES, false);
#elif defined(_3DS)
	screen_pointer += GetPixelPosition(x, y, _3DS_SCREEN_WIDTH, _3DS_SCREEN_HEIGHT, SCREEN_BYTES, true);
#endif
	return screen_pointer;
}

inline void RGB15TORGB24(uint16_t color, uint8_t &red, uint8_t &green, uint8_t &blue)
{
	red   =  (color & 0x001f       ) * 8.225806451612903;
	green = ((color & 0x3E0)  >> 5 ) * 8.225806451612903;
	blue  = ((color & 0x7C00) >> 10) * 8.225806451612903;
}
inline uint16_t RGB24TORGB15(uint8_t red, uint8_t green, uint8_t blue)
{
	return ARGB16(1, (red >> 3), (green >> 3), (blue >> 3));
}

void resize_crop(const char* input_image, char* output_image, uint16_t input_width, uint16_t input_height, uint16_t output_width, uint16_t output_height, uint8_t image_bytes)
{
	int crop_x = 0;
	int crop_y = 0;
	if(output_width > input_width)
		output_width = input_width;
	if(output_height > input_height)
		output_height = input_height;
	if(output_width < input_width)
		crop_x = (input_width - output_width) / 2;
	if(output_height < input_height)
		crop_y = (input_height - output_height) / 2;

	for(int x = 0; x < output_width; x++)
	{
		for(int y = 0; y < output_height; y++)
		{
			int output_memory_position = GetPixelPosition(x, y, output_width, output_height, image_bytes, true);
			int input_memory_position = GetPixelPosition(x + crop_x, y + crop_y, input_width, input_height, image_bytes, false);

			for(uint8_t i = 0; i < image_bytes; i++)
			{
				output_image[output_memory_position + i] = input_image[input_memory_position + i];
			}
		}
	}
}

// https://rosettacode.org/wiki/Bilinear_interpolation
float lerp(float s, float e, float t)
{
	return s+(e-s)*t;
}
float blerp(float c00, float c10, float c01, float c11, float tx, float ty)
{
	return lerp(lerp(c00, c10, tx), lerp(c01, c11, tx), ty);
}
void resize_bilinear(const char* input_image, char* output_image, uint16_t input_width, uint16_t input_height, uint16_t output_width, uint16_t output_height, uint8_t image_bytes) 
{
	int x, y;
	for(x= 0, y=0; y < output_height; x++)
	{
		if(x >= output_width)
		{
			x = 0; y++;
		}
		float gx = x / (float)(output_width ) * (input_width - 1 );
		float gy = y / (float)(output_height) * (input_height - 1);
		int gxi = (int)gx;
		int gyi = (int)gy;
		const char* c00 = &input_image[GetPixelPosition(gxi + 0, gyi + 0, input_width, input_height, image_bytes, false)];
		const char* c10 = &input_image[GetPixelPosition(gxi + 1, gyi + 0, input_width, input_height, image_bytes, false)];
		const char* c01 = &input_image[GetPixelPosition(gxi + 0, gyi + 1, input_width, input_height, image_bytes, false)];
		const char* c11 = &input_image[GetPixelPosition(gxi + 1, gyi + 1, input_width, input_height, image_bytes, false)];
		char* output_memory_position = &output_image[GetPixelPosition(x, y, output_width, output_height, image_bytes, true)];
		for(uint8_t i = 0; i < image_bytes; i++)
		{
			output_memory_position[i] = (uint8_t)blerp(c00[i], c10[i], c01[i], c11[i], gx - gxi, gy -gyi);
		}
	}
}

char* background_image{};
// TODO:handle deallocation of background_image
// TODO:seperate cropping function
// TODO:logs
bool LoadBackgroundImage()
{
	const int background_height = MAX_Y;
	const int background_width  = MAX_X;
	const int background_image_size = background_width * background_height * IMAGE_BYTES;
	int png_width, png_height;
	char* png_image{};
	if(LoadPngImage(GetBackground(), png_width, png_height, false, (unsigned char**)&png_image))
	{
		if(background_image != nullptr)
		{
			free(background_image);
		}
		background_image = (char*)malloc(background_image_size);
		
		if(png_width  == background_width
		&& png_height == background_height)
		{
			resize_crop((const char*)png_image, background_image, png_width, png_height, background_width, background_height, IMAGE_BYTES);
		}
		else
		{
			resize_bilinear((const char*)png_image, background_image, png_width, png_height, background_width, background_height, IMAGE_BYTES);
		}

		free(png_image);
		return true;
	}
	else if(background_image == nullptr)
	{
		background_image = (char*)malloc(background_image_size);

		for(int x = 0; x < background_width; x++)
		{
			for(int y = 0; y < background_height; y++)
			{
				int output_memory_position = GetPixelPosition(x, y, background_width, background_height, IMAGE_BYTES, true);
				uint8_t blue, green, red{};
				RGB15TORGB24(Color[COLOR_BACKGROUND], red, green, blue);
				
				background_image[output_memory_position + 0] = red;
				background_image[output_memory_position + 1] = green;
				background_image[output_memory_position + 2] = blue;
			}
		}
	}
	return false;
}
bool DrawBackgroundImage(uint8_t screen, GUI::Rect rect, uint8_t color)
{
	if(background_image == nullptr)
		LoadBackgroundImage();

// TODO: This could be optimized into a simple memcpy after the first run
	if(background_image)
	{
		for(int x = rect.GetX(); x <= rect.GetX2(); x++)
		{
			for(int y = rect.GetY(); y <= rect.GetY2(); y++)
			{
				int background_memory_position = GetPixelPosition(x, y, MAX_X, MAX_Y, IMAGE_BYTES, true);

				SetPixel(screen, x, y, background_image[background_memory_position + 0], background_image[background_memory_position + 1], background_image[background_memory_position + 2]);
			}
		}
		return true;
	}
	else
	{
// TODO: Check when background fails to load
		DrawFilledRect(screen, rect, color);
		return false;
	}
}
bool DrawBackgroundImage(uint8_t screen, uint8_t color)
{
	GUI::Rect rect = { 0, 0, MAX_X, MAX_Y };
	return DrawBackgroundImage(screen, rect, color);
}

char* button_image{};
const int button_max_h = 21;
const int button_max_w = 21;
bool LoadButtonImage()
{
	char* button_filename = "/ds2key/button.png";
	int png_width, png_height;
	char* png_image{};
	if(LoadPngImage(button_filename, png_width, png_height, true, (unsigned char**)&png_image))
	{
		int target_height = button_max_h;
		int target_width  = button_max_w;
		if(png_width < target_width)
			target_width = png_width;
		if(png_height < target_height)
			target_height = png_height;

		int target_image_size = png_width * png_height * (ALPHA_IMAGE_BYTES);

		if(button_image == nullptr)
		{
			button_image = png_image;
			return true;
		}
	}
	else
	{
		LOG(INFO) << "Failed to open:" << button_filename << ".";
	}
	return false;
}
bool DrawButtonImage(uint8_t screen, std::string letter, uint16_t x, uint16_t y)
{
	if(letter.length() == 1)
	{
		if(button_image == nullptr)
		{
			if(LoadButtonImage() == false)
			{
				return false;
			}
		}
		for(int button_x = 0; button_x < button_max_w; button_x++)
		{
			for(int button_y = 0; button_y < button_max_h; button_y++)
			{
				int button_memory_position = GetPixelPosition(button_x, button_y, button_max_w, button_max_h, ALPHA_IMAGE_BYTES, true);

				SetPixel(screen, x + button_x, y + button_y,
				         RGB24TORGB15(button_image[button_memory_position + 0],
				                      button_image[button_memory_position + 1],
				                      button_image[button_memory_position + 2]),
				                      button_image[button_memory_position + 3]);
			}
		}
		DrawString(screen, letter, TTF::FONT_SIZE_BUTTON_IMAGE, TTF::FONT_BOLD, x + 5, y + 7, Color[COLOR_BUTTON_TEXT]);
	}
	else if(letter.length() > 1)
	{
		// DrawFilledRect doesn't work since we use raw x/y not scaled ;_;
		for(int button_x = 0; button_x < button_max_w; button_x++)
		{
			for(int button_y = 0; button_y < button_max_h; button_y++)
			{
				SetPixel(screen,  + button_x, y + button_y, Color[COLOR_BUTTON_BACKGROUND]);
			}
		}
		DrawString(screen, letter, TTF::FONT_SIZE_BUTTON_IMAGE, TTF::FONT_BOLD, x + 5 - 25, y + 7, Color[COLOR_BUTTON_TEXT]);
	}

	return true;
}

void SetUpdate(bool value)
{
	Update = value;
}
bool IsUpdated()
{
	return Update;
}

void GetPixel(uint8_t screen, uint16_t x, uint16_t y, uint16_t& color)
{
	uint8_t* screen_pointer = GetScreenPointer(screen, x, y);
#if defined(_NDS)
	color = (uint16_t)screen_pointer[0];
#elif defined(_3DS)
	color = RGB24TORGB15(screen_pointer[2], screen_pointer[1], screen_pointer[1]);
#endif
}
void GetPixel(uint8_t screen, uint16_t x, uint16_t y, uint8_t& red, uint8_t& green, uint8_t& blue)
{
	uint8_t* screen_pointer = GetScreenPointer(screen, x, y);
#if defined(_NDS)
	RGB15TORGB24(((uint16_t*)screen_pointer)[0], red, green, blue);
#elif defined(_3DS)
	blue  = screen_pointer[0];
	green = screen_pointer[1];
	red   = screen_pointer[2];
#endif
}

struct LookupLevel
{
	uint8_t Values[256];
};

struct LookupTable
{
	LookupLevel Levels[256];
} AlphaTable;

__inline uint8_t clipByte(int value)
{
	value = (0 & (-(int)(value < 0))) | (value & (-(int)!(value < 0)));
	value = (255 & (-(int)(value > 255))) | (value & (-(int)!(value > 255)));
	return value;
}

int InitTable()
{
	static bool initialized = false;
	if(initialized)
		return true;
	initialized = true;

	float fValue, fAlpha;
	int iValue, iAlpha;
	for (iAlpha = 0; iAlpha < 256; iAlpha++)
	{
		fAlpha = ((float)iAlpha) / 255;
		for (iValue = 0; iValue < 256; iValue++)
		{
			fValue = ((float)iValue) / 255;
			AlphaTable.Levels[iAlpha].Values[iValue] = clipByte((int)((fValue * fAlpha) * 255));
		}
	}
	return true;
}

LookupLevel* LookupPointer(int alpha)
{
	return &AlphaTable.Levels[clipByte(alpha)];
}

uint8_t AlphaBlend(uint8_t color_1, uint8_t color_2, uint8_t alpha)
{
#if 1
	InitTable();

	uint8_t NewColor;
	LookupLevel* SourceTable = LookupPointer(255 - alpha);
	LookupLevel* DestTable = LookupPointer(alpha);

	// The clipByte's aren't really necessary, but they're there to remove the
	// chance for an occasional overflow. They only eat a couple clock cycles anyway.
	NewColor = clipByte(SourceTable->Values[color_2] +
	                    DestTable->Values[color_1]);
	return NewColor;
#else
	return (alpha  * color_1 + (100 - alpha) * color_2) / 100;
#endif
}
void SetPixel(uint8_t screen, uint16_t x, uint16_t y, uint16_t color)
{
	if((x >= MAX_X) // if we're not drawing on screen
	|| (y >= MAX_Y))
		return;
#if defined(_NDS)
	uint16_t* screen_pointer = (uint16_t*)GetScreenPointer(screen, x, y);

	screen_pointer[0] = color;
#elif defined(_3DS)
	uint8_t blue, green, red{};
	RGB15TORGB24(color, red, green, blue);
		
	SetPixel(screen, x, y, red, green, blue);
#endif
}
void SetPixel(uint8_t screen, uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue)
{
	if((x >= MAX_X) // if we're not drawing on screen
	|| (y >= MAX_Y))
		return;
#if defined(_3DS)
	uint8_t* screen_pointer = GetScreenPointer(screen, x, y);

	screen_pointer[0] = blue;
	screen_pointer[1] = green;
	screen_pointer[2] = red;
#elif defined(_NDS)
	SetPixel(screen, x, y, RGB24TORGB15(red, green, blue));
#endif
}
// TODO: add errors or fatals if the boundary checks fail
void SetPixel(uint8_t screen, uint16_t x, uint16_t y, uint16_t color, uint8_t alpha)
{
	if((x >= MAX_X) // if we're not drawing on screen
	|| (y >= MAX_Y))
		return;

	if(alpha == 255 || background_image == nullptr)
	{
		SetPixel(screen, x, y, color);
		return;
	}
	else if(alpha == 0)
		return;

	uint8_t color_blue, color_green, color_red{};
	RGB15TORGB24(color,
		     color_red,
		     color_green,
		     color_blue);

	uint8_t screen_pointer_blue, screen_pointer_green, screen_pointer_red{};
	GetPixel(screen, x, y,
		 screen_pointer_red,
		 screen_pointer_green,
		 screen_pointer_blue);

	SetPixel(screen, x, y,
		 AlphaBlend(color_red,   screen_pointer_red  , alpha),
		 AlphaBlend(color_green, screen_pointer_green, alpha),
		 AlphaBlend(color_blue,  screen_pointer_blue , alpha));
}

void ClearScreen(uint8_t screen, uint16_t color)
{
	DrawBackgroundImage(screen, color);
}
// TODO: add errors or fatals if the boundary checks fail
void DrawFastHorizontleLine(uint8_t screen, uint16_t x, uint16_t y, uint16_t w, uint16_t color)
{
	if((x > MAX_X) // if we're not drawing on screen
	|| (y > MAX_Y))
		return;

	if(x + w >= MAX_X)
	{
		if(MAX_X - x > 0)
			w = MAX_X - x;
		else
			return;
	}
	if(w == 0)
		return;
#if defined(_NDS)
	uint16_t* screen_pointer = (uint16_t*)GetScreenPointer(screen, x, y);
	dmaFillHalfWords(color, &screen_pointer[0], w * SCREEN_BYTES);
#elif defined(_3DS)
	for(int i = 0; i < w; i++)
	{
		SetPixel(screen, x + i, y, color);
	}
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
	{
//#if defined(_NDS)
//		DrawFastHorizontleLine(screen, rect.GetX(), y, rect.GetW(), c);
//#elif defined(_3DS)
		for(int x = rect.GetX(); x <= rect.GetX2(); x++)
		{
			SetPixel(screen, x, y, c, alpha_setting);
		}
//#endif
	}
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
		for(unsigned int i = 0; i < text.length(); i++)
		{
			if(text.at(i) != ' ')
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
void DrawString(uint8_t screen, std::string text, uint8_t font_size, uint8_t face_type, uint16_t x, uint16_t y, uint16_t c)
{
// TODO:Disabled TTF functions for now, REALLY SLOW!
//#if defined(_3DS)
//	TTF::DrawString(screen, x, y, font_size, face_type, c, text.c_str());
//#elif defined(_NDS)
	for(unsigned int i = 0; i < text.length(); i++)
	{
		DrawLetter(screen, text.at(i), x + (i * 6), y, c);
	}
//#endif
}

}} // namespace D2K::GUI
