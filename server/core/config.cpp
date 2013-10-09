/*
	configuration loading/saving
*/

#include <sstream>//ostringstream
#include "key.h"
#include "config.h"
#include "client.h"
#include "core/iniParser.h"

namespace D2K
{
	namespace Core
	{
		Config *config = (Config*)NULL;
		Config::Config()
		{
			Load();
		}

		Config::~Config()
		{
			Save();
		}

		bool Config::Save()
		{
			FILE *file = fopen("ds2key.ini", "w");

			if(file)
			{
				fprintf(file, "[Settings]\n");
				fprintf(file, "Port=%i\n", Port);
				fprintf(file, "Debug=%i\n", Debug);
				fclose(file);
				Load();

				return 0;
			}

			return 1;
		}

		bool Config::SaveProfile(uint16_t *Profile, uint8_t profileNumber)
		{
			FILE *file;
			char filename[16];
			sprintf(filename, "ds2key.p%i.ini", profileNumber);
			file = fopen(filename, "w");

			if(file)
			{
				fprintf(file, "[Profile]\n");
				if(Profile[pMouse] == mAbsolute)
					fprintf(file, "Mouse=Absolute\n");
				else if(Profile[pMouse] == mButtons)
					fprintf(file, "Mouse=Buttons\n");
				else
					fprintf(file, "Mouse=Relative\n");
				fprintf(file, "Joy=%i\n", Profile[pJoy]);
				fprintf(file, "Up=%s\n", Key::GetString(Profile[pUp]));
				fprintf(file, "Down=%s\n", Key::GetString(Profile[pDown]));
				fprintf(file, "Left=%s\n", Key::GetString(Profile[pLeft]));
				fprintf(file, "Right=%s\n", Key::GetString(Profile[pRight]));
				fprintf(file, "A=%s\n", Key::GetString(Profile[pA]));
				fprintf(file, "B=%s\n", Key::GetString(Profile[pB]));
				fprintf(file, "X=%s\n", Key::GetString(Profile[pX]));
				fprintf(file, "Y=%s\n", Key::GetString(Profile[pY]));
				fprintf(file, "L=%s\n", Key::GetString(Profile[pL]));
				fprintf(file, "R=%s\n", Key::GetString(Profile[pR]));
				fprintf(file, "Start=%s\n", Key::GetString(Profile[pStart]));
				fprintf(file, "Select=%s\n", Key::GetString(Profile[pSelect]));
				fprintf(file, "Lid=%s\n", Key::GetString(Profile[pLid]));
				fprintf(file, "Blue=%s\n", Key::GetString(Profile[pBlue]));
				fprintf(file, "Yellow=%s\n", Key::GetString(Profile[pYellow]));
				fprintf(file, "Red=%s\n", Key::GetString(Profile[pRed]));
				fprintf(file, "Green=%s\n", Key::GetString(Profile[pGreen]));
				fprintf(file, "Touch00=%s\n", Key::GetString(Profile[pTouch00]));
				fprintf(file, "Touch01=%s\n", Key::GetString(Profile[pTouch01]));
				fprintf(file, "Touch02=%s\n", Key::GetString(Profile[pTouch02]));
				fprintf(file, "Touch03=%s\n", Key::GetString(Profile[pTouch03]));
				fprintf(file, "Touch04=%s\n", Key::GetString(Profile[pTouch04]));
				fprintf(file, "Touch05=%s\n", Key::GetString(Profile[pTouch05]));
				fprintf(file, "Touch06=%s\n", Key::GetString(Profile[pTouch06]));
				fprintf(file, "Touch07=%s\n", Key::GetString(Profile[pTouch07]));
				fprintf(file, "Touch08=%s\n", Key::GetString(Profile[pTouch08]));
				fprintf(file, "Touch09=%s\n", Key::GetString(Profile[pTouch09]));
				fprintf(file, "Touch10=%s\n", Key::GetString(Profile[pTouch10]));
				fprintf(file, "Touch11=%s\n", Key::GetString(Profile[pTouch11]));

				fprintf(file, "Touch00X=%i\n", Profile[pTouch00X]);
				fprintf(file, "Touch01X=%i\n", Profile[pTouch01X]);
				fprintf(file, "Touch02X=%i\n", Profile[pTouch02X]);
				fprintf(file, "Touch03X=%i\n", Profile[pTouch03X]);
				fprintf(file, "Touch04X=%i\n", Profile[pTouch04X]);
				fprintf(file, "Touch05X=%i\n", Profile[pTouch05X]);
				fprintf(file, "Touch06X=%i\n", Profile[pTouch06X]);
				fprintf(file, "Touch07X=%i\n", Profile[pTouch07X]);
				fprintf(file, "Touch08X=%i\n", Profile[pTouch08X]);
				fprintf(file, "Touch09X=%i\n", Profile[pTouch09X]);
				fprintf(file, "Touch10X=%i\n", Profile[pTouch10X]);
				fprintf(file, "Touch11X=%i\n", Profile[pTouch11X]);

				fprintf(file, "Touch00Y=%i\n", Profile[pTouch00Y]);
				fprintf(file, "Touch01Y=%i\n", Profile[pTouch01Y]);
				fprintf(file, "Touch02Y=%i\n", Profile[pTouch02Y]);
				fprintf(file, "Touch03Y=%i\n", Profile[pTouch03Y]);
				fprintf(file, "Touch04Y=%i\n", Profile[pTouch04Y]);
				fprintf(file, "Touch05Y=%i\n", Profile[pTouch05Y]);
				fprintf(file, "Touch06Y=%i\n", Profile[pTouch06Y]);
				fprintf(file, "Touch07Y=%i\n", Profile[pTouch07Y]);
				fprintf(file, "Touch08Y=%i\n", Profile[pTouch08Y]);
				fprintf(file, "Touch09Y=%i\n", Profile[pTouch09Y]);
				fprintf(file, "Touch10Y=%i\n", Profile[pTouch10Y]);
				fprintf(file, "Touch11Y=%i\n", Profile[pTouch11Y]);

				fprintf(file, "Touch00W=%i\n", Profile[pTouch00W]);
				fprintf(file, "Touch01W=%i\n", Profile[pTouch01W]);
				fprintf(file, "Touch02W=%i\n", Profile[pTouch02W]);
				fprintf(file, "Touch03W=%i\n", Profile[pTouch03W]);
				fprintf(file, "Touch04W=%i\n", Profile[pTouch04W]);
				fprintf(file, "Touch05W=%i\n", Profile[pTouch05W]);
				fprintf(file, "Touch06W=%i\n", Profile[pTouch06W]);
				fprintf(file, "Touch07W=%i\n", Profile[pTouch07W]);
				fprintf(file, "Touch08W=%i\n", Profile[pTouch08W]);
				fprintf(file, "Touch09W=%i\n", Profile[pTouch09W]);
				fprintf(file, "Touch10W=%i\n", Profile[pTouch10W]);
				fprintf(file, "Touch11W=%i\n", Profile[pTouch11W]);

				fprintf(file, "Touch00H=%i\n", Profile[pTouch00H]);
				fprintf(file, "Touch01H=%i\n", Profile[pTouch01H]);
				fprintf(file, "Touch02H=%i\n", Profile[pTouch02H]);
				fprintf(file, "Touch03H=%i\n", Profile[pTouch03H]);
				fprintf(file, "Touch04H=%i\n", Profile[pTouch04H]);
				fprintf(file, "Touch05H=%i\n", Profile[pTouch05H]);
				fprintf(file, "Touch06H=%i\n", Profile[pTouch06H]);
				fprintf(file, "Touch07H=%i\n", Profile[pTouch07H]);
				fprintf(file, "Touch08H=%i\n", Profile[pTouch08H]);
				fprintf(file, "Touch09H=%i\n", Profile[pTouch09H]);
				fprintf(file, "Touch10H=%i\n", Profile[pTouch10H]);
				fprintf(file, "Touch11H=%i\n", Profile[pTouch11H]);

				fclose(file);

				return 0;
			}

			return 1;
		}
		bool Config::SaveProfile(uint8_t profileNumber)
		{
			return SaveProfile(Core::ClientArray[profileNumber]->GetSettings(), profileNumber);
		}

