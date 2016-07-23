// DS2Key Core functions

#include <iostream>  // std::cout, std::clog
#include <algorithm> // std::max, std::min
#include <sstream>   // ostringstream
#include <chrono>
#include <thread>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
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
#include "vjoy.h"

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


// Changes master volume to (volume)
//  (volume) ranges from 0.0f, 1.0f. 
void ChangeVolume(float volume)
{
#ifdef _WIN32
	HRESULT hresult{ };

	//Initialize the COM library 
	CoInitialize(nullptr);

	//Creates a single uninitialized object
	IMMDeviceEnumerator *device_enumerator{ }; 
	hresult = CoCreateInstance(__uuidof(MMDeviceEnumerator),
	              NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator),
	              (LPVOID *)&device_enumerator);
	if(hresult != S_OK)
	{
		std::clog << "Error (CoCreateInstance): #" << hresult << "\n";
		//Uninitialize the COM library 
		CoUninitialize();

		return;
	}

	//Retrieve the default audio endpoint using (device_enumerator)
	IMMDevice *default_audio_endpoint{ };
	hresult = device_enumerator->GetDefaultAudioEndpoint(eRender, eConsole,
	              &default_audio_endpoint);
	//we're done with device_enumerator
	device_enumerator->Release();
	if(hresult != S_OK)
	{
		std::clog << "Error (GetDefaultAudioEndpoint): #" << hresult << "\n";
		//Uninitialize the COM library 
		CoUninitialize();

		return;
	}

	//Retrieve the volume controls using (default_audio_endpoint)
	IAudioEndpointVolume *endpoint_volume{ };
	hresult = default_audio_endpoint->Activate(__uuidof(IAudioEndpointVolume),
	              CLSCTX_INPROC_SERVER, nullptr, (LPVOID*)&endpoint_volume);
	//we're done with default_audio_endpoint
	default_audio_endpoint->Release();
	if(hresult != S_OK)
	{
		std::clog << "Error (IAudioEndpointVolume->Activate): #" << hresult << "\n";
		//Uninitialize the COM library 
		CoUninitialize();

		return;
	}

	//Change the volume
	hresult = endpoint_volume->SetMasterVolumeLevelScalar(volume, nullptr);	
	//we're done with endpoint_volume
	endpoint_volume->Release();
	if(hresult != S_OK)
	{
		std::clog << "Error (SetMasterVolumeLevelScalar): #" << hresult << "\n";
		//Uninitialize the COM library 
		CoUninitialize();

		return;
	}

	//Uninitialize the COM library 
	CoUninitialize();
#endif
}

