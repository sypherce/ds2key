//configuration loading/saving

#include <cstdio>
#include <iostream>
#include <errno.h>
#include "config.h"
#include "common/udp.h"
#include "common/iniParserWrapper.h"
#include "common/misc.h"

namespace D2K {namespace Config {

const std::string DEFAULT_IP = "192.168.1.255";
const std::string DEFAULT_PROFILE = "0";
const std::string INI_FILENAME = "/ds2key.ini";

int Load()
{
	dictionary* ini = iniParser::load(INI_FILENAME);

	//if we failed to open the ini file, setup defaults
	if(ini == nullptr)
	{
		int err = errno;
		std::clog << "Error (iniParser::load): " << strerror(err) << "\n" <<
		             "Failed to open file: " << INI_FILENAME << "\n";
		UDP::SetRemoteIP(DEFAULT_IP);
		UDP::SetConfigPort(D2K::DEFAULT_PORT);
		UDP::SetProfile(DEFAULT_PROFILE);
		if(err == ENOENT)//if file doesn't exist
			Save();

		return err;
	}

	//display file
	iniParser::dump(ini, stderr);

	UDP::SetRemoteIP(iniParser::getstring(ini, "settings:ip", DEFAULT_IP));
	UDP::SetConfigPort(iniParser::getstring(ini, "settings:port", D2K::ltos(D2K::DEFAULT_PORT)));
	UDP::SetProfile(iniParser::getstring(ini, "settings:profile", DEFAULT_PROFILE));

	//close file
	iniParser::freedict(ini);

	return 0;
}
int Save()
{
	FILE* file = fopen(INI_FILENAME.c_str(), "w");

	//if we failed to open the ini file
	if(file == nullptr)
	{
		int err = errno;
		std::clog << "Error (fopen): " << strerror(err) << "\n" <<
		             "Failed to open file: " << INI_FILENAME << "\n";

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

}}//namespace D2K::Config
