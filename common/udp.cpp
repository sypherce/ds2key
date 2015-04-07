/*
	UDP Communication
*/

#if !defined(D2KCLIENT) && !defined(D2KSERVER)
#error "Must define D2KCLIENT or D2KSERVER"
#endif//D2KCLIENT || D2KSERVER

#include <cstdlib>//atoi
#include <iostream>//std::cout, std::clog
#include <sstream>//std::stringstream
#include <cstring>//strerror

#ifdef _WIN32
#include <ws2tcpip.h>//socklen_t
#define NETerrno WSAGetLastError()
#define NETEADDRINUSE WSAEADDRINUSE
#define NETEWOULDBLOCK WSAEWOULDBLOCK
#define NETclosesocket closesocket
#define NETioctlsocket ioctlsocket
#elif defined __linux__
#include <unistd.h>		//close
#include <arpa/inet.h>	//inet_ntoa
#include <sys/ioctl.h>	//ioctl
#define NETerrno errno
#define NETEADDRINUSE EADDRINUSE
#define NETEWOULDBLOCK EWOULDBLOCK
#define NETclosesocket close
#define NETioctlsocket ioctl
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#elif defined ARM9
#include <nds/ndstypes.h>//dswifi9.h
#include <dswifi9.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#define socklen_t int
#define NETerrno errno
#define NETEADDRINUSE EADDRINUSE
#define NETEWOULDBLOCK EWOULDBLOCK
#define NETclosesocket closesocket
#define NETioctlsocket ioctl
#define INVALID_SOCKET -1
#elif defined PSP_DEF
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>
//#include <pspnet_resolver.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/fd_set.h>
//#define socklen_t int
#define NETerrno sceNetInetGetErrno()
#define NETEADDRINUSE EADDRINUSE
#define NETEWOULDBLOCK EWOULDBLOCK
#define NETclosesocket close
#define NETioctlsocket ioctl
#define SO_NONBLOCK 0x1009
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif//_WIN32
#include "udp.h"
#include "misc.h"
namespace D2K {
	namespace UDP {
		bool Block = true;//non-blocking == true
		bool Connected = false;
		uint16_t Port = DefaultPort;
		SOCKET Sock = INVALID_SOCKET;
#ifdef D2KCLIENT
		DS2KeyPacket Packet = {0};
		uint8_t Profile = 0;
		std::string RemoteIP = "";
#else//D2KSERVER
		struct sockaddr_in LocalAddr = {};
#endif//D2KCLIENT
		struct sockaddr_in RemoteAddr = {};

		void Init() {
		#ifdef _WIN32
			WSADATA wsaData;
			WSAStartup(0x0202, &wsaData); //Startup with Winsock version 2.2
		#endif//_WIN32
		}
		void DeInit() {
			Disconnect();
		#ifdef _WIN32
			WSACleanup();
		#endif//_WIN32
		}

		bool IsConnected() {
			return Connected;
		}

		int Connect() {
			return Connect(Block, Port);
		}

		int Connect(uint16_t port) {
			return Connect(Block, port);
		}

