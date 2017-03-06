#pragma once

#include <string> // std::string
#include "rect.h"

namespace D2K {namespace GUI {

enum OBJECT_STATUS : uint8_t
{
	IDLE,
	PRESSED,
	HELD,
};

class Object
{
public:
	Object(uint8_t screen, GUI::Rect rect);
	virtual ~Object();

	// Check's if object or GUI has been updated, then clears value
	// return: (true) if updated, else (false)
	virtual bool Draw();
	void (*Function)();

	virtual void SetText(std::string text);
	virtual std::string GetText();

	// sets visibility to (visible) and forces an update
	virtual void SetVisible(bool visible);

	// sets status to (value) and forces an update
	virtual void SetStatus(uint8_t value);
	uint8_t GetStatus();

	bool IsVisible();
	// checks if object is clicked at (x), (y)
	// return: (true) if clicked, (false) if not
	virtual bool IsClicked(uint16_t x, uint16_t y);
	// return: current screen of object

	bool GetScreen();

	GUI::Rect GetRect();
	void SetRect(GUI::Rect rect);

	uint16_t GetColor();
	void SetColor(uint16_t color);

protected:
	uint8_t m_status{}; // possible values 0-2

	void SetUpdate(bool value);
	bool IsUpdated();
	// Clears entire area with color (color)
	void Clear(uint16_t color);
	// Clears entire area with the background image
	void Clear();

private:
	uint16_t m_color{UINT16_MAX}; // color used by Clear function
	bool m_visible;
	bool m_update;
	GUI::Rect m_rect;
	uint8_t m_screen;
};

}} // namespace D2K::GUI
