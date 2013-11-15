#ifndef __UDP_H__
#define __UDP_H__

#define SOCKET int

#include <nds.h>
#include <netinet/in.h>//sockaddr_in
#include <string>//std::string

namespace D2K {
	namespace Core {
		namespace C {
			class UDP {
				public:
					UDP();
					~UDP();
					bool IsConnected();
					int Connect();
					int Connect(uint16_t port);
					int Connect(bool block, uint16_t port);
					int Disconnect();
					int Send(const void *buf, unsigned int len);
					int Recv(void *buf, unsigned int len);

					void SendCommand(uint8_t command);
					void Update(uint32_t keys, uint32_t keysTurbo, uint32_t gripKeys, uint32_t gripKeysTurbo, touchPosition *pos);
					void ServerLookup();

					unsigned long GetLocalIP();
					std::string GetLocalIPString();
					unsigned long GetRemoteIP();
					std::string GetRemoteIPString();
					uint16_t GetPort();
					std::string GetPortString();
					uint8_t GetProfile();
					std::string GetProfileString();

					void SetRemoteIP(const std::string& text);
					void SetRemoteIP(unsigned long ip);
					void SetPort(const std::string& port);
					void SetPort(char *port);
					void SetPort(unsigned int port);
					void SetProfile(const std::string& profile);
					void SetProfile(char *profile);
					void SetProfile(unsigned int profile);

				private:
					bool Block;
					bool Connected;
					uint16_t Port;
					SOCKET Sock;
					uint8_t Profile;
					struct sockaddr_in RemoteAddr;
					std::string RemoteIP;
			};
		}
		extern C::UDP *UDP;
	}
}
#endif//__UDP_H__
