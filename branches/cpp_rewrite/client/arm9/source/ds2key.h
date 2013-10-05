#ifndef __D2K_H__
#define __D2K_H__

#include <nds.h>
#include <string>//std::string

namespace D2K {
	namespace DS2Key {
		unsigned long GetIP();
		std::string GetIPString();
		uint16_t GetPort();
		std::string GetPortString();
		uint8_t GetProfile();
		std::string GetProfileString();

		void SetIP(const std::string& _ip);
		void SetPort(const std::string& _port);
		void SetPort(char *_port);
		void SetPort(unsigned int _port);
		void SetProfile(const std::string& _profile);
		void SetProfile(char *_profile);
		void SetProfile(unsigned int _profile);

		void Init();
		void DeInit();
		//void Update(uint32_t keys, uint32_t gripKeys, touchPosition *pos);
		void Update(uint32_t keys, uint32_t keysTurbo, uint32_t gripKeys, uint32_t gripKeysTurbo, touchPosition *pos);
		void ServerLookup();
	};
}

#endif//__D2K_H__
