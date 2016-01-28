#pragma once

#include <stdint.h>
#include <string>//std::string
#include "rect.h"

namespace D2K {namespace GUI {

extern void VoidFunction();
extern uint16_t* g_screen[2];
extern bool m_update;
void SetUpdate(bool value);
bool IsUpdated();
void SetPixel(uint8_t screen, uint8_t x, uint8_t y, uint16_t c);
void ClearScreen(uint8_t screen, uint16_t c);
void DrawRect(uint8_t screen, GUI::Rect rect, uint16_t c);
void DrawFilledRect(uint8_t screen, GUI::Rect rect, uint16_t c);
void DrawLine(uint8_t screen, std::string text, uint8_t x, uint8_t y, uint16_t c);
void DrawLetter(uint8_t screen, char letter, uint8_t x, uint8_t y, uint16_t c);
void DrawString(uint8_t screen, std::string text, uint8_t x, uint8_t y, uint16_t c);
enum { 	COLOR_BACKGROUND,
		COLOR_LABEL_TEXT, COLOR_LABEL_BACKGROUND,
		COLOR_BUTTON_TEXT, COLOR_BUTTON_BACKGROUND, COLOR_BUTTON_OUTLINE, COLOR_BUTTON_OUTLINE_ACTIVE,
		COLOR_EDIT_TEXT, COLOR_EDIT_BACKGROUND, COLOR_EDIT_OUTLINE, COLOR_EDIT_OUTLINE_ACTIVE,
		colorMax};
extern uint16_t Color[colorMax];

}}//namespace D2K::GUI
