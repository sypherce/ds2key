#include "misc.h"
#include <sstream>	//std::stringstream

namespace D2K {
	#ifdef WIN32GUI
	const std::string VERSION_STRING = "DS2Key (GUI) SVN Build";
	#elif defined(_WIN32)
	const std::string VERSION_STRING = "DS2Key (Console) SVN Build";
	#elif defined(__linux__)
	const std::string VERSION_STRING = "DS2Key (Linux) SVN Build";
	#elif defined(ARM9)
	const std::string VERSION_STRING = "DS2Key (DS) SVN Build";
	#endif//WIN32GUI

	std::string itos(long i) {
		std::stringstream stream;
		stream << i;

		return stream.str();
	}

	const char *itoa(long t) {
		return itos(t).c_str();
	}

	long stoi(std::string str) {
		std::stringstream stream(str);
		long retVal;
		return !(stream >> retVal) ? 0 : retVal;
	}
}
