#ifndef __UDP_H__
#define __UDP_H__

#ifdef WIN32
#include <winsock2.h>
#else//LINUX
#include <netdb.h>
#define SOCKET int
#endif//WIN32

#include <stdint.h>
#include <string>

namespace D2K {
	namespace Core {
		class UDP {
			public:
				UDP();
				~UDP();
				bool IsConnected();
				bool Connect();
				bool Connect(uint16_t port);
				bool Connect(bool block, uint16_t port);
				bool Disconnect();
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
}
#endif//__UDP_H__
