#pragma once

#include <string> // std::string
#include "rect.h"
#include "label.h"

namespace D2K {namespace GUI {

class LetterButton : public Label
{
public:
	// (screen), (rect), and (text) are setup by calling the label function.
	// (sub_text) is setup in this function
//TODO: adjust (rect) to text, sub_text, and the letter button
	// (rect) is then adjusted to fit the contents of (text).
	// (function) is assigned here, and is called when "LetterButton" is clicked.
	LetterButton(uint8_t screen, GUI::Rect rect, char letter, std::string text, std::string sub_text, void (*function)());
	~LetterButton();
	// Draws Button onto screen if it OR the gui has been updated
	// return: true if we updated, false if not
	bool Draw();

	// set Sub Text to (sub_text) and force update
	void SetSubText(std::string sub_text);
	std::string GetSubText();

protected:
	uint8_t m_letter;
	std::string m_sub_text;
};

}} // namespace D2K::GUI
