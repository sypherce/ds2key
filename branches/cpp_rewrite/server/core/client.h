#ifndef HEADER_Client
#define HEADER_Client

#include <string>		//std::string
//#include <stdint.h>	//included in config.h
#include "config.h"

namespace D2K
{
	namespace Core {
		#define BIT(n) (1 << (n))
		#define DS2KEY_A BIT(0)
		#define DS2KEY_B BIT(1)
		#define DS2KEY_SELECT BIT(2)
		#define DS2KEY_START BIT(3)
		#define DS2KEY_RIGHT BIT(4)
		#define DS2KEY_LEFT BIT(5)
		#define DS2KEY_UP BIT(6)
		#define DS2KEY_DOWN BIT(7)
		#define DS2KEY_R BIT(8)
		#define DS2KEY_L BIT(9)
		#define DS2KEY_X BIT(10)
		#define DS2KEY_Y BIT(11)
		#define DS2KEY_TOUCH BIT(12)
		#define DS2KEY_LID BIT(13)
		#define GH_BLUE	0x08
		#define GH_YELLOW 0x10
		#define GH_RED 0x20
		#define GH_GREEN 0x40

		#pragma pack(1)
		typedef struct ds2keyPacket
		{
			uint8_t type;
			uint8_t profile;
			uint16_t keys;
			uint16_t keysTurbo;
			uint8_t ghKeys;
			uint16_t ghKeysTurbo;
			uint8_t touchX;
			uint8_t touchY;
		} ds2keyPacket;
		#pragma pack()
		class Client
		{
			public:
				Client();
				~Client();
				void Scan(void);
				uint16_t *GetSettings();
				std::string GetButtonString(int Button);
				uint16_t GetButton(int Button);
				void SetButton(int Button, uint16_t Value);
				void SetPacket(ds2keyPacket p);
				uint16_t Held(uint16_t key);
				uint16_t Down(uint16_t key);
				uint16_t Up(uint16_t key);
				uint16_t Turbo(uint16_t key);
				uint8_t GetX();
				uint8_t GetY();

				uint8_t GetProfileNumber();
		private:
				void SetTouchPos(uint8_t x, uint8_t y);
				void Press(uint16_t key);
				void Release(uint16_t key);

				void GHPress(uint8_t key);
				void GHRelease(uint8_t key);
				uint8_t GHHeld(uint8_t key);
				uint8_t GHDown(uint8_t key);
				uint8_t GHUp(uint8_t key);
				uint8_t GHTurbo(uint8_t key);

		private:
				ds2keyPacket packet;
				uint16_t Profile[pEND];
			private:
				uint16_t keys;
				uint16_t keysOld;
				uint8_t ghKeys;
				uint8_t ghKeysOld;
		};
	}
}

#endif//HEADER_Client
