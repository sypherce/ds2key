#ifndef __UDP_H__
#define __UDP_H__

#ifdef WIN32
#include <WS2tcpip.h>//socklen_t
#else//LINUX
#include <netdb.h>
#define SOCKET int
#endif//WIN32

#include <stdint.h>//uint16_t
#include <string>

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
					int Send(void *buf, unsigned int len);
					int RecvFrom(void *buf, unsigned int len);
					char *GetServerIP();
					char *GetClientIP();

				private:
					bool Block;
					bool Connected;
					uint16_t Port;
					SOCKET Sock;
					struct sockaddr_in ServerAddr;
					std::string ServerIP;
					struct sockaddr_in ClientAddr;
					std::string ClientIP;
			};
		}
		extern C::UDP *UDP;
	}
}
#endif//__UDP_H__
