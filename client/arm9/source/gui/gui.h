#pragma once

#include <stdint.h>
#include <string>//std::string
#include "rect.h"

namespace D2K {namespace GUI {

extern void VoidFunction();
extern uint16_t *Screen[2];
extern bool Update;
void SetUpdate(bool value);
bool IsUpdated();
void SetPixel(uint8_t screen, uint8_t x, uint8_t y, uint16_t c);
void ClearScreen(uint8_t screen, uint16_t c);
void DrawRect(uint8_t screen, GUI::Rect rect, uint16_t c);
void DrawFilledRect(uint8_t screen, GUI::Rect rect, uint16_t c);
void DrawLine(uint8_t screen, std::string text, uint8_t x, uint8_t y, uint16_t c);
void DrawLetter(uint8_t screen, char letter, uint8_t x, uint8_t y, uint16_t c);
void DrawString(uint8_t screen, std::string text, uint8_t x, uint8_t y, uint16_t c);
enum { 	colorBackground,
		colorLabelText, colorLabelBackground,
		colorButtonText, colorButtonBackground, colorButtonOutline, colorButtonOutlineActive,
		colorEditText, colorEditBackground, colorEditOutline, colorEditOutlineActive,
		colorMax};
extern uint16_t Color[colorMax];

}}//namespace D2K::GUI
