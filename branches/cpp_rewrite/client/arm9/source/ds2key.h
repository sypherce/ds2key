#ifndef __D2K_H__
#define __D2K_H__

#include <nds.h>//types
#include <string>//std::string

namespace D2K {
	namespace DS2Key {
		unsigned long GetIP();
		std::string GetIPString();
		uint16 GetPort();
		std::string GetPortString();
		uint8 GetProfile();
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
		void Update(uint32 keys, uint32 gripKeys, touchPosition *pos);
		void ServerLookup();
	};
}

#endif//__D2K_H__
