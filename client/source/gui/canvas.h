#pragma once

#include <string> // std::string
#include "rect.h"
#include "label.h"

namespace D2K {namespace GUI {

class Canvas : public Object
{
protected:
	// This is where we actually keep the image
	uint16_t* m_buffer{};
	uint16_t m_w;
	uint16_t m_h;
public:
	// (screen), (rect), and (text) are setup by calling the label function.
	// (sub_text) is setup in this function
// TODO: adjust documentation
	// this is done differently than every other object
	// BECAUSE: it draws a specific image. it gets corrupted when using the Rect scaling on the 3ds
	Canvas(uint8_t screen, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
	~Canvas();
	// Draws Button onto screen if it OR the gui has been updated
	// return: true if we updated, false if not
	bool Draw();
// Doesn't respond to clicks, always returns false
	bool IsClicked(uint16_t x, uint16_t y);
// TODO: comment
//is this correct? it should be read-only
	const uint16_t* GetBuffer();
// TODO: comment
//is this correct? buffer should be read-only
	bool SetBuffer(const uint16_t* buffer);
};

}} // namespace D2K::GUI
