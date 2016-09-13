#pragma once

#include <string> // std::string
#include "rect.h"
#include "button.h"

namespace D2K {namespace GUI {

class CheckButton : public Button
{
public:
	// (screen), (rect), (text), and (function) are setup by calling the Button function
	// SetChecked(false) is called.
	CheckButton(uint8_t screen, GUI::Rect rect, std::string text, void (*function)());
	~CheckButton();

	// Draws CheckButton onto screen if it OR the gui has been updated
	// return:  true if we updated, false if not
	bool Draw();

	// return:  (true) if checked, (false) if not
	bool GetChecked();
	// sets state to (checked) and forces update
	void SetChecked(bool checked);
	// toggles checked state forces update
	void ToggleChecked();

private:
	bool m_checked;
};

}} // namespace D2K::GUI
