#ifndef __UDP_H__
#define __UDP_H__

#ifdef _WIN32
#include <WS2tcpip.h>//socklen_t
#elif defined __linux__
#include <netdb.h>
#elif defined ARM9
#include <nds.h>
#include <netinet/in.h>//sockaddr_in
#endif//_WIN32

#if defined(__linux__) || defined(ARM9)
#define SOCKET int
#endif//defined(__linux__) || defined(ARM9)

#include <stdint.h>//uint16_t
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

					unsigned long GetLocalIP();
					std::string GetLocalIPString();
					unsigned long GetRemoteIP();
					std::string GetRemoteIPString();
					uint16_t GetPort();
					std::string GetPortString();

					void SetRemoteIP(const std::string& text);
					void SetRemoteIP(unsigned long ip);

					void SetPort(const std::string& port);
					void SetPort(char *port);
					void SetPort(unsigned int port);
#ifdef D2KCLIENT
					void SendCommand(uint8_t command);
					void Update(uint32_t keys, uint32_t keysTurbo, uint32_t gripKeys, uint32_t gripKeysTurbo, touchPosition *pos);
					void ServerLookup();

					uint8_t GetProfile();
					std::string GetProfileString();

					void SetProfile(const std::string& profile);
					void SetProfile(char *profile);
					void SetProfile(unsigned int profile);
#endif//D2KCLIENT
				private:
					bool Block;
					bool Connected;
					uint16_t Port;
					SOCKET Sock;
#ifdef D2KCLIENT
					uint8_t Profile;
					std::string RemoteIP;
#else//D2KSERVER
					struct sockaddr_in LocalAddr;
#endif//D2KCLIENT
					struct sockaddr_in RemoteAddr;
			};
		}
		extern C::UDP *UDP;
	}
}
#endif//__UDP_H__
