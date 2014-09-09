#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <string>//std::string
#include "rect.h"


namespace D2K {
	namespace GUI {
		static const uint8_t ObjectStatusIdle = 0;
		static const uint8_t ObjectStatusPressed = 1;
		static const uint8_t ObjectStatusHeld = 2;
		class Object {
			private:
				bool Visible;
				bool Update;
				GUI::Rect Rect;	
				uint8_t Screen;	

			protected:
				uint8_t Status;//possible values 0-2
				void SetUpdate(bool value);
				bool IsUpdated();
				//Clears entire screen with color (c)
				void Clear(uint16_t c);

			public:
				Object(uint8_t screen, GUI::Rect rect);
				virtual ~Object();
				//Check's if object or GUI has been updated, then clears value
				//@return (true) if updated, else (false)
				virtual bool Draw();
				void (*Function)();
				virtual void SetText(std::string text);
				virtual std::string GetText();
				//sets visibility to (visible) and forces an update
				void SetVisible(bool visible);
				//sets status to (value) and forces an update
				virtual void SetStatus(uint8_t value);
				uint8_t GetStatus();
				bool IsVisible();
				//checks if object is clicked at (x), (y)
				//@return (true) if clicked, (false) if not
				bool IsClicked(uint8_t x, uint8_t y);
				//@return current screen of object
				bool GetScreen();
				GUI::Rect GetRect();
				void SetRect(GUI::Rect rect);
		};
	}
}

#endif//__OBJECT_H__
