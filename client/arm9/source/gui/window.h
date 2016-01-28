#pragma once

#include <vector>
#include "object.h"

namespace D2K {namespace GUI {

class Window
{
public:
	//Initializes to (screen) 0 and sets visibility to false
	Window();
	//Deletes all attached child objects
	~Window();

	//Add's (object) to window
	void AppendObject(Object* object);
	//Removes (object) from window and delete's from memory
	void RemoveObject(Object* object);

	bool m_screen;
	//Draws windows and all child objects

	void Draw();

	//Sets window, and all child objects visibility
	//@param visible true if visible, false if not
	void SetVisible(bool visible);
	bool IsVisible();

	//@param object Pointer to object to be checked
	//@return (true) if clicked, (false) if not
	bool CheckClick(Object* object);

	bool Update();

private:
	std::vector<GUI::Object*> m_objects;
	bool m_visible;
};

}}//namespace D2K::GUI
