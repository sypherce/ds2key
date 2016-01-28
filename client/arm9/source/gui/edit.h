#pragma once

#include <string>	//std::string
#include "rect.h"
#include "button.h"

namespace D2K {namespace GUI {

class Edit : public Button
{
public:
	//(screen), (rect), (text), and (function) are setup by calling the Button function
	Edit(uint8_t screen, GUI::Rect rect, std::string text, void (*function)());
	~Edit();
	//Draws Edit onto screen if it OR the gui has been updated
	//@return true if we updated, false if not
	bool Draw();
};

}}//namespace D2K::GUI
