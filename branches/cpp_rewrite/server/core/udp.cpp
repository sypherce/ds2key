/*
	UDP Communication
*/

#include <stdio.h>		//stderr, fprintf
#include <sstream>//std::stringstream

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
#include <string.h>		//memset
#define NETclosesocket close
#define NETioctlsocket ioctl
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define NETerrno errno
#define NETEADDRINUSE EADDRINUSE
#define NETEWOULDBLOCK EWOULDBLOCK
#endif//__linux__
#include "udp.h"

namespace D2K {
	namespace Core {
		namespace C {

			UDP::UDP() {
			#ifdef _WIN32
				WSADATA wsaData;
				WSAStartup(0x0202, &wsaData); //windows socket startup
			#endif//_WIN32
				Connected = false;
				Block = false;
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
				if(IsConnected())
					Disconnect();
				if(port != 0)
					SetPort(port);
				else
					SetPort(9501);

				UDP::Block = block;

				int sockaddrLen = sizeof(struct sockaddr_in);
				memset((char*)&LocalAddr, 0, sockaddrLen);
				LocalAddr.sin_family = AF_INET; //set local address protocol family
				LocalAddr.sin_addr.s_addr = INADDR_ANY;

				LocalAddr.sin_port = htons(GetPort());//set port

				Sock = socket(PF_INET, SOCK_DGRAM, 0);//create a socket
				if(Sock == INVALID_SOCKET) {
					int err = NETerrno;
					fprintf(stderr, "Error (socket): #%d\n", err);
					Disconnect();

					return err;
				}

				if(bind(Sock, (struct sockaddr*)&LocalAddr, sockaddrLen) == SOCKET_ERROR) {//bind a local address and port
					int err = NETerrno;
					if(err == NETEADDRINUSE)
						fprintf(stderr, "Error (bind)\nPort #%d already in use\n", GetPort());
					else
						fprintf(stderr, "Error (bind): #%d\n", err);
					Disconnect();
					return err;
				}

				if(NETioctlsocket(Sock, FIONBIO, (unsigned long*)&Block) == SOCKET_ERROR) {
					int err = NETerrno;
					fprintf(stderr, "Error (NETioctlsocket): #%d\n", err);
					Disconnect();
					return err;
				}
				Connected = true;

				return 0;
			}

			int UDP::Disconnect() {
				if(IsConnected()) {
					Connected = false;
					if(NETclosesocket(Sock) == SOCKET_ERROR) {
						int err = NETerrno;
						fprintf(stderr, "Error (NETclosesocket): #%d\n", err);

						return err;
					}
				}

				return 0;
			}

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
					memset(buf, 0, len);//this needed erased on the ds just to function.
					if(recvfrom(Sock, (char*)buf, len, 0, (struct sockaddr*)&RemoteAddr, &sockaddrLen) == SOCKET_ERROR) {
						int err = NETerrno;
						if(err != NETEWOULDBLOCK)
							fprintf(stderr, "Error (recvfrom): #%d\n", err);
						return err;
					}
				}

				return 0;
			}

			//private
			template <class T>
			inline std::string itos (const T& t) {
				std::stringstream ss;
				ss << t;

				return ss.str();
			}

			template <typename T>
			long stoi(const std::basic_string<T> &str) {
				std::basic_stringstream<T> stream(str);
				long res;
				return !(stream >>res)?0:res;
			}
			//end

			char *UDP::GetLocalIP() {
				std::string IP = inet_ntoa(LocalAddr.sin_addr);

				return (char*)IP.c_str();
			}

			char *UDP::GetRemoteIP() {
				std::string IP = inet_ntoa(RemoteAddr.sin_addr);

				return (char*)IP.c_str();
			}

			uint16_t UDP::GetPort() {
				return Port;
			}
			std::string UDP::GetPortString() {
				return itos(Port);
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
		}
		C::UDP *UDP = (C::UDP*)NULL;
	}
}
