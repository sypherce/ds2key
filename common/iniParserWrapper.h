#pragma once

#include <string> // std::string
#include "iniparser/src/dictionary.h"

namespace D2K {namespace iniParser {

void dump(dictionary* d, FILE* f);
std::string getstring(dictionary* d, std::string key, std::string def);
int getint(dictionary* d, std::string key, int notfound);
dictionary* load(std::string ininame);
void freedict(dictionary* d);

}} // namespace D2K::iniParser
