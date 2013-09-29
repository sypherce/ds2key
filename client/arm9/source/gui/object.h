#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <nds/ndstypes.h>	//u8
#include "gui.h"	//Rect

namespace D2K {
	namespace GUI {
		enum { ObjectObject, ObjectLabel, ObjectButton, ObjectEdit };
		class Object
		{
			private:
				bool Visible;
				bool Update;
			protected:
				GUI::Rect *Rect;
				u8 Screen;
				u8 Type;
			public:
				Object(u8 Screen, GUI::Rect *Rect);
				~Object();
				bool Draw();
				void Clear(uint16_t C);
				void (*function)();
				void setVisible(bool Visible);
				bool isVisible();
				bool isClicked(uint8_t X, uint8_t Y);
				void setUpdate(bool value);
				bool isUpdated();
				u8 getType();
				GUI::Rect *getRect();
				void setRect(GUI::Rect *rect);
		};
	}
}

#endif//__OBJECT_H__
