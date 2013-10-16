/*
	UDP Communication
*/

#include <stdio.h>//stderr, fprintf
#ifndef WIN32
#include <unistd.h>		//close
#include <arpa/inet.h>	//inet_ntoa
#include <sys/ioctl.h>	//ioctl
#include <string.h>	//memset
#define closesocket close
#define INVALID_SOCKET -1
#define SOCKET_ERRORSOCKET_ERROR -1
#define WSAGetLastError() errno
#define LPSOCKADDR struct sockaddr*
#define WSAEADDRINUSE EADDRINUSE
#define WSAEWOULDBLOCK EWOULDBLOCK
#define ioctlsocket ioctl
#endif//WIN32
#include "udp.h"

namespace D2K
{
	namespace Core {
		C::UDP *UDP = (C::UDP*)NULL;
		namespace C {
			int UDP::Send(void *buf, unsigned int len) {
				if((buf != NULL) && (len > 0))
					return sendto(Sock, (char*)buf, len, 0, (sockaddr*)&ClientAddr, sizeof(ClientAddr));

				return -2;//(void *buf) is an invalid pointer or len == 0
			}

			int UDP::RecvFrom(void *buf, unsigned int len) {
				int sockaddrLen = sizeof(struct sockaddr_in);
				int returnVal;

				if((buf != NULL) && (len > 0)) {
					memset(buf, 0, len);
					returnVal = recvfrom(Sock, (char*)buf, len, 0, (LPSOCKADDR)&ClientAddr, (socklen_t*)&sockaddrLen);

					if(returnVal == SOCKET_ERROR) {
						int err = WSAGetLastError();
						if(err != WSAEWOULDBLOCK)
							fprintf(stderr, "socket failed with error %d\n", err);
					}
				}
				else
					returnVal = -2;//(void *buf) is an invalid pointer or len == 0

				return returnVal;
			}

			char *UDP::GetServerIP() {
				ServerIP = inet_ntoa(ServerAddr.sin_addr);

				return (char*)ServerIP.c_str();
			}

			char *UDP::GetClientIP() {
				ClientIP = inet_ntoa(ClientAddr.sin_addr);

				return (char*)ClientIP.c_str();
			}

			UDP::UDP() {
			#ifdef WIN32
				WSADATA wsaData;
				WSAStartup(0x0202, &wsaData); //windows socket startup
			#endif//WIN32
				Connected = false;
				Block = false;
				Port = 9501;
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
				int sockaddrLen = sizeof(struct sockaddr_in);
				if(port != 0)
					UDP::Port = port;
				else
					UDP::Port = 9501;

				memset((char*)&ServerAddr, 0, sockaddrLen);
				ServerAddr.sin_family = AF_INET; //set server address protocol family
				ServerAddr.sin_addr.s_addr = INADDR_ANY;

				ServerAddr.sin_port = htons((u_short)UDP::Port);//set port

				Sock = socket(PF_INET, SOCK_DGRAM, 0);//create a socket
				if(Sock == INVALID_SOCKET) {
					int err = WSAGetLastError();
					fprintf(stderr, "socket creation failed with error %d\n", err);

					return err;
				}

				if(bind(Sock, (LPSOCKADDR)&ServerAddr, sockaddrLen) == SOCKET_ERROR) {//bind a server address and port
					int err = WSAGetLastError();
					if(err == WSAEADDRINUSE)
						fprintf(stderr, "bind failed, port already in use\n");
					else
						fprintf(stderr, "bind failed with error %d\n", err);
					return err;
				}

				if(ioctlsocket(Sock, FIONBIO, (unsigned long*)&Block) == SOCKET_ERROR) {
					int err = WSAGetLastError();
					fprintf(stderr, "ioctlsocket failed with error %d\n", err);

					return err;
				}
				Connected = true;

				return 0;
			}

			int UDP::Disconnect() {
				Connected = false;
				if(IsConnected())
				{
					if(closesocket(Sock) == SOCKET_ERROR)//needs checked
					{
						int err = WSAGetLastError();
						fprintf(stderr, "closesocket failed with error %d\n", err);

						return err;
					}
				}

				return 0;
			}

			bool UDP::IsConnected() {
				return Connected;
			}

			UDP::~UDP() {
				Disconnect();//needs checked
			#ifdef WIN32
				WSACleanup();
			#endif//WIN32
			}
		}
	}
}
