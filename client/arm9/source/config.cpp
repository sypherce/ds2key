/*
	configuration loading/saving
*/
#include <cstdio>
#include <iostream>
#include <errno.h>
#include "config.h"
#include "common/udp.h"
#include "common/iniParserWrapper.h"
#include "common/misc.h"

namespace D2K {
	const char *DefaultIP = "0.0.0.0";
	const char *DefaultProfile = "0";
	const char *iniFilename = "/ds2key.ini";

	namespace Config {
		int Load() {
			dictionary *ini = iniParser::load(iniFilename);

			//if we failed to open the ini file, setup defaults
			if(ini == NULL) {
				int err = errno;
				std::clog << "Error (iniParser::load): " << strerror(err) << "\nFailed to open file: " << iniFilename << "\n";
				UDP::SetRemoteIP(DefaultIP);
				UDP::SetPort(DefaultPort);
				UDP::SetProfile(DefaultProfile);
				if(err == ENOENT)//if file doesn't exist
					Save();

				return err;
			}

			//display file
			iniParser::dump(ini, stderr);

			UDP::SetRemoteIP(iniParser::getstring(ini, (char*)"settings:ip", (char*)DefaultIP));
			UDP::SetPort(iniParser::getstring(ini, (char*)"settings:port", (char*)D2K::itoa(DefaultPort)));
			UDP::SetProfile(iniParser::getstring(ini, (char*)"settings:profile", (char*)DefaultProfile));

			//close file
			iniParser::freedict(ini);

			return 0;
		}
		int Save() {
			FILE *file = fopen(iniFilename, "w");

			//if we failed to open the ini file
			if(file == NULL) {
				int err = errno;
				std::clog << "Error (fopen): " << strerror(err) << "\nFailed to open file: " << iniFilename << "\n";

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
	}
}
