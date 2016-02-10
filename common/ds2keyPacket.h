#pragma once

#include <stdint.h>//uint8_t, uint16_t

namespace D2K {namespace UDP{

#pragma pack(1)
typedef struct DS2KeyPacket final
{
	uint8_t type;
	uint8_t profile;
	uint16_t keys;
	uint16_t keys_turbo;//Enabled Turbo keys
	uint8_t gh_keys;
	uint8_t gh_keys_turbo;//Enabled GuitarGrip Turbo keys
	uint16_t touch_keys;
	uint16_t touch_keys_turbo;//Enabled Touch Turbo keys
	uint8_t touch_x;
	uint8_t touch_y;
} DS2KeyPacket;

const int SETTINGS_PACKET_MAX_BUTTONS = 12;  // 12 buttons
const int SETTINGS_PACKET_MAX_TEXT = 10;  // 10 chars, 1 null terminator

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
} DS2KeySettingsPacket;
#pragma pack()

}}//namespace D2K::UDP
