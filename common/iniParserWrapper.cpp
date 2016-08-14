// ini file parser C++ wrapper

#include <string>//std::string
#include "easylogging++Wrapper.h"
#include "iniparser/src/iniparser.h"


namespace D2K {namespace iniParser {

void dump(dictionary* d, FILE* f)
{
#ifdef D2K_DUMP_CONFIGS
	iniparser_dump(d, f);
#endif
}
std::string getstring(dictionary* d, std::string key, std::string def)
{
	return iniparser_getstring(d, key.c_str(), (char*)def.c_str());
}
int getint(dictionary* d, std::string key, int notfound)
{
	return iniparser_getint(d, key.c_str(), notfound);
}
dictionary* load(std::string ininame)
{
	return iniparser_load(ininame.c_str());
}
void freedict(dictionary* d)
{
	return iniparser_freedict(d);
}

}}//namespace D2K::iniParser
