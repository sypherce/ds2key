//configuration loading/saving

#include <cstdio>
#include <iostream>
#include <errno.h>
#include "config.h"
#include "common/udp.h"
#include "common/iniParserWrapper.h"
#include "common/misc.h"

namespace D2K {

const char* g_default_ip = "0.0.0.0";
const char* g_default_profile = "0";
const char* g_ini_filename = "/ds2key.ini";

namespace Config {

int Load()
{
	dictionary* ini = iniParser::load(g_ini_filename);

	//if we failed to open the ini file, setup defaults
	if(ini == nullptr)
	{
		int err = errno;
		std::clog << "Error (iniParser::load): " << strerror(err) << "\nFailed to open file: " << g_ini_filename << "\n";
		UDP::SetRemoteIP(g_default_ip);
		UDP::SetConfigPort(D2K::DEFAULT_PORT);
		UDP::SetProfile(g_default_profile);
		if(err == ENOENT)//if file doesn't exist
			Save();

		return err;
	}

	//display file
	iniParser::dump(ini, stderr);

	UDP::SetRemoteIP(iniParser::getstring(ini, (char*)"settings:ip", (char*)g_default_ip));
	UDP::SetConfigPort(iniParser::getstring(ini, (char*)"settings:port", (char*)D2K::ltoa(D2K::DEFAULT_PORT)));
	UDP::SetProfile(iniParser::getstring(ini, (char*)"settings:profile", (char*)g_default_profile));

	//close file
	iniParser::freedict(ini);

	return 0;
}
int Save()
{
	FILE* file = fopen(g_ini_filename, "w");

	//if we failed to open the ini file
	if(file == nullptr)
	{
		int err = errno;
		std::clog << "Error (fopen): " << strerror(err) << "\nFailed to open file: " << g_ini_filename << "\n";

		return err;
	}

	fprintf(file, "[Settings]\n");
	fprintf(file, "IP=%s\n", UDP::GetRemoteIPString().c_str());
	fprintf(file, "Port=%u\n", UDP::GetPort());
	fprintf(file, "Profile=%u\n", UDP::GetProfile());
	fclose(file);
	Load();//reload settings

	return 0;
}

}//namespace Config
}//namespace D2K