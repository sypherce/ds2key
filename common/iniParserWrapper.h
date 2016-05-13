#pragma once

#include <string>//std::string
extern "C"
{
#include "iniparser/src/dictionary.h"
}

namespace D2K {namespace iniParser {
extern void dump(dictionary* d, FILE* f);
extern std::string getstring(dictionary* d, std::string key, std::string def);
extern int getint(dictionary* d, std::string key, int notfound);
extern dictionary* load(std::string ininame);
extern void freedict(dictionary* d);
}}//namespace D2K::iniParser
