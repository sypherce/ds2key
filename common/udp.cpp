// UDP Communication

#if !defined(D2KCLIENT) && !defined(D2KSERVER)
#error "Must define D2KCLIENT or D2KSERVER"
#endif//D2KCLIENT || D2KSERVER

#include <cstdlib>//atoi
#include <iostream>//std::cout, std::clog
#include <sstream>//std::stringstream
#include <cstring>//strerror

//system specific includes
#ifdef _WIN32
#include <ws2tcpip.h>//socklength_t
#elif defined __linux__
#include <unistd.h>		//close
#include <arpa/inet.h>	//inet_ntoa
#include <sys/ioctl.h>	//ioctl
#elif defined ARM9
#include <nds/ndstypes.h>//dswifi9.h
#include <dswifi9.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#endif//_WIN32

//shared (linux, nds) defines
#if defined(__linux__) || defined(ARM9)
#define NETerrno errno
#define NETEADDRINUSE EADDRINUSE
#define NETEWOULDBLOCK EWOULDBLOCK
#define NETioctlsocket ioctl
#define INVALID_SOCKET -1
#endif

//system specific defines
#ifdef _WIN32
#define NETerrno WSAGetLastError()
#define NETEADDRINUSE WSAEADDRINUSE
#define NETEWOULDBLOCK WSAEWOULDBLOCK
#define NETclosesocket closesocket
#define NETioctlsocket ioctlsocket
#elif defined __linux__
#define NETclosesocket close
#define SOCKET_ERROR -1
#elif defined ARM9
typedef int socklen_t;
#define NETclosesocket closesocket
#endif//_WIN32

#include "udp.h"
#include "misc.h"

