#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <windows.h>

namespace D2K {
	namespace GUI {
		class Object {
			public:
				Object(int x, int y, int width, int height);
				~Object();
				virtual bool Attach(Object *parentObject);
				int GetID();
				HWND GetParentHWND();
				Object *GetParent();
				void SetVisible(bool visible = true);
			protected:
				HWND hwnd;               /* This is the handle for our window */
				Object *parent;
				int X;
				int Y;
				int Width;
				int Height;
				int ID;
		};
	}
}
#endif//__OBJECT_H__
