#ifndef __INIPARSER_H__
#define __INIPARSER_H__

#include <string>
extern "C"
{
	#include "iniparser/src/iniparser.h"
}
using std::string;

namespace D2K {
	namespace Core {
		namespace iniParser {
			extern int getnsec(dictionary *d);
			extern string getsecname(dictionary *d, int n);
			extern void dump_ini(dictionary *d, FILE *f);
			extern void dumpsection_ini(dictionary *d, string s, FILE *f);
			extern void dump(dictionary *d, FILE *f);
			extern int getsecnkeys(dictionary *d, string s);
			extern char **getseckeys(dictionary *d, string s);
			extern string getstring(dictionary *d, string key, string def);
			extern int getint(dictionary *d, string key, int notfound);
			extern double getdouble(dictionary *d, string key, double notfound);
			extern int getboolean(dictionary *d, string key, int notfound);
			extern int set(dictionary *ini, string entry, string val);
			extern void unset(dictionary *ini, string entry);
			extern int find_entry(dictionary *ini, string entry);
			extern dictionary *load(string ininame);
			extern void freedict(dictionary *d);
		}
	}
}
#endif//__INIPARSER_H__
