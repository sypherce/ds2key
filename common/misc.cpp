#include "misc.h"
#include <sstream>	//std::stringstream

namespace D2K {
	namespace Core {
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
}
