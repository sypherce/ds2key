#pragma once

#include <windows.h>
#include <shellapi.h>
#include <string>//std::string
#include "object.h"
#include "label.h"

namespace D2K {namespace GUI {

class TrayIcon : public Label
{
public:
	TrayIcon(std::string text);
	~TrayIcon();
	bool Attach(Object *parentObject);
	void SetText(std::string Text);
	void Delete();

private:
	NOTIFYICONDATA niData;
};

}}//namespace D2K::GUI
