#include <string> // std::string
#include <png.h>
#include "common/easylogging++Wrapper.h"
#include "core.h"
#include "edit.h"
#include "gui.h"

#include <cstdio>
#include <cstring>
#include <math.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace D2K {namespace GUI{namespace TTF{

FT_Library library{};
FT_Face face{};

FT_Error Init()
{
	static bool initialized = false;
	// If already initialized, return
	if(initialized)
		return 0;
	
	FT_Error error{};

	// initialize library
	error = FT_Init_FreeType(&library);
	if(error)
	{
		LOG(DEBUG) << "FT_Init_FreeType ERROR#" << (int)error;
		return error;
	}

	// create face object
	error = FT_New_Face(library, GUI::GetFont().c_str(), 0, &face);
	if(error)
	{
		LOG(DEBUG) << "FT_New_Face ERROR#" << (int)error;
		FT_Done_Face(face);
		return error;
	}

	initialized = true;

	return 0;
}

void DeInit()
{
	// If never initialized, return
	if(TTF::Init())
		return;

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

void Draw(uint8_t screen, uint16_t x, uint16_t y, uint16_t color, FT_Bitmap *bitmap)
{
	uint8_t *buffer = bitmap->buffer;
	int buffer_gap = bitmap->pitch - bitmap->width;
	for(unsigned int y_i = 0; y_i < bitmap->rows; y_i++)
	{
		for(unsigned int x_i = 0; x_i < bitmap->width; x_i++)
		{
			//uint8_t alpha = ((*(buffer++)) * 100) / 255;
			uint8_t alpha = (*(buffer++));// *0.392156862745098;
			
			SetPixel(screen, x + x_i, y + y_i, color, alpha);
		}

		buffer += buffer_gap;
	}

}

const double angle = (0.0 / 360)* 3.14159 * 2; // set angle
// transformation matrix
FT_Matrix matrix = { 
	matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L),
	matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L),
	matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L),
	matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L),
};
FT_Error DrawString(uint8_t screen, uint16_t x, uint16_t y, uint8_t font_size, uint16_t color, const char* text)
{
//TODO:why does this need adjusted?
	y = y + 8;
	FT_Error error;
	int num_chars = strlen(text);

	// untransformed origin
	FT_Vector pen = {
		// the pen position in 26.6 cartesian space coordinates;
		// start at(300,200)relative to the upper left corner
		pen.x =  x * 64,
		pen.y = -y * 64
	};

	// Initialize fonts
	error = TTF::Init();
	if(error)
	{
		LOG(DEBUG) << "TTF::Init() ERROR#" << (int)error;
		return error;
	}

	// set character size
	error = FT_Set_Char_Size(face, font_size * 64, 0, 100, 0); 
	if(error)
	{
		LOG(DEBUG) << "FT_Set_Char_Size ERROR#" << (int)error;
		return error;
	}

	FT_GlyphSlot slot = face->glyph;

	for(int n = 0; n < num_chars; n++)
	{
		// set transformation
		FT_Set_Transform(face, &matrix, &pen);

		// load glyph image into the slot(erase previous one)
		error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
		if(error)
		{
			LOG(DEBUG) << "FT_Load_Char ERROR#" << (int)error;
			continue; // ignore errors
		}

		// now, draw to our target surface(convert position)
		TTF::Draw(screen, slot->bitmap_left, -slot->bitmap_top, color, &slot->bitmap);

		// increment pen position
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}

	return false;
}

}}} // namespace D2K::GUI::TTF
