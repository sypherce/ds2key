// DS2Key Core functions

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

// This looks wrong because it's a macro
#include "common/easylogging++Wrapper.h"
INITIALIZE_EASYLOGGINGPP

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
// (volume) ranges from 0.0f, 1.0f. 
void SetMasterVolume(float volume)
{
#ifdef _WIN32
	HRESULT hresult{};

	// Initialize the COM library 
	CoInitialize(nullptr);

	// Creates a single uninitialized object
	IMMDeviceEnumerator *device_enumerator{}; 
	hresult = CoCreateInstance(__uuidof(MMDeviceEnumerator),
	          NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator),
	          (LPVOID *)&device_enumerator);
	if(hresult != S_OK)
	{
		LOG(ERROR) << "Error (CoCreateInstance): #" << hresult;
		// Uninitialize the COM library 
		CoUninitialize();

		return;
	}

	// Retrieve the default audio endpoint using (device_enumerator)
	IMMDevice *default_audio_endpoint{};
	hresult = device_enumerator->GetDefaultAudioEndpoint(eRender, eConsole,
	              &default_audio_endpoint);
	// we're done with device_enumerator
	device_enumerator->Release();
	if(hresult != S_OK)
	{
		LOG(ERROR) << "Error (GetDefaultAudioEndpoint): #" << hresult;
		// Uninitialize the COM library 
		CoUninitialize();

		return;
	}

	// Retrieve the volume controls using (default_audio_endpoint)
	IAudioEndpointVolume *endpoint_volume{};
	hresult = default_audio_endpoint->Activate(__uuidof(IAudioEndpointVolume),
	              CLSCTX_INPROC_SERVER, nullptr, (LPVOID*)&endpoint_volume);
	// we're done with default_audio_endpoint
	default_audio_endpoint->Release();
	if(hresult != S_OK)
	{
		LOG(ERROR) << "Error (IAudioEndpointVolume->Activate): #" << hresult;
		// Uninitialize the COM library 
		CoUninitialize();

		return;
	}

	// Change the volume
	hresult = endpoint_volume->SetMasterVolumeLevelScalar(volume, nullptr);
	// we're done with endpoint_volume
	endpoint_volume->Release();
	if(hresult != S_OK)
	{
		LOG(ERROR) << "Error (SetMasterVolumeLevelScalar): #" << hresult;
		// Uninitialize the COM library 
		CoUninitialize();

		return;
	}

	// Uninitialize the COM library 
	CoUninitialize();
#endif
}

// Returns true if (button) is analog
bool DSButtonIsAnalog(uint32_t button)
{
	switch(button)
	{
	case DS2KEY_CPAD_LEFT:
	case DS2KEY_CPAD_RIGHT:
	case DS2KEY_CPAD_UP:
	case DS2KEY_CPAD_DOWN:
	case DS2KEY_CSTICK_LEFT:
	case DS2KEY_CSTICK_RIGHT:
	case DS2KEY_CSTICK_UP:
	case DS2KEY_CSTICK_DOWN:
	case DS2KEY_SLIDER_VOLUME:
	case DS2KEY_SLIDER_3D:
		return true;
	}

	return false;
}

