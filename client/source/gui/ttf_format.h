#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

namespace D2K {namespace GUI {namespace TTF{

FT_Error DrawString(uint8_t screen, uint16_t x, uint16_t y, uint8_t font_size, uint16_t color, const char* text);

}}} // namespace D2K::GUI::TTF
