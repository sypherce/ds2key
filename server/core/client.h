#pragma once

#include <string>   //std::string
#include <cstdint>  //uint8_t, etc
#include "common/ds2keyPacket.h"
#include "common/enum_keys.h"

namespace D2K {

static const int CLIENT_MAX = 256;
static const char* D2K_COMMAND = "*";
static const int D2K_COMMAND_LENGTH = 1;
static const uint32_t DS2KEY_A = (1 << (0));
static const uint32_t DS2KEY_B = (1 << (1));
static const uint32_t DS2KEY_SELECT = (1 << (2));
static const uint32_t DS2KEY_START = (1 << (3));
static const uint32_t DS2KEY_DRIGHT = (1 << (4)); 
static const uint32_t DS2KEY_DLEFT = (1 << (5)); 
static const uint32_t DS2KEY_DUP = (1 << (6)); 
static const uint32_t DS2KEY_DDOWN = (1 << (7)); 
static const uint32_t DS2KEY_R = (1 << (8));
static const uint32_t DS2KEY_L = (1 << (9));
static const uint32_t DS2KEY_X = (1 << (10));
static const uint32_t DS2KEY_Y = (1 << (11));
static const uint32_t DS2KEY_TOUCH = (1 << (12)) | (1 << (20));
static const uint32_t DS2KEY_LID = (1 << (13));

static const uint32_t DS2KEY_ZL = (1 << (14));
static const uint32_t DS2KEY_ZR = (1 << (15));

static const uint32_t DS2KEY_BLUE = (1 << (16));
static const uint32_t DS2KEY_YELLOW = (1 << (17));
static const uint32_t DS2KEY_RED = (1 << (18));
static const uint32_t DS2KEY_GREEN = (1 << (19));

static const uint32_t DS2KEY_CSTICK_RIGHT = (1 << (24));
static const uint32_t DS2KEY_CSTICK_LEFT = (1 << (25));
static const uint32_t DS2KEY_CSTICK_UP = (1 << (26));
static const uint32_t DS2KEY_CSTICK_DOWN = (1 << (27));
static const uint32_t DS2KEY_CPAD_RIGHT = (1 << (28));
static const uint32_t DS2KEY_CPAD_LEFT = (1 << (29));
static const uint32_t DS2KEY_CPAD_UP = (1 << (30));
static const uint32_t DS2KEY_CPAD_DOWN = (1 << (31));

extern uint32_t EnumKeyToNDSKeypadBit(int enum_key);

class ProfileData final
{
public:
	ProfileData();
	~ProfileData();
	
	//Returns profile data as a human readable string
	//
	//Example: enum_key KEYS::UP(0x03) returns std::string "KEY_UP"
	//
	//@param enum_key (enum KEYS) Example: KEYS::A, KEYS::UP, KEYS::GREEN
	//@return Human readable version of (enum_key)'s current setting. Example: KEY_A, KEY_UP
	std::string& GetStringReference(int enum_key);
	
	//Currently only used in config.cpp as of 3-30-16.
	//Checks if enum_key qualifies as a button, then returns GetStringReference
	//If it doesn't qualify, it returns GetCommand, which also uses GetStringReference
	//
	//Example: enum_key KEYS::UP(0x03) returns std::string "KEY_UP"
	//
	//@param enum_key (enum KEYS) Example: KEYS::A, KEYS::UP, KEYS::GREEN
	//@return Human readable version of (enum_key)'s current setting. Example: KEY_A, KEY_UP
	std::string GetButtonString(int enum_key);
	
	//Currently only used in core.cpp as of 3-30-16 to retrieve the joystick #.
	//Retrieves a value with GetStringReference and converts it to a uin8_t value
	//
	//@param enum_key (enum KEYS) Example: KEYS::A, KEYS::UP, KEYS::GREEN
	//@return uint8_t(0-255) version of (enum_key)'s current setting.
	uint8_t GetValue8(int enum_key);
	//Currently unused as of 3-30-16
	//Retrieves a value with GetStringReference and converts it to a uin8_t value
	//
	//@param enum_key (enum KEYS) Example: KEYS::A, KEYS::UP, KEYS::GREEN
	//@return uint16_t(0-65535) version of (enum_key)'s current setting.
	uint16_t GetValue16(int enum_key);

