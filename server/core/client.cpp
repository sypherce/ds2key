// Virtual DS status

#include "client.h"
#include "key.h"
#include "common/misc.h"
#include <sstream>  // std::stringstream

namespace D2K {

Client* g_client_array[CLIENT_MAX] = { };

uint32_t EnumKeyToNDSKeypadBit(int enum_key)
{
	switch(enum_key)
	{
	case KEYS::DUP:
		return DS2KEY_DUP;
	case KEYS::DDOWN:
		return DS2KEY_DDOWN;
	case KEYS::DLEFT:
		return DS2KEY_DLEFT;
	case KEYS::DRIGHT:
		return DS2KEY_DRIGHT;
	case KEYS::A:
		return DS2KEY_A;
	case KEYS::B:
		return DS2KEY_B;
	case KEYS::X:
		return DS2KEY_X;
	case KEYS::Y:
		return DS2KEY_Y;

	case KEYS::L:
		return DS2KEY_L;
	case KEYS::R:
		return DS2KEY_R;

	case KEYS::START:
		return DS2KEY_START;
	case KEYS::SELECT:
		return DS2KEY_SELECT;

	case KEYS::LID:
		return DS2KEY_LID;

	case KEYS::ZL:
		return DS2KEY_ZL;
	case KEYS::ZR:
		return DS2KEY_ZR;
	
	case KEYS::CSTICK_UP:
		return DS2KEY_CSTICK_UP;
	case KEYS::CSTICK_DOWN:
		return DS2KEY_CSTICK_DOWN;
	case KEYS::CSTICK_LEFT:
		return DS2KEY_CSTICK_LEFT;
	case KEYS::CSTICK_RIGHT:
		return DS2KEY_CSTICK_RIGHT;

	case KEYS::CPAD_UP:
		return DS2KEY_CPAD_UP;
	case KEYS::CPAD_DOWN:
		return DS2KEY_CPAD_DOWN;
	case KEYS::CPAD_LEFT:
		return DS2KEY_CPAD_LEFT;
	case KEYS::CPAD_RIGHT:
		return DS2KEY_CPAD_RIGHT;

	case KEYS::BLUE:
		return DS2KEY_BLUE;
	case KEYS::YELLOW:
		return DS2KEY_YELLOW;
	case KEYS::RED:
		return DS2KEY_RED;
	case KEYS::GREEN:
		return DS2KEY_GREEN;
	default:
		return 0;
	}
}

bool ProfileData::isVirtualKey(std::string button)
{
	if(button.substr(0, D2K_COMMAND_LENGTH) != D2K_COMMAND)	//if button
		return true;
	//if command
		return false;
}
uint16_t ProfileData::StringToVirtualKey(std::string button)
{
	if(isVirtualKey(button))
		return D2K::string_to_uint16_t(button);
	else
		return 0;
}
//this currently is the same as D2K::ltos
std::string ProfileData::VirtualKeyToString(uint16_t button)
{
	std::stringstream stream;
	stream << D2K::ltos(button);

	return stream.str();
}
ProfileData::ProfileData()
{

}
ProfileData::~ProfileData()
{

}
std::string& ProfileData::GetStringReference(int enum_key)
{
	switch(enum_key)
	{
	case KEYS::MOUSE:
		return m_mouse;
	case KEYS::JOY:
		return m_joy;
	case KEYS::A:
		return m_a;
	case KEYS::B:
		return m_b;
	case KEYS::X:
		return m_x;
	case KEYS::Y:
		return m_y;
	case KEYS::L:
		return m_l;
	case KEYS::R:
		return m_r;
	case KEYS::DUP:
		return m_dpad_up;
	case KEYS::DDOWN:
		return m_dpad_down;
	case KEYS::DLEFT:
		return m_dpad_left;
	case KEYS::DRIGHT:
		return m_dpad_right;
	case KEYS::START:
		return m_start;
	case KEYS::SELECT:
		return m_select;
	case KEYS::LID:
		return m_lid;
	case KEYS::BLUE:
		return m_blue;
	case KEYS::YELLOW:
		return m_yellow;
	case KEYS::RED:
		return m_red;
	case KEYS::GREEN:
		return m_green;
		
	case KEYS::ZL:
		return m_zl;
	case KEYS::ZR:
		return m_zr;

	case KEYS::CSTICK_UP:
		return m_cstick_up;
	case KEYS::CSTICK_DOWN:
		return m_cstick_down;
	case KEYS::CSTICK_LEFT:
		return m_cstick_left;
	case KEYS::CSTICK_RIGHT:
		return m_cstick_right;

	case KEYS::CPAD_UP:
		return m_cpad_up;
	case KEYS::CPAD_DOWN:
		return m_cpad_down;
	case KEYS::CPAD_LEFT:
		return m_cpad_left;
	case KEYS::CPAD_RIGHT:
		return m_cpad_right;

	case KEYS::TOUCH_00:
		return m_touch_command[0];
	case KEYS::TOUCH_01:
		return m_touch_command[1];
	case KEYS::TOUCH_02:
		return m_touch_command[2];
	case KEYS::TOUCH_03:
		return m_touch_command[3];
	case KEYS::TOUCH_04:
		return m_touch_command[4];
	case KEYS::TOUCH_05:
		return m_touch_command[5];
	case KEYS::TOUCH_06:
		return m_touch_command[6];
	case KEYS::TOUCH_07:
		return m_touch_command[7];
	case KEYS::TOUCH_08:
		return m_touch_command[8];
	case KEYS::TOUCH_09:
		return m_touch_command[9];
	case KEYS::TOUCH_10:
		return m_touch_command[10];
	case KEYS::TOUCH_11:
		return m_touch_command[11];
	case KEYS::TOUCH_STRING_00:
		return m_touch_string[0];
	case KEYS::TOUCH_STRING_01:
		return m_touch_string[1];
	case KEYS::TOUCH_STRING_02:
		return m_touch_string[2];
	case KEYS::TOUCH_STRING_03:
		return m_touch_string[3];
	case KEYS::TOUCH_STRING_04:
		return m_touch_string[4];
	case KEYS::TOUCH_STRING_05:
		return m_touch_string[5];
	case KEYS::TOUCH_STRING_06:
		return m_touch_string[6];
	case KEYS::TOUCH_STRING_07:
		return m_touch_string[7];
	case KEYS::TOUCH_STRING_08:
		return m_touch_string[8];
	case KEYS::TOUCH_STRING_09:
		return m_touch_string[9];
	case KEYS::TOUCH_STRING_10:
		return m_touch_string[10];
	case KEYS::TOUCH_STRING_11:
		return m_touch_string[11];
	default:
		return std::string("");
	}
}

std::string ProfileData::GetButtonString(int enum_key)
{
	if(isVirtualKey(GetStringReference(enum_key)))
		return Key::GetString(GetVirtualKey(enum_key));
	else
		return GetCommand(enum_key);
}
uint8_t ProfileData::GetValue8(int enum_key)
{
	std::string& pointer = GetStringReference(enum_key);
	if(pointer == "")
		return 0;
	else
		return D2K::string_to_uint8_t(pointer);
}
uint16_t ProfileData::GetValue16(int enum_key)
{
	std::string& pointer = GetStringReference(enum_key);
	if(pointer == "")
		return 0;
	else
		return D2K::string_to_uint16_t(pointer);
}
uint16_t ProfileData::GetVirtualKey(int enum_key)
{
	std::string& pointer = GetStringReference(enum_key);
	if(pointer == "" || pointer == "0")
		return 0;
	else
		return StringToVirtualKey(pointer);
}

void ProfileData::SetVirtualKey(int enum_key, uint16_t value)
{
	std::string& pointer = GetStringReference(enum_key);
	pointer = VirtualKeyToString(value);
}

void ProfileData::SetCommand(int enum_key, std::string value)
{
	std::string& Pointer = GetStringReference(enum_key);
	if(isVirtualKey(value))
		SetVirtualKey(enum_key, Key::GetNumber(value.c_str()));
	else
		Pointer = value;
}
void ProfileData::SetTouchPos(uint8_t i, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
	if(i < UDP::SETTINGS_PACKET_MAX_BUTTONS)
	{
		m_touch_x[i] = x;
		m_touch_y[i] = y;
		m_touch_w[i] = w;
		m_touch_h[i] = h;
	}
}
const std::string& ProfileData::GetCommand(int enum_key)
{
	std::string& pointer = GetStringReference(enum_key);
	if(!isVirtualKey(pointer))
		return pointer;

	return std::string("");
}

Client::Client()
{
	m_packet = UDP::DS2KeyPacket{ };
	m_keys =
	m_keys_old = 0;
	SetAlive(CLIENT_STATUS::ALIVE);
}

Client::~Client()
{

}

void Client::Scan(void)
{
	m_keys_old = m_keys;
	m_keys = m_packet.keys;
}

ProfileData* Client::GetProfileDataPointer()
{
	return &m_profile_data;
}

void Client::SetPacket(UDP::DS2KeyPacket packet)
{
	m_packet = packet;
}

void Client::SetTouchPos(uint16_t x, uint16_t y)
{
	m_packet.touch_x = x;
	m_packet.touch_y = y;
}

void Client::Press(uint32_t key)
{
	m_packet.keys |= key;
}

void Client::Release(uint32_t key)
{
	m_packet.keys &= ~key;
}

bool Client::Held(uint32_t key)
{
	return (m_keys&key) != 0;
}

bool Client::Down(uint32_t key)
{
	return ((m_keys &~ m_keys_old)&key) != 0;
}

bool Client::Up(uint32_t key)
{
	return (((m_keys ^ m_keys_old) & (~m_keys))&key) != 0;
}

bool Client::Turbo(uint32_t key)
{
	return (m_packet.keys_turbo&key) != 0;
}

uint32_t Client::GetIP()
{
	return m_packet.ip_address;
}

uint16_t Client::GetX()
{
	return m_packet.touch_x;
}

uint16_t Client::GetY()
{
	return m_packet.touch_y;
}

bool Client::IsAlive()
{
	return m_alive;
}

void Client::SetAlive(bool client_status)
{
	m_alive = client_status;
}

}//namespace D2K
