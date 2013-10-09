#ifndef __INPUT_H__
#define __INPUT_H__

#ifdef WIN32
#include "PPJoy.h"
#else//LINUX
#include <X11/extensions/XTest.h>
#endif//WIN32
#include <stdint.h>

//Function Defines
#define isExtended(key) \
((key == VK_INSERT) || \
(key == VK_DELETE) || \
(key == VK_HOME) || \
(key == VK_END) || \
(key == VK_PRIOR) || \
(key == VK_NEXT) || \
(key == VK_NUMLOCK) || \
(key == VK_PAUSE) || \
(key == VK_PRINT) || \
(key == VK_DIVIDE) || \
(key == VK_RCONTROL) || \
(key == VK_UP) || \
(key == VK_DOWN) || \
(key == VK_LEFT) || \
(key == VK_RIGHT))

namespace D2K {
	namespace Core {
		class Input {
			public:
				Input();
				~Input();
				void Press(uint16_t key, unsigned char joy);
				void Release(uint16_t key, unsigned char joy);
				void Move(signed long int X, signed long int Y);
				void MoveAbsolute(signed long int X, signed long int Y);

			private:
		#ifdef WIN32
				PPJoy *ppjoy[16];
		#else//LINUX
				Display *display;
		#endif//WIN32
				void Keyboard(uint16_t key, bool state);
				void Mouse(unsigned short type, signed long int X, signed long int Y);
		};

		extern Input *input;
	}
}
#endif//__UDP_H__
