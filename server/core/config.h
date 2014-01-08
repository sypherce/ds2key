#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <cstdint>

#include "client.h"
#undef SetPort

namespace D2K {
	namespace Core {
		//this represents each value of Profile[kMouse]. 0 = mRelative and so on
		enum Mouse {
			mRelative, mAbsolute, mButtons,
			mEND };

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
					int LoadProfile(ProfileData *Profile, uint8_t profileNumber);
					
					//Saves settings to disk
					//@return (0) upon success, else (errno)
					int Save();
					
					//Saves Profile settings to disk
					//@param Profile Pointer to actual data
					//@param profileNumber Fills in # in ds2key.p#.ini. Example: ds2key.p1.ini
					//@return (0) upon success, else (errno)
					int SaveProfile(ProfileData *Profile, uint8_t profileNumber);
					
					//@return Currently assigned port number, values range from 1 - 65535
					uint16_t GetPort();
					
					//Assigns new port number
					//@param Port Values range 1 - 65535, setting 0 defaults to 9501
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
