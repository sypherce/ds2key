#pragma once

#include <string>  // std::string
#include "rect.h"
#include "label.h"

namespace D2K {namespace GUI {

class Button : public Label
{
public:
	//(screen), (rect), and (text) are setup by calling the label function.
	//(rect) is then adjusted to fit the contents of (text).
	//(function) is assigned here, and is called when "Button" is clicked.
	Button(uint8_t screen, GUI::Rect rect, std::string text, void (*function)());
	~Button();
	//Draws Button onto screen if it OR the gui has been updated
	//@return true if we updated, false if not
	bool Draw();
};

}}//namespace D2K::GUI
