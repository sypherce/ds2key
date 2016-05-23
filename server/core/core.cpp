// DS2Key Core functions

#include <iostream>  // std::cout, std::clog
#include <algorithm>  // std::max, std::min
#include <sstream>  // ostringstream
#include <chrono>
#include <thread>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#ifdef WIN32GUI
#include "gui/gui.h"
#endif
#elif defined(__linux__)
#include <cstring>
#endif

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
#elif defined(__linux__)
		std::ostringstream stringStream;
		stringStream << Command << " &";
		system(stringStream.str().c_str());
#endif
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

	uint8_t Joystick = Profile->GetValue8(KEYS::JOY);
	//buttons
	for(int enum_key = _START_OF_BUTTONS_ + 1; enum_key < KEYS::_END_OF_BUTTONS_; enum_key++)
	{
		uint32_t DSButton = EnumKeyToNDSKeypadBit(enum_key);
		uint16_t PCButton = Profile->GetVirtualKey(enum_key);

		if(PCButton)
		{
			// Pressed
			if(Client->Down(DSButton))
			{
				Input::Press(PCButton, Joystick);
				//TODO:log std::clog << "press:" << PCButton << "\n";
			}
			// Released, even in turbo mode
			else if(Client->Up(DSButton))
			{
				Input::Release(PCButton, Joystick);
				//TODO:log std::clog << "release:" << PCButton << "\n";
			}
			// Turbo set, button enabled for turbo and pressed
			else if(turbo && Client->Turbo(DSButton))
				Input::Press(PCButton, Joystick);
			//turbo UNSET, button enabled for turbo and pressed
			else if(Client->Turbo(DSButton))
				//we release because turbo is UNSET
				Input::Release(PCButton, Joystick);
		}
		//pressed
		else if(Client->Down(DSButton))
		{
			std::string Command = Profile->GetCommand(enum_key);
			ExecuteCommand(Command);
		}
	}

	// Toggle turbo status
	turbo = !turbo;

	// Touch screen
	uint16_t x = Client->GetX();
	uint16_t y = Client->GetY();
	bool screen_touched = Client->Held(DS2KEY_TOUCH);
	std::string moveType = Profile->m_mouse;

	//if touched
	if(screen_touched)
	{
		// How much movement to ignore, this helps jitter
		static const int s_ignore = 25;
		// Relative movement sensitivity scale
		static const int s_sensitivity = 3;
		// Deadzone Border: This helps the whole screen be accesible with absolute movement
		static const int s_deadzone = 5;

		// If newly pressed
		if(last_screen_touched == false)
		{
			//Input::Press(KEY_LBUTTON);
			last_x = x;
			last_y = y;
			last_screen_touched = true;
		}

		//check that we've moved
		if(!((x - last_x < -s_ignore)
		|| (x - last_x > s_ignore)
		|| (y - last_y < -s_ignore)
		|| (y - last_y > s_ignore)))
		{
			//relative movement
			if(moveType == "Relative")
			{
				Input::Move((x - last_x) * s_sensitivity, (y - last_y) * s_sensitivity);
			}
			//absolute movement
			else if(moveType == "Absolute")
			{
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
				Input::MoveAbsolute(
					temporary_x * (65535  / (255 - s_deadzone - s_deadzone)),
					temporary_y * (65535  / (191 - s_deadzone - s_deadzone)));
			}
			last_x = x;
			last_y = y;
		}
	}
	// If newly released
	else if(last_screen_touched == true)
	{
		last_x = last_y = 0;
		last_screen_touched = false;
	}
}

//TODO this needs tested with multiple devices connected
void ReleaseDeadClient(D2K::Client* Client)
{
	ProfileData* Profile = Client->GetProfileDataPointer();

	//buttons
	for(int enum_key = _START_OF_BUTTONS_ + 1; enum_key < KEYS::_END_OF_BUTTONS_; enum_key++)
	{
		uint32_t DSButton = EnumKeyToNDSKeypadBit(enum_key);
		uint8_t Joystick = Profile->GetValue8(KEYS::JOY);
		uint16_t PCButton = Profile->GetVirtualKey(enum_key);

		if(PCButton)
		{
			// Held
			if(Client->Held(DSButton))
			{
				Input::Release(PCButton, Joystick);
				//TODO:log std::clog << "press:" << PCButton << "\n";
			}
		}
	}
}

void CheckForDeadClients()
{
	static std::chrono::steady_clock::time_point time_previous = std::chrono::high_resolution_clock::now();
	       std::chrono::steady_clock::time_point time_current  = std::chrono::high_resolution_clock::now();
	long long time_difference =std::chrono::duration_cast<std::chrono::milliseconds>(time_current - time_previous).count();
	if(time_difference >= 1000)
	{
		std::clog << time_difference << "\n";
		time_previous = time_current;

		UDP::DS2KeyPacket Packet{ };
		Packet.type = UDP::PACKET::ALIVE;

		for(int i = 0; i < D2K::CLIENT_MAX; i++)
		{
			if(g_client_array[i] == nullptr)
			{
				// don't do anything, the array is empty
			}
			else if(g_client_array[i]->IsAlive() == CLIENT_STATUS::CHECKING)
			{
				ReleaseDeadClient(g_client_array[i]);
				delete(g_client_array[i]);
				g_client_array[i] = nullptr;
				std::cout << "\nClient #:" << i << " removed from inactivity.\n";
			}
			else
			{
				g_client_array[i]->SetAlive(CLIENT_STATUS::CHECKING);
				UDP::Send(&Packet, sizeof(UDP::DS2KeyPacket));
			}
		}
	}
}

