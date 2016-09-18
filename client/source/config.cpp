// configuration loading/saving

#include <cstdio>
#include <iostream>
#include <cerrno>
#include "config.h"
#include "gui/gui.h"
#include "common/easylogging++Wrapper.h"
#include "common/udp.h"
#include "common/iniParserWrapper.h"
#include "common/misc.h"

namespace D2K {namespace Config {

const std::string DEFAULT_IP = "192.168.1.255";
const std::string DEFAULT_PROFILE = "0";
const std::string INI_FILENAME = "/ds2key/settings.ini";
const std::string DEFAULT_BACKGROUND = "/ds2key/background.png";
const std::string DEFAULT_FONT = "/ds2key/font.ttf";

int Load()
{
	dictionary* ini = iniParser::load(INI_FILENAME);

	// If we failed to open the ini file, setup defaults
	if(ini == nullptr)
	{
		int err = errno;
		LOG(ERROR) << "Error #" << err << " (iniParser::load): " << strerror(err) << "\n" <<
		              "Failed to open file: " << INI_FILENAME;
		UDP::SetRemoteIP(DEFAULT_IP);
		UDP::SetConfigPort(D2K::DEFAULT_PORT);
		UDP::SetProfile(DEFAULT_PROFILE);
		GUI::SetBackground(DEFAULT_BACKGROUND);
		GUI::SetFont(DEFAULT_FONT);
		// If the file doesn't exist
		if(err == ENOENT)
			Save();

		return err;
	}

#ifndef ELPP_DISABLE_DEBUG_LOGS
	// display file
	iniParser::dump(ini, stderr);
#endif

	UDP::SetRemoteIP(iniParser::getstring(ini, "settings:ip", DEFAULT_IP));
	UDP::SetConfigPort(iniParser::getstring(ini, "settings:port", D2K::ltos(D2K::DEFAULT_PORT)));
	UDP::SetProfile(iniParser::getstring(ini, "settings:profile", DEFAULT_PROFILE));
	GUI::SetBackground(iniParser::getstring(ini, "settings:background", DEFAULT_BACKGROUND));
	GUI::SetFont(iniParser::getstring(ini, "settings:font", DEFAULT_FONT));

	// close file
	iniParser::freedict(ini);

	return 0;
}
int Save()
{
	FILE* file = fopen(INI_FILENAME.c_str(), "w");

	// If we failed to open the ini file
	if(file == nullptr)
	{
		int err = errno;
		LOG(ERROR) << "Error #" << err << " (fopen): " << strerror(err) << "\n" <<
		              "Failed to open file: " << INI_FILENAME;

		return err;
	}

	fprintf(file, "[Settings]\n");
	fprintf(file, "IP=%s\n", UDP::GetRemoteIPString().c_str());
	fprintf(file, "Port=%u\n", UDP::GetPort());
	fprintf(file, "Profile=%u\n", UDP::GetProfile());
	fprintf(file, "Background=%s\n", GUI::GetBackground().c_str());
	fprintf(file, "Font=%s\n", GUI::GetFont().c_str());
	fclose(file);
	Load(); // Reload settings

	return 0;
}

}} // namespace D2K::Config