// Example: Converts Digital (JOY_AXIS_X_MINUS) into analog (&-X) if (DSButtonIsAnalog) returns true
std::string ConvertButtonToAxis(ProfileData* profile_data, int enum_key)
{
	uint32_t ds_button_bit = EnumKeyToNDSKeypadBit(enum_key);
	std::string ds_axis    = profile_data->GetAxis(enum_key);
	uint16_t pc_key        = profile_data->GetVirtualKey(enum_key);

	if((DSButtonIsAnalog(ds_button_bit))
	&&  (ds_axis == "")
	&& ((pc_key >= Key::JOY_AXIS1 && pc_key <= Key::JOY_AXIS5_MAX)
		|| (pc_key == Key::KEY_VOLUME_UP)
		|| (pc_key == Key::KEY_VOLUME_DOWN)))
	{
		switch(pc_key)
		{
		case Key::JOY_AXIS_X_MINUS:
			profile_data->SetValue(enum_key, "&-X");
			break;
		case Key::JOY_AXIS_X_PLUS:
			profile_data->SetValue(enum_key, "&+X");
			break;
		case Key::JOY_AXIS_Y_MINUS:
			profile_data->SetValue(enum_key, "&-Y");
			break;
		case Key::JOY_AXIS_Y_PLUS:
			profile_data->SetValue(enum_key, "&+Y");
			break;
		case Key::JOY_AXIS_Z_MINUS:
			profile_data->SetValue(enum_key, "&-Z");
			break;
		case Key::JOY_AXIS_Z_PLUS:
			profile_data->SetValue(enum_key, "&+Z");
			break;
		case Key::JOY_AXIS_RX_MINUS:
			profile_data->SetValue(enum_key, "&-RX");
			break;
		case Key::JOY_AXIS_RX_PLUS:
			profile_data->SetValue(enum_key, "&+RX");
			break;
		case Key::JOY_AXIS_RY_MINUS:
			profile_data->SetValue(enum_key, "&-RY");
			break;
		case Key::JOY_AXIS_RY_PLUS:
			profile_data->SetValue(enum_key, "&+RY");
			break;
		case Key::JOY_AXIS_RZ_MINUS:
			profile_data->SetValue(enum_key, "&-RZ");
			break;
		case Key::JOY_AXIS_RZ_PLUS:
			profile_data->SetValue(enum_key, "&+RZ");
			break;
		case Key::JOY_AXIS_SL0_MINUS:
			profile_data->SetValue(enum_key, "&-SL0");
			break;
		case Key::JOY_AXIS_SL0_PLUS:
			profile_data->SetValue(enum_key, "&+SL0");
			break;
		case Key::JOY_AXIS_SL1_MINUS:
			profile_data->SetValue(enum_key, "&-SL1");
			break;
		case Key::JOY_AXIS_SL1_PLUS:
			profile_data->SetValue(enum_key, "&+SL1");
			break;
		case Key::JOY_AXIS_WHL_MINUS:
			profile_data->SetValue(enum_key, "&-WHL");
			break;
		case Key::JOY_AXIS_WHL_PLUS:
			profile_data->SetValue(enum_key, "&+WHL");
			break;
		case Key::KEY_VOLUME_UP:
			profile_data->SetValue(enum_key, "&+VOL");
			break;
		case Key::KEY_VOLUME_DOWN:
			profile_data->SetValue(enum_key, "&-VOL");
			break;
		default:
			break;
		}
	}
	return profile_data->GetAxis(enum_key);
}

