/*
	configuration loading/saving
*/

#include <iostream>//std::cout, std::clog
#include <sstream>//ostringstream
#include "config.h"
#include "key.h"
#include "common/iniParserWrapper.h"
#include "common/misc.h"
#include "client.h"
#undef SetPort

namespace D2K {
	namespace Core {
		C::Config *Config = (C::Config*)NULL;
		namespace C {
			const char *iniFilename = "ds2key.ini";

			Config::Config() {
				for(int i = 0; i < 256; i++) {
					Commands[i] = (char*)NULL;
				}
				Load();
			}

			Config::~Config() {
				Save();
			}

			int Config::Load() {
				dictionary *ini = D2K::Core::iniParser::load(iniFilename);

				for(int i = 0; i < 256; i++) {
					if(Commands[i])
						delete Commands[i];
					Commands[i] = (char*)NULL;
				}

				if(ini == NULL) {
					int err = errno;
					std::clog << "Error (iniParser::load): #" << err << "\nFailed to open file: " << iniFilename << "\n";
					SetPort(DefaultPort);
					Save();

					return err;
				}

				iniParser::dump(ini, stderr);

				SetPort(iniParser::getint(ini, "settings:port", DefaultPort));
				for(int i = 0; i < 256; i++) {
					std::ostringstream commandString;
					commandString << "settings:command" << i;
					std::string commandPointer = iniParser::getstring(ini, commandString.str(), "");
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

			int Config::LoadProfile(ProfileData *Profile, uint8_t profileNumber) {
				std::ostringstream ssfilename;
				ssfilename << "ds2key.p" << (int)profileNumber << ".ini";

				dictionary *ini = iniParser::load(ssfilename.str().c_str());
				iniParser::dump(ini, stderr);
				if(ini == NULL) {
					int err = errno;
					std::clog << "Error (iniParser::load): #" << err << "\nFailed to open file: " << iniFilename << "\n";
					Profile->Mouse = "Relative";
					Profile->Joy = "0";
					Profile->SetVirtualKey(kUp, KEY_UP);
					Profile->SetVirtualKey(kDown, KEY_DOWN);
					Profile->SetVirtualKey(kLeft, KEY_LEFT);
					Profile->SetVirtualKey(kRight, KEY_RIGHT);
					Profile->SetVirtualKey(kA, KEY_A);
					Profile->SetVirtualKey(kB, KEY_B);
					Profile->SetVirtualKey(kX, KEY_X);
					Profile->SetVirtualKey(kY, KEY_Y);
					Profile->SetVirtualKey(kL, KEY_L);
					Profile->SetVirtualKey(kR, KEY_R);
					Profile->SetVirtualKey(kStart, KEY_RETURN);
					Profile->SetVirtualKey(kSelect, KEY_RSHIFT);
					Profile->SetVirtualKey(kLid, KEY_ESCAPE);
					Profile->SetVirtualKey(kBlue, KEY_1);
					Profile->SetVirtualKey(kYellow, KEY_2);
					Profile->SetVirtualKey(kRed, KEY_3);
					Profile->SetVirtualKey(kGreen, KEY_4);

					Profile->SetVirtualKey(kTouch00, KEY_NUMPAD0);
					Profile->SetVirtualKey(kTouch01, KEY_NUMPAD1);
					Profile->SetVirtualKey(kTouch02, KEY_NUMPAD2);
					Profile->SetVirtualKey(kTouch03, KEY_NUMPAD3);
					Profile->SetVirtualKey(kTouch04, KEY_NUMPAD4);
					Profile->SetVirtualKey(kTouch05, KEY_NUMPAD5);
					Profile->SetVirtualKey(kTouch06, KEY_NUMPAD6);
					Profile->SetVirtualKey(kTouch07, KEY_NUMPAD7);
					Profile->SetVirtualKey(kTouch08, KEY_NUMPAD8);
					Profile->SetVirtualKey(kTouch09, KEY_NUMPAD9);
					Profile->SetVirtualKey(kTouch10, KEY_5);
					Profile->SetVirtualKey(kTouch11, KEY_6);

					int screenW = 256;
					int screenH = 192;
					int TouchButton = 0;
					for(int y = 0; y < screenH; y += screenH / 3)
						for(int x = 0; x < screenW; x += screenW / 4)
							Profile->SetTouchPos(	TouchButton++,	//buttons 0 - 11
													x,				//x
													y,				//y
													screenW / 4,	//w
													screenH / 3		//h
													);

					SaveProfile(Profile, profileNumber);

					return 1;
				}

				iniParser::dump(ini, stderr);

				std::string mouse = iniParser::getstring(ini, "profile:mouse", "Relative");

				if((mouse == "Absolute") || (mouse == "Buttons"))
					Profile->Mouse = mouse;
				else
					Profile->Mouse = "Relative";

				Profile->Joy = iniParser::getstring(ini, "profile:joy", "0");
				Profile->SetCommand(kUp, iniParser::getstring(ini, "profile:up", "Key_None"));
				Profile->SetCommand(kDown, iniParser::getstring(ini, "profile:down", "Key_None"));
				Profile->SetCommand(kLeft, iniParser::getstring(ini, "profile:left", "Key_None"));
				Profile->SetCommand(kRight, iniParser::getstring(ini, "profile:right", "Key_None"));
				Profile->SetCommand(kA, iniParser::getstring(ini, "profile:a", "Key_None"));
				Profile->SetCommand(kB, iniParser::getstring(ini, "profile:b", "Key_None"));
				Profile->SetCommand(kX, iniParser::getstring(ini, "profile:x", "Key_None"));
				Profile->SetCommand(kY, iniParser::getstring(ini, "profile:y", "Key_None"));
				Profile->SetCommand(kL, iniParser::getstring(ini, "profile:l", "Key_None"));
				Profile->SetCommand(kR, iniParser::getstring(ini, "profile:r", "Key_None"));
				Profile->SetCommand(kStart, iniParser::getstring(ini, "profile:start", "Key_None"));
				Profile->SetCommand(kSelect, iniParser::getstring(ini, "profile:select", "Key_None"));
				Profile->SetCommand(kLid, iniParser::getstring(ini, "profile:lid", "Key_None"));
				Profile->SetCommand(kBlue, iniParser::getstring(ini, "profile:blue", "Key_None"));
				Profile->SetCommand(kYellow, iniParser::getstring(ini, "profile:yellow", "Key_None"));
				Profile->SetCommand(kRed, iniParser::getstring(ini, "profile:red", "Key_None"));
				Profile->SetCommand(kGreen, iniParser::getstring(ini, "profile:green", "Key_None"));
				Profile->SetCommand(kTouch00, iniParser::getstring(ini, "profile:touch00", "Key_None"));
				Profile->SetCommand(kTouch01, iniParser::getstring(ini, "profile:touch01", "Key_None"));
				Profile->SetCommand(kTouch02, iniParser::getstring(ini, "profile:touch02", "Key_None"));
				Profile->SetCommand(kTouch03, iniParser::getstring(ini, "profile:touch03", "Key_None"));
				Profile->SetCommand(kTouch04, iniParser::getstring(ini, "profile:touch04", "Key_None"));
				Profile->SetCommand(kTouch05, iniParser::getstring(ini, "profile:touch05", "Key_None"));
				Profile->SetCommand(kTouch06, iniParser::getstring(ini, "profile:touch06", "Key_None"));
				Profile->SetCommand(kTouch07, iniParser::getstring(ini, "profile:touch07", "Key_None"));
				Profile->SetCommand(kTouch08, iniParser::getstring(ini, "profile:touch08", "Key_None"));
				Profile->SetCommand(kTouch09, iniParser::getstring(ini, "profile:touch09", "Key_None"));
				Profile->SetCommand(kTouch10, iniParser::getstring(ini, "profile:touch10", "Key_None"));
				Profile->SetCommand(kTouch11, iniParser::getstring(ini, "profile:touch11", "Key_None"));

				Profile->TouchX[0] = Core::stoi(iniParser::getstring(ini, "profile:touch00X", "0"));
				Profile->TouchX[1] = Core::stoi(iniParser::getstring(ini, "profile:touch01X", "0"));
				Profile->TouchX[2] = Core::stoi(iniParser::getstring(ini, "profile:touch02X", "0"));
				Profile->TouchX[3] = Core::stoi(iniParser::getstring(ini, "profile:touch03X", "0"));
				Profile->TouchX[4] = Core::stoi(iniParser::getstring(ini, "profile:touch04X", "0"));
				Profile->TouchX[5] = Core::stoi(iniParser::getstring(ini, "profile:touch05X", "0"));
				Profile->TouchX[6] = Core::stoi(iniParser::getstring(ini, "profile:touch06X", "0"));
				Profile->TouchX[7] = Core::stoi(iniParser::getstring(ini, "profile:touch07X", "0"));
				Profile->TouchX[8] = Core::stoi(iniParser::getstring(ini, "profile:touch08X", "0"));
				Profile->TouchX[9] = Core::stoi(iniParser::getstring(ini, "profile:touch09X", "0"));
				Profile->TouchX[10] = Core::stoi(iniParser::getstring(ini, "profile:touch10X", "0"));
				Profile->TouchX[11] = Core::stoi(iniParser::getstring(ini, "profile:touch11X", "0"));

				Profile->TouchY[0] = Core::stoi(iniParser::getstring(ini, "profile:touch00Y", "0"));
				Profile->TouchY[1] = Core::stoi(iniParser::getstring(ini, "profile:touch01Y", "0"));
				Profile->TouchY[2] = Core::stoi(iniParser::getstring(ini, "profile:touch02Y", "0"));
				Profile->TouchY[3] = Core::stoi(iniParser::getstring(ini, "profile:touch03Y", "0"));
				Profile->TouchY[4] = Core::stoi(iniParser::getstring(ini, "profile:touch04Y", "0"));
				Profile->TouchY[5] = Core::stoi(iniParser::getstring(ini, "profile:touch05Y", "0"));
				Profile->TouchY[6] = Core::stoi(iniParser::getstring(ini, "profile:touch06Y", "0"));
				Profile->TouchY[7] = Core::stoi(iniParser::getstring(ini, "profile:touch07Y", "0"));
				Profile->TouchY[8] = Core::stoi(iniParser::getstring(ini, "profile:touch08Y", "0"));
				Profile->TouchY[9] = Core::stoi(iniParser::getstring(ini, "profile:touch09Y", "0"));
				Profile->TouchY[10] = Core::stoi(iniParser::getstring(ini, "profile:touch10Y", "0"));
				Profile->TouchY[11] = Core::stoi(iniParser::getstring(ini, "profile:touch11Y", "0"));

				Profile->TouchW[0] = Core::stoi(iniParser::getstring(ini, "profile:touch00W", "0"));
				Profile->TouchW[1] = Core::stoi(iniParser::getstring(ini, "profile:touch01W", "0"));
				Profile->TouchW[2] = Core::stoi(iniParser::getstring(ini, "profile:touch02W", "0"));
				Profile->TouchW[3] = Core::stoi(iniParser::getstring(ini, "profile:touch03W", "0"));
				Profile->TouchW[4] = Core::stoi(iniParser::getstring(ini, "profile:touch04W", "0"));
				Profile->TouchW[5] = Core::stoi(iniParser::getstring(ini, "profile:touch05W", "0"));
				Profile->TouchW[6] = Core::stoi(iniParser::getstring(ini, "profile:touch06W", "0"));
				Profile->TouchW[7] = Core::stoi(iniParser::getstring(ini, "profile:touch07W", "0"));
				Profile->TouchW[8] = Core::stoi(iniParser::getstring(ini, "profile:touch08W", "0"));
				Profile->TouchW[9] = Core::stoi(iniParser::getstring(ini, "profile:touch09W", "0"));
				Profile->TouchW[10] = Core::stoi(iniParser::getstring(ini, "profile:touch10W", "0"));
				Profile->TouchW[11] = Core::stoi(iniParser::getstring(ini, "profile:touch11W", "0"));

				Profile->TouchH[0] = Core::stoi(iniParser::getstring(ini, "profile:touch00H", "0"));
				Profile->TouchH[1] = Core::stoi(iniParser::getstring(ini, "profile:touch01H", "0"));
				Profile->TouchH[2] = Core::stoi(iniParser::getstring(ini, "profile:touch02H", "0"));
				Profile->TouchH[3] = Core::stoi(iniParser::getstring(ini, "profile:touch03H", "0"));
				Profile->TouchH[4] = Core::stoi(iniParser::getstring(ini, "profile:touch04H", "0"));
				Profile->TouchH[5] = Core::stoi(iniParser::getstring(ini, "profile:touch05H", "0"));
				Profile->TouchH[6] = Core::stoi(iniParser::getstring(ini, "profile:touch06H", "0"));
				Profile->TouchH[7] = Core::stoi(iniParser::getstring(ini, "profile:touch07H", "0"));
				Profile->TouchH[8] = Core::stoi(iniParser::getstring(ini, "profile:touch08H", "0"));
				Profile->TouchH[9] = Core::stoi(iniParser::getstring(ini, "profile:touch09H", "0"));
				Profile->TouchH[10] = Core::stoi(iniParser::getstring(ini, "profile:touch10H", "0"));
				Profile->TouchH[11] = Core::stoi(iniParser::getstring(ini, "profile:touch11H", "0"));
				iniParser::freedict(ini);

				return 0;
			}

			int Config::Save() {
				FILE *file = fopen(iniFilename, "w");

				if(file == NULL) {
					int err = errno;
					std::clog << "Error (fopen): #" << err << "\nFailed to open file: " << iniFilename << "\n";

					return err;
				}

				fprintf(file, "[Settings]\n");
				fprintf(file, "Port=%u\n", Port);

				for(int i = 0; i < 256; i++) {
					if(Commands[i])
						fprintf(file, "Command%i=\"%s\"\n", i, Commands[i]);
				}

				fclose(file);
				Load();

				return 0;
			}

			int Config::SaveProfile(ProfileData *Profile, uint8_t profileNumber) {
				std::ostringstream ssfilename;
				ssfilename << "ds2key.p" << (int)profileNumber << ".ini";

				FILE *file = fopen(ssfilename.str().c_str(), "w");
				if(file == NULL) {
					int err = errno;
					std::clog << "Error (fopen): #" << err << "\nFailed to save file: " << iniFilename << "\n";

					return err;
				}

				fprintf(file, "[Profile]\n");
				fprintf(file, "Mouse=%s\n", Profile->Mouse.c_str());
				fprintf(file, "Joy=%s\n", Profile->Joy.c_str());
				fprintf(file, "Up=%s\n", Profile->GetButtonString(kUp).c_str());
				fprintf(file, "Down=%s\n", Profile->GetButtonString(kDown).c_str());
				fprintf(file, "Left=%s\n", Profile->GetButtonString(kLeft).c_str());
				fprintf(file, "Right=%s\n", Profile->GetButtonString(kRight).c_str());
				fprintf(file, "A=%s\n", Profile->GetButtonString(kA).c_str());
				fprintf(file, "B=%s\n", Profile->GetButtonString(kB).c_str());
				fprintf(file, "X=%s\n", Profile->GetButtonString(kX).c_str());
				fprintf(file, "Y=%s\n", Profile->GetButtonString(kY).c_str());
				fprintf(file, "L=%s\n", Profile->GetButtonString(kL).c_str());
				fprintf(file, "R=%s\n", Profile->GetButtonString(kR).c_str());
				fprintf(file, "Start=%s\n", Profile->GetButtonString(kStart).c_str());
				fprintf(file, "Select=%s\n", Profile->GetButtonString(kSelect).c_str());
				fprintf(file, "Lid=%s\n", Profile->GetButtonString(kLid).c_str());
				fprintf(file, "Blue=%s\n", Profile->GetButtonString(kBlue).c_str());
				fprintf(file, "Yellow=%s\n", Profile->GetButtonString(kYellow).c_str());
				fprintf(file, "Red=%s\n", Profile->GetButtonString(kRed).c_str());
				fprintf(file, "Green=%s\n", Profile->GetButtonString(kGreen).c_str());
				fprintf(file, "Touch00=%s\n", Profile->GetButtonString(kTouch00).c_str());
				fprintf(file, "Touch01=%s\n", Profile->GetButtonString(kTouch01).c_str());
				fprintf(file, "Touch02=%s\n", Profile->GetButtonString(kTouch02).c_str());
				fprintf(file, "Touch03=%s\n", Profile->GetButtonString(kTouch03).c_str());
				fprintf(file, "Touch04=%s\n", Profile->GetButtonString(kTouch04).c_str());
				fprintf(file, "Touch05=%s\n", Profile->GetButtonString(kTouch05).c_str());
				fprintf(file, "Touch06=%s\n", Profile->GetButtonString(kTouch06).c_str());
				fprintf(file, "Touch07=%s\n", Profile->GetButtonString(kTouch07).c_str());
				fprintf(file, "Touch08=%s\n", Profile->GetButtonString(kTouch08).c_str());
				fprintf(file, "Touch09=%s\n", Profile->GetButtonString(kTouch09).c_str());
				fprintf(file, "Touch10=%s\n", Profile->GetButtonString(kTouch10).c_str());
				fprintf(file, "Touch11=%s\n", Profile->GetButtonString(kTouch11).c_str());

				fprintf(file, "Touch00X=%i\n", Profile->TouchX[0]);
				fprintf(file, "Touch01X=%i\n", Profile->TouchX[1]);
				fprintf(file, "Touch02X=%i\n", Profile->TouchX[2]);
				fprintf(file, "Touch03X=%i\n", Profile->TouchX[3]);
				fprintf(file, "Touch04X=%i\n", Profile->TouchX[4]);
				fprintf(file, "Touch05X=%i\n", Profile->TouchX[5]);
				fprintf(file, "Touch06X=%i\n", Profile->TouchX[6]);
				fprintf(file, "Touch07X=%i\n", Profile->TouchX[7]);
				fprintf(file, "Touch08X=%i\n", Profile->TouchX[8]);
				fprintf(file, "Touch09X=%i\n", Profile->TouchX[9]);
				fprintf(file, "Touch10X=%i\n", Profile->TouchX[10]);
				fprintf(file, "Touch11X=%i\n", Profile->TouchX[11]);

				fprintf(file, "Touch00Y=%i\n", Profile->TouchY[0]);
				fprintf(file, "Touch01Y=%i\n", Profile->TouchY[1]);
				fprintf(file, "Touch02Y=%i\n", Profile->TouchY[2]);
				fprintf(file, "Touch03Y=%i\n", Profile->TouchY[3]);
				fprintf(file, "Touch04Y=%i\n", Profile->TouchY[4]);
				fprintf(file, "Touch05Y=%i\n", Profile->TouchY[5]);
				fprintf(file, "Touch06Y=%i\n", Profile->TouchY[6]);
				fprintf(file, "Touch07Y=%i\n", Profile->TouchY[7]);
				fprintf(file, "Touch08Y=%i\n", Profile->TouchY[8]);
				fprintf(file, "Touch09Y=%i\n", Profile->TouchY[9]);
				fprintf(file, "Touch10Y=%i\n", Profile->TouchY[10]);
				fprintf(file, "Touch11Y=%i\n", Profile->TouchY[11]);

				fprintf(file, "Touch00W=%i\n", Profile->TouchW[0]);
				fprintf(file, "Touch01W=%i\n", Profile->TouchW[1]);
				fprintf(file, "Touch02W=%i\n", Profile->TouchW[2]);
				fprintf(file, "Touch03W=%i\n", Profile->TouchW[3]);
				fprintf(file, "Touch04W=%i\n", Profile->TouchW[4]);
				fprintf(file, "Touch05W=%i\n", Profile->TouchW[5]);
				fprintf(file, "Touch06W=%i\n", Profile->TouchW[6]);
				fprintf(file, "Touch07W=%i\n", Profile->TouchW[7]);
				fprintf(file, "Touch08W=%i\n", Profile->TouchW[8]);
				fprintf(file, "Touch09W=%i\n", Profile->TouchW[9]);
				fprintf(file, "Touch10W=%i\n", Profile->TouchW[10]);
				fprintf(file, "Touch11W=%i\n", Profile->TouchW[11]);

				fprintf(file, "Touch00H=%i\n", Profile->TouchH[0]);
				fprintf(file, "Touch01H=%i\n", Profile->TouchH[1]);
				fprintf(file, "Touch02H=%i\n", Profile->TouchH[2]);
				fprintf(file, "Touch03H=%i\n", Profile->TouchH[3]);
				fprintf(file, "Touch04H=%i\n", Profile->TouchH[4]);
				fprintf(file, "Touch05H=%i\n", Profile->TouchH[5]);
				fprintf(file, "Touch06H=%i\n", Profile->TouchH[6]);
				fprintf(file, "Touch07H=%i\n", Profile->TouchH[7]);
				fprintf(file, "Touch08H=%i\n", Profile->TouchH[8]);
				fprintf(file, "Touch09H=%i\n", Profile->TouchH[9]);
				fprintf(file, "Touch10H=%i\n", Profile->TouchH[10]);
				fprintf(file, "Touch11H=%i\n", Profile->TouchH[11]);

				fclose(file);

				return 0;
			}

			uint16_t Config::GetPort() {
				return Port;
			}

			void Config::SetPort(uint16_t Port) {
				if(Port == 0)
					Config::Port = DefaultPort;
				else
					Config::Port = Port;
			}

			char *Config::GetCommand(uint8_t Command) {
				return Commands[Command];
			}
		}
	}
}
