/*
	ini file parser C++ wrapper
*/

#include "core/iniParser.h"

namespace D2K {
	namespace Core {
		namespace iniParser {
			int getnsec(dictionary *d)
			{
 				return iniparser_getnsec(d);
			}
			string getsecname(dictionary *d, int n)
			{
				return iniparser_getsecname(d, n);
			}
			void dump_ini(dictionary *d, FILE *f)
			{
				return iniparser_dump_ini(d, f);
			}
			void dumpsection_ini(dictionary *d, string s, FILE *f)
			{
				return iniparser_dumpsection_ini(d, (char*)s.c_str(), f);
			}
			void dump(dictionary *d, FILE *f)
			{
				return iniparser_dump(d, f);
			}
			int getsecnkeys(dictionary *d, string s)
			{
				return iniparser_getsecnkeys(d, (char*)s.c_str());
			}
			char **getseckeys(dictionary *d, char *s)
			{
				return iniparser_getseckeys(d, s);
			}
			string getstring(dictionary *d, string key, string def)
			{
				return iniparser_getstring(d, key.c_str(), (char*)def.c_str());
			}
			int getint(dictionary *d, string key, int notfound)
			{
				return iniparser_getint(d, key.c_str(), notfound);
			}
			double getdouble(dictionary *d, string key, double notfound)
			{
				return iniparser_getdouble(d, key.c_str(), notfound);
			}
			int getboolean(dictionary *d, string key, int notfound)
			{
				return iniparser_getboolean(d, key.c_str(), notfound);
			}
			int set(dictionary *ini, string entry, const char *val)
			{
				return iniparser_set(ini, entry.c_str(), val);
			}
			void unset(dictionary *ini, string entry)
			{
				return iniparser_unset(ini, entry.c_str());
			}
			int find_entry(dictionary *ini, string entry)
			{
				return iniparser_find_entry(ini, entry.c_str());
			}
			dictionary *load(string ininame)
			{
				return iniparser_load(ininame.c_str());
			}
			void freedict(dictionary *d)
			{
				return iniparser_freedict(d);
			}

		}
	}
}