		int Connect(bool block, uint16_t port) {
			if(EMULATOR) return 0;					//skip if emulating
			if(IsConnected())						//if already connected
				Disconnect();						//disconnect first
			SetPort(port);                          //set port

			UDP::Block = block;
#ifdef D2KCLIENT
			RemoteAddr.sin_family = AF_INET;
			RemoteAddr.sin_port = htons(GetPort());
#elif defined D2KSERVER
			int sockaddrLen = sizeof(struct sockaddr_in);
			LocalAddr = sockaddr_in{0};
			LocalAddr.sin_family = AF_INET;
			LocalAddr.sin_addr.s_addr = INADDR_ANY;

			LocalAddr.sin_port = htons(GetPort());
#endif//D2KCLIENT
			Sock = socket(PF_INET, SOCK_DGRAM, 0);	//create a socket
			if(Sock == INVALID_SOCKET) {
				int err = NETerrno;
				std::clog << "Error (socket): " << strerror(err) << "\n";
				Disconnect();

				return err;
			}

#ifdef D2KSERVER
			//bind a local address and port
			if(bind(Sock, (struct sockaddr*)&LocalAddr, sockaddrLen) == SOCKET_ERROR) {
				int err = NETerrno;
				if(err == NETEADDRINUSE)
					std::clog << "Error (bind) Port #" << GetPort() << " currently in use\n";
				else
					std::clog << "Error (bind): " << strerror(err) << "\n";
				Disconnect();
				return err;
			}
#endif//D2KSERVER

			//set blocking mode
#ifdef PSP_DEF
			if(setsockopt(Sock, SOL_SOCKET, SO_NONBLOCK, &UDP::Block, sizeof(UDP::Block)) == SOCKET_ERROR) {
#else
			if(NETioctlsocket(Sock, FIONBIO, (unsigned long*)&UDP::Block) == SOCKET_ERROR) {
#endif
				int err = NETerrno;
				std::clog << "Error (NETioctlsocket): " << strerror(err) << "\n";
				Disconnect();

				return err;
			}
			Connected = true;

			return 0;
		}

		int Disconnect() {
			if(EMULATOR) return 0;	//skip if emulating
			if(IsConnected()) {
				Connected = false;	//udp system disconnects even if NETclosesocket returns error
				if(NETclosesocket(Sock) == SOCKET_ERROR) {
					int err = NETerrno;
					std::clog << "Error (NETclosesocket): " << strerror(err) << "\n";

					return err;
				}
			}

			return 0;
		}

		int Send(const void *buf, unsigned int len) {
			if(!IsConnected()) {
				std::clog << "Error (UDP::Send): Not connected\n";
				return -1;
			}
			else if(len <= 0) {
				std::clog << "Error (UDP::Send) Length: #" << len << "\n";
				return -2;
			}
			else if(buf == NULL) {
				std::clog << "Error (UDP::Send) Invalid pointer\n";
				return -3;
			}
			else {//successful
				int sockaddrLen = sizeof(RemoteAddr);
				if(sendto(Sock, (char*)buf, len, 0, (struct sockaddr*)&RemoteAddr, sockaddrLen) == SOCKET_ERROR) {
					int err = NETerrno;
					std::clog << "Error (sendto): " << strerror(err) << "\n";
					return err;
				}
			}

			return 0;
		}

		int Recv(void *buf, unsigned int len) {
			if(!IsConnected()) {
				std::clog << "Error (UDP::Recv): Not connected\n";
				return -1;
			}
			else if(len <= 0) {
				std::clog << "Error (UDP::Recv) Length: #" << len << "\n";
				return -2;
			}
			else if(buf == NULL) {
				std::clog << "Error (UDP::Recv) Invalid pointer\n";
				return -3;
			}
			else {//successful
				socklen_t sockaddrLen = sizeof(RemoteAddr);
				if(recvfrom(Sock, (char*)buf, len, 0, (struct sockaddr*)&RemoteAddr, &sockaddrLen) == SOCKET_ERROR) {
					int err = NETerrno;
					if(err != NETEWOULDBLOCK)
						std::clog << "Error (recvfrom): " << strerror(err) << "\n";
					return err;
				}
			}

			return 0;
		}

		unsigned long GetLocalIP() {
#ifdef ARM9
			return Wifi_GetIP();
#elif defined(_WIN32) || defined(__linux__)
			return LocalAddr.sin_addr.s_addr;
#endif//ARM9
			return 0;
		}
		std::string GetLocalIPString() {
#if defined(ARM9) || defined(PSP_DEF)
			struct in_addr sin_addr;
			sin_addr.s_addr = GetLocalIP();
			std::string IP = inet_ntoa(sin_addr);
#elif defined(_WIN32) || defined(__linux__)
			std::string IP = inet_ntoa(LocalAddr.sin_addr);
#endif//ARM9

			return IP;
		}

		unsigned long GetRemoteIP() {
			return RemoteAddr.sin_addr.s_addr;
		}
		std::string GetRemoteIPString() {
			std::string IP = inet_ntoa(RemoteAddr.sin_addr);
			return IP;
		}

		uint16_t GetPort() {
			return Port;
		}
		std::string GetPortString() {
			return itos(GetPort());
		}

		void SetRemoteIP(const std::string& text) {
			RemoteAddr.sin_addr.s_addr = inet_addr(text.c_str());
		}
		void SetRemoteIP(unsigned long ip) {
			RemoteAddr.sin_addr.s_addr = ip;
		}

		void SetPort(const std::string& port) {
			SetPort(D2K::stoi(port));
		}
		void SetPort(char *port) {
			SetPort(atoi(port));
		}
		void SetPort(unsigned int port) {
			if(port == 0)					//if port 0
                Port = DefaultPort;         //use default port 9501
			else
                Port = port;
		}

#ifdef D2KCLIENT
		void SendCommand(uint8_t command) {
			if(command > 11)										//valid range is 0 - 11
				return;

			Packet = (DS2KeyPacket){0};								//clear the packet
			Packet.Type = UDP_PACKET_COMMAND;
			Packet.Profile = GetProfile();
			Packet.Keys = command;

			Send((char*)&Packet, sizeof(DS2KeyPacket));				//send packet
		}

		void Update(uint32_t keys, uint32_t keysTurbo, uint32_t gripKeys, uint32_t gripKeysTurbo, touchPosition *pos) {
			if(EMULATOR) return;									//skip if emulating
			Packet = (DS2KeyPacket){0};								//clear the packet
			Packet.Type = UDP_PACKET_NORMAL;
			Packet.Profile = GetProfile();
			Packet.Keys = keys;
			Packet.KeysTurbo = keysTurbo;
			if(pos != (touchPosition*)NULL) {						//touch status active
				Packet.TouchX = pos->px;							//update x
				Packet.TouchY = pos->py;							//update y
			}
			else {													//touch status inactive
				Packet.Keys &= ~KEY_TOUCH;							//clear touch status
			}
			Packet.GHKeys = gripKeys;
			Packet.GHKeysTurbo = gripKeysTurbo;

			Send((char*)&Packet, sizeof(DS2KeyPacket));				//send packet
		}

		void ServerLookup() {
			if(EMULATOR) return;									//skip if emulating
			unsigned long SavedRemoteIP = GetRemoteIP();			//save the remote IP
			unsigned long LocalIP = GetLocalIP();					//get the local IP
			SetRemoteIP(((LocalIP) & 0xFF) |						//setup the broadcast IP
						(((LocalIP >> 8) & 0xFF) << 8) |
						(((LocalIP >> 16) & 0xFF) << 16) |
						(0xFF << 24));

			Packet = (DS2KeyPacket){0};								//clear the packet
			Packet.Type = UDP_PACKET_LOOKUP;						//set as a lookup packet

			Send((char*)&Packet, sizeof(DS2KeyPacket));				//send the packet out

			if(Recv((char*)&Packet, sizeof(DS2KeyPacket)) != 0) {	//didn't receive anything
				SetRemoteIP(SavedRemoteIP);							//reset the remote IP
			}
			else if(Packet.Type == UDP_PACKET_LOOKUP) {				//received a lookup packet
				if(GetLocalIP() == GetRemoteIP())					//if it's from the local IP
					SetRemoteIP(SavedRemoteIP);						//reset the remote IP
			}

		}

		uint8_t GetProfile() {
			return Profile;
		}
		std::string GetProfileString() {
			return itos(GetProfile());
		}

		void SetProfile(const std::string& profile) {
			SetProfile(stoi(profile));
		}
		void SetProfile(char *profile) {
			SetProfile(atoi(profile));
		}
		void SetProfile(unsigned int profile) {
			Profile = profile;
		}
		DS2KeySettingsPacket GetCommandSettings() {
			DS2KeySettingsPacket settings = (DS2KeySettingsPacket){0};
			Packet = (DS2KeyPacket){0};								//clear the packet
			Packet.Type = UDP_PACKET_COMMAND_SETTINGS;
			Packet.Profile = GetProfile();
			Send((char*)&Packet, sizeof(DS2KeyPacket));				//send the packet out

			//try 3 times
			for(int i = 0; i < 3; i++) {
				if(Recv((char*)&settings, sizeof(DS2KeySettingsPacket)) == 0) {	//received something
					if(settings.Type == UDP_PACKET_COMMAND_SETTINGS) {			//received a lookup packet
						return settings;
					}
					std::clog << "Error (GetCommandSettings): Received invalid packet\n";
				}
				swiWaitForVBlank();//wait a second before trying again
			}
			//if we didn't receive anything, or something invalid we return NULL
			settings = (DS2KeySettingsPacket){0};
			std::clog << "Error (GetCommandSettings): Failed to receive packet\n";
			return settings;
		}
#else//D2KSERVER
		void SendCommandSettings(DS2KeySettingsPacket settings) {
			Send((char*)&settings, sizeof(DS2KeySettingsPacket));	//send packet
		}
#endif//D2KCLIENT
	}
}
