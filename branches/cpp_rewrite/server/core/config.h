#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>

namespace D2K
{
	namespace Core
	{
		//Normal Defines
		#define DEFAULT_SERVER_PORT 9501
		#define pTouch pTouch00
		enum mType { mRelative = 0, mAbsolute = 1, mButtons = 2 };
		enum pKeys { pProfile, pMouse, pJoy, pUp, pDown, pLeft, pRight, pA, pB, pX, pY, pL, pR, pStart, pSelect, pLid,
					 pBlue, pYellow, pRed, pGreen,
					 pTouch00, pTouch01, pTouch02, pTouch03, pTouch04, pTouch05, pTouch06, pTouch07, pTouch08, pTouch09, pTouch10, pTouch11,
					 pTouch00X, pTouch01X, pTouch02X, pTouch03X, pTouch04X, pTouch05X, pTouch06X, pTouch07X, pTouch08X, pTouch09X, pTouch10X, pTouch11X,
					 pTouch00Y, pTouch01Y, pTouch02Y, pTouch03Y, pTouch04Y, pTouch05Y, pTouch06Y, pTouch07Y, pTouch08Y, pTouch09Y, pTouch10Y, pTouch11Y,
					 pTouch00W, pTouch01W, pTouch02W, pTouch03W, pTouch04W, pTouch05W, pTouch06W, pTouch07W, pTouch08W, pTouch09W, pTouch10W, pTouch11W,
					 pTouch00H, pTouch01H, pTouch02H, pTouch03H, pTouch04H, pTouch05H, pTouch06H, pTouch07H, pTouch08H, pTouch09H, pTouch10H, pTouch11H,
					 pEND };
		enum lLevel { lNone, lDefault, lWarnings, lAll, lEND };

		class Config
		{
			public:
				Config();
				~Config();
				bool Load();
				bool ReadProfile(uint16_t *Profile, uint8_t profileNumber);
				bool ReadProfile(uint8_t profileNumber);
				bool ReadProfileArray(uint8_t profileNumber);
				bool Save();
				bool SaveProfile(uint16_t *Profile, uint8_t profileNumber);
				bool SaveProfile(uint8_t profileNumber);
				uint16_t GetPort();
				void SetPort(uint16_t port);
				uint8_t GetDebugLevel();

			private:
				uint16_t Port;
				uint8_t Debug;
		};
		extern Config *config;
	}
}

#endif//__CONFIG_H__
