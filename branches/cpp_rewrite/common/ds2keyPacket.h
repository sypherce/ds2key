#ifndef __DS2KEYPACKET_H__
#define __DS2KEYPACKET_H__

namespace D2K {
	namespace Core {
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
		#pragma pack()
	}
}

#endif//__DS2KEYPACKET_H__