void ProcessButtons(D2K::Client* client)
{
	// static values
	static int16_t axis_max_value = 120;

	ProfileData* profile_data = client->GetProfileDataPointer();

	uint8_t joystick = profile_data->GetValue8(KEYS::JOY);
	// buttons
	for(int enum_key = _START_OF_BUTTONS_ + 1; enum_key < KEYS::_END_OF_BUTTONS_; enum_key++)
	{
		uint32_t ds_button_bit = EnumKeyToNDSKeypadBit(enum_key);
		uint16_t pc_key        = profile_data->GetVirtualKey(enum_key);
		std::string command    = profile_data->GetCommand(enum_key);
		std::string ds_axis    = ConvertButtonToAxis(profile_data, enum_key);

		// if (enum_key) is an analog axis
		if(ds_axis != "")
		{
			// crop off the D2K_AXIS char
			ds_axis = ds_axis.substr(D2K_AXIS_LENGTH);

			int16_t input_value = 1;
			if(ds_axis.at(0) == '-')
				input_value = -1;
			// crop off the '-' or '+' char
			ds_axis = ds_axis.substr(1);

			if(ds_button_bit == DS2KEY_CPAD_LEFT || ds_button_bit == DS2KEY_CPAD_RIGHT)
				input_value *= client->GetCircleX();
			else if(ds_button_bit == DS2KEY_CPAD_UP || ds_button_bit == DS2KEY_CPAD_DOWN)
				input_value *= client->GetCircleY();
			else if(ds_button_bit == DS2KEY_CSTICK_LEFT || ds_button_bit == DS2KEY_CSTICK_RIGHT)
				input_value *= client->GetCstickX();
			else if(ds_button_bit == DS2KEY_CSTICK_UP || ds_button_bit == DS2KEY_CSTICK_DOWN)
				input_value *= client->GetCstickY();
			else if(ds_button_bit == DS2KEY_SLIDER_VOLUME)
				input_value = (int16_t)(client->GetSliderVolume() * (axis_max_value / 50.0f)) - axis_max_value;
			else if(ds_button_bit == DS2KEY_SLIDER_3D)
				input_value = (int16_t)(client->GetSlider3D() * (axis_max_value / 50.0f)) - axis_max_value;

			uint8_t output_axis = 0;
			if(ds_axis == "X")
				output_axis = HID_USAGE_X;
			else if(ds_axis == "Y")
				output_axis = HID_USAGE_Y;
			else if(ds_axis == "Z")
				output_axis = HID_USAGE_Z;
			else if(ds_axis == "RX")
				output_axis = HID_USAGE_RX;
			else if(ds_axis == "RY")
				output_axis = HID_USAGE_RY;
			else if(ds_axis == "RZ")
				output_axis = HID_USAGE_RZ;
			else if(ds_axis == "SL0")
				output_axis = HID_USAGE_SL0;
			else if(ds_axis == "SL1")
				output_axis = HID_USAGE_SL1;
			else if(ds_axis == "WHL")
				output_axis = HID_USAGE_WHL;
			else if(ds_axis == "POV")
				output_axis = HID_USAGE_POV;

			if(ds_axis == "VOL")
			{
				// this variable stops us from changing the volume every second
				// the user can now also manually change the volume
				static int16_t last_volume_input{};
				if(input_value != last_volume_input)
				{
					last_volume_input = input_value;

					SetMasterVolume((input_value + axis_max_value) / (axis_max_value * 2.0f));
				}
			}
			else if(output_axis != 0)
			{
				D2K::Input::Joystick::SetAxisSignedMax(joystick, output_axis, input_value, axis_max_value);
			}
		}
		// if (enum_key) is a digital button
		else if(pc_key)
		{
			// button enabled for turbo and pressed
			if(client->Turbo(ds_button_bit))
			{
				Input::Tap(pc_key, joystick);
				LOG(TRACE) << client->GetIPString() << ": tap:" << pc_key;
			}
			// Pressed
			else if(client->Down(ds_button_bit))
			{
				Input::Press(pc_key, joystick);
				LOG(TRACE) << client->GetIPString() << ": press:" << pc_key;
			}
			// Released, even in turbo mode
			else if(client->Up(ds_button_bit))
			{
				Input::Release(pc_key, joystick);
				LOG(TRACE) << client->GetIPString() << ": release:" << pc_key;
			}
		}
		// if (enum_key) is a command button AND was just pressed
		else if(command != ""
		&& client->Down(ds_button_bit))
		{
			ExecuteCommand(command);
		}
	}
	D2K::Input::Joystick::Update(joystick);
}