bool g_running = false;

// Setup everything, handle command arguments, and return 0 if UDP connected
// --block sets UDP blocking mode, otherwise we start in non-blocking mode
// --console enables a console window for the GUI interface
// --port assigns a custom port
//
// return (0) if connected, else (errno)
int Setup(int argc, char* argv[])
{
	bool non_blocking = true;
	Config::Load();
	UDP::Init();
	Input::Init();
	g_running = true;

	for(int arg = 1; arg < argc; arg++)
	{
		if(strcmp(argv[arg], "--block") == 0)
		{
			non_blocking = false;
		}
		else if(strcmp(argv[arg], "--console") == 0)
		{
#ifdef WINXP
			BOOL f = AllocConsole();
			freopen("CONIN$", "r", stdin);
			freopen("CONOUT$", "w", stdout);
			freopen("CONOUT$", "w", stderr);
#endif
		}
		else if(strncmp(argv[arg], "--port=", 7) == 0)
		{
			Config::SetConfigPort(atoi(&argv[arg][7]));
			std::cout << "\nPort: " << Config::GetPort() << "\n";
		}
	}

	return UDP::Connect(non_blocking, Config::GetPort());
}

void Loop()
{
	UDP::DS2KeyPacket Packet{ };

	//if we  are running, connected, and receive something without error
	if(g_running
	&& UDP::IsConnected()
	&& UDP::Recv(&Packet, sizeof(UDP::DS2KeyPacket)) == 0)
	{
		// If profile is active
		if(g_client_array[Packet.profile] != nullptr)
		{
			//Set Alive Status
			g_client_array[Packet.profile]->SetAlive(CLIENT_STATUS::ALIVE);
		}

		switch(Packet.type)
		{
		case UDP::PACKET::LOOKUP:
		case UDP::PACKET::ALIVE:
		{
			UDP::Send(&Packet, sizeof(UDP::DS2KeyPacket));
			break;
		}
		case UDP::PACKET::COMMAND_SETTINGS:
		{
			// If profile is active
			if(g_client_array[Packet.profile] != nullptr)
			{
				UDP::DS2KeySettingsPacket settings = UDP::DS2KeySettingsPacket{ };
				settings.type = UDP::PACKET::COMMAND_SETTINGS;

				// Make a pointer to the profile
				ProfileData* profile_data = g_client_array[Packet.profile]->GetProfileDataPointer();

				for(int i = 0; i < UDP::SETTINGS_PACKET_MAX_BUTTONS; i++)
				{
					settings.x_1[i] = profile_data->m_touch_x[i];
					settings.x_2[i] = profile_data->m_touch_w[i];
					settings.y_1[i] = profile_data->m_touch_y[i];
					settings.y_2[i] = profile_data->m_touch_h[i];
					strncpy(settings.text[i],
						profile_data->m_touch_string[i].c_str(),
						UDP::SETTINGS_PACKET_MAX_TEXT);
					// TODO: This shouldn't be needed?
					settings.text[i][UDP::SETTINGS_PACKET_MAX_TEXT] = 0;
				}

				// Send settings packet
				UDP::SendCommandSettings(settings);
			}
			break;
		}
		case UDP::PACKET::NORMAL:
		{
			D2K::Client* pClient = Config::GetClient(Packet.profile);
			//insert packet data
			pClient->SetPacket(Packet);
			//update
			pClient->Scan();
			ProcessPacket(pClient);
			break;
		}
		case UDP::PACKET::COMMAND:
		{
			D2K::Client* client_pointer = Config::GetClient(Packet.profile);
			ProfileData* data_pointer = client_pointer->GetProfileDataPointer();
			uint8_t virtual_joystick_id = data_pointer->GetValue8(KEYS::JOY);
			int touch_screen_button = KEYS::TOUCH_00 + Packet.keys;
			uint16_t virtual_key = data_pointer->GetVirtualKey(touch_screen_button);

			if(virtual_key)
			{
				Input::Press(virtual_key, virtual_joystick_id);
				// Sleep so the Press is recognised
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				Input::Release(virtual_key, virtual_joystick_id);
			}
			else
			{
				std::string Command = data_pointer->GetCommand(touch_screen_button);
				ExecuteCommand(Command);
			}
			break;
		}
		}
	}

	CheckForDeadClients();

	// Sleep to avoid 99% cpu when not using -O2
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	#ifdef WIN32GUI
	//Take care of GUI stuff
	D2K::GUI::GetMessages();
	#endif
}
void Destroy()
{
	g_running = false;

	for(int i = 0; i < D2K::CLIENT_MAX; i++)
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
