#pragma once

#include <stdint.h>
#include <string>//std::string
#include "rect.h"

#if defined(_3DS)
//this is all patchwork right now
#include <stdint.h>
#define SCREEN_HEIGHT 192
#define SCREEN_WIDTH 256
#define _3DS_SCREEN_WIDTH 320
#define _3DS_SCREEN_HEIGHT 240
#define ARGB16(a, r, g, b) (((a) << 15) | (r) | ((g) << 5) | ((b) << 10))

//guitar grip dummy defines
#define guitarGripKeysHeld() ((uint8_t)0) 
#define guitarGripIsInserted() ((bool)false)

#define GUITARGRIP_GREEN BIT(6)
#define GUITARGRIP_RED BIT(5)
#define GUITARGRIP_YELLOW BIT(4)
#define GUITARGRIP_BLUE BIT(3)

#endif

namespace D2K {namespace GUI {

extern void VoidFunction();
extern uint16_t* g_screen[2];
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
