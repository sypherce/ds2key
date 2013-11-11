/*
	configuration loading/saving
*/

#include <sstream>//ostringstream
#include "config.h"
#include "key.h"
#include "core/iniParser.h"

namespace D2K {
	namespace Core {
		C::Config *Config = (C::Config*)NULL;
		namespace C {
			char *iniFilename = (char*)"ds2key.ini";
			Config::Config() {
				for(int i = 0; i < 256; i++) {
					Commands[i] = (char*)NULL;
				}
				Load();
			}

			Config::~Config() {
				Save();
			}

			bool Config::Save() {
				FILE *file = fopen(iniFilename, "w");

				if(file) {
					fprintf(file, "[Settings]\n");
					fprintf(file, "Port=%i\n", Port);
					fprintf(file, "Debug=%i\n", Debug);

					for(int i = 0; i < 256; i++) {
						if(Commands[i])
							fprintf(file, "Command%i=\"%s\"\n", i, Commands[i]);
					}

					fclose(file);
					Load();

					return 0;
				}

				return 1;
			}

			bool Config::SaveProfile(uint16_t *Profile, uint8_t profileNumber) {
				std::ostringstream ssfilename;
				ssfilename << "ds2key.p" << (int)profileNumber << ".ini";

				FILE *file = fopen(ssfilename.str().c_str(), "w");
				if(file) {
					fprintf(file, "[Profile]\n");
					if(Profile[kMouse] == mAbsolute)
						fprintf(file, "Mouse=Absolute\n");
					else if(Profile[kMouse] == mButtons)
						fprintf(file, "Mouse=Buttons\n");
					else
						fprintf(file, "Mouse=Relative\n");
					fprintf(file, "Joy=%i\n", Profile[kJoy]);
					fprintf(file, "Up=%s\n", Key::GetString(Profile[kUp]));
					fprintf(file, "Down=%s\n", Key::GetString(Profile[kDown]));
					fprintf(file, "Left=%s\n", Key::GetString(Profile[kLeft]));
					fprintf(file, "Right=%s\n", Key::GetString(Profile[kRight]));
					fprintf(file, "A=%s\n", Key::GetString(Profile[kA]));
					fprintf(file, "B=%s\n", Key::GetString(Profile[kB]));
					fprintf(file, "X=%s\n", Key::GetString(Profile[kX]));
					fprintf(file, "Y=%s\n", Key::GetString(Profile[kY]));
					fprintf(file, "L=%s\n", Key::GetString(Profile[kL]));
					fprintf(file, "R=%s\n", Key::GetString(Profile[kR]));
					fprintf(file, "Start=%s\n", Key::GetString(Profile[kStart]));
					fprintf(file, "Select=%s\n", Key::GetString(Profile[kSelect]));
					fprintf(file, "Lid=%s\n", Key::GetString(Profile[kLid]));
					fprintf(file, "Blue=%s\n", Key::GetString(Profile[kBlue]));
					fprintf(file, "Yellow=%s\n", Key::GetString(Profile[kYellow]));
					fprintf(file, "Red=%s\n", Key::GetString(Profile[kRed]));
					fprintf(file, "Green=%s\n", Key::GetString(Profile[kGreen]));
					fprintf(file, "Touch00=%s\n", Key::GetString(Profile[kTouch00]));
					fprintf(file, "Touch01=%s\n", Key::GetString(Profile[kTouch01]));
					fprintf(file, "Touch02=%s\n", Key::GetString(Profile[kTouch02]));
					fprintf(file, "Touch03=%s\n", Key::GetString(Profile[kTouch03]));
					fprintf(file, "Touch04=%s\n", Key::GetString(Profile[kTouch04]));
					fprintf(file, "Touch05=%s\n", Key::GetString(Profile[kTouch05]));
					fprintf(file, "Touch06=%s\n", Key::GetString(Profile[kTouch06]));
					fprintf(file, "Touch07=%s\n", Key::GetString(Profile[kTouch07]));
					fprintf(file, "Touch08=%s\n", Key::GetString(Profile[kTouch08]));
					fprintf(file, "Touch09=%s\n", Key::GetString(Profile[kTouch09]));
					fprintf(file, "Touch10=%s\n", Key::GetString(Profile[kTouch10]));
					fprintf(file, "Touch11=%s\n", Key::GetString(Profile[kTouch11]));

					fprintf(file, "Touch00X=%i\n", Profile[kTouch00X]);
					fprintf(file, "Touch01X=%i\n", Profile[kTouch01X]);
					fprintf(file, "Touch02X=%i\n", Profile[kTouch02X]);
					fprintf(file, "Touch03X=%i\n", Profile[kTouch03X]);
					fprintf(file, "Touch04X=%i\n", Profile[kTouch04X]);
					fprintf(file, "Touch05X=%i\n", Profile[kTouch05X]);
					fprintf(file, "Touch06X=%i\n", Profile[kTouch06X]);
					fprintf(file, "Touch07X=%i\n", Profile[kTouch07X]);
					fprintf(file, "Touch08X=%i\n", Profile[kTouch08X]);
					fprintf(file, "Touch09X=%i\n", Profile[kTouch09X]);
					fprintf(file, "Touch10X=%i\n", Profile[kTouch10X]);
					fprintf(file, "Touch11X=%i\n", Profile[kTouch11X]);

					fprintf(file, "Touch00Y=%i\n", Profile[kTouch00Y]);
					fprintf(file, "Touch01Y=%i\n", Profile[kTouch01Y]);
					fprintf(file, "Touch02Y=%i\n", Profile[kTouch02Y]);
					fprintf(file, "Touch03Y=%i\n", Profile[kTouch03Y]);
					fprintf(file, "Touch04Y=%i\n", Profile[kTouch04Y]);
					fprintf(file, "Touch05Y=%i\n", Profile[kTouch05Y]);
					fprintf(file, "Touch06Y=%i\n", Profile[kTouch06Y]);
					fprintf(file, "Touch07Y=%i\n", Profile[kTouch07Y]);
					fprintf(file, "Touch08Y=%i\n", Profile[kTouch08Y]);
					fprintf(file, "Touch09Y=%i\n", Profile[kTouch09Y]);
					fprintf(file, "Touch10Y=%i\n", Profile[kTouch10Y]);
					fprintf(file, "Touch11Y=%i\n", Profile[kTouch11Y]);

					fprintf(file, "Touch00W=%i\n", Profile[kTouch00W]);
					fprintf(file, "Touch01W=%i\n", Profile[kTouch01W]);
					fprintf(file, "Touch02W=%i\n", Profile[kTouch02W]);
					fprintf(file, "Touch03W=%i\n", Profile[kTouch03W]);
					fprintf(file, "Touch04W=%i\n", Profile[kTouch04W]);
					fprintf(file, "Touch05W=%i\n", Profile[kTouch05W]);
					fprintf(file, "Touch06W=%i\n", Profile[kTouch06W]);
					fprintf(file, "Touch07W=%i\n", Profile[kTouch07W]);
					fprintf(file, "Touch08W=%i\n", Profile[kTouch08W]);
					fprintf(file, "Touch09W=%i\n", Profile[kTouch09W]);
					fprintf(file, "Touch10W=%i\n", Profile[kTouch10W]);
					fprintf(file, "Touch11W=%i\n", Profile[kTouch11W]);

					fprintf(file, "Touch00H=%i\n", Profile[kTouch00H]);
					fprintf(file, "Touch01H=%i\n", Profile[kTouch01H]);
					fprintf(file, "Touch02H=%i\n", Profile[kTouch02H]);
					fprintf(file, "Touch03H=%i\n", Profile[kTouch03H]);
					fprintf(file, "Touch04H=%i\n", Profile[kTouch04H]);
					fprintf(file, "Touch05H=%i\n", Profile[kTouch05H]);
					fprintf(file, "Touch06H=%i\n", Profile[kTouch06H]);
					fprintf(file, "Touch07H=%i\n", Profile[kTouch07H]);
					fprintf(file, "Touch08H=%i\n", Profile[kTouch08H]);
					fprintf(file, "Touch09H=%i\n", Profile[kTouch09H]);
					fprintf(file, "Touch10H=%i\n", Profile[kTouch10H]);
					fprintf(file, "Touch11H=%i\n", Profile[kTouch11H]);

					fclose(file);

					return 0;
				}

				return 1;
			}

