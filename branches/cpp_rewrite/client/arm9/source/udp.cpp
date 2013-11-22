/*
	UDP Communication
*/

#include <nds/ndstypes.h>//dswifi9.h
#include <dswifi9.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>//stderr, fprintf
#include <sstream>//std::stringstream
#include <errno.h>

#include "udp.h"

#define NETerrno errno
#define NETEADDRINUSE EADDRINUSE
#define NETEWOULDBLOCK EWOULDBLOCK
#define NETclosesocket closesocket
#define NETioctlsocket ioctl
#define INVALID_SOCKET -1
typedef int socklen_t;

#pragma pack(1)
typedef struct DS2KeyPacket {
	uint8_t Type;
	uint8_t Profile;
	uint16_t Keys;
	uint16_t KeysTurbo;
	uint8_t GHKeys;
	uint16_t GHKeysTurbo;
	uint8_t TouchX;
	uint8_t TouchY;
} DS2KeyPacket;
#pragma pack()
DS2KeyPacket Packet = {0};
namespace D2K {
	namespace Core {
		namespace C {
			UDP::UDP() {
				Connected = false;
				Block = false;
			}
			UDP::~UDP() {
				Disconnect();//needs checked
			}

			///return (true) if connected, else (false)
			bool UDP::IsConnected() {
				return Connected;
			}

			int UDP::Connect() {
				return Connect(Block, Port);
			}

			int UDP::Connect(uint16_t port) {
				return Connect(Block, port);
			}

			///connect udp system
			///return (0) if connected, else (errno)
			int UDP::Connect(bool block, uint16_t port) {
				if(EMULATOR) return 0;					//skip if emulating
				if(IsConnected())						//if already connected
					Disconnect();						//disconnect first
				if(port == 0)							//if port 0
					SetPort(9501);						//use default port
				else
					SetPort(port);

				UDP::Block = block;
				RemoteAddr.sin_family = AF_INET;
				RemoteAddr.sin_port = htons(GetPort());

				//create a socket
				Sock = socket(PF_INET, SOCK_DGRAM, 0);
				if(Sock == INVALID_SOCKET) {
					int err = NETerrno;
					fprintf(stderr, "Error (socket): #%d\n", err);
					Disconnect();

					return err;
				}

				//set blocking mode
				if(NETioctlsocket(Sock, FIONBIO, (unsigned long*)&Block) == SOCKET_ERROR) {
					int err = NETerrno;
					fprintf(stderr, "Error (NETioctlsocket): #%d\n", err);
					Disconnect();

					return err;
				}
				Connected = true;

				return 0;
			}

			///disconnect udp system
			///return (0) without error, else (errno)
			int UDP::Disconnect() {
				if(EMULATOR) return 0;	//skip if emulating
				if(IsConnected()) {
					Connected = false;	//udp system disconnects even if socket doesn't close
					if(NETclosesocket(Sock) == SOCKET_ERROR) {
						int err = NETerrno;
						fprintf(stderr, "Error (NETclosesocket): #%d\n", err);

						return err;
					}
				}

				return 0;
			}

			///sends data
			///sends raw contents of (buf) up to (len) in size
			///return (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, else (errno)
			int UDP::Send(const void *buf, unsigned int len) {
				if(!IsConnected()) {
					fprintf(stderr, "Error (UDP::Recv): not connected\n");
					return -1;
				}
				else if(len <= 0) {
					fprintf(stderr, "Error (UDP::Recv) length: #%d\n", len);
					return -2;
				}
				else if(buf == NULL) {
					fprintf(stderr, "Error (UDP::Recv) invalid pointer\n");
					return -3;
				}
				else {//successful
					int sockaddrLen = sizeof(RemoteAddr);
					if(sendto(Sock, (char*)buf, len, 0, (struct sockaddr*)&RemoteAddr, sockaddrLen) == SOCKET_ERROR) {
						int err = NETerrno;
						fprintf(stderr, "Error (sendto): #%d\n", err);
						return err;
					}
				}

				return 0;
			}

