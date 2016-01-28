#pragma once

#include <cstdint>

#include "client.h"

namespace D2K {namespace Config{

	//loads settings from disk
	//@return (0) upon success, else (errno)
	int Load();

	//Saves Profile settings to disk
	//@param Profile Pointer to actual data
	//@param profileNumber Fills in # in ds2key.p#.ini. Example: ds2key.p1.ini
	//@return (0) upon success, else (errno)
	int LoadProfile(ProfileData* profile, uint8_t profile_number);

	//Saves settings to disk
	//@return (0) upon success, else (errno)
	int Save();

	//Saves Profile settings to disk
	//@param Profile Pointer to actual data
	//@param profileNumber Fills in # in ds2key.p#.ini. Example: ds2key.p1.ini
	//@return (0) upon success, else (errno)
	int SaveProfile(ProfileData* profile, uint8_t profile_number);

	//@return Currently assigned port number, values range from 1 - 65535
	uint16_t GetPort();

	//Assigns new port number
	//@param new_port Values range 1 - 65535, setting 0 defaults to 9501
	void SetConfigPort(uint16_t port);//SetPort clashes in windows

}}//namespace D2K::Config