		bool Config::Load()
		{
			dictionary *ini = D2K::Core::iniParser::load("ds2key.ini");

			if(ini == NULL)
			{
				fprintf(stderr, "cannot parse file: %s\n", "ds2key.ini");
				SetPort(DEFAULT_SERVER_PORT);
				Debug = lDefault;
				Save();

				return 1;
			}

			iniParser::dump(ini, stderr);

			SetPort(iniParser::getint(ini, "settings:port", DEFAULT_SERVER_PORT));
			Debug = iniParser::getint(ini, "settings:debug", lDefault);
			iniParser::freedict(ini);

			return 0;
		}

		bool Config::ReadProfile(uint16_t *Profile, uint8_t profileNumber)
		{
			dictionary *ini;
			std::ostringstream ssfilename;
			ssfilename << "ds2key.p" << (int)profileNumber << ".ini";

			ini = iniParser::load(ssfilename.str().c_str());
			iniParser::dump(ini, stderr);

			if(ini == NULL)
			{
				fprintf(stderr, "cannot parse file: %s\n", ssfilename.str().c_str());
				Profile[pMouse] = mRelative;
				Profile[pJoy] = 0;
				Profile[pUp] = KEY_UP;
				Profile[pDown] = KEY_DOWN;
				Profile[pLeft] = KEY_LEFT;
				Profile[pRight] = KEY_RIGHT;
				Profile[pA] = KEY_A;
				Profile[pB] = KEY_B;
				Profile[pX] = KEY_X;
				Profile[pY] = KEY_Y;
				Profile[pL] = KEY_L;
				Profile[pR] = KEY_R;
				Profile[pStart] = KEY_RETURN;
				Profile[pSelect] = KEY_RSHIFT;
				Profile[pLid] = KEY_ESCAPE;
				Profile[pBlue] = KEY_1;
				Profile[pYellow] = KEY_2;
				Profile[pRed] = KEY_3;
				Profile[pGreen] = KEY_4;

				Profile[pTouch00] = KEY_NUMPAD7;
				Profile[pTouch01] = KEY_NUMPAD7;
				Profile[pTouch02] = KEY_NUMPAD7;
				Profile[pTouch03] = KEY_NUMPAD7;
				Profile[pTouch04] = KEY_NUMPAD7;
				Profile[pTouch05] = KEY_NUMPAD7;
				Profile[pTouch06] = KEY_NUMPAD7;
				Profile[pTouch07] = KEY_NUMPAD7;
				Profile[pTouch08] = KEY_NUMPAD7;
				Profile[pTouch09] = KEY_NUMPAD7;
				Profile[pTouch10] = KEY_NUMPAD7;
				Profile[pTouch11] = KEY_NUMPAD7;

				Profile[pTouch00X] = 0;
				Profile[pTouch01X] = 0;
				Profile[pTouch02X] = 0;
				Profile[pTouch03X] = 0;
				Profile[pTouch04X] = 0;
				Profile[pTouch05X] = 0;
				Profile[pTouch06X] = 0;
				Profile[pTouch07X] = 0;
				Profile[pTouch08X] = 0;
				Profile[pTouch09X] = 0;
				Profile[pTouch10X] = 0;
				Profile[pTouch11X] = 0;

				Profile[pTouch00Y] = 0;
				Profile[pTouch01Y] = 0;
				Profile[pTouch02Y] = 0;
				Profile[pTouch03Y] = 0;
				Profile[pTouch04Y] = 0;
				Profile[pTouch05Y] = 0;
				Profile[pTouch06Y] = 0;
				Profile[pTouch07Y] = 0;
				Profile[pTouch08Y] = 0;
				Profile[pTouch09Y] = 0;
				Profile[pTouch10Y] = 0;
				Profile[pTouch11Y] = 0;

				Profile[pTouch00W] = 0;
				Profile[pTouch01W] = 0;
				Profile[pTouch02W] = 0;
				Profile[pTouch03W] = 0;
				Profile[pTouch04W] = 0;
				Profile[pTouch05W] = 0;
				Profile[pTouch06W] = 0;
				Profile[pTouch07W] = 0;
				Profile[pTouch08W] = 0;
				Profile[pTouch09W] = 0;
				Profile[pTouch10W] = 0;
				Profile[pTouch11W] = 0;

				Profile[pTouch00H] = 0;
				Profile[pTouch01H] = 0;
				Profile[pTouch02H] = 0;
				Profile[pTouch03H] = 0;
				Profile[pTouch04H] = 0;
				Profile[pTouch05H] = 0;
				Profile[pTouch06H] = 0;
				Profile[pTouch07H] = 0;
				Profile[pTouch08H] = 0;
				Profile[pTouch09H] = 0;
				Profile[pTouch10H] = 0;
				Profile[pTouch11H] = 0;

				SaveProfile(Profile, profileNumber);

				return 1;
			}
			iniParser::dump(ini, stderr);

			{
				std::string mouse = iniParser::getstring(ini, "profile:mouse", "Relative");

				if(strcmp(mouse.c_str(), "Absolute") == 0)
					Profile[pMouse] = mAbsolute;
				else if(strcmp(mouse.c_str(), "Buttons") == 0)
					Profile[pMouse] = mButtons;
				else
					Profile[pMouse] = mRelative;
			}

			Profile[pJoy] = atoi(iniParser::getstring(ini, "profile:joy", "0").c_str());
			Profile[pUp] = Key::GetNumber(iniParser::getstring(ini, "profile:up", "Key_None").c_str());
			Profile[pDown] = Key::GetNumber(iniParser::getstring(ini, "profile:down", "Key_None").c_str());
			Profile[pLeft] = Key::GetNumber(iniParser::getstring(ini, "profile:left", "Key_None").c_str());
			Profile[pRight] = Key::GetNumber(iniParser::getstring(ini, "profile:right", "Key_None").c_str());
			Profile[pA] = Key::GetNumber(iniParser::getstring(ini, "profile:a", "Key_None").c_str());
			Profile[pB] = Key::GetNumber(iniParser::getstring(ini, "profile:b", "Key_None").c_str());
			Profile[pX] = Key::GetNumber(iniParser::getstring(ini, "profile:x", "Key_None").c_str());
			Profile[pY] = Key::GetNumber(iniParser::getstring(ini, "profile:y", "Key_None").c_str());
			Profile[pL] = Key::GetNumber(iniParser::getstring(ini, "profile:l", "Key_None").c_str());
			Profile[pR] = Key::GetNumber(iniParser::getstring(ini, "profile:r", "Key_None").c_str());
			Profile[pStart] = Key::GetNumber(iniParser::getstring(ini, "profile:start", "Key_None").c_str());
			Profile[pSelect] = Key::GetNumber(iniParser::getstring(ini, "profile:select", "Key_None").c_str());
			Profile[pLid] = Key::GetNumber(iniParser::getstring(ini, "profile:lid", "Key_None").c_str());
			Profile[pBlue] = Key::GetNumber(iniParser::getstring(ini, "profile:blue", "Key_None").c_str());
			Profile[pYellow] = Key::GetNumber(iniParser::getstring(ini, "profile:yellow", "Key_None").c_str());
			Profile[pRed] = Key::GetNumber(iniParser::getstring(ini, "profile:red", "Key_None").c_str());
			Profile[pGreen] = Key::GetNumber(iniParser::getstring(ini, "profile:green", "Key_None").c_str());
			Profile[pTouch00] = Key::GetNumber(iniParser::getstring(ini, "profile:touch00", "0").c_str());
			Profile[pTouch01] = Key::GetNumber(iniParser::getstring(ini, "profile:touch01", "0").c_str());
			Profile[pTouch02] = Key::GetNumber(iniParser::getstring(ini, "profile:touch02", "0").c_str());
			Profile[pTouch03] = Key::GetNumber(iniParser::getstring(ini, "profile:touch03", "0").c_str());
			Profile[pTouch04] = Key::GetNumber(iniParser::getstring(ini, "profile:touch04", "0").c_str());
			Profile[pTouch05] = Key::GetNumber(iniParser::getstring(ini, "profile:touch05", "0").c_str());
			Profile[pTouch06] = Key::GetNumber(iniParser::getstring(ini, "profile:touch06", "0").c_str());
			Profile[pTouch07] = Key::GetNumber(iniParser::getstring(ini, "profile:touch07", "0").c_str());
			Profile[pTouch08] = Key::GetNumber(iniParser::getstring(ini, "profile:touch08", "0").c_str());
			Profile[pTouch09] = Key::GetNumber(iniParser::getstring(ini, "profile:touch09", "0").c_str());
			Profile[pTouch10] = Key::GetNumber(iniParser::getstring(ini, "profile:touch10", "0").c_str());
			Profile[pTouch11] = Key::GetNumber(iniParser::getstring(ini, "profile:touch11", "0").c_str());

			Profile[pTouch00X] = atoi(iniParser::getstring(ini, "profile:touch00X", "0").c_str());
			Profile[pTouch01X] = atoi(iniParser::getstring(ini, "profile:touch01X", "0").c_str());
			Profile[pTouch02X] = atoi(iniParser::getstring(ini, "profile:touch02X", "0").c_str());
			Profile[pTouch03X] = atoi(iniParser::getstring(ini, "profile:touch03X", "0").c_str());
			Profile[pTouch04X] = atoi(iniParser::getstring(ini, "profile:touch04X", "0").c_str());
			Profile[pTouch05X] = atoi(iniParser::getstring(ini, "profile:touch05X", "0").c_str());
			Profile[pTouch06X] = atoi(iniParser::getstring(ini, "profile:touch06X", "0").c_str());
			Profile[pTouch07X] = atoi(iniParser::getstring(ini, "profile:touch07X", "0").c_str());
			Profile[pTouch08X] = atoi(iniParser::getstring(ini, "profile:touch08X", "0").c_str());
			Profile[pTouch09X] = atoi(iniParser::getstring(ini, "profile:touch09X", "0").c_str());
			Profile[pTouch10X] = atoi(iniParser::getstring(ini, "profile:touch10X", "0").c_str());
			Profile[pTouch11X] = atoi(iniParser::getstring(ini, "profile:touch11X", "0").c_str());

			Profile[pTouch00Y] = atoi(iniParser::getstring(ini, "profile:touch00Y", "0").c_str());
			Profile[pTouch01Y] = atoi(iniParser::getstring(ini, "profile:touch01Y", "0").c_str());
			Profile[pTouch02Y] = atoi(iniParser::getstring(ini, "profile:touch02Y", "0").c_str());
			Profile[pTouch03Y] = atoi(iniParser::getstring(ini, "profile:touch03Y", "0").c_str());
			Profile[pTouch04Y] = atoi(iniParser::getstring(ini, "profile:touch04Y", "0").c_str());
			Profile[pTouch05Y] = atoi(iniParser::getstring(ini, "profile:touch05Y", "0").c_str());
			Profile[pTouch06Y] = atoi(iniParser::getstring(ini, "profile:touch06Y", "0").c_str());
			Profile[pTouch07Y] = atoi(iniParser::getstring(ini, "profile:touch07Y", "0").c_str());
			Profile[pTouch08Y] = atoi(iniParser::getstring(ini, "profile:touch08Y", "0").c_str());
			Profile[pTouch09Y] = atoi(iniParser::getstring(ini, "profile:touch09Y", "0").c_str());
			Profile[pTouch10Y] = atoi(iniParser::getstring(ini, "profile:touch10Y", "0").c_str());
			Profile[pTouch11Y] = atoi(iniParser::getstring(ini, "profile:touch11Y", "0").c_str());

			Profile[pTouch00W] = atoi(iniParser::getstring(ini, "profile:touch00W", "0").c_str());
			Profile[pTouch01W] = atoi(iniParser::getstring(ini, "profile:touch01W", "0").c_str());
			Profile[pTouch02W] = atoi(iniParser::getstring(ini, "profile:touch02W", "0").c_str());
			Profile[pTouch03W] = atoi(iniParser::getstring(ini, "profile:touch03W", "0").c_str());
			Profile[pTouch04W] = atoi(iniParser::getstring(ini, "profile:touch04W", "0").c_str());
			Profile[pTouch05W] = atoi(iniParser::getstring(ini, "profile:touch05W", "0").c_str());
			Profile[pTouch06W] = atoi(iniParser::getstring(ini, "profile:touch06W", "0").c_str());
			Profile[pTouch07W] = atoi(iniParser::getstring(ini, "profile:touch07W", "0").c_str());
			Profile[pTouch08W] = atoi(iniParser::getstring(ini, "profile:touch08W", "0").c_str());
			Profile[pTouch09W] = atoi(iniParser::getstring(ini, "profile:touch09W", "0").c_str());
			Profile[pTouch10W] = atoi(iniParser::getstring(ini, "profile:touch10W", "0").c_str());
			Profile[pTouch11W] = atoi(iniParser::getstring(ini, "profile:touch11W", "0").c_str());

			Profile[pTouch00H] = atoi(iniParser::getstring(ini, "profile:touch00H", "0").c_str());
			Profile[pTouch01H] = atoi(iniParser::getstring(ini, "profile:touch01H", "0").c_str());
			Profile[pTouch02H] = atoi(iniParser::getstring(ini, "profile:touch02H", "0").c_str());
			Profile[pTouch03H] = atoi(iniParser::getstring(ini, "profile:touch03H", "0").c_str());
			Profile[pTouch04H] = atoi(iniParser::getstring(ini, "profile:touch04H", "0").c_str());
			Profile[pTouch05H] = atoi(iniParser::getstring(ini, "profile:touch05H", "0").c_str());
			Profile[pTouch06H] = atoi(iniParser::getstring(ini, "profile:touch06H", "0").c_str());
			Profile[pTouch07H] = atoi(iniParser::getstring(ini, "profile:touch07H", "0").c_str());
			Profile[pTouch08H] = atoi(iniParser::getstring(ini, "profile:touch08H", "0").c_str());
			Profile[pTouch09H] = atoi(iniParser::getstring(ini, "profile:touch09H", "0").c_str());
			Profile[pTouch10H] = atoi(iniParser::getstring(ini, "profile:touch10H", "0").c_str());
			Profile[pTouch11H] = atoi(iniParser::getstring(ini, "profile:touch11H", "0").c_str());
			iniParser::freedict(ini);

			return 0;
		}

		bool Config::ReadProfile(uint8_t profileNumber)
		{
			return ReadProfile(Core::ClientArray[profileNumber]->GetSettings(), profileNumber);
		}

		bool Config::ReadProfileArray(uint8_t profileNumber)
		{
			if(ClientArray[profileNumber] == (Core::Client*)NULL) {	//if profile not active,
				ClientArray[profileNumber] = new Core::Client();	//create it
				return config->ReadProfile(profileNumber);			//then load it
			}
			return 0;
		}

		uint16_t Config::GetPort()
		{
			return Port;
		}

		void Config::SetPort(uint16_t port)
		{
			if(port == 0)
				Port = DEFAULT_SERVER_PORT;
			else
				Port = port;
		}

		uint8_t Config::GetDebugLevel()
		{
			return Debug;
		}
	}
}
