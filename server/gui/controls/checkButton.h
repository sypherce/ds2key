#pragma once

#include <string>//std::string
#include "label.h"

namespace D2K {namespace GUI {

class CheckButton : public Label
{
public:
	CheckButton(std::string text, int x, int y, int width, int height);
	~CheckButton();
	bool Attach(Object *parentObject);
	void SetChecked(bool checked);
};

}}//namespace D2K::GUI
