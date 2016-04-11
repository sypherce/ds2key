#pragma once

#include <windows.h>
#include <string>//std::string
#include "object.h"
#include "label.h"

namespace D2K {namespace GUI {

class ComboButton : public Label
{
public:
	ComboButton(int x, int y, int width, int height);
	~ComboButton();
	bool Attach(Object *parentObject);
	void Append(std::string text);
	void SetSelection(unsigned row);
	int GetSelection();
};

}}//namespace D2K::GUI
