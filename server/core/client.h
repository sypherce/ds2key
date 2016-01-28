#pragma once

#include <string>		//std::string
#include <cstdint>		//uint8_t, etc
#include "common/ds2keyPacket.h"

static const char* D2K_COMMAND = "*";
static const int D2K_COMMAND_LENGTH = 1;

namespace D2K {
static const uint16_t DS2KEY_A = (1 << (0));
static const uint16_t DS2KEY_B = (1 << (1));
static const uint16_t DS2KEY_SELECT = (1 << (2));
static const uint16_t DS2KEY_START = (1 << (3));
static const uint16_t DS2KEY_RIGHT = (1 << (4));
static const uint16_t DS2KEY_LEFT = (1 << (5));
static const uint16_t DS2KEY_UP = (1 << (6));
static const uint16_t DS2KEY_DOWN = (1 << (7));
static const uint16_t DS2KEY_R = (1 << (8));
static const uint16_t DS2KEY_L = (1 << (9));
static const uint16_t DS2KEY_X = (1 << (10));
static const uint16_t DS2KEY_Y = (1 << (11));
static const uint16_t DS2KEY_TOUCH = (1 << (12));
static const uint16_t DS2KEY_LID = (1 << (13));

static const uint8_t DS2KEY_BLUE = (1 << (3));
static const uint8_t DS2KEY_YELLOW = (1 << (4));
static const uint8_t DS2KEY_RED = (1 << (5));
static const uint8_t DS2KEY_GREEN = (1 << (6));
//this represents each value in a Keys Profile array. Example: Profile[KEYS::UP] is defined as KEY_UP by default
//this was stored in config.h but will be garbage soon
enum KEYS
{
	Profile, MOUSE, JOY, UP, DOWN, LEFT, RIGHT, A, B, X, Y, L, R, START, SELECT, LID,
	BLUE, YELLOW, RED, GREEN,
	TOUCH_00, TOUCH_01, TOUCH_02, TOUCH_03, TOUCH_04, TOUCH_05, TOUCH_06, TOUCH_07, TOUCH_08, TOUCH_09, TOUCH_10, TOUCH_11,
	TOUCH_STRING_00, TOUCH_STRING_01, TOUCH_STRING_02, TOUCH_STRING_03, TOUCH_STRING_04, TOUCH_STRING_05, TOUCH_STRING_06, TOUCH_STRING_07, TOUCH_STRING_08, TOUCH_STRING_09, TOUCH_STRING_10, TOUCH_STRING_11,
	TOUCH_00X, TOUCH_01X, TOUCH_02X, TOUCH_03X, TOUCH_04X, TOUCH_05X, TOUCH_06X, TOUCH_07X, TOUCH_08X, TOUCH_09X, TOUCH_10X, TOUCH_11X,
	TOUCH_00Y, TOUCH_01Y, TOUCH_02Y, TOUCH_03Y, TOUCH_04Y, TOUCH_05Y, TOUCH_06Y, TOUCH_07Y, TOUCH_08Y, TOUCH_09Y, TOUCH_10Y, TOUCH_11Y,
	TOUCH_00W, TOUCH_01W, TOUCH_02W, TOUCH_03W, TOUCH_04W, TOUCH_05W, TOUCH_06W, TOUCH_07W, TOUCH_08W, TOUCH_09W, TOUCH_10W, TOUCH_11W,
	TOUCH_00H, TOUCH_01H, TOUCH_02H, TOUCH_03H, TOUCH_04H, TOUCH_05H, TOUCH_06H, TOUCH_07H, TOUCH_08H, TOUCH_09H, TOUCH_10H, TOUCH_11H,
	END
};
//probably not the best place for BitToButton
extern uint16_t BitToButton(int bit);

class ProfileData final
{
public:
	ProfileData();
	~ProfileData();

	std::string& GetStringReference(int button);

	//@param Button (enum Keys) value. Example: KEYS::A, KEYS::UP, KEYS::GREEN
	//@return Human readable version of (Button)'s current setting. Example: KEY_A, EY_UP
	std::string GetButtonString(int button);
	uint8_t GetValue8(int button);
	uint16_t GetValue16(int button);
	uint16_t GetVirtualKey(int button);

	void SetVirtualKey(int button, uint16_t value);
	void SetCommand(int button, std::string value);
	void SetTouchPos(uint8_t i, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
	const std::string& GetCommand(int button);

	uint8_t m_id;//uint8_t
	std::string m_mouse;//uint8_t
	std::string m_joy;//uint8_t
	//buttons
	std::string m_up, m_down, m_left, m_right, m_a, m_b, m_x, m_y, m_l, m_r, m_start, m_select, m_lid;
	//guitar grip buttons
	std::string m_blue, m_yellow, m_red, m_green;
	std::string m_empty;//this probably shouldn't be used but I don't know what to do
	std::string m_touch_command[12];//Command
	std::string m_touch_string[12];//String
	uint8_t m_touch_x[12];//uint8_t
	uint8_t m_touch_y[12];//uint8_t
	uint8_t m_touch_w[12];//uint8_t
	uint8_t m_touch_h[12];//uint8_t

private:
	bool isVirtualKey(std::string button);
	uint16_t StringToVirtualKey(std::string button);
	std::string VirtualKeyToString(uint16_t button);
};

//Contains a client's current state
class Client final
{
public:
	Client();
	~Client();

	//Updates current button state after calls made to Press, Release, GHPress, and GHRelease
	void Scan(void);

	//@return Pointer to Profile array
	ProfileData* GetProfileDataPointer();

	void SetPacket(UDP::DS2KeyPacket p);
	bool Held(uint16_t key);
	bool Down(uint16_t key);
	bool Up(uint16_t key);

	//@return true if (key) has Turbo mode enabled
	bool Turbo(uint16_t key);

	bool GHHeld(uint8_t key);
	bool GHDown(uint8_t key);
	bool GHUp(uint8_t key);

	//@return true if (key) has Turbo mode enabled
	bool GHTurbo(uint8_t key);

	//@return Stylus current X Position. Values range 0-255
	uint8_t GetX();

	//@return Stylus current X Position. Values range 0-191
	uint8_t GetY();

private:
	uint8_t GetProfileNumber();
	void SetTouchPos(uint8_t x, uint8_t y);
	void Press(uint16_t key);
	void Release(uint16_t key);

	void GHPress(uint8_t key);
	void GHRelease(uint8_t key);

	UDP::DS2KeyPacket m_packet;
	ProfileData m_profile_data;

	uint16_t m_keys;
	uint16_t m_keys_old;
	uint8_t m_gh_keys;
	uint8_t m_gh_keys_old;
};

extern Client* g_client_array[256];

}//namespace D2K
