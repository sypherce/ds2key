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
	namespace Core {
		C::Config *Config = (C::Config*)NULL;
		namespace C {
			const char *DefaultIP = "0.0.0.0";
			const char *DefaultProfile = "0";
			const char *iniFilename = "/ds2key.ini";

			Config::Config() {
				Load();
			}

			Config::~Config() {
				Save();
			}

			int Config::Load() {
				if(Core::UDP == NULL) return 1;
				dictionary *ini = iniParser::load(iniFilename);

				if(ini == NULL) {
					int err = errno;
					std::clog << "Error (iniParser::load): #" << err << "\nFailed to open file: " << iniFilename << "\n";
					Core::UDP->SetRemoteIP(DefaultIP);
					Core::UDP->SetPort(DefaultPort);
					Core::UDP->SetProfile(DefaultProfile);
					Save();

					return err;
				}

				iniParser::dump(ini, stderr);

				Core::UDP->SetRemoteIP(iniParser::getstring(ini, (char*)"settings:ip", (char*)DefaultIP));
				Core::UDP->SetPort(iniParser::getstring(ini, (char*)"settings:port", (char*)Core::itoa(DefaultPort)));
				Core::UDP->SetProfile(iniParser::getstring(ini, (char*)"settings:profile", (char*)DefaultProfile));

				iniParser::freedict(ini);

				return 0;
			}
			int Config::Save() {
				if(Core::UDP == NULL) return 1;
				FILE *file = fopen(iniFilename, "w");

				if(file == NULL) {
					int err = errno;
					std::clog << "Error (fopen): #" << err << "\nFailed to open file: " << iniFilename << "\n";

					return err;
				}

				fprintf(file, "[Settings]\n");
				fprintf(file, "IP=%s\n", Core::UDP->GetRemoteIPString().c_str());
				fprintf(file, "Port=%u\n", Core::UDP->GetPort());
				fprintf(file, "Profile=%u\n", Core::UDP->GetProfile());
				fclose(file);
				Load();

				return 0;
			}
		}
	}
}
