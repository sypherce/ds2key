#pragma once

#include <windows.h>
#include <string>//std::string
#include "object.h"

namespace D2K {namespace GUI {

class Label : public Object
{
public:
	Label(std::string text, int x, int y, int width, int height);
	virtual ~Label();
	bool Attach(Object *parentObject);
	std::string GetText();
	void SetText(std::string text);
	void SetEnabled(bool enabled);
	long GetStyle();
	long SetStyle(long style);
protected:
	std::string Text;
};

}}//namespace D2K::GUI
