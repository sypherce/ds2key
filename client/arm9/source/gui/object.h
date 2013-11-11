#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "gui.h"			//Rect

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
				void setUpdate(bool value);
				bool isUpdated();
				void Clear(uint16_t C);

			public:
				Object(uint8_t Screen, GUI::Rect Rect);
				virtual ~Object();
				virtual bool Draw();
				void (*Function)();
				virtual void setText(std::string Text);
				virtual std::string getText();
				void setVisible(bool Visible);
				virtual void setStatus(uint8_t Value);
				uint8_t getStatus();
				bool isVisible();
				bool isClicked(uint8_t X, uint8_t Y);
				bool getScreen();
				GUI::Rect getRect();
				void setRect(GUI::Rect rect);
		};
	}
}

#endif//__OBJECT_H__
