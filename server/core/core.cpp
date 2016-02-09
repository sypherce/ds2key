// DS2Key Core functions

#include <iostream>//std::cout, std::clog
#include <algorithm>	//std::max,std::min
#include <sstream>//ostringstream
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#ifdef WIN32GUI
#include "gui/gui.h"
#endif//WIN32GUI
#elif defined __linux__
#include <cstring>
#include <chrono>
#include <thread>
#define Sleep(a) std::this_thread::sleep_for(std::chrono::milliseconds(a))
#endif//_WIN32

#include "common/udp.h"
#include "key.h"
#include "input.h"
#include "config.h"
#include "client.h"
#include "core.h"

namespace D2K{

void ExecuteCommand(std::string Command)
{
	if(Command != "")
	{
		if(Command.substr(0, D2K_COMMAND_LENGTH) == D2K_COMMAND)
			Command = Command.substr(D2K_COMMAND_LENGTH);
#ifdef _WIN32
		WinExec(Command.c_str(), SW_SHOWNORMAL);
#elif defined __linux__
		std::ostringstream stringStream;
		stringStream << Command << " &";
		system(stringStream.str().c_str());
#endif//_WIN32
	}
}
void ProcessPacket(D2K::Client* Client)
{
	//static values
	static uint16_t last_x = 0, last_y = 0;
	static bool last_screen_touched = false;

	//turbo status, true == press, false == release
	static bool turbo = false;

	ProfileData* Profile = Client->GetProfileDataPointer();

	//buttons
	for(int i = KEYS::UP; i <= KEYS::LID; i++)
	{
		uint16_t DSButton = BitToButton(i);
		uint8_t Joystick = Profile->GetValue8(KEYS::JOY);
		uint16_t PCButton = Profile->GetVirtualKey(i);

		if(PCButton)
		{
			if(Client->Down(DSButton))			//pressed
				Input::Press(PCButton, Joystick);
			else if(Client->Up(DSButton))			//released, even in turbo mode
				Input::Release(PCButton, Joystick);
			else if(turbo && Client->Turbo(DSButton))	//turbo set, button enabled for turbo and pressed
				Input::Press(PCButton, Joystick);
			else if(Client->Turbo(DSButton))		//turbo UNSET, button enabled for turbo and pressed
				Input::Release(PCButton, Joystick);	//we release because turbo is UNSET
		}
		else if(Client->Down(DSButton))				//pressed
		{
			std::string Command = Profile->GetCommand(i);
			ExecuteCommand(Command);
		}
	}
	//gh buttons
	for(int i = KEYS::BLUE; i <= KEYS::GREEN; i++)
	{
		uint8_t DSButton = (uint8_t)BitToButton(i);		//all the guitar hero buttons are 8bit values, type casting BitToButton should be fine
		uint8_t Joystick = Profile->GetValue8(KEYS::JOY);
		uint16_t PCButton = Profile->GetVirtualKey(i);

		if(PCButton)
		{
			if(Client->GHDown(DSButton))			//pressed
				Input::Press(PCButton, Joystick);
			else if(Client->GHUp(DSButton))			//released, even in turbo mode
				Input::Release(PCButton, Joystick);
			else if(turbo && Client->GHTurbo(DSButton))	//turbo set, button enabled for turbo and pressed
				Input::Press(PCButton, Joystick);
			else if(Client->GHTurbo(DSButton))		//turbo UNSET, button enabled for turbo and pressed
				Input::Release(PCButton, Joystick);	//we release because turbo is UNSET
		}
		else
		{
			std::string Command = Profile->GetCommand(i);
			ExecuteCommand(Command);
		}
	}

	turbo = !turbo;//toggle turbo status

	//touch screen
	uint8_t x = Client->GetX();
	uint8_t y = Client->GetY();
	bool screen_touched = Client->Held(DS2KEY_TOUCH);
	std::string moveType = Profile->m_mouse;

	if(screen_touched)					//if touched
	{
		static const int s_ignore = 25;			//how much movement may be ignored, this helps jitter
		static const int s_sensitivity = 3;		//relative movement sensitivity scale
		static const int s_deadzone = 5;		//deadzone border, this helps the whole screen be accesible with absolute movement

		if(last_screen_touched == false)		//if newly pressed
		{
			//Input::Press(KEY_LBUTTON);
			last_x = x;
			last_y = y;
			last_screen_touched = true;
		}
		if(!((x - last_x < -s_ignore)
		|| (x - last_x > s_ignore)
		|| (y - last_y < -s_ignore)
		|| (y - last_y > s_ignore)))									//check that we've moved
		{
			if(moveType == "Relative")										//relative movement
			{
				Input::Move((x - last_x) * s_sensitivity, (y - last_y) * s_sensitivity);
			}
			else if(moveType == "Absolute")									//absolute movement
			{
				//the next 8 lines apply our border to X and Y values
				int temporary_x = x;
				int temporary_y = y;
				if(temporary_x < s_deadzone)
					temporary_x = s_deadzone;
				if(temporary_x > 255 - s_deadzone)
					temporary_x = 255 - s_deadzone;
				if(temporary_y < s_deadzone)
					temporary_y = s_deadzone;
				if(temporary_y > 191 - s_deadzone)
					temporary_y = 191 - s_deadzone;
				temporary_x -= s_deadzone;
				temporary_y -= s_deadzone;
				Input::MoveAbsolute(temporary_x * (65535  / (255 - s_deadzone - s_deadzone)), temporary_y * (65535  / (191 - s_deadzone - s_deadzone)));
			}
			last_x = x;
			last_y = y;
		}
	}
	else if(last_screen_touched == true)		//if newly released
	{
		last_x = last_y = 0;
		last_screen_touched = false;
	}
}

bool g_running = false;

int Setup(int argc, char* argv[])
{
	bool non_blocking = true;//non-blocking == true
	Config::Load();
	UDP::Init();
	Input::Init();
	g_running = true;

	for(int arg = 1; arg < argc; arg++)						//command arguments
	{
		if(strcmp(argv[arg], "--block") == 0)				//setup blocking mode
		{
			non_blocking = false;
		}
		else if(strcmp(argv[arg], "--console") == 0)		//setup console mode
		{
#ifdef WINXP
			BOOL f = AllocConsole();
			freopen("CONIN$", "r", stdin);
			freopen("CONOUT$", "w", stdout);
			freopen("CONOUT$", "w", stderr);
#endif
		}
		else if(strncmp(argv[arg], "--port=", 7) == 0)		//assign a specific port
		{
			Config::SetConfigPort(atoi(&argv[arg][7]));
			std::cout << "\nPort: " << Config::GetPort() << "\n";
		}
	}

	return UDP::Connect(non_blocking, Config::GetPort());			//startup networking
}

void Loop()
{
	UDP::DS2KeyPacket Packet;
	if(g_running 
	&& UDP::IsConnected()
	&& UDP::Recv(&Packet, sizeof(UDP::DS2KeyPacket)) == 0)									//if we receive something without error
	{
		switch(Packet.type)
		{
		case UDP::PACKET::LOOKUP:
		{
			UDP::Send(&Packet, sizeof(UDP::DS2KeyPacket));
			break;
		}
		case UDP::PACKET::COMMAND_SETTINGS:
		{
			UDP::DS2KeySettingsPacket settings = UDP::DS2KeySettingsPacket{ };
			if(g_client_array[Packet.profile] != nullptr)							//if profile is active
			{
				ProfileData* profile = g_client_array[Packet.profile]->GetProfileDataPointer();//make a pointer to the profile
				settings.type = UDP::PACKET::COMMAND_SETTINGS;
				for(int i = 0; i < UDP::SETTINGS_PACKET_MAX_BUTTONS; i++)
				{
					settings.x_1[i] = profile->m_touch_x[i];
					settings.x_2[i] = profile->m_touch_w[i];
					settings.y_1[i] = profile->m_touch_y[i];
					settings.y_2[i] = profile->m_touch_h[i];
					strncpy(settings.text[i], profile->m_touch_string[i].c_str(), UDP::SETTINGS_PACKET_MAX_TEXT);
					settings.text[i][UDP::SETTINGS_PACKET_MAX_TEXT] = 0;
				}
				UDP::SendCommandSettings(settings);											//send settings packet
			}
			break;
		}
		case UDP::PACKET::NORMAL:
		{
			D2K::Client* pClient = Config::GetClient(Packet.profile);
			pClient->SetPacket(Packet);													//insert packet data
			pClient->Scan();															//update
			ProcessPacket(pClient);
			break;
		}
		case UDP::PACKET::COMMAND:
		{
			int TouchButton = KEYS::TOUCH_00 + Packet.keys;
			D2K::Client* pClient = Config::GetClient(Packet.profile);
			ProfileData* Data = pClient->GetProfileDataPointer();						//pointer to profile data
			uint8_t Joystick = Data->GetValue8(KEYS::JOY);
			uint16_t PCButton = Data->GetVirtualKey(TouchButton);

			if(PCButton)
			{
				Input::Press(PCButton, Joystick);
				Sleep(100);
				Input::Release(PCButton, Joystick);
			}
			else
			{
				std::string Command = Data->GetCommand(TouchButton);
				ExecuteCommand(Command);
			}
			break;
		}
		}
	}

	Sleep(1);																		//sleep to avoid 99% cpu when not using -O2
	#ifdef WIN32GUI
	D2K::GUI::GetMessages();														//Take care of GUI stuff
	#endif//WIN32GUI
}
void Destroy()
{
	g_running = false;
	for(int i = 0; i < 255; i++)
	{
		if(g_client_array[i] != nullptr)
		{
			delete(g_client_array[i]);
			g_client_array[i] = nullptr;
		}
	}
	UDP::DeInit();
	Input::DeInit();
	Config::Save();
}

}//namespace D2K
