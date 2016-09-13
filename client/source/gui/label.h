#pragma once

#include <string> // std::string
#include "object.h"

namespace D2K {namespace GUI {

class Label : public Object
{
protected:
	std::string m_text;
public:
	// (screen), and (rect) are setup by calling the object function.
	// SetText(text) is called.
	// (rect) is adjusted to fit the contents of (text), then reassigned
	Label(uint8_t screen, GUI::Rect rect, std::string text);
	~Label();

	// set Text to (text) and force update
	void SetText(std::string text);
	std::string GetText();

	// Draws Label onto screen if it OR the gui has been updated
	// return: true if we updated, false if not
	bool Draw();
};

}} // namespace D2K::GUI
