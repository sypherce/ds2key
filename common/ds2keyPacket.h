#pragma once

#include <cstdint> // uint8_t, uint16_t, uint32_t

namespace D2K {namespace UDP{

#ifdef _MSC_VER
#pragma pack(1)
#define __attribute__(x)
#endif
typedef struct DS2KeyPacket final
{
	uint8_t type;              // byte [0] has to by `type` across all packets
	uint8_t profile;           // byte [1] has to by `profile` across all packets
	uint32_t keys;
	uint32_t keys_turbo;       // Enabled Turbo keys
	uint16_t touch_x;
	uint16_t touch_y;
	int16_t circle_x;
	int16_t circle_y;
	int16_t cstick_x;
	int16_t cstick_y;
	int16_t accel_x;
	int16_t accel_y;
	int16_t accel_z;
	int16_t gyro_x;
	int16_t gyro_y;
	int16_t gyro_z;
	uint8_t slider_volume;     // Range is between 0-100%
	uint8_t slider_3d;         // Range is between 0-100%
	uint16_t keyboard;         // Keyboard Key Pressed
} __attribute__((packed)) DS2KeyPacket;

const int SETTINGS_PACKET_MAX_BUTTONS = 12; // 12 buttons
const int SETTINGS_PACKET_MAX_TEXT    = 10; // 10 chars, 1 null terminator

//contains button layout and text for command buttons
//there's a max of 12 buttons
//text max is currently 10 + null terminator
typedef struct DS2KeyCommandSettingsPacket final
{
	uint8_t type;              // byte [0] has to by `type` across all packets
	uint8_t profile;           // byte [1] has to by `profile` across all packets
	uint8_t x_1[SETTINGS_PACKET_MAX_BUTTONS];
	uint8_t x_2[SETTINGS_PACKET_MAX_BUTTONS];
	uint8_t y_1[SETTINGS_PACKET_MAX_BUTTONS];
	uint8_t y_2[SETTINGS_PACKET_MAX_BUTTONS];
	char text[SETTINGS_PACKET_MAX_BUTTONS][SETTINGS_PACKET_MAX_TEXT+1];
} __attribute__((packed)) DS2KeyCommandSettingsPacket;

//TODO: this will change
//currently this contains 1 setting an 1 value, eventually we may include the
//entire config file when we update and a CRC or some other verification
typedef struct DS2KeyNormalSettingsPacket final
{
	uint8_t type;              // byte [0] has to by `type` across all packets
	uint8_t profile;           // byte [1] has to by `profile` across all packets
	uint16_t setting;
	uint16_t value;
} __attribute__((packed)) DS2KeyNormalSettingsPacket;
#ifdef _MSC_VER
#pragma pack()
#undef __attribute__
#endif

}}//namespace D2K::UDP
