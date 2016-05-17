#pragma once

#include <cstdint>//uint8_t, uint16_t, uint32_t

namespace D2K {namespace UDP{

#ifdef _MSC_VER
#pragma pack(1)
#define __attribute__(x)
#endif
typedef struct DS2KeyPacket final
{
	uint8_t type;
	uint8_t profile;
	uint32_t ip_address;
	uint32_t keys;
	uint32_t keys_turbo;       //Enabled Turbo keys
	uint16_t touch_keys;
	uint16_t touch_keys_turbo; //Enabled Touch Turbo keys
	uint16_t touch_x;
	uint16_t touch_y;
} __attribute__((packed)) DS2KeyPacket;

const int SETTINGS_PACKET_MAX_BUTTONS = 12; // 12 buttons
const int SETTINGS_PACKET_MAX_TEXT    = 10; // 10 chars, 1 null terminator

//contains button layout and text for command buttons
//there's a max of 12 buttons
//text max is currently 10 + null terminator
typedef struct DS2KeySettingsPacket final
{
	uint8_t type;
	uint8_t profile;
	uint8_t x_1[SETTINGS_PACKET_MAX_BUTTONS];
	uint8_t x_2[SETTINGS_PACKET_MAX_BUTTONS];
	uint8_t y_1[SETTINGS_PACKET_MAX_BUTTONS];
	uint8_t y_2[SETTINGS_PACKET_MAX_BUTTONS];
	char text[SETTINGS_PACKET_MAX_BUTTONS][SETTINGS_PACKET_MAX_TEXT+1];
} __attribute__((packed)) DS2KeySettingsPacket;
#ifdef _MSC_VER
#pragma pack()
#undef __attribute__
#endif

}}//namespace D2K::UDP
