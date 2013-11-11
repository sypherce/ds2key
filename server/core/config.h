#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>

namespace D2K {
	namespace Core {
		//Normal Defines
		enum Mouse {
			mRelative, mAbsolute, mButtons,
			mEND };
		enum Keys {
			kProfile, kMouse, kJoy, kUp, kDown, kLeft, kRight, kA, kB, kX, kY, kL, kR, kStart, kSelect, kLid,
			kBlue, kYellow, kRed, kGreen,
			kTouch00, kTouch01, kTouch02, kTouch03, kTouch04, kTouch05, kTouch06, kTouch07, kTouch08, kTouch09, kTouch10, kTouch11,
			kTouch00X, kTouch01X, kTouch02X, kTouch03X, kTouch04X, kTouch05X, kTouch06X, kTouch07X, kTouch08X, kTouch09X, kTouch10X, kTouch11X,
			kTouch00Y, kTouch01Y, kTouch02Y, kTouch03Y, kTouch04Y, kTouch05Y, kTouch06Y, kTouch07Y, kTouch08Y, kTouch09Y, kTouch10Y, kTouch11Y,
			kTouch00W, kTouch01W, kTouch02W, kTouch03W, kTouch04W, kTouch05W, kTouch06W, kTouch07W, kTouch08W, kTouch09W, kTouch10W, kTouch11W,
			kTouch00H, kTouch01H, kTouch02H, kTouch03H, kTouch04H, kTouch05H, kTouch06H, kTouch07H, kTouch08H, kTouch09H, kTouch10H, kTouch11H,
			kEND };
		enum {
			dNone, dDefault, dWarnings, dAll,
			dEND };

		namespace C {
			class Config {
				public:
					Config();
					~Config();
					bool Load();
					bool ReadProfile(uint16_t *Profile, uint8_t profileNumber);
					bool Save();
					bool SaveProfile(uint16_t *Profile, uint8_t profileNumber);
					uint16_t GetPort();
					void SetPort(uint16_t port);
					uint8_t GetDebugLevel();
					char* GetCommand(uint8_t Command);
					const static uint16_t DefaultPort = 9501;

				private:
					uint16_t Port;
					uint8_t Debug;
					char* Commands[256];
			};
		}
		extern C::Config *Config;
	}
}

#endif//__CONFIG_H__