	//Returns profile data as a system specific uint16_t (0-65535)
	//
	//Example: enum_key KEYS::UP(0x03) returns uint16_t VK_UP(0x26)
	//
	//@param enum_key (enum KEYS) Example: KEYS::A, KEYS::UP, KEYS::GREEN
	//@return system specific uint16_t (0-65535)
	uint16_t GetVirtualKey(int enum_key);
	
	//Currently only used in config.cpp as of 3-30-16.
	//Checks that enum_key DOES NOT qualify as a button, we assume IT IS a command after that
	//If it is a button, it returns ""
	//
	//Example: enum_key KEYS::UP(0x03) returns std::string "KEY_UP"
	//
	//@param enum_key (enum KEYS) Example: KEYS::A, KEYS::UP, KEYS::GREEN
	//@return (std::string) containing command info, if enum_key is a button it returns "".
	const std::string& GetCommand(int enum_key);

	void SetVirtualKey(int enum_key, uint16_t value);
	void SetCommand(int enum_key, std::string value);
	void SetTouchPos(uint8_t i, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

	uint8_t m_id;//TODO:Is this just garbage?
	std::string m_mouse;
	std::string m_joy;
	std::string m_dpad_up, m_dpad_down, m_dpad_left, m_dpad_right, m_a, m_b, m_x, m_y, m_l, m_r, m_start, m_select, m_lid;

	std::string m_zl, m_zr;

	std::string m_cstick_up, m_cstick_down, m_cstick_left, m_cstick_right;

	std::string m_cpad_up, m_cpad_down, m_cpad_left, m_cpad_right;

	std::string m_blue, m_yellow, m_red, m_green;
	std::string m_touch_command[12];
	std::string m_touch_string[12];
	uint8_t m_touch_x[12];
	uint8_t m_touch_y[12];
	uint8_t m_touch_w[12];
	uint8_t m_touch_h[12];
	std::string m_null;

private:
	//returns true if `button` is a virtual keypress, false otherwise
	bool isVirtualKey(std::string button);
	uint16_t StringToVirtualKey(std::string button);
	std::string VirtualKeyToString(uint16_t button);
};

enum CLIENT_STATUS : bool
{
	ALIVE = true,
	CHECKING = false,
};

//Contains a client's current state
class Client final
{
public:
	Client();
	~Client();

	//Updates current button state after calls made to Press, Release
	void Scan(void);

	//@return Pointer to Profile array
	ProfileData* GetProfileDataPointer();

	void SetPacket(UDP::DS2KeyPacket p);
	bool Held(uint32_t key);
	bool Down(uint32_t key);
	bool Up(uint32_t key);

	//@return true if (key) has Turbo mode enabled
	bool Turbo(uint32_t key);
	
	//@return IP address. 0 if not assigned
	uint32_t GetIP();

	//@return Stylus current X Position. Values range 0-255(NDS), 0-319(3DS)
	uint16_t GetX();

	//@return Stylus current Y Position. Values range 0-191(NDS), 0-239(3DS)
	uint16_t GetY();

	//@return CLIENT_STATUS::ALIVE if confirmed, CLIENT_STATUS::CHECKING if not
	bool IsAlive();

	//@return CLIENT_STATUS::ALIVE if confirmed, CLIENT_STATUS::CHECKING if not
	void SetAlive(bool client_status);

	//TODO finish/update comment... 
	//@return DS KeyboardPress, currently only uint8_t values come through (1-255), 0 = no press
	uint16_t GetKeyboardPress();

private:
	void SetTouchPos(uint16_t x, uint16_t y);
	void Press(uint32_t key);
	void Release(uint32_t key);

	UDP::DS2KeyPacket m_packet;
	ProfileData m_profile_data;

	bool m_alive;

	uint32_t m_keys;
	uint32_t m_keys_old;
};

extern Client* g_client_array[CLIENT_MAX];

}//namespace D2K
