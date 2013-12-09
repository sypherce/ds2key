#ifndef __MISC_H__
#define __MISC_H__

#include <cstdint>	//uint16_t
#include <string>	//std::string

namespace D2K {
	namespace Core {
		#ifdef WIN32GUI
		const static std::string VERSION_STRING = "DS2Key (GUI) SVN Build";
		#elif defined(_WIN32)
		const static std::string VERSION_STRING = "DS2Key (Console) SVN Build";
		#elif defined(__linux__)
		const static std::string VERSION_STRING = "DS2Key (Linux) SVN Build";
		#elif defined(ARM9)
		const static std::string VERSION_STRING = "DS2Key (DS) SVN Build";
		#endif//WIN32GUI

		//Default port
		const static uint16_t DefaultPort = 9501;

		//@return converts long into std::string
		extern std::string itos (long t);

		//@return converts long into const char*
		extern const char *itoa (long t);

		//@return converts std::string into long
		extern long stoi(std::string str);
	}
}

#endif//__MISC_H__
