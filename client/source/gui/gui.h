#pragma once

#include <stdint.h>
#include <string>//std::string
#include "rect.h"

namespace D2K {namespace GUI {

//TODO Clean up SCREEN_WIDTH, _3DS_SCREEN_WIDTH 
//when I know what I am wanting as a standard
#ifdef _3DS
const uint16_t SCREEN_HEIGHT      = 192;
const uint16_t SCREEN_WIDTH       = 256;
const uint16_t _3DS_SCREEN_WIDTH  = 320;
const uint16_t _3DS_SCREEN_HEIGHT = 240;
#define ARGB16(a, r, g, b) (((a) << 15) | (r) | ((g) << 5) | ((b) << 10))
#endif

#if defined(_NDS)
extern const uint16_t MAX_X;
extern const uint16_t MAX_Y;
#elif defined(_3DS)
extern const uint16_t MAX_X;
extern const uint16_t MAX_Y;
#endif

extern void VoidFunction();
extern uint16_t* g_screen[2];
void SetUpdate(bool value);
bool IsUpdated();
void SetPixel(uint8_t screen, uint16_t x, uint16_t y, uint16_t c);
void ClearScreen(uint8_t screen, uint16_t c);
void DrawRect(uint8_t screen, GUI::Rect rect, uint16_t c);
void DrawFilledRect(uint8_t screen, GUI::Rect rect, uint16_t c);
void DrawLine(uint8_t screen, std::string text, uint16_t x, uint16_t y, uint16_t c);
void DrawLetter(uint8_t screen, char letter, uint16_t x, uint16_t y, uint16_t c);
void DrawString(uint8_t screen, std::string text, uint16_t x, uint16_t y, uint16_t c);
enum { 	COLOR_BACKGROUND,
		COLOR_LABEL_TEXT, COLOR_LABEL_BACKGROUND,
		COLOR_BUTTON_TEXT, COLOR_BUTTON_BACKGROUND, COLOR_BUTTON_OUTLINE, COLOR_BUTTON_OUTLINE_ACTIVE,
		COLOR_EDIT_TEXT, COLOR_EDIT_BACKGROUND, COLOR_EDIT_OUTLINE, COLOR_EDIT_OUTLINE_ACTIVE,
		colorMax};
extern uint16_t Color[colorMax];

}}//namespace D2K::GUI
