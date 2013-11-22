#ifndef __INIPARSER_H__
#define __INIPARSER_H__

#include <string>//std::string
extern "C" {
	#include "iniparser/src/dictionary.h"
}

namespace D2K {
	namespace Core {
		namespace iniParser {
			extern int getnsec(dictionary *d);
			extern std::string getsecname(dictionary *d, int n);
			extern void dump_ini(dictionary *d, FILE *f);
			extern void dumpsection_ini(dictionary *d, std::string s, FILE *f);
			extern void dump(dictionary *d, FILE *f);
			extern int getsecnkeys(dictionary *d, std::string s);
			extern char **getseckeys(dictionary *d, std::string s);
			extern std::string getstring(dictionary *d, std::string key, std::string def);
			extern int getint(dictionary *d, std::string key, int notfound);
			extern double getdouble(dictionary *d, std::string key, double notfound);
			extern int getboolean(dictionary *d, std::string key, int notfound);
			extern int set(dictionary *ini, std::string entry, std::string val);
			extern void unset(dictionary *ini, std::string entry);
			extern int find_entry(dictionary *ini, std::string entry);
			extern dictionary *load(std::string ininame);
			extern void freedict(dictionary *d);
		}
	}
}
#endif//__INIPARSER_H__
