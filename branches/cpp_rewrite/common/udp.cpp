/*
	UDP Communication
*/

#include <cstdlib>//atoi
#include <iostream>//std::cout, std::clog
#include <sstream>//std::stringstream

#if !defined(D2KCLIENT) && !defined(D2KSERVER)
#error "Must define D2KCLIENT or D2KSERVER"
#endif//D2KCLIENT || D2KSERVER

#ifdef _WIN32
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
#define NETerrno errno
#define NETEADDRINUSE EADDRINUSE
#define NETEWOULDBLOCK EWOULDBLOCK
#define NETclosesocket closesocket
#define NETioctlsocket ioctl
#define INVALID_SOCKET -1
#endif//_WIN32
#include "udp.h"
#include "misc.h"

#ifdef D2KCLIENT
#include "common/ds2keyPacket.h"
#endif//D2KCLIENT
namespace D2K {
	namespace Core {
		#ifdef D2KCLIENT
		DS2KeyPacket Packet = {0};
		#endif//D2KCLIENT
		namespace C {
			UDP::UDP() {
			#ifdef _WIN32
				WSADATA wsaData;
				WSAStartup(0x0202, &wsaData); //windows socket startup
			#endif//_WIN32
				Connected = false;
				Block = false;
				Sock = INVALID_SOCKET;
				Port = 0;
			}
			UDP::~UDP() {
				Disconnect();//needs checked
			#ifdef _WIN32
				WSACleanup();
			#endif//_WIN32
			}

			bool UDP::IsConnected() {
				return Connected;
			}

			int UDP::Connect() {
				return Connect(Block, Port);
			}

			int UDP::Connect(uint16_t port) {
				return Connect(Block, port);
			}

			int UDP::Connect(bool block, uint16_t port) {
				if(EMULATOR) return 0;					//skip if emulating
				if(IsConnected())						//if already connected
					Disconnect();						//disconnect first
				if(port == 0)							//if port 0
					SetPort(DefaultPort);				//use default port
				else
					SetPort(port);

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
				//create a socket
				Sock = socket(PF_INET, SOCK_DGRAM, 0);
				if(Sock == INVALID_SOCKET) {
					int err = NETerrno;
					std::clog << "Error (socket): " << strerror(err) << "\n";
					Disconnect();

					return err;
				}

#ifdef D2KSERVER
				if(bind(Sock, (struct sockaddr*)&LocalAddr, sockaddrLen) == SOCKET_ERROR) {//bind a local address and port
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
				if(NETioctlsocket(Sock, FIONBIO, (unsigned long*)&Block) == SOCKET_ERROR) {
					int err = NETerrno;
					std::clog << "Error (NETioctlsocket): " << strerror(err) << "\n";
					Disconnect();

					return err;
				}
				Connected = true;

				return 0;
			}

			int UDP::Disconnect() {
				if(EMULATOR) return 0;	//skip if emulating
				if(IsConnected()) {
					Connected = false;	//udp system disconnects even if socket doesn't close
					if(NETclosesocket(Sock) == SOCKET_ERROR) {
						int err = NETerrno;
						std::clog << "Error (NETclosesocket):" << strerror(err) << "\n";

						return err;
					}
				}

				return 0;
			}

			int UDP::Send(const void *buf, unsigned int len) {
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

			int UDP::Recv(void *buf, unsigned int len) {
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

			unsigned long UDP::GetLocalIP() {
#ifdef ARM9
				return Wifi_GetIP();
#elif defined(_WIN32) || defined(__linux__)
				return LocalAddr.sin_addr.s_addr;
#endif//ARM9
			}
			std::string UDP::GetLocalIPString() {
#ifdef ARM9
				struct in_addr sin_addr;
				sin_addr.s_addr = GetLocalIP();
				std::string IP = inet_ntoa(sin_addr);
#elif defined(_WIN32) || defined(__linux__)
				std::string IP = inet_ntoa(LocalAddr.sin_addr);
#endif//ARM9

				return IP;
			}

			unsigned long UDP::GetRemoteIP() {
				return RemoteAddr.sin_addr.s_addr;
			}
			std::string UDP::GetRemoteIPString() {
				std::string IP = inet_ntoa(RemoteAddr.sin_addr);
				return IP;
			}

			uint16_t UDP::GetPort() {
				return Port;
			}
			std::string UDP::GetPortString() {
				return itos(Port);
			}

			void UDP::SetRemoteIP(const std::string& text) {
				RemoteAddr.sin_addr.s_addr = inet_addr(text.c_str());
			}
			void UDP::SetRemoteIP(unsigned long ip) {
				RemoteAddr.sin_addr.s_addr = ip;
			}

			void UDP::SetPort(const std::string& port) {
				Port = Core::stoi(port);
			}
			void UDP::SetPort(char *port) {
				Port = atoi(port);
			}
			void UDP::SetPort(unsigned int port) {
				Port = port;
			}

#ifdef D2KCLIENT
			void UDP::SendCommand(uint8_t command) {
				if(command > 11)										//valid range is 0 - 11
					return;

				Packet = (DS2KeyPacket){0};								//clear the packet
				Packet.Type = '/' + 2;
				Packet.Profile = GetProfile();
				Packet.Keys = command;

				Send((char*)&Packet, sizeof(DS2KeyPacket));				//send packet
			}

			void UDP::Update(uint32_t keys, uint32_t keysTurbo, uint32_t gripKeys, uint32_t gripKeysTurbo, touchPosition *pos) {
				if(EMULATOR) return;									//skip if emulating
				Packet = (DS2KeyPacket){0};								//clear the packet
				Packet.Type = '/' + 1;
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

			void UDP::ServerLookup() {
				if(EMULATOR) return;									//skip if emulating
				unsigned long SavedRemoteIP = GetRemoteIP();			//save the remote IP
				unsigned long LocalIP = GetLocalIP();					//get the local IP
				SetRemoteIP(((LocalIP ) & 0xFF) |						//setup the broadcast IP
							(((LocalIP >> 8) & 0xFF) << 8 ) |
							(((LocalIP >> 16) & 0xFF) << 16) |
							(0xFF << 24));

				Packet = (DS2KeyPacket){0};								//clear the packet
				Packet.Type = 0xFF;										//set as a lookup packet

				Send((char*)&Packet, sizeof(DS2KeyPacket));				//send the packet out

				if(Recv((char*)&Packet, sizeof(DS2KeyPacket)) != 0) {	//didn't receive anything
					SetRemoteIP(SavedRemoteIP);							//reset the remote IP
				}
				else if(Packet.Type == 0xFF) {							//received a lookup packet
					if(GetLocalIP() == GetRemoteIP())					//if it's from the local IP
						SetRemoteIP(SavedRemoteIP);						//reset the remote IP
				}

			}

			uint8_t UDP::GetProfile() {
				return Profile;
			}
			std::string UDP::GetProfileString() {
				return itos((uint16_t)Profile);
			}

			void UDP::SetProfile(const std::string& profile) {
				Profile = stoi(profile);
			}
			void UDP::SetProfile(char *profile) {
				Profile = atoi(profile);
			}
			void UDP::SetProfile(unsigned int profile) {
				Profile = profile;
			}
#endif//D2KCLIENT
		}
		C::UDP *UDP = (C::UDP*)NULL;
	}
}
