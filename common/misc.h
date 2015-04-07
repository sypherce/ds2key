#ifndef __MISC_H__
#define __MISC_H__

#include <cstdint>	//uint16_t
#include <string>	//std::string

namespace D2K {
	extern const std::string VERSION_STRING;

	//Default port
	const static uint16_t DefaultPort = 9501;

	//@return converts long into std::string
	extern std::string itos (long t);

	//@return converts long into const char*
	extern const char *itoa (long t);

	//@return converts std::string into long
	extern long stoi(std::string str);
}

#endif//__MISC_H__
