#ifndef __DS2KEYPACKET_H__
#define __DS2KEYPACKET_H__

#include <stdint.h>//uint8_t, uint16_t

namespace D2K {
	namespace UDP {
		#pragma pack(1)
		typedef struct DS2KeyPacket {
			uint8_t Type;
			uint8_t Profile;
			uint16_t Keys;
			uint16_t KeysTurbo;		//Enabled Turbo keys
			uint8_t GHKeys;
			uint8_t GHKeysTurbo;	//Enabled GuitarGrip Turbo keys
			uint16_t TouchKeys;
			uint16_t TouchKeysTurbo;//Enabled Touch Turbo keys
			uint8_t TouchX;
			uint8_t TouchY;
		} DS2KeyPacket;


		//contains button layout and text for command buttons
		//there's a max of 12 buttons
		//text max is currently 10 + null terminator
		typedef struct DS2KeySettingsPacket {
			uint8_t Type;
			uint8_t Profile;
			uint8_t X1[12];
			uint8_t X2[12];
			uint8_t Y1[12];
			uint8_t Y2[12];
			char text[12][11];
		} DS2KeySettingsPacket;
		#pragma pack()
	}
}

#endif//__DS2KEYPACKET_H__