namespace D2K {namespace UDP {

bool g_non_blocking = true;//non-blocking == true
bool g_connected = false;
uint16_t g_port = DEFAULT_PORT;
SOCKET g_socket = INVALID_SOCKET;
#ifdef D2KCLIENT
DS2KeyPacket g_packet = DS2KeyPacket{ };
uint8_t g_profile = 0;
std::string g_remote_ip = "";
#else//D2KSERVER
struct sockaddr_in g_local_sockaddr = { };
#endif//D2KCLIENT
struct sockaddr_in g_remote_sockaddr = { };

void Init()
{
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0202, &wsa_data); //Startup with Winsock version 2.2
#endif//_WIN32
}
void DeInit()
{
	Disconnect();
#ifdef _WIN32
	WSACleanup();
#endif//_WIN32
}

bool IsConnected()
{
	return g_connected;
}

int Connect()
{
	return Connect(g_non_blocking, g_port);
}

int Connect(uint16_t port)
{
	return Connect(g_non_blocking, port);
}

int Connect(bool non_blocking, uint16_t port)
{
	if(EMULATOR)							//skip if emulating
		return 0;					
	if(IsConnected())						//if already g_connected
		Disconnect();						//disconnect first
	SetConfigPort(port);                          //set port

	UDP::g_non_blocking = non_blocking;
#ifdef D2KCLIENT
	g_remote_sockaddr.sin_family = AF_INET;
	g_remote_sockaddr.sin_port = htons(GetPort());
#elif defined D2KSERVER
	int sockaddrlength = sizeof(struct sockaddr_in);
	g_local_sockaddr = sockaddr_in{ };
	g_local_sockaddr.sin_family = AF_INET;
	g_local_sockaddr.sin_addr.s_addr = INADDR_ANY;

	g_local_sockaddr.sin_port = htons(GetPort());
#endif//D2KCLIENT
	g_socket = socket(PF_INET, SOCK_DGRAM, 0);	//create a socket
	if(g_socket == INVALID_SOCKET)
	{
		int err = NETerrno;
		std::clog << "Error (socket): " << strerror(err) << "\n";
		Disconnect();

		return err;
	}

#ifdef D2KSERVER
	//bind a local address and port
	if(bind(g_socket, (struct sockaddr*)&g_local_sockaddr, sockaddrlength) == SOCKET_ERROR)
	{
		int err = NETerrno;
		if(err == NETEADDRINUSE)
			std::clog << "Error (bind) g_port #" << GetPort() << " currently in use\n";
		else
			std::clog << "Error (bind): " << strerror(err) << "\n";
		Disconnect();
		return err;
	}
#endif//D2KSERVER

	//set blocking mode
	if(NETioctlsocket(g_socket, FIONBIO, (unsigned long*)&UDP::g_non_blocking) == SOCKET_ERROR)
	{
		int err = NETerrno;
		std::clog << "Error (NETioctlsocket): " << strerror(err) << "\n";
		Disconnect();

		return err;
	}
	g_connected = true;

	return 0;
}

int Disconnect()
{
	if(EMULATOR)				//skip if emulating
		return 0;
	if(IsConnected())
	{
		g_connected = false;	//udp system disconnects even if NETclosesocket returns error
		if(NETclosesocket(g_socket) == SOCKET_ERROR)
		{
			int err = NETerrno;
			std::clog << "Error (NETclosesocket): " << strerror(err) << "\n";

			return err;
		}
	}

	return 0;
}

int Send(const void* buffer, unsigned int length)
{
	if(!IsConnected())
	{
		std::clog << "Error (UDP::Send): Not g_connected\n";
		return -1;
	}
	else if(length <= 0)
	{
		std::clog << "Error (UDP::Send) length: #" << length << "\n";
		return -2;
	}
	else if(buffer == nullptr)
	{
		std::clog << "Error (UDP::Send) Invalid pointer\n";
		return -3;
	}
	else//successful
	{
		int sockaddrlength = sizeof(g_remote_sockaddr);
		if(sendto(g_socket, (char*)buffer, length, 0, (struct sockaddr*)&g_remote_sockaddr, sockaddrlength) == SOCKET_ERROR)
		{
			int err = NETerrno;
			std::clog << "Error (sendto): " << strerror(err) << "\n";
			return err;
		}
	}

	return 0;
}

int Recv(void* buffer, unsigned int length)
{
	if(!IsConnected())
	{
		std::clog << "Error (UDP::Recv): Not g_connected\n";
		return -1;
	}
	else if(length <= 0)
	{
		std::clog << "Error (UDP::Recv) length: #" << length << "\n";
		return -2;
	}
	else if(buffer == nullptr)
	{
		std::clog << "Error (UDP::Recv) Invalid pointer\n";
		return -3;
	}
	else//successful
	{
		socklen_t sockaddrlength = sizeof(g_remote_sockaddr);
		if(recvfrom(g_socket, (char*)buffer, length, 0, (struct sockaddr*)&g_remote_sockaddr, &sockaddrlength) == SOCKET_ERROR)
		{
			int err = NETerrno;
			if(err != NETEWOULDBLOCK)
				std::clog << "Error (recvfrom): " << strerror(err) << "\n";
			return err;
		}
	}

	return 0;
}

unsigned long GetLocalIP()
{
#ifdef ARM9
	return Wifi_GetIP();
#elif defined(_WIN32) || defined(__linux__)
	return g_local_sockaddr.sin_addr.s_addr;
#endif//ARM9
	return 0;
}
std::string GetLocalIPString()
{
#ifdef ARM9
	struct in_addr sin_addr;
	sin_addr.s_addr = GetLocalIP();
	std::string IP = inet_ntoa(sin_addr);
#elif defined(_WIN32) || defined(__linux__)
	std::string IP = inet_ntoa(g_local_sockaddr.sin_addr);
#endif//ARM9

	return IP;
}

unsigned long GetRemoteIP()
{
	return g_remote_sockaddr.sin_addr.s_addr;
}
std::string GetRemoteIPString()
{
	std::string IP = inet_ntoa(g_remote_sockaddr.sin_addr);
	return IP;
}

uint16_t GetPort()
{
	return g_port;
}
std::string GetPortString()
{
	return ltos(GetPort());
}

void SetRemoteIP(const std::string& text)
{
	g_remote_sockaddr.sin_addr.s_addr = inet_addr(text.c_str());
}
void SetRemoteIP(unsigned long ip)
{
	g_remote_sockaddr.sin_addr.s_addr = ip;
}

void SetConfigPort(const std::string& port)
{
	SetConfigPort(D2K::stol(port));
}
void SetConfigPort(char* port)
{
	SetConfigPort(atoi(port));
}
void SetConfigPort(unsigned int port)
{
	if(port == 0)						//if port 0
        g_port = DEFAULT_PORT;			//use default port 9501
	else
        g_port = port;
}

#ifdef D2KCLIENT
void SendCommand(uint8_t command)
{
	if(command >= UDP::SETTINGS_PACKET_MAX_BUTTONS)										//valid range is 0 - 11
		return;

	g_packet = DS2KeyPacket{ };											//clear the g_packet
	g_packet.type = UDP::PACKET::COMMAND;
	g_packet.profile = GetProfile();
	g_packet.keys = command;

	Send(&g_packet, sizeof(DS2KeyPacket));				//send g_packet
}

void Update(uint32_t keys, uint32_t keysTurbo, uint32_t gripKeys, uint32_t gripKeysTurbo, touchPosition* pos)
{
	if(EMULATOR) return;									//skip if emulating
	g_packet = DS2KeyPacket{ };								//clear the g_packet
	g_packet.type = UDP::PACKET::NORMAL;
	g_packet.profile = GetProfile();
	g_packet.keys = keys;
	g_packet.keys_turbo = keysTurbo;
	if(pos != nullptr)							//touch status active
	{
		g_packet.touch_x = pos->px;							//update x
		g_packet.touch_y = pos->py;							//update y
	}
	else													//touch status inactive
	{
		g_packet.keys &= ~KEY_TOUCH;						//clear touch status
	}
	g_packet.gh_keys = gripKeys;
	g_packet.gh_keys_turbo = gripKeysTurbo;

	Send(&g_packet, sizeof(DS2KeyPacket));					//send packet
}

void ServerLookup()
{
	if(EMULATOR) return;									//skip if emulating
	unsigned long saved_remote_ip = GetRemoteIP();			//save the remote IP
	unsigned long LocalIP = GetLocalIP();					//get the local IP
	SetRemoteIP(((LocalIP) & 0xFF) |						//setup the broadcast IP
				(((LocalIP >> 8) & 0xFF) << 8) |
				(((LocalIP >> 16) & 0xFF) << 16) |
				(0xFF << 24));

	g_packet = DS2KeyPacket{ };									//clear the packet
	g_packet.type = UDP::PACKET::LOOKUP;						//set as a lookup packet

	Send(&g_packet, sizeof(DS2KeyPacket));				//send the packet out

	if(Recv((char*)&g_packet, sizeof(DS2KeyPacket)) != 0)	//didn't receive anything
	{
		SetRemoteIP(saved_remote_ip);							//reset the remote IP
	}
	else if(g_packet.type == UDP::PACKET::LOOKUP)				//received a lookup packet
	{
		if(GetLocalIP() == GetRemoteIP())					//if it's from the local IP
			SetRemoteIP(saved_remote_ip);						//reset the remote IP
	}

}

uint8_t GetProfile()
{
	return g_profile;
}
std::string GetProfileString()
{
	return ltos(GetProfile());
}

void SetProfile(const std::string& g_profile)
{
	SetProfile(stol(g_profile));
}
void SetProfile(char* g_profile)
{
	SetProfile(atoi(g_profile));
}
void SetProfile(unsigned int g_profile)
{
	g_profile = g_profile;
}
DS2KeySettingsPacket GetCommandSettings()
{
	DS2KeySettingsPacket settings = DS2KeySettingsPacket{ };
	g_packet = DS2KeyPacket{ };									//clear the g_packet
	g_packet.type = UDP::PACKET::COMMAND_SETTINGS;
	g_packet.profile = GetProfile();
	Send(&g_packet, sizeof(DS2KeyPacket));				//send the g_packet out

	//try 3 times
	for(int i = 0; i < 3; i++)
	{
		if(Recv((char*)&settings, sizeof(DS2KeySettingsPacket)) == 0)		//received something
		{
			if(settings.type == UDP::PACKET::COMMAND_SETTINGS)			//received a lookup g_packet
			{
				return settings;
			}
			std::clog << "Error (GetCommandSettings): Received invalid g_packet\n";
		}
		swiWaitForVBlank();//wait a second before trying again
	}
	//if we didn't receive anything, or something invalid we return NULL_VALUE
	settings = DS2KeySettingsPacket{ };
	std::clog << "Error (GetCommandSettings): Failed to receive g_packet\n";
	return settings;
}
#else//D2KSERVER
void SendCommandSettings(DS2KeySettingsPacket settings)
{
	Send(&settings, sizeof(DS2KeySettingsPacket));	//send g_packet
}
#endif//D2KCLIENT

}}//namespace D2K::UDP
