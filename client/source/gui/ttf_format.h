#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

namespace D2K {namespace GUI {namespace TTF{

enum FontFaceType { FONT_REGULAR, FONT_BOLD, FONT_ITALIC, FONT_BOLD_ITALIC, FACE_TYPE_MAX };
enum FontSize{ FONT_SIZE_REGULAR = 7, FONT_SIZE_TITLE = 12, FONT_SIZE_BUTTON_IMAGE = 10 };

FT_Error DrawString(uint8_t screen, uint16_t x, uint16_t y, uint8_t font_size, uint8_t face_type, uint16_t color, const char* text);

}}} // namespace D2K::GUI::TTF