void ProcessTouchScreen(D2K::Client* client)
{
	// static values
	static uint16_t last_x = 0, last_y = 0;
	static bool last_screen_touched = false;

	bool screen_touched = client->Held(DS2KEY_TOUCH);
	// if touched
	if(screen_touched)
	{
		// How much movement to ignore, this helps jitter
		static const int s_ignore = 25;
		// Relative movement sensitivity scale
		static const int s_sensitivity = 3;
		// Deadzone Border: This helps the whole screen be accesible with absolute movement
		static const int s_deadzone = 5;

		uint16_t x = client->GetX();
		uint16_t y = client->GetY();
		ProfileData* profile_data = client->GetProfileDataPointer();
		std::string moveType = profile_data->m_mouse;

		// If newly pressed
		if(last_screen_touched == false)
		{
			//Input::Press(KEY_LBUTTON);
			last_x = x;
			last_y = y;
			last_screen_touched = true;
		}

		// check that we've moved
		if(!((x - last_x < -s_ignore)
		|| (x - last_x > s_ignore)
		|| (y - last_y < -s_ignore)
		|| (y - last_y > s_ignore)))
		{
			// relative movement
			if(moveType == "Relative")
			{
				Input::Move((x - last_x) * s_sensitivity, (y - last_y) * s_sensitivity);
			}
			// absolute movement
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

void ProcessPacket(D2K::Client* client)
{
	ProcessButtons(client);
	ProcessTouchScreen(client);

#ifdef false
	// gyro/accel test stuff
	{
		uint8_t device = 1;
		int16_t scale = 64;
		int16_t value = client->GetGyroX();
		int16_t diff_value = (int16_t)1.3*scale;
		int16_t axis_max_value = 60;
		static int16_t adjustment_value_x = 0.0;
		static int16_t adjustment_value_y = 0.0;
		static int16_t adjustment_value_z = 0.0;
		//if(value > -diff_value && value < diff_value)
		{
		//	adjustment_value_x = -client->GetAccelX();
		//	adjustment_value_z = client->GetAccelZ();
		}
		int16_t accelX = client->GetAccelX();
		int16_t accelY = client->GetAccelY();
		int16_t accelZ = client->GetAccelZ();
		int16_t gyroX = client->GetGyroX() * 0.05;
		int16_t gyroY = client->GetGyroY() * 0.05;
		int16_t gyroZ = client->GetGyroZ() * 0.05;
		//D2K::Input::Joystick::SetAxisSignedMax(device, HID_USAGE_X,  client->GetAccelX() + adjustment_value_x, axis_max_value);
		//D2K::Input::Joystick::SetAxisSignedMax(device, HID_USAGE_Y, -client->GetAccelY() + adjustment_value_y, axis_max_value);
		//D2K::Input::Joystick::SetAxisSignedMax(device, HID_USAGE_Z, -client->GetAccelZ() + adjustment_value_z, axis_max_value);
		if(client->Held(DS2KEY_ZR))
		{
			D2K::Input::Joystick::SetAxisSignedMax(device, HID_USAGE_RX, gyroX + adjustment_value_x, axis_max_value);
			D2K::Input::Joystick::SetAxisSignedMax(device, HID_USAGE_RY, -gyroY + adjustment_value_y, axis_max_value);
			D2K::Input::Joystick::SetAxisSignedMax(device, HID_USAGE_RZ, -gyroZ + adjustment_value_z, axis_max_value);

			D2K::Input::Joystick::Update(device);
		}
	}
#endif

	uint16_t keyboard_press = client->GetKeyboardPress();
	if(keyboard_press != 0)
	{
		Input::Press(keyboard_press, Input::Joystick::NO_JOYSTICK);
		Input::Release(keyboard_press, Input::Joystick::NO_JOYSTICK);
	}
}

// TODO this needs tested with multiple devices connected
void ReleaseDeadClient(D2K::Client* client)
{
	ProfileData* profile_data = client->GetProfileDataPointer();
	uint8_t joystick = profile_data->GetValue8(KEYS::JOY);

	for(int enum_key = _START_OF_BUTTONS_ + 1; enum_key < KEYS::_END_OF_BUTTONS_; enum_key++)
	{
		uint32_t ds_button_bit = EnumKeyToNDSKeypadBit(enum_key);
		uint16_t pc_key = profile_data->GetVirtualKey(enum_key);

		// if pc_key is valid and ds_button_bit is held
		if(pc_key
		&& client->Held(ds_button_bit))
			Input::Release(pc_key, joystick);
	}

	D2K::Input::Joystick::DeInit(joystick);
}

void CheckForDeadClients()
{
	using namespace std::chrono;
	static high_resolution_clock::time_point time_previous = high_resolution_clock::now();
	       high_resolution_clock::time_point time_current  = high_resolution_clock::now();
	long long time_difference = duration_cast<milliseconds>(time_current - time_previous).count();
	if(time_difference >= 1000)
	{
		time_previous = time_current;

		UDP::DS2KeyPacket Packet{};
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
				LOG(INFO) << "Client #:" << i << " removed from inactivity.";
			}
			else
			{
				g_client_array[i]->SetAlive(CLIENT_STATUS::CHECKING);
				UDP::SetRemoteIP(g_client_array[i]->GetIP());
				UDP::Send(&Packet, sizeof(UDP::DS2KeyPacket));
				LOG_EVERY_N(10,DEBUG) << "Client #:" << i << " is active.";
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
	D2K::InitLogging(argc, argv);

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
			Config::SetConfigPort((uint16_t)atoi(&argv[arg][7]));
		}
	}

	return UDP::Connect(non_blocking, Config::GetPort());
}

void Loop()
{
	UDP::DS2KeyCommandSettingsPacket Packet{};

	// if we  are running, connected, and receive something without error
	if(g_running
	&& UDP::IsConnected()
	&& UDP::Recv(&Packet, sizeof(UDP::DS2KeyCommandSettingsPacket)) == 0)
	{
		// If profile is active
		if(g_client_array[Packet.profile] != nullptr)
		{
			// Set Alive Status
			g_client_array[Packet.profile]->SetAlive(CLIENT_STATUS::ALIVE);
		}

		switch(Packet.type)
		{
		case UDP::PACKET::LOOKUP:
		case UDP::PACKET::ALIVE:
		{
			UDP::DS2KeyPacket* normal_packet = (UDP::DS2KeyPacket*)&Packet;
			UDP::Send(&normal_packet, sizeof(UDP::DS2KeyPacket));
			LOG_EVERY_N(10, TRACE) << "Received UDP::PACKET::LOOKUP";
			break;
		}
		case UDP::PACKET::COMMAND_SETTINGS:
		{
			LOG(TRACE) << "Received UDP::PACKET::COMMAND_SETTINGS";
			// If profile is active
			if(g_client_array[Packet.profile] != nullptr)
			{
				UDP::DS2KeyCommandSettingsPacket settings = UDP::DS2KeyCommandSettingsPacket{};

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
				}

				// Send settings packet
				UDP::SendCommandSettings(settings);
			}
			break;
		}
		case UDP::PACKET::NORMAL:
		{
			UDP::DS2KeyPacket* normal_packet = (UDP::DS2KeyPacket*)&Packet;

			D2K::Client* client_pointer = Config::GetClient(normal_packet->profile);
			if(client_pointer->GetIP() == 0)
				client_pointer->SetIP(UDP::GetRemoteIP());
			// insert packet data
			client_pointer->SetPacket(*normal_packet);
			// update
			client_pointer->Scan();
			ProcessPacket(client_pointer);
			LOG_EVERY_N(300, TRACE) << client_pointer->GetIPString() << ":Received UDP::PACKET::NORMAL";
			break;
		}
		case UDP::PACKET::NORMAL_SETTING:
		{
			LOG(TRACE) << "Received UDP::PACKET::NORMAL_SETTING";
			UDP::DS2KeyNormalSettingsPacket* normal_setting_packet = (UDP::DS2KeyNormalSettingsPacket*)&Packet;
			// If profile is active
			if(g_client_array[normal_setting_packet->profile] != nullptr)
			{
				LOG(TRACE) << "Profile is active";
				ProfileData* profile_data = g_client_array[normal_setting_packet->profile]->GetProfileDataPointer();
				Config::SetProfileSetting(profile_data, normal_setting_packet->profile, normal_setting_packet->setting, normal_setting_packet->value);
			}
			break;
		}
		case UDP::PACKET::COMMAND:
		{
			UDP::DS2KeyPacket* normal_packet = (UDP::DS2KeyPacket*)&Packet;

			D2K::Client* client_pointer = Config::GetClient(normal_packet->profile);
			if(client_pointer->GetIP() == 0)
				client_pointer->SetIP(UDP::GetRemoteIP());
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
			LOG(TRACE) << "Received UDP::PACKET::COMMAND";
			break;
		}
		}
	}

	CheckForDeadClients();

	// Sleep to avoid 99% cpu when not using -O2
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	#ifdef WIN32GUI
	// Take care of GUI stuff
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
	D2K::DeInitLogging();
}

} // namespace D2K