void ProcessPacket(D2K::Client* Client)
{
	//static values
	static uint16_t last_x = 0, last_y = 0;
	static bool last_screen_touched = false;

	ProfileData* Profile = Client->GetProfileDataPointer();

	uint8_t Joystick = Profile->GetValue8(KEYS::JOY);
	//buttons
	for(int enum_key = _START_OF_BUTTONS_ + 1; enum_key < KEYS::_END_OF_BUTTONS_; enum_key++)
	{
		uint32_t DSButton   = EnumKeyToNDSKeypadBit(enum_key);
		uint16_t PCButton   = Profile->GetVirtualKey(enum_key);
		std::string DSAxis  = Profile->GetAxis(enum_key);
		std::string Command = Profile->GetCommand(enum_key);

		if(PCButton)
		{
			// button enabled for turbo and pressed
			if(Client->Turbo(DSButton))
			{
				Input::Tap(PCButton, Joystick);
				//TODO:log std::clog << "tap:" << PCButton << "\n";
			}
			// Pressed
			else if(Client->Down(DSButton))
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
		}
		else if(DSAxis != "")
		{
			//crop off the D2K_AXIS char
			DSAxis = DSAxis.substr(D2K_AXIS_LENGTH);

			int16_t input_value = 1;
			if(DSAxis.at(0) == '-')
				input_value = -1;
			//crop off the '-' or '+' char
			DSAxis = DSAxis.substr(1);

			if(DSButton == DS2KEY_CPAD_LEFT || DSButton == DS2KEY_CPAD_RIGHT)
				input_value *= Client->GetCircleX();
			else if(DSButton == DS2KEY_CPAD_UP || DSButton == DS2KEY_CPAD_DOWN)
				input_value *= Client->GetCircleY();
			else if(DSButton == DS2KEY_CSTICK_LEFT || DSButton == DS2KEY_CSTICK_RIGHT)
				input_value *= Client->GetCstickX();
			else if(DSButton == DS2KEY_CSTICK_UP || DSButton == DS2KEY_CSTICK_DOWN)
				input_value *= Client->GetCstickY();
			else if(DSButton == DS2KEY_SLIDER_VOLUME)
				input_value = (Client->GetSliderVolume() * 1.2) - 60;
			else if(DSButton == DS2KEY_SLIDER_3D)
				input_value = (Client->GetSlider3D() * 1.2) - 60;
			
			uint8_t output_axis = 0;
			if(DSAxis == "X")
				output_axis = HID_USAGE_X;
			else if(DSAxis == "Y")
				output_axis = HID_USAGE_Y;
			else if(DSAxis == "Z")
				output_axis = HID_USAGE_Z;
			else if(DSAxis == "RX")
				output_axis = HID_USAGE_RX;
			else if(DSAxis == "RZ")
				output_axis = HID_USAGE_RZ;
			else if(DSAxis == "SL0")
				output_axis = HID_USAGE_SL0;
			else if(DSAxis == "SL1")
				output_axis = HID_USAGE_SL1;
			else if(DSAxis == "WHL")
				output_axis = HID_USAGE_WHL;
			else if(DSAxis == "POV")
				output_axis = HID_USAGE_POV;

			if(DSAxis == "VOL")
			{
				// this variable stops us from changing the volume every second
				// the user can now also manually change the volume
				static int16_t last_volume_input{ };
				if(input_value != last_volume_input)
				{
					last_volume_input = input_value;

					ChangeVolume((input_value + 60) / 120.0f);
				}
			}
			else if(output_axis != 0)
			{
				int16_t axis_max_value = 60;
				D2K::Input::Joystick::SetAxisSignedMax(Joystick, output_axis, input_value, axis_max_value);
			}
		}
		else if(Command != ""
		&& Client->Down(DSButton))
		{
			ExecuteCommand(Command);
		}
	}
	D2K::Input::Joystick::Update(Joystick);

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

#ifdef false
	//gyro/accel test stuff
	{
		uint8_t device = 1;
		int16_t scale = 64;
		int16_t value = Client->GetGyroX();
		int16_t diff_value = (int16_t)1.3*scale;
		int16_t axis_max_value = 60;
		static int16_t adjustment_value_x = 0;
		static int16_t adjustment_value_y = 0;
		static int16_t adjustment_value_z = 0;
		if(value > -diff_value && value < diff_value)
		{
			adjustment_value_x = -Client->GetAccelX();
			adjustment_value_z = Client->GetAccelZ();
		}
		int16_t accelX = Client->GetAccelX();
		int16_t accelY = Client->GetAccelY();
		int16_t accelZ = Client->GetAccelZ();
		int16_t gyroX = Client->GetGyroX();
		int16_t gyroY = Client->GetGyroY();
		int16_t gyroZ = Client->GetGyroZ();
		/*D2K::Input::joystick_id::SetAxisSignedMax(device, HID_USAGE_X,  client->GetAccelX() + adjustment_value_x, axis_max_value);
		D2K::Input::joystick_id::SetAxisSignedMax(device, HID_USAGE_Y, -client->GetAccelY() + adjustment_value_y, axis_max_value);
		D2K::Input::joystick_id::SetAxisSignedMax(device, HID_USAGE_Z, -client->GetAccelZ() + adjustment_value_z, axis_max_value);
		D2K::Input::joystick_id::SetAxisSignedMax(device, HID_USAGE_RX,  client->GetGyroX()  + adjustment_value_x, axis_max_value);
		D2K::Input::joystick_id::SetAxisSignedMax(device, HID_USAGE_RY, -client->GetGyroY()  + adjustment_value_y, axis_max_value);
		D2K::Input::joystick_id::SetAxisSignedMax(device, HID_USAGE_RZ, -client->GetGyroZ()  + adjustment_value_z, axis_max_value);

		D2K::Input::joystick_id::Update(device);*/
	}
#endif

	uint16_t keyboard_press = Client->GetKeyboardPress();
	if(keyboard_press != NULL_VALUE)
	{
		Input::Press(keyboard_press, NULL_VALUE);
		Input::Release(keyboard_press, NULL_VALUE);
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

	uint8_t Joystick = Profile->GetValue8(KEYS::JOY);
	D2K::Input::Joystick::DeInit(Joystick);
}

void CheckForDeadClients()
{
	static std::chrono::high_resolution_clock::time_point time_previous = std::chrono::high_resolution_clock::now();
	       std::chrono::high_resolution_clock::time_point time_current  = std::chrono::high_resolution_clock::now();
	long long time_difference =std::chrono::duration_cast<std::chrono::milliseconds>(time_current - time_previous).count();
	if(time_difference >= 1000)
	{
//TODO::std::clog << time_difference << "\n";
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
				UDP::SetRemoteIP(g_client_array[i]->GetIP());
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
	UDP::DS2KeyCommandSettingsPacket Packet{ };

	//if we  are running, connected, and receive something without error
	if(g_running
	&& UDP::IsConnected()
	&& UDP::Recv(&Packet, sizeof(UDP::DS2KeyCommandSettingsPacket)) == 0)
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
			UDP::DS2KeyPacket* normal_packet = (UDP::DS2KeyPacket*)&Packet;
			UDP::Send(&normal_packet, sizeof(UDP::DS2KeyPacket));
			break;
		}
		case UDP::PACKET::COMMAND_SETTINGS:
		{
			// If profile is active
			if(g_client_array[Packet.profile] != nullptr)
			{
				UDP::DS2KeyCommandSettingsPacket settings = UDP::DS2KeyCommandSettingsPacket{ };

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
			UDP::DS2KeyPacket* normal_packet = (UDP::DS2KeyPacket*)&Packet;

			D2K::Client* pClient = Config::GetClient(normal_packet->profile);
			//insert packet data
			pClient->SetPacket(*normal_packet);
			//update
			pClient->Scan();
			ProcessPacket(pClient);
			break;
		}
		case UDP::PACKET::NORMAL_SETTING:
		{
			UDP::DS2KeyNormalSettingsPacket* normal_setting_packet = (UDP::DS2KeyNormalSettingsPacket*)&Packet;
			// If profile is active
			if(g_client_array[normal_setting_packet->profile] != nullptr)
			{
				ProfileData* profile_data = g_client_array[normal_setting_packet->profile]->GetProfileDataPointer();
				Config::SetProfileSetting(profile_data, normal_setting_packet->profile, normal_setting_packet->setting, normal_setting_packet->value);
			}
			break;
		}
		case UDP::PACKET::COMMAND:
		{
			UDP::DS2KeyPacket* normal_packet = (UDP::DS2KeyPacket*)&Packet;

			D2K::Client* client_pointer = Config::GetClient(normal_packet->profile);
			ProfileData* data_pointer = client_pointer->GetProfileDataPointer();
			uint8_t virtual_joystick_id = data_pointer->GetValue8(KEYS::JOY);
			int touch_screen_button = KEYS::TOUCH_00 + normal_packet->keys;
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
