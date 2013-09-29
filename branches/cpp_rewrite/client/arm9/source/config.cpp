/*
	configuration loading/saving
*/
#include <stdio.h>
#include <errno.h>
#include "config.h"
#include "ds2key.h"
#include "iniparser/src/iniparser.h"

const char *DEFAULT_IP = "0.0.0.0";
const char *DEFAULT_PORT = "9501";
const char *DEFAULT_PROFILE = "0";
const char *DS2KEY_INI = "ds2key.ini";

namespace D2K {
	bool Config::Load() {
		dictionary *ini = iniparser_load((char*)DS2KEY_INI);

		if(ini == NULL) {
			printf("Error (fopen): #%d\n", errno);
			printf("Failed to open file: %s\n", DS2KEY_INI);
			DS2Key::SetIP(DEFAULT_IP);
			DS2Key::SetPort((char*)DEFAULT_PORT);
			DS2Key::SetProfile((char*)DEFAULT_PROFILE);
			Save();

			return 1;
		}
		iniparser_dump(ini, stderr);

		DS2Key::SetIP(iniparser_getstring(ini, (char*)"settings:ip", (char*)DEFAULT_IP));
		DS2Key::SetPort(iniparser_getstring(ini, (char*)"settings:port", (char*)DEFAULT_PORT));
		DS2Key::SetProfile(iniparser_getstring(ini, (char*)"settings:profile", (char*)DEFAULT_PROFILE));

		return 0;
	}

	bool Config::Save() {
		FILE *file = fopen(DS2KEY_INI, "w");

		if(file) {
			fprintf(file, "[Settings]\n");
			fprintf(file, "IP=%s\n", DS2Key::GetIPString().c_str());
			fprintf(file, "Port=%d\n", DS2Key::GetPort());
			fprintf(file, "Profile=%d\n", DS2Key::GetProfile());
			fclose(file);
		}
		else {
			printf("Error (fopen): #%d\nFailed to save file: %s\n", errno, DS2KEY_INI);
			return 1;
		}

		return 0;
	}
}
