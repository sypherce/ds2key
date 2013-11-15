#ifndef __UDP_H__
#define __UDP_H__

#ifdef _WIN32
#include <WS2tcpip.h>//socklen_t
#elif defined __linux__
#define SOCKET int
#include <netdb.h>
#endif//_WIN32

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

					char *GetLocalIP();
					char *GetRemoteIP();
					uint16_t GetPort();
					std::string GetPortString();

					void SetPort(const std::string& port);
					void SetPort(char *port);
					void SetPort(unsigned int port);
				private:
					bool Block;
					bool Connected;
					uint16_t Port;
					SOCKET Sock;
					struct sockaddr_in LocalAddr;
					struct sockaddr_in RemoteAddr;
			};
		}
		extern C::UDP *UDP;
	}
}
#endif//__UDP_H__