			///receives data
			///receives raw contents into (buf) up to (len) in size
			///buf must be allocated before calling
		///note: (buf) is not cleared on DS but is on PC. there is probably a bug
			///return (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, else (errno)
			int UDP::Recv(void *buf, unsigned int len) {
				if(!IsConnected()) {
					fprintf(stderr, "Error (UDP::Recv): not connected\n");
					return -1;
				}
				else if(len <= 0) {
					fprintf(stderr, "Error (UDP::Recv) length: #%d\n", len);
					return -2;
				}
				else if(buf == NULL) {
					fprintf(stderr, "Error (UDP::Recv) invalid pointer\n");
					return -3;
				}
				else {//successful
					socklen_t sockaddrLen = sizeof(RemoteAddr);
					//memset(buf, 0, len);//this needed erased on the ds just to function.
					if(recvfrom(Sock, (char*)buf, len, 0, (struct sockaddr*)&RemoteAddr, &sockaddrLen) == SOCKET_ERROR) {
						int err = NETerrno;
						if(err != NETEWOULDBLOCK)
							fprintf(stderr, "Error (recvfrom): #%d\n", err);
						return err;
					}
				}

				return 0;
			}

			///sends a command packet
			void UDP::SendCommand(uint8_t command) {
				if(command > 11)										//valid range is 0 - 11
					return;

				memset(&Packet, 0, sizeof(DS2KeyPacket));				//clear the packet
				Packet.Type = '/' + 2;
				Packet.Profile = command;

				Send((char*)&Packet, sizeof(DS2KeyPacket));				//send packet
			}

			///updates current button & touchscreen status
			void UDP::Update(uint32_t keys, uint32_t keysTurbo, uint32_t gripKeys, uint32_t gripKeysTurbo, touchPosition *pos) {
				if(EMULATOR) return;									//skip if emulating
				memset(&Packet, 0, sizeof(DS2KeyPacket));				//clear the packet
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

			///searches for running servers on current (port)
		///note: this should actually just return any found ip and not change our currently connected IP
			void UDP::ServerLookup() {
				if(EMULATOR) return;									//skip if emulating
				unsigned long SavedRemoteIP = GetRemoteIP();			//save the remote IP
				unsigned long LocalIP = GetLocalIP();					//get the local IP
				SetRemoteIP(((LocalIP ) & 0xFF) |						//setup the broadcast IP
							(((LocalIP >> 8) & 0xFF) << 8 ) |
							(((LocalIP >> 16) & 0xFF) << 16) |
							(0xFF << 24));

				memset(&Packet, 0, sizeof(DS2KeyPacket));				//clear the packet
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

			//private
			///converts long into std::string
			template <class T>
			inline std::string itos (const T& t) {
				std::stringstream ss;
				ss << t;

				return ss.str();
			}

			///converts std::string into long
			template <typename T>
			long stoi(const std::basic_string<T> &str) {
				std::basic_stringstream<T> stream(str);
				long res;
				return !(stream >>res)?0:res;
			}
			//end


			unsigned long UDP::GetLocalIP() {
				return Wifi_GetIP();
			}
			std::string UDP::GetLocalIPString() {
				struct in_addr sin_addr;
				sin_addr.s_addr = GetLocalIP();
				std::string IP = inet_ntoa(sin_addr);
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

			uint8_t UDP::GetProfile() {
				return Profile;
			}
			std::string UDP::GetProfileString() {
				return itos((uint16_t)Profile);
			}

			void UDP::SetRemoteIP(const std::string& text) {
				inet_aton(text.c_str(), &RemoteAddr.sin_addr);
			}
			void UDP::SetRemoteIP(unsigned long ip) {
				RemoteAddr.sin_addr.s_addr = ip;
			}

			void UDP::SetPort(const std::string& port) {
				Port = stoi(port);
			}
			void UDP::SetPort(char *port) {
				Port = atoi(port);
			}
			void UDP::SetPort(unsigned int port) {
				Port = port;
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
		}
		C::UDP *UDP = (C::UDP*)NULL;
	}
}
