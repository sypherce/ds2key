/*
	UDP Communication
*/

#include <stdio.h>//stderr, fprintf
#ifndef WIN32
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <string.h>
#define closesocket close
#endif//WIN32
#include "udp.h"

namespace D2K
{
	namespace Core {
		UDP *udp = (UDP*)NULL;

		int UDP::Send(void *buf, unsigned int len)
		{
			if((buf != NULL) && (len > 0))
				return sendto(Sock, (char*)buf, len, 0, (sockaddr*)&ClientAddr, sizeof(ClientAddr));

			return -2;//(void *buf) is an invalid pointer or len == 0
		}

		int UDP::RecvFrom(void *buf, unsigned int len)
		{
			int sockaddrLen = sizeof(struct sockaddr_in);
			int returnVal;

			if((buf != NULL) && (len > 0))
			{
				memset(buf, 0, len);
		#ifdef WIN32
				returnVal = recvfrom(Sock, (char*)buf, len, 0, (LPSOCKADDR)&ClientAddr, &sockaddrLen);

				if(returnVal == -1)
				{
					int err = WSAGetLastError();
					if(err != WSAEWOULDBLOCK)
						fprintf(stderr, "socket failed with error %d\n", err);
				}
		#else//LINUX
				returnVal = recvfrom(Sock, (char*)buf, len, 0, (sockaddr*)&ClientAddr, (socklen_t*)&sockaddrLen);
		#endif//WIN32
			}
			else
				returnVal = -2;//(void *buf) is an invalid pointer or len == 0

			return returnVal;
		}

		char *UDP::GetServerIP()
		{
			ServerIP = inet_ntoa(ServerAddr.sin_addr);

			return (char*)ServerIP.c_str();
		}

		char *UDP::GetClientIP()
		{
			ClientIP = inet_ntoa(ClientAddr.sin_addr);

			return (char*)ClientIP.c_str();
		}

		UDP::UDP()
		{
		#ifdef WIN32
			WSADATA wsaData;
			WSAStartup(0x0202, &wsaData); //windows socket startup
		#endif//WIN32
			Connected = false;
			Block = false;
			Port = 9501;
		}

		bool UDP::Connect()
		{
			return Connect(Block, Port);
		}

		bool UDP::Connect(uint16_t port)
		{
			return Connect(Block, port);
		}

		bool UDP::Connect(bool block, uint16_t port)
		{
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
		#ifdef WIN32
			if(Sock == INVALID_SOCKET)
			{
				int err = WSAGetLastError();
				fprintf(stderr, "socket creation failed with error %d\n", err);

				return(err);
			}
		#else//LINUX
			if(Sock == -1)
			{
				int err = errno;
				fprintf(stderr, "socket creation failed with error %d\n", err);

				return(err);
			}
		#endif//WIN32
		{
			/*bool yes=1;
			//char yes='1'; // Solaris people use this

			// lose the pesky "Address already in use" error message
			if (setsockopt(Sock,SOL_SOCKET,SO_REUSEADDR,(char*)&yes,sizeof(bool)) == -1) {
			perror("setsockopt");
			exit(1);
			}*/
		}
		#ifdef WIN32
			if(bind(Sock, (LPSOCKADDR)&ServerAddr, sockaddrLen) == SOCKET_ERROR)
			{//bind a server address and port
				int err = WSAGetLastError();
				if(err == WSAEADDRINUSE)
					fprintf(stderr, "bind failed, port already in use\n");
				else
					fprintf(stderr, "bind failed with error %d\n", err);
				return (err);
			}
		#else//LINUX
			if(bind(Sock, (const sockaddr*)&ServerAddr, sockaddrLen) == -1)
			{
			    int err = errno;
			    if(err == EADDRINUSE)
					fprintf(stderr, "bind failed, port already in use\n");
				else
					fprintf(stderr, "bind failed with error %d\n", err);

				return (err);
			}
		#endif//WIN32

		#ifdef WIN32
			if(ioctlsocket(Sock, FIONBIO, (unsigned long*)&Block) == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				fprintf(stderr, "ioctlsocket failed with error %d\n", err);

				return(err);
			}
		#else//LINUX
			if(ioctl(Sock, FIONBIO, (unsigned long*)&Block) == -1)
			{
			    int err = errno;
                fprintf(stderr, "ioctl failed with error %d\n", err);

				return (err);
			}
		#endif//WIN32
			Connected = true;
			return 0;
		}

		bool UDP::Disconnect()
		{
			if(IsConnected())
			{
				closesocket(Sock);//needs checked
			}

			Connected = false;
			return 0;
		}

		bool UDP::IsConnected()
		{
			return Connected;
		}

		UDP::~UDP()
		{
			Disconnect();//needs checked
		#ifdef WIN32
			WSACleanup();
		#endif//WIN32
		}
	}
}
