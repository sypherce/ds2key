#include <string> // std::string
#include <png.h>
#include "common/easylogging++Wrapper.h"
#include "core.h"
#include "edit.h"
#include "gui.h"

namespace D2K {namespace GUI{

// example1.c
//
// This small program shows how to print a rotated string with the 
// FreeType 2 library.

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define WIDTH 320
#define HEIGHT 240

// origin is the upper left corner
uint16_t *image = D2K::GUI::g_screen[0];

// Replace this function with something useful.
void draw_bitmap(FT_Bitmap* bitmap, FT_Int x, FT_Int y)
{
	FT_Int  x_max = x + bitmap->width;
	FT_Int  y_max = y + bitmap->rows;

	for(FT_Int i = x, p = 0; i < x_max; i++, p++)
	{
		for(FT_Int j = y, q = 0; j < y_max; j++, q++)
		{
			if(i < 0
			|| j < 0
			|| i >= WIDTH
			|| j >= HEIGHT)
				continue;

			image[j * bitmap->width + i] |= bitmap->buffer[q * bitmap->width + p];
		}
	}
}

void show_image(void)
{
	int i, j;

	for(i = 0; i < HEIGHT; i++)
	{
		for(j = 0; j < WIDTH; j++)
		{
			putchar(image[j*WIDTH+i] == 0 ? ' '
			: image[j*WIDTH+i] < 128 ? '+'
			: '*');
		}

		putchar('\n');
	}
}

bool show_font()
{
	const double angle = (25.0 / 360)* 3.14159 * 2;                  // use 25 degrees
	const int target_height = HEIGHT;
	const char* filename = "/ds2key/font.ttf";
	const char* text = "Hello World!";
	const int num_chars = strlen(text);

	FT_Error error;
	FT_Library library;
	FT_Face face;
	// transformation matrix
	FT_Matrix matrix = { 
		matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L),
		matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L),
		matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L),
		matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L),
	};

	// untransformed origin
	FT_Vector pen = {    
		// the pen position in 26.6 cartesian space coordinates;
		// start at(300,200)relative to the upper left corner
		pen.x = 300 * 64,
		pen.y = (target_height - 200) * 64
	};

	error = FT_Init_FreeType(&library);                 // initialize library
	// error handling omitted

	error = FT_New_Face(library, filename, 0, &face);   // create face object
	// error handling omitted

	// use 50pt at 100dpi
	error = FT_Set_Char_Size(face, 50 * 64, 0, 100, 0); // set character size
	// error handling omitted

	FT_GlyphSlot slot = face->glyph;

	for(int n = 0; n < num_chars; n++)
	{
		// set transformation
		FT_Set_Transform(face, &matrix, &pen);

		// load glyph image into the slot(erase previous one)
		error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
		if(error)
			continue;     // ignore errors

		// now, draw to our target surface(convert position)
		draw_bitmap(&slot->bitmap,
		slot->bitmap_left,
		target_height - slot->bitmap_top);

		// increment pen position
		pen.x += slot->advance.x;
		pen.y += slot->advance.y;
	}

	show_image();

	FT_Done_Face (face);
	FT_Done_FreeType(library);

	return 0;
}

}} // namespace D2K::GUI