			bool Config::Load() {
				dictionary *ini = D2K::Core::iniParser::load(iniFilename);

				for(int i = 0; i < 256; i++) {
					if(Commands[i])
						delete Commands[i];
					Commands[i] = (char*)NULL;
				}

				if(ini == NULL) {
					fprintf(stderr, "cannot parse file: %s\n", iniFilename);
					SetPort(Config::DefaultPort);
					Debug = dDefault;
					Save();

					return 1;
				}

				iniParser::dump(ini, stderr);

				SetPort(iniParser::getint(ini, "settings:port", Config::DefaultPort));
				Debug = iniParser::getint(ini, "settings:debug", dDefault);
				for(int i = 0; i < 256; i++) {
					std::ostringstream commandString;
					commandString << "settings:command" << i;
					string commandPointer = iniParser::getstring(ini, commandString.str(), "");
					unsigned int commandLength = commandPointer.size();
					if(commandLength > 0) {
						Commands[i] = new char[commandLength + 1];
						strncpy(Commands[i], commandPointer.c_str(), commandLength);
						Commands[i][commandLength] = 0;
					}
					else {
						Commands[i] = (char*)NULL;
					}
				}
				iniParser::freedict(ini);

				return 0;
			}

			bool Config::ReadProfile(uint16_t *Profile, uint8_t profileNumber) {
				std::ostringstream ssfilename;
				ssfilename << "ds2key.p" << (int)profileNumber << ".ini";

				dictionary *ini = iniParser::load(ssfilename.str().c_str());
				iniParser::dump(ini, stderr);
				if(ini == NULL) {
					fprintf(stderr, "cannot parse file: %s\n", ssfilename.str().c_str());
					Profile[kMouse] = mRelative;
					Profile[kJoy] = 0;
					Profile[kUp] = KEY_UP;
					Profile[kDown] = KEY_DOWN;
					Profile[kLeft] = KEY_LEFT;
					Profile[kRight] = KEY_RIGHT;
					Profile[kA] = KEY_A;
					Profile[kB] = KEY_B;
					Profile[kX] = KEY_X;
					Profile[kY] = KEY_Y;
					Profile[kL] = KEY_L;
					Profile[kR] = KEY_R;
					Profile[kStart] = KEY_RETURN;
					Profile[kSelect] = KEY_RSHIFT;
					Profile[kLid] = KEY_ESCAPE;
					Profile[kBlue] = KEY_1;
					Profile[kYellow] = KEY_2;
					Profile[kRed] = KEY_3;
					Profile[kGreen] = KEY_4;

					Profile[kTouch00] = KEY_NUMPAD7;
					Profile[kTouch01] = KEY_NUMPAD7;
					Profile[kTouch02] = KEY_NUMPAD7;
					Profile[kTouch03] = KEY_NUMPAD7;
					Profile[kTouch04] = KEY_NUMPAD7;
					Profile[kTouch05] = KEY_NUMPAD7;
					Profile[kTouch06] = KEY_NUMPAD7;
					Profile[kTouch07] = KEY_NUMPAD7;
					Profile[kTouch08] = KEY_NUMPAD7;
					Profile[kTouch09] = KEY_NUMPAD7;
					Profile[kTouch10] = KEY_NUMPAD7;
					Profile[kTouch11] = KEY_NUMPAD7;

					Profile[kTouch00X] = 0;
					Profile[kTouch01X] = 0;
					Profile[kTouch02X] = 0;
					Profile[kTouch03X] = 0;
					Profile[kTouch04X] = 0;
					Profile[kTouch05X] = 0;
					Profile[kTouch06X] = 0;
					Profile[kTouch07X] = 0;
					Profile[kTouch08X] = 0;
					Profile[kTouch09X] = 0;
					Profile[kTouch10X] = 0;
					Profile[kTouch11X] = 0;

					Profile[kTouch00Y] = 0;
					Profile[kTouch01Y] = 0;
					Profile[kTouch02Y] = 0;
					Profile[kTouch03Y] = 0;
					Profile[kTouch04Y] = 0;
					Profile[kTouch05Y] = 0;
					Profile[kTouch06Y] = 0;
					Profile[kTouch07Y] = 0;
					Profile[kTouch08Y] = 0;
					Profile[kTouch09Y] = 0;
					Profile[kTouch10Y] = 0;
					Profile[kTouch11Y] = 0;

					Profile[kTouch00W] = 0;
					Profile[kTouch01W] = 0;
					Profile[kTouch02W] = 0;
					Profile[kTouch03W] = 0;
					Profile[kTouch04W] = 0;
					Profile[kTouch05W] = 0;
					Profile[kTouch06W] = 0;
					Profile[kTouch07W] = 0;
					Profile[kTouch08W] = 0;
					Profile[kTouch09W] = 0;
					Profile[kTouch10W] = 0;
					Profile[kTouch11W] = 0;

					Profile[kTouch00H] = 0;
					Profile[kTouch01H] = 0;
					Profile[kTouch02H] = 0;
					Profile[kTouch03H] = 0;
					Profile[kTouch04H] = 0;
					Profile[kTouch05H] = 0;
					Profile[kTouch06H] = 0;
					Profile[kTouch07H] = 0;
					Profile[kTouch08H] = 0;
					Profile[kTouch09H] = 0;
					Profile[kTouch10H] = 0;
					Profile[kTouch11H] = 0;

					SaveProfile(Profile, profileNumber);

					return 1;
				}
				iniParser::dump(ini, stderr);

				std::string mouse = iniParser::getstring(ini, "profile:mouse", "Relative");

				if(strcmp(mouse.c_str(), "Absolute") == 0)
					Profile[kMouse] = mAbsolute;
				else if(strcmp(mouse.c_str(), "Buttons") == 0)
					Profile[kMouse] = mButtons;
				else
					Profile[kMouse] = mRelative;

				Profile[kJoy] = atoi(iniParser::getstring(ini, "profile:joy", "0").c_str());
				Profile[kUp] = Key::GetNumber(iniParser::getstring(ini, "profile:up", "Key_None").c_str());
				Profile[kDown] = Key::GetNumber(iniParser::getstring(ini, "profile:down", "Key_None").c_str());
				Profile[kLeft] = Key::GetNumber(iniParser::getstring(ini, "profile:left", "Key_None").c_str());
				Profile[kRight] = Key::GetNumber(iniParser::getstring(ini, "profile:right", "Key_None").c_str());
				Profile[kA] = Key::GetNumber(iniParser::getstring(ini, "profile:a", "Key_None").c_str());
				Profile[kB] = Key::GetNumber(iniParser::getstring(ini, "profile:b", "Key_None").c_str());
				Profile[kX] = Key::GetNumber(iniParser::getstring(ini, "profile:x", "Key_None").c_str());
				Profile[kY] = Key::GetNumber(iniParser::getstring(ini, "profile:y", "Key_None").c_str());
				Profile[kL] = Key::GetNumber(iniParser::getstring(ini, "profile:l", "Key_None").c_str());
				Profile[kR] = Key::GetNumber(iniParser::getstring(ini, "profile:r", "Key_None").c_str());
				Profile[kStart] = Key::GetNumber(iniParser::getstring(ini, "profile:start", "Key_None").c_str());
				Profile[kSelect] = Key::GetNumber(iniParser::getstring(ini, "profile:select", "Key_None").c_str());
				Profile[kLid] = Key::GetNumber(iniParser::getstring(ini, "profile:lid", "Key_None").c_str());
				Profile[kBlue] = Key::GetNumber(iniParser::getstring(ini, "profile:blue", "Key_None").c_str());
				Profile[kYellow] = Key::GetNumber(iniParser::getstring(ini, "profile:yellow", "Key_None").c_str());
				Profile[kRed] = Key::GetNumber(iniParser::getstring(ini, "profile:red", "Key_None").c_str());
				Profile[kGreen] = Key::GetNumber(iniParser::getstring(ini, "profile:green", "Key_None").c_str());
				Profile[kTouch00] = Key::GetNumber(iniParser::getstring(ini, "profile:touch00", "0").c_str());
				Profile[kTouch01] = Key::GetNumber(iniParser::getstring(ini, "profile:touch01", "0").c_str());
				Profile[kTouch02] = Key::GetNumber(iniParser::getstring(ini, "profile:touch02", "0").c_str());
				Profile[kTouch03] = Key::GetNumber(iniParser::getstring(ini, "profile:touch03", "0").c_str());
				Profile[kTouch04] = Key::GetNumber(iniParser::getstring(ini, "profile:touch04", "0").c_str());
				Profile[kTouch05] = Key::GetNumber(iniParser::getstring(ini, "profile:touch05", "0").c_str());
				Profile[kTouch06] = Key::GetNumber(iniParser::getstring(ini, "profile:touch06", "0").c_str());
				Profile[kTouch07] = Key::GetNumber(iniParser::getstring(ini, "profile:touch07", "0").c_str());
				Profile[kTouch08] = Key::GetNumber(iniParser::getstring(ini, "profile:touch08", "0").c_str());
				Profile[kTouch09] = Key::GetNumber(iniParser::getstring(ini, "profile:touch09", "0").c_str());
				Profile[kTouch10] = Key::GetNumber(iniParser::getstring(ini, "profile:touch10", "0").c_str());
				Profile[kTouch11] = Key::GetNumber(iniParser::getstring(ini, "profile:touch11", "0").c_str());

				Profile[kTouch00X] = atoi(iniParser::getstring(ini, "profile:touch00X", "0").c_str());
				Profile[kTouch01X] = atoi(iniParser::getstring(ini, "profile:touch01X", "0").c_str());
				Profile[kTouch02X] = atoi(iniParser::getstring(ini, "profile:touch02X", "0").c_str());
				Profile[kTouch03X] = atoi(iniParser::getstring(ini, "profile:touch03X", "0").c_str());
				Profile[kTouch04X] = atoi(iniParser::getstring(ini, "profile:touch04X", "0").c_str());
				Profile[kTouch05X] = atoi(iniParser::getstring(ini, "profile:touch05X", "0").c_str());
				Profile[kTouch06X] = atoi(iniParser::getstring(ini, "profile:touch06X", "0").c_str());
				Profile[kTouch07X] = atoi(iniParser::getstring(ini, "profile:touch07X", "0").c_str());
				Profile[kTouch08X] = atoi(iniParser::getstring(ini, "profile:touch08X", "0").c_str());
				Profile[kTouch09X] = atoi(iniParser::getstring(ini, "profile:touch09X", "0").c_str());
				Profile[kTouch10X] = atoi(iniParser::getstring(ini, "profile:touch10X", "0").c_str());
				Profile[kTouch11X] = atoi(iniParser::getstring(ini, "profile:touch11X", "0").c_str());

				Profile[kTouch00Y] = atoi(iniParser::getstring(ini, "profile:touch00Y", "0").c_str());
				Profile[kTouch01Y] = atoi(iniParser::getstring(ini, "profile:touch01Y", "0").c_str());
				Profile[kTouch02Y] = atoi(iniParser::getstring(ini, "profile:touch02Y", "0").c_str());
				Profile[kTouch03Y] = atoi(iniParser::getstring(ini, "profile:touch03Y", "0").c_str());
				Profile[kTouch04Y] = atoi(iniParser::getstring(ini, "profile:touch04Y", "0").c_str());
				Profile[kTouch05Y] = atoi(iniParser::getstring(ini, "profile:touch05Y", "0").c_str());
				Profile[kTouch06Y] = atoi(iniParser::getstring(ini, "profile:touch06Y", "0").c_str());
				Profile[kTouch07Y] = atoi(iniParser::getstring(ini, "profile:touch07Y", "0").c_str());
				Profile[kTouch08Y] = atoi(iniParser::getstring(ini, "profile:touch08Y", "0").c_str());
				Profile[kTouch09Y] = atoi(iniParser::getstring(ini, "profile:touch09Y", "0").c_str());
				Profile[kTouch10Y] = atoi(iniParser::getstring(ini, "profile:touch10Y", "0").c_str());
				Profile[kTouch11Y] = atoi(iniParser::getstring(ini, "profile:touch11Y", "0").c_str());

				Profile[kTouch00W] = atoi(iniParser::getstring(ini, "profile:touch00W", "0").c_str());
				Profile[kTouch01W] = atoi(iniParser::getstring(ini, "profile:touch01W", "0").c_str());
				Profile[kTouch02W] = atoi(iniParser::getstring(ini, "profile:touch02W", "0").c_str());
				Profile[kTouch03W] = atoi(iniParser::getstring(ini, "profile:touch03W", "0").c_str());
				Profile[kTouch04W] = atoi(iniParser::getstring(ini, "profile:touch04W", "0").c_str());
				Profile[kTouch05W] = atoi(iniParser::getstring(ini, "profile:touch05W", "0").c_str());
				Profile[kTouch06W] = atoi(iniParser::getstring(ini, "profile:touch06W", "0").c_str());
				Profile[kTouch07W] = atoi(iniParser::getstring(ini, "profile:touch07W", "0").c_str());
				Profile[kTouch08W] = atoi(iniParser::getstring(ini, "profile:touch08W", "0").c_str());
				Profile[kTouch09W] = atoi(iniParser::getstring(ini, "profile:touch09W", "0").c_str());
				Profile[kTouch10W] = atoi(iniParser::getstring(ini, "profile:touch10W", "0").c_str());
				Profile[kTouch11W] = atoi(iniParser::getstring(ini, "profile:touch11W", "0").c_str());

				Profile[kTouch00H] = atoi(iniParser::getstring(ini, "profile:touch00H", "0").c_str());
				Profile[kTouch01H] = atoi(iniParser::getstring(ini, "profile:touch01H", "0").c_str());
				Profile[kTouch02H] = atoi(iniParser::getstring(ini, "profile:touch02H", "0").c_str());
				Profile[kTouch03H] = atoi(iniParser::getstring(ini, "profile:touch03H", "0").c_str());
				Profile[kTouch04H] = atoi(iniParser::getstring(ini, "profile:touch04H", "0").c_str());
				Profile[kTouch05H] = atoi(iniParser::getstring(ini, "profile:touch05H", "0").c_str());
				Profile[kTouch06H] = atoi(iniParser::getstring(ini, "profile:touch06H", "0").c_str());
				Profile[kTouch07H] = atoi(iniParser::getstring(ini, "profile:touch07H", "0").c_str());
				Profile[kTouch08H] = atoi(iniParser::getstring(ini, "profile:touch08H", "0").c_str());
				Profile[kTouch09H] = atoi(iniParser::getstring(ini, "profile:touch09H", "0").c_str());
				Profile[kTouch10H] = atoi(iniParser::getstring(ini, "profile:touch10H", "0").c_str());
				Profile[kTouch11H] = atoi(iniParser::getstring(ini, "profile:touch11H", "0").c_str());
				iniParser::freedict(ini);

				return 0;
			}

			uint16_t Config::GetPort() {
				return Port;
			}

			void Config::SetPort(uint16_t Port) {
				if(Port == 0)
					Config::Port = Config::DefaultPort;
				else
					Config::Port = Port;
			}

			uint8_t Config::GetDebugLevel() {
				return Debug;
			}

			char *Config::GetCommand(uint8_t Command) {
				return Commands[Command];
			}
		}
	}
}
