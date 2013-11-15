#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <string>//std::string
#include "rect.h"

namespace D2K {
	namespace GUI {
		class Object {
			private:
				bool Visible;
				bool Update;
				GUI::Rect Rect;
				uint8_t Screen;

			protected:
				uint8_t Status;//0 = not active, 1 = pressed, 2 = held
				void SetUpdate(bool value);
				bool IsUpdated();
				void Clear(uint16_t c);

			public:
				Object(uint8_t screen, GUI::Rect rect);
				virtual ~Object();
				virtual bool Draw();
				void (*Function)();
				virtual void SetText(std::string text);
				virtual std::string GetText();
				void SetVisible(bool visible);
				virtual void SetStatus(uint8_t value);
				uint8_t GetStatus();
				bool IsVisible();
				bool IsClicked(uint8_t x, uint8_t y);
				bool GetScreen();
				GUI::Rect GetRect();
				void SetRect(GUI::Rect rect);
		};
	}
}

#endif//__OBJECT_H__
