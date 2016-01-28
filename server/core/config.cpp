// configuration loading/saving

#include <iostream>//std::cout, std::clog
#include <sstream>//ostringstream
#include "config.h"
#include "key.h"
#include "common/iniParserWrapper.h"
#include "common/misc.h"
#include "client.h"

namespace D2K {namespace Config {

const char* g_ini_filename = "ds2key.ini";

int Load()
{
	dictionary* ini = D2K::iniParser::load(g_ini_filename);

	if(ini == nullptr)
	{
		int err = errno;
		std::clog << "Error (iniParser::load): #" << err << "\nFailed to open file: " << g_ini_filename << "\n";
		SetConfigPort(DEFAULT_PORT);
		Save();

		return err;
	}

	iniParser::dump(ini, stderr);

	SetConfigPort(iniParser::getint(ini, "settings:port", DEFAULT_PORT));
	iniParser::freedict(ini);

	return 0;
}

int LoadProfile(ProfileData* profile_data, uint8_t profile_number)
{
	std::ostringstream ssfilename;
	ssfilename << "ds2key.p" << (int)profile_number << ".ini";

	dictionary* ini = iniParser::load(ssfilename.str().c_str());
	iniParser::dump(ini, stderr);
	if(ini == nullptr)
	{
		int err = errno;
		std::clog << "Error (iniParser::load): #" << err << "\nFailed to open file: " << g_ini_filename << "\n";
		profile_data->m_mouse = "Relative";
		profile_data->m_joy = "0";
		profile_data->SetVirtualKey(KEYS::UP, KEY_UP);
		profile_data->SetVirtualKey(KEYS::DOWN, KEY_DOWN);
		profile_data->SetVirtualKey(KEYS::LEFT, KEY_LEFT);
		profile_data->SetVirtualKey(KEYS::RIGHT, KEY_RIGHT);
		profile_data->SetVirtualKey(KEYS::A, KEY_A);
		profile_data->SetVirtualKey(KEYS::B, KEY_B);
		profile_data->SetVirtualKey(KEYS::X, KEY_X);
		profile_data->SetVirtualKey(KEYS::Y, KEY_Y);
		profile_data->SetVirtualKey(KEYS::L, KEY_L);
		profile_data->SetVirtualKey(KEYS::R, KEY_R);
		profile_data->SetVirtualKey(KEYS::START, KEY_RETURN);
		profile_data->SetVirtualKey(KEYS::SELECT, KEY_RSHIFT);
		profile_data->SetVirtualKey(KEYS::LID, KEY_ESCAPE);
		profile_data->SetVirtualKey(KEYS::BLUE, KEY_C);
		profile_data->SetVirtualKey(KEYS::YELLOW, KEY_D);
		profile_data->SetVirtualKey(KEYS::RED, KEY_E);
		profile_data->SetVirtualKey(KEYS::GREEN, KEY_F);

		profile_data->SetVirtualKey(KEYS::TOUCH_00, KEY_0);
		profile_data->SetVirtualKey(KEYS::TOUCH_01, KEY_1);
		profile_data->SetVirtualKey(KEYS::TOUCH_02, KEY_2);
		profile_data->SetVirtualKey(KEYS::TOUCH_03, KEY_3);
		profile_data->SetVirtualKey(KEYS::TOUCH_04, KEY_4);
		profile_data->SetVirtualKey(KEYS::TOUCH_05, KEY_5);
		profile_data->SetVirtualKey(KEYS::TOUCH_06, KEY_6);
		profile_data->SetVirtualKey(KEYS::TOUCH_07, KEY_7);
		profile_data->SetVirtualKey(KEYS::TOUCH_08, KEY_8);
		profile_data->SetVirtualKey(KEYS::TOUCH_09, KEY_9);
		profile_data->SetVirtualKey(KEYS::TOUCH_10, KEY_G);
		profile_data->SetVirtualKey(KEYS::TOUCH_11, KEY_H);

		for(int i = 0; i < UDP::SETTINGS_PACKET_MAX_BUTTONS; i++)
		{
			profile_data->m_touch_string[i] = "Command ";
			profile_data->m_touch_string[i].append(ltos(i));
			profile_data->m_touch_string[i].resize(10);
		}

		const int screenW = 256;
		const int screenH = 192;
		int TouchButton = 0;
		for(int y = 0; y < screenH; y += screenH / 3)
			for(int x = 0; x < screenW; x += screenW / 4)
				profile_data->SetTouchPos(	TouchButton++,	//buttons 0 - 11
										x,				//x
										y,				//y
										screenW / 4,	//w
										screenH / 3		//h
										);

		SaveProfile(profile_data, profile_number);

		return 1;
	}

	iniParser::dump(ini, stderr);

	std::string mouse = iniParser::getstring(ini, "profile:mouse", "Relative");

	if(mouse == "Absolute")
		profile_data->m_mouse = mouse;
	else
		profile_data->m_mouse = "Relative";

	profile_data->m_joy = iniParser::getstring(ini, "profile:joy", "0");
	profile_data->SetCommand(KEYS::UP, iniParser::getstring(ini, "profile:up", "Key_None"));
	profile_data->SetCommand(KEYS::DOWN, iniParser::getstring(ini, "profile:down", "Key_None"));
	profile_data->SetCommand(KEYS::LEFT, iniParser::getstring(ini, "profile:left", "Key_None"));
	profile_data->SetCommand(KEYS::RIGHT, iniParser::getstring(ini, "profile:right", "Key_None"));
	profile_data->SetCommand(KEYS::A, iniParser::getstring(ini, "profile:a", "Key_None"));
	profile_data->SetCommand(KEYS::B, iniParser::getstring(ini, "profile:b", "Key_None"));
	profile_data->SetCommand(KEYS::X, iniParser::getstring(ini, "profile:x", "Key_None"));
	profile_data->SetCommand(KEYS::Y, iniParser::getstring(ini, "profile:y", "Key_None"));
	profile_data->SetCommand(KEYS::L, iniParser::getstring(ini, "profile:l", "Key_None"));
	profile_data->SetCommand(KEYS::R, iniParser::getstring(ini, "profile:r", "Key_None"));
	profile_data->SetCommand(KEYS::START, iniParser::getstring(ini, "profile:start", "Key_None"));
	profile_data->SetCommand(KEYS::SELECT, iniParser::getstring(ini, "profile:select", "Key_None"));
	profile_data->SetCommand(KEYS::LID, iniParser::getstring(ini, "profile:lid", "Key_None"));
	profile_data->SetCommand(KEYS::BLUE, iniParser::getstring(ini, "profile:blue", "Key_None"));
	profile_data->SetCommand(KEYS::YELLOW, iniParser::getstring(ini, "profile:yellow", "Key_None"));
	profile_data->SetCommand(KEYS::RED, iniParser::getstring(ini, "profile:red", "Key_None"));
	profile_data->SetCommand(KEYS::GREEN, iniParser::getstring(ini, "profile:green", "Key_None"));

	profile_data->SetCommand(KEYS::TOUCH_00, iniParser::getstring(ini, "profile:touch00", "Key_None"));
	profile_data->SetCommand(KEYS::TOUCH_01, iniParser::getstring(ini, "profile:touch01", "Key_None"));
	profile_data->SetCommand(KEYS::TOUCH_02, iniParser::getstring(ini, "profile:touch02", "Key_None"));
	profile_data->SetCommand(KEYS::TOUCH_03, iniParser::getstring(ini, "profile:touch03", "Key_None"));
	profile_data->SetCommand(KEYS::TOUCH_04, iniParser::getstring(ini, "profile:touch04", "Key_None"));
	profile_data->SetCommand(KEYS::TOUCH_05, iniParser::getstring(ini, "profile:touch05", "Key_None"));
	profile_data->SetCommand(KEYS::TOUCH_06, iniParser::getstring(ini, "profile:touch06", "Key_None"));
	profile_data->SetCommand(KEYS::TOUCH_07, iniParser::getstring(ini, "profile:touch07", "Key_None"));
	profile_data->SetCommand(KEYS::TOUCH_08, iniParser::getstring(ini, "profile:touch08", "Key_None"));
	profile_data->SetCommand(KEYS::TOUCH_09, iniParser::getstring(ini, "profile:touch09", "Key_None"));
	profile_data->SetCommand(KEYS::TOUCH_10, iniParser::getstring(ini, "profile:touch10", "Key_None"));
	profile_data->SetCommand(KEYS::TOUCH_11, iniParser::getstring(ini, "profile:touch11", "Key_None"));

	profile_data->m_touch_string[0] = iniParser::getstring(ini, "profile:touchstring00", "Command 0");
	profile_data->m_touch_string[1] = iniParser::getstring(ini, "profile:touchstring01", "Command 1");
	profile_data->m_touch_string[2] = iniParser::getstring(ini, "profile:touchstring02", "Command 2");
	profile_data->m_touch_string[3] = iniParser::getstring(ini, "profile:touchstring03", "Command 3");
	profile_data->m_touch_string[4] = iniParser::getstring(ini, "profile:touchstring04", "Command 4");
	profile_data->m_touch_string[5] = iniParser::getstring(ini, "profile:touchstring05", "Command 5");
	profile_data->m_touch_string[6] = iniParser::getstring(ini, "profile:touchstring06", "Command 6");
	profile_data->m_touch_string[7] = iniParser::getstring(ini, "profile:touchstring07", "Command 7");
	profile_data->m_touch_string[8] = iniParser::getstring(ini, "profile:touchstring08", "Command 8");
	profile_data->m_touch_string[9] = iniParser::getstring(ini, "profile:touchstring09", "Command 9");
	profile_data->m_touch_string[10] = iniParser::getstring(ini, "profile:touchstring10", "Command 10");
	profile_data->m_touch_string[11] = iniParser::getstring(ini, "profile:touchstring11", "Command 11");

	profile_data->m_touch_x[0] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch00X", "0"));
	profile_data->m_touch_x[1] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch01X", "0"));
	profile_data->m_touch_x[2] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch02X", "0"));
	profile_data->m_touch_x[3] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch03X", "0"));
	profile_data->m_touch_x[4] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch04X", "0"));
	profile_data->m_touch_x[5] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch05X", "0"));
	profile_data->m_touch_x[6] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch06X", "0"));
	profile_data->m_touch_x[7] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch07X", "0"));
	profile_data->m_touch_x[8] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch08X", "0"));
	profile_data->m_touch_x[9] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch09X", "0"));
	profile_data->m_touch_x[10] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch10X", "0"));
	profile_data->m_touch_x[11] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch11X", "0"));

	profile_data->m_touch_y[0] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch00Y", "0"));
	profile_data->m_touch_y[1] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch01Y", "0"));
	profile_data->m_touch_y[2] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch02Y", "0"));
	profile_data->m_touch_y[3] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch03Y", "0"));
	profile_data->m_touch_y[4] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch04Y", "0"));
	profile_data->m_touch_y[5] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch05Y", "0"));
	profile_data->m_touch_y[6] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch06Y", "0"));
	profile_data->m_touch_y[7] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch07Y", "0"));
	profile_data->m_touch_y[8] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch08Y", "0"));
	profile_data->m_touch_y[9] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch09Y", "0"));
	profile_data->m_touch_y[10] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch10Y", "0"));
	profile_data->m_touch_y[11] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch11Y", "0"));

	profile_data->m_touch_w[0] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch00W", "0"));
	profile_data->m_touch_w[1] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch01W", "0"));
	profile_data->m_touch_w[2] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch02W", "0"));
	profile_data->m_touch_w[3] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch03W", "0"));
	profile_data->m_touch_w[4] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch04W", "0"));
	profile_data->m_touch_w[5] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch05W", "0"));
	profile_data->m_touch_w[6] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch06W", "0"));
	profile_data->m_touch_w[7] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch07W", "0"));
	profile_data->m_touch_w[8] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch08W", "0"));
	profile_data->m_touch_w[9] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch09W", "0"));
	profile_data->m_touch_w[10] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch10W", "0"));
	profile_data->m_touch_w[11] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch11W", "0"));

	profile_data->m_touch_h[0] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch00H", "0"));
	profile_data->m_touch_h[1] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch01H", "0"));
	profile_data->m_touch_h[2] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch02H", "0"));
	profile_data->m_touch_h[3] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch03H", "0"));
	profile_data->m_touch_h[4] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch04H", "0"));
	profile_data->m_touch_h[5] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch05H", "0"));
	profile_data->m_touch_h[6] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch06H", "0"));
	profile_data->m_touch_h[7] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch07H", "0"));
	profile_data->m_touch_h[8] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch08H", "0"));
	profile_data->m_touch_h[9] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch09H", "0"));
	profile_data->m_touch_h[10] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch10H", "0"));
	profile_data->m_touch_h[11] = D2K::string_to_uint8_t(iniParser::getstring(ini, "profile:touch11H", "0"));
	iniParser::freedict(ini);

	for(int i = 0; i < UDP::SETTINGS_PACKET_MAX_BUTTONS; i++)
	{
        if(profile_data->m_touch_w[i] == 0 &&  profile_data->m_touch_h[i] == 0)     //if the button won't even show
            profile_data->m_touch_string[i].clear();
        else                                //otherwise shrink down to our max chars
            profile_data->m_touch_string[i].resize(10);
	}

	return 0;
}

int Save()
{
	FILE* file = fopen(g_ini_filename, "w");

	if(file == nullptr)
	{
		int err = errno;
		std::clog << "Error (fopen): #" << err << "\nFailed to open file: " << g_ini_filename << "\n";

		return err;
	}

	fprintf(file, "[Settings]\n");
	fprintf(file, "Port=%u\n", GetPort());

	fclose(file);
	Load();

	return 0;
}

int SaveProfile(ProfileData* Profile, uint8_t profileNumber)
{
	std::ostringstream ssfilename;
	ssfilename << "ds2key.p" << (int)profileNumber << ".ini";

	FILE* file = fopen(ssfilename.str().c_str(), "w");
	if(file == nullptr)
	{
		int err = errno;
		std::clog << "Error (fopen): #" << err << "\nFailed to save file: " << g_ini_filename << "\n";

		return err;
	}

	fprintf(file, "[Profile]\n");
	fprintf(file, "Mouse=%s\n", Profile->m_mouse.c_str());
	fprintf(file, "Joy=%s\n", Profile->m_joy.c_str());
	fprintf(file, "Up=%s\n", Profile->GetButtonString(KEYS::UP).c_str());
	fprintf(file, "Down=%s\n", Profile->GetButtonString(KEYS::DOWN).c_str());
	fprintf(file, "Left=%s\n", Profile->GetButtonString(KEYS::LEFT).c_str());
	fprintf(file, "Right=%s\n", Profile->GetButtonString(KEYS::RIGHT).c_str());
	fprintf(file, "A=%s\n", Profile->GetButtonString(KEYS::A).c_str());
	fprintf(file, "B=%s\n", Profile->GetButtonString(KEYS::B).c_str());
	fprintf(file, "X=%s\n", Profile->GetButtonString(KEYS::X).c_str());
	fprintf(file, "Y=%s\n", Profile->GetButtonString(KEYS::Y).c_str());
	fprintf(file, "L=%s\n", Profile->GetButtonString(KEYS::L).c_str());
	fprintf(file, "R=%s\n", Profile->GetButtonString(KEYS::R).c_str());
	fprintf(file, "Start=%s\n", Profile->GetButtonString(KEYS::START).c_str());
	fprintf(file, "Select=%s\n", Profile->GetButtonString(KEYS::SELECT).c_str());
	fprintf(file, "Lid=%s\n", Profile->GetButtonString(KEYS::LID).c_str());
	fprintf(file, "Blue=%s\n", Profile->GetButtonString(KEYS::BLUE).c_str());
	fprintf(file, "Yellow=%s\n", Profile->GetButtonString(KEYS::YELLOW).c_str());
	fprintf(file, "Red=%s\n", Profile->GetButtonString(KEYS::RED).c_str());
	fprintf(file, "Green=%s\n", Profile->GetButtonString(KEYS::GREEN).c_str());

	for(int i = 0; i < UDP::SETTINGS_PACKET_MAX_BUTTONS; i++)
	{
		fprintf(file, "Touch%.2d=%s\n", i, Profile->GetButtonString(KEYS::TOUCH_00 + i).c_str());
		fprintf(file, "m_touch_string%.2d=%s\n", i, Profile->m_touch_string[i].c_str());
		fprintf(file, "Touch%.2dX=%i\n", i, Profile->m_touch_x[i]);
		fprintf(file, "Touch%.2dY=%i\n", i, Profile->m_touch_y[i]);
		fprintf(file, "Touch%.2dW=%i\n", i, Profile->m_touch_w[i]);
		fprintf(file, "Touch%.2dH=%i\n", i, Profile->m_touch_h[i]);
	}

	fclose(file);

	return 0;
}

//private
//Currently assigned port
uint16_t g_port = DEFAULT_PORT;

//public
uint16_t GetPort()
{
	return Config::g_port;
}

void SetConfigPort(uint16_t port)
{
	if(port == 0)
		Config::g_port = DEFAULT_PORT;
	else
		Config::g_port = port;
}

}}//namespace D2K::Config
