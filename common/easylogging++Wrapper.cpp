#include "easylogging++Wrapper.h"

namespace D2K {

void InitLogging(int argc, char* argv[])
{
	const char* LOGGING_CONFIG = "\
* GLOBAL:\n \
FORMAT=\"[%datetime{%H:%m:%s}] %msg\"\n \
FILENAME=\"./ds2key.log\"\n \
ENABLED=true\n \
TO_FILE=true\n \
TO_STANDARD_OUTPUT=true\n \
MILLISECONDS_WIDTH=6\n \
PERFORMANCE_TRACKING=false\n \
MAX_LOG_FILE_SIZE=2097152##2MB\n \
LOG_FLUSH_THRESHOLD=100\n \
* INFO:\n \
FORMAT=\"%msg\"\n \
* FATAL:\n \
FORMAT=\"[%datetime{%H:%m:%s}]%levshort:%msg\"\n \
* ERROR:\n \
FORMAT=\"[%datetime{%H:%m:%s}]%levshort:%msg\"\n \
* TRACE:\n \
FORMAT=\"[%datetime{%H:%m:%s}]%levshort:%msg\"\n \
* DEBUG:\n \
FORMAT=\"[%datetime{%H:%m:%s}]%levshort:%msg\"\n";

	START_EASYLOGGINGPP(argc, argv);
	el::Configurations c;
	c.setToDefault();
	c.parseFromText(LOGGING_CONFIG);
	el::Loggers::reconfigureLogger("default", c);
}

void DeInitLogging()
{
	el::Loggers::flushAll();
}

}//namespace D2K
