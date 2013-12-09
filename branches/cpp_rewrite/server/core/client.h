#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>		//std::string
#include <cstdint>		//uint8_t, etc
#include "config.h"		//pEND
#include "common/ds2keyPacket.h"

namespace D2K {
	namespace Core {
		static const uint16_t DS2KEY_A = (1 << (0));
		static const uint16_t DS2KEY_B = (1 << (1));
		static const uint16_t DS2KEY_SELECT = (1 << (2));
		static const uint16_t DS2KEY_START = (1 << (3));
		static const uint16_t DS2KEY_RIGHT = (1 << (4));
		static const uint16_t DS2KEY_LEFT = (1 << (5));
		static const uint16_t DS2KEY_UP = (1 << (6));
		static const uint16_t DS2KEY_DOWN = (1 << (7));
		static const uint16_t DS2KEY_R = (1 << (8));
		static const uint16_t DS2KEY_L = (1 << (9));
		static const uint16_t DS2KEY_X = (1 << (10));
		static const uint16_t DS2KEY_Y = (1 << (11));
		static const uint16_t DS2KEY_TOUCH = (1 << (12));
		static const uint16_t DS2KEY_LID = (1 << (13));
		static const uint8_t DS2KEY_BLUE = (1 << (3));
		static const uint8_t DS2KEY_YELLOW = (1 << (4));
		static const uint8_t DS2KEY_RED = (1 << (5));
		static const uint8_t DS2KEY_GREEN = (1 << (6));
		//probably not the best place for button2bit
		extern uint16_t button2bit(uint16_t Button);
		extern uint16_t bit2button(uint16_t Bit);

		namespace C {
			//Contains a client's current state
			class Client {
				public:
					Client();
					~Client();

					//Updates current button state after calls made to Press, Release, GHPress, and GHRelease
					void Scan(void);

					//@return Pointer to Profile array
					uint16_t *GetSettings();

					//@param Button (enum Keys) value. Example: kA, kUp, kGreen
					//@return Human readable version of (Button)'s current setting. Example: KEY_A, KEY_UP
					std::string GetButtonString(int Button);

					//@param Button (enum Keys) value. Example: kA, kUp, kGreen
					//@return Platform specific virtual key value
					uint16_t GetButton(int Button);

					//@param Button (enum Keys) value. Example: kA, kUp, kGreen
					//@param Value Platform specific virtual key value 
					void SetButton(int Button, uint16_t Value);

					void SetPacket(DS2KeyPacket p);
					bool Held(uint16_t key);
					bool Down(uint16_t key);
					bool Up(uint16_t key);

					//@return true if (key) has Turbo mode enabled
					bool Turbo(uint16_t key);
					
					bool GHHeld(uint8_t key);
					bool GHDown(uint8_t key);
					bool GHUp(uint8_t key);

					//@return true if (key) has Turbo mode enabled
					bool GHTurbo(uint8_t key);
					
					//@return Stylus current X Position. Values range 0-255
					uint8_t GetX();

					//@return Stylus current X Position. Values range 0-191
					uint8_t GetY();

				private:
					uint8_t GetProfileNumber();
					void SetTouchPos(uint8_t x, uint8_t y);
					void Press(uint16_t key);
					void Release(uint16_t key);

					void GHPress(uint8_t key);
					void GHRelease(uint8_t key);

					DS2KeyPacket packet;
					uint16_t Profile[kEND];

					uint16_t keys;
					uint16_t keysOld;
					uint8_t ghKeys;
					uint8_t ghKeysOld;
			};
		}
		extern C::Client *Client[256];
	}
}

#endif//__CLIENT_H__
