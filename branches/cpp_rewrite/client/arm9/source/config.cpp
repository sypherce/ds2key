/*
	configuration loading/saving
*/
#include <stdio.h>
#include <errno.h>
#include "config.h"
#include "udp.h"
#include "iniparser/src/iniparser.h"

namespace D2K {
	namespace Core {
		C::Config *Config = (C::Config*)NULL;
		namespace C {

			const char *DefaultIP = "0.0.0.0";
			const char *DefaultPort = "9501";
			const char *DefaultProfile = "0";
			const char *iniFilename = "ds2key.ini";

			Config::Config() {
				Load();
			}

			Config::~Config() {
				Save();
			}

			int Config::Load() {
				if(Core::UDP == NULL) return 1;
				dictionary *ini = iniparser_load(iniFilename);

				if(ini == NULL) {
					int err = errno;
					fprintf(stderr, "Error (iniparser_load): #%d\nFailed to open file: %s\n", err, iniFilename);
					Core::UDP->SetRemoteIP(DefaultIP);
					Core::UDP->SetPort(DefaultPort);
					Core::UDP->SetProfile(DefaultProfile);
					Save();

					return err;
				}

				iniparser_dump(ini, stderr);

				Core::UDP->SetRemoteIP(iniparser_getstring(ini, (char*)"settings:ip", (char*)DefaultIP));
				Core::UDP->SetPort(iniparser_getstring(ini, (char*)"settings:port", (char*)DefaultPort));
				Core::UDP->SetProfile(iniparser_getstring(ini, (char*)"settings:profile", (char*)DefaultProfile));

				iniparser_freedict(ini);

				return 0;
			}
			int Config::Save() {
				if(Core::UDP == NULL) return 1;
				FILE *file = fopen(iniFilename, "w");

				if(file == NULL) {
					int err = errno;
					fprintf(stderr, "Error (fopen): #%d\nFailed to save file: %s\n", errno, iniFilename);

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
