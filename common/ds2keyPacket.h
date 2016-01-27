#pragma once

#include <stdint.h>//uint8_t, uint16_t

namespace D2K
{
namespace UDP
{
#pragma pack(1)
typedef struct DS2KeyPacket
{
	uint8_t type;
	uint8_t profile;
	uint16_t keys;
	uint16_t keys_turbo;		//Enabled Turbo keys
	uint8_t gh_keys;
	uint8_t gh_keys_turbo;	//Enabled GuitarGrip Turbo keys
	uint16_t touch_keys;
	uint16_t touch_keys_turbo;//Enabled Touch Turbo keys
	uint8_t touch_x;
	uint8_t touch_y;
} DS2KeyPacket;


//contains button layout and text for command buttons
//there's a max of 12 buttons
//text max is currently 10 + null terminator
typedef struct DS2KeySettingsPacket
{
	uint8_t type;
	uint8_t profile;
	uint8_t x_1[12];
	uint8_t x_2[12];
	uint8_t y_1[12];
	uint8_t y_2[12];
	char text[12][11];
} DS2KeySettingsPacket;
#pragma pack()
}
}
