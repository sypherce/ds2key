#pragma once

#include <cstdint>

#include "client.h"

namespace D2K
{
namespace Config
{
	//loads settings from disk
	//@return (0) upon success, else (errno)
	int load();

	//Saves Profile settings to disk
	//@param Profile Pointer to actual data
	//@param profileNumber Fills in # in ds2key.p#.ini. Example: ds2key.p1.ini
	//@return (0) upon success, else (errno)
	int load_profile(ProfileData* profile, uint8_t profile_number);

	//Saves settings to disk
	//@return (0) upon success, else (errno)
	int save();

	//Saves Profile settings to disk
	//@param Profile Pointer to actual data
	//@param profileNumber Fills in # in ds2key.p#.ini. Example: ds2key.p1.ini
	//@return (0) upon success, else (errno)
	int save_profile(ProfileData* profile, uint8_t profile_number);

	//@return Currently assigned port number, values range from 1 - 65535
	uint16_t get_port();

	//Assigns new port number
	//@param new_port Values range 1 - 65535, setting 0 defaults to 9501
	void set_port(uint16_t new_port);
}
}
