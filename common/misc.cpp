#include "misc.h"
#include <sstream>  // std::stringstream

namespace D2K {

#ifdef WIN32GUI
const std::string VERSION_STRING = "DS2Key (GUI) SVN Build";
#elif defined(_WIN32)
const std::string VERSION_STRING = "DS2Key (Console) SVN Build";
#elif defined(__linux__)
const std::string VERSION_STRING = "DS2Key (Linux) SVN Build";
#elif defined(ARM9)
const std::string VERSION_STRING = "DS2Key (DS) SVN Build";
#endif

std::string ltos(long i)
{
	std::stringstream stream;
	stream << i;

	return stream.str();
}

const char* ltoa(long t)
{
	return ltos(t).c_str();
}

long stol(std::string str)
{
	std::stringstream stream(str);
	long return_value;

	return !(stream >> return_value) ? 0 : return_value;
}

uint8_t string_to_uint8_t(std::string str)
{
	long return_value = D2K::stol(str);
	// Clamp to uint8_t
	if (return_value > 255
	|| return_value < 0)
		return_value = 0;

	return (uint8_t)D2K::stol(str);
}

uint16_t string_to_uint16_t(std::string str)
{
	long return_value = D2K::stol(str);
	// Clamp to uint16_t
	if (return_value > 65535
	|| return_value < 0)
		return_value = 0;

	return (uint16_t)D2K::stol(str);
}

}//namespace D2K
