#pragma once

#include <cstdint>

#include "client.h"

namespace D2K {namespace Config{

	//Loads settings from disk
	//@return (0) upon success, else (errno)
	int Load();

	//Loads Profile settings to disk
	//@param profile Pointer to actual data
	//@param profile_number Fills in # in ds2key.p#.ini. Example: ds2key.p1.ini
	//@return (0) upon success, else (errno)
	int LoadProfile(ProfileData* profile, uint8_t profile_number);

	//Saves settings to disk
	//@return (0) upon success, else (errno)
	int Save();

	//Saves Profile settings to disk
	//@param profile Pointer to actual data
	//@param profile_number Fills in # in ds2key.p#.ini. Example: ds2key.p1.ini
	//@return (0) upon success, else (errno)
	int SaveProfile(ProfileData* profile, uint8_t profile_number);

	//TODO: this will change
	int SetProfileSetting(ProfileData* Profile, uint8_t profileNumber, uint16_t setting, uint16_t value);

	//@return Currently assigned port number, values range from 1 - 65535
	uint16_t GetPort();

	//Assigns new port number
	//@param new_port Values range 1 - 65535, setting 0 defaults to 9501
	void SetConfigPort(uint16_t port);//SetPort clashes in windows

	//Load a client by profile id
	//@param profile Values range 0 - 255
	//@return (Client*) automatically loads profile into memory if needed
	Client* GetClient(uint8_t profile);

}}//namespace D2K::Config
