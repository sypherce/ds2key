#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <cstdint>
#undef SetPort

namespace D2K {
	namespace Core {
		//this represents each value of Profile[kMouse]. 0 = mRelative and so on
		enum Mouse {
			mRelative, mAbsolute, mButtons,
			mEND };
		//this represents each value in a Keys Profile array. Example: Profile[kUp] is defined as KEY_UP by default
		enum Keys {
			kProfile, kMouse, kJoy, kUp, kDown, kLeft, kRight, kA, kB, kX, kY, kL, kR, kStart, kSelect, kLid,
			kBlue, kYellow, kRed, kGreen,
			kTouch00, kTouch01, kTouch02, kTouch03, kTouch04, kTouch05, kTouch06, kTouch07, kTouch08, kTouch09, kTouch10, kTouch11,
			kTouch00X, kTouch01X, kTouch02X, kTouch03X, kTouch04X, kTouch05X, kTouch06X, kTouch07X, kTouch08X, kTouch09X, kTouch10X, kTouch11X,
			kTouch00Y, kTouch01Y, kTouch02Y, kTouch03Y, kTouch04Y, kTouch05Y, kTouch06Y, kTouch07Y, kTouch08Y, kTouch09Y, kTouch10Y, kTouch11Y,
			kTouch00W, kTouch01W, kTouch02W, kTouch03W, kTouch04W, kTouch05W, kTouch06W, kTouch07W, kTouch08W, kTouch09W, kTouch10W, kTouch11W,
			kTouch00H, kTouch01H, kTouch02H, kTouch03H, kTouch04H, kTouch05H, kTouch06H, kTouch07H, kTouch08H, kTouch09H, kTouch10H, kTouch11H,
			kEND };

		namespace C {
			class Config {
				public:
					//Assigns each Command pointer to NULL, then calls Load()
					Config();
					
					//Calls Save()
					~Config();
					
					//loads settings from disk
					//@return (0) upon success, else (errno)
					int Load();
					
					//Saves Profile settings to disk
					//@param Profile Pointer to actual data
					//@param profileNumber Fills in # in ds2key.p#.ini. Example: ds2key.p1.ini
					//@return (0) upon success, else (errno)
					int LoadProfile(uint16_t *Profile, uint8_t profileNumber);
					
					//Saves settings to disk
					//@return (0) upon success, else (errno)
					int Save();
					
					//Saves Profile settings to disk
					//@param Profile Pointer to actual data
					//@param profileNumber Fills in # in ds2key.p#.ini. Example: ds2key.p1.ini
					//@return (0) upon success, else (errno)
					int SaveProfile(uint16_t *Profile, uint8_t profileNumber);
					
					//@return Currently assigned port number, values range from 1 - 65535
					uint16_t GetPort();
					
					//Assigns new port number
					//@param Port Values range 1 - 65535, setting 0 defaults to DefaultPort
					void SetPort(uint16_t port);
					
					//@param Command Values range 0 - 255
					//@return (NULL) if unassigned, otherwise a NULL terminated string
					char* GetCommand(uint8_t Command);

				private:
					uint16_t Port;//Currently assigned port
					char* Commands[256];//NULL terminated command pointers
			};
		}
		extern C::Config *Config;
	}
}

#endif//__CONFIG_H__
