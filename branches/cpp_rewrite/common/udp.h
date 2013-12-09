#ifndef __UDP_H__
#define __UDP_H__


#include <cstdint>//uint16_t
#include <string>//std::string

#ifdef _WIN32
#include <ws2tcpip.h>//socklen_t
#elif defined __linux__
#include <netdb.h>
#elif defined ARM9
#include <nds.h>
#include <netinet/in.h>//sockaddr_in
#define socklen_t int
#endif//_WIN32

#if defined(__linux__) || defined(ARM9)
typedef int SOCKET;
#endif//defined(__linux__) || defined(ARM9)

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
					//connect udp system
					//\return (0) if connected, else (errno)
					int Connect(bool block, uint16_t port);
					
					//disconnect udp system
					//@return (0) without error, else (errno)
					int Disconnect();

					//Sends contents of (buf) up to (len) in size
					//@param buf Pointer to data
					//@param len Length of data to send
					//@return (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, else (errno)
					int Send(const void *buf, unsigned int len);

					//Receives contents into (buf) up to (len) in size
					//@param buf Pointer to data.
					//(buf) must be allocated before calling.
					//(buf) is only modified up to (len) if recvfrom is successful, otherwise it should be left un modified.
					//@param len Length of data to receive
					//\return (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, else (errno)
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
					//sends a command packet
					void SendCommand(uint8_t command);

					//updates current button and touch screen status
					void Update(uint32_t keys, uint32_t keysTurbo, uint32_t gripKeys, uint32_t gripKeysTurbo, touchPosition *pos);
					
					//searches for running servers on current port
					//todo: this should actually just return any found ip and not change our currently connected IP
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
