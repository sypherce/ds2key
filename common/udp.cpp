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

bool non_blocking = true;//non-blocking == true
bool connected = false;
uint16_t port = DEFAULT_PORT;
SOCKET socket_id = INVALID_SOCKET;
#ifdef D2KCLIENT
DS2KeyPacket packet = DS2KeyPacket{ };
uint8_t profile = 0;
std::string remote_ip = "";
#else//D2KSERVER
struct sockaddr_in local_sockaddr = { };
#endif//D2KCLIENT
struct sockaddr_in remote_sockaddr = { };

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
	return connected;
}

int Connect()
{
	return Connect(non_blocking, port);
}

int Connect(uint16_t port)
{
	return Connect(non_blocking, port);
}

int Connect(bool non_blocking, uint16_t port)
{
	if(EMULATOR)					//skip if emulating
		return 0;					
	if(IsConnected())				//if already connected
		Disconnect();				//disconnect first
	SetConfigPort(port);				//set port

	UDP::non_blocking = non_blocking;
#ifdef D2KCLIENT
	remote_sockaddr.sin_family = AF_INET;
	remote_sockaddr.sin_port = htons(GetPort());
#elif defined D2KSERVER
	int sockaddrlength = sizeof(struct sockaddr_in);
	local_sockaddr = sockaddr_in{ };
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;

	local_sockaddr.sin_port = htons(GetPort());
#endif//D2KCLIENT
	socket_id = socket(PF_INET, SOCK_DGRAM, 0);	//create a socket
	if(socket_id == INVALID_SOCKET)
	{
		int err = NETerrno;
		std::clog << "Error (socket): " << strerror(err) << "\n";
		Disconnect();

		return err;
	}

#ifdef D2KSERVER
	//bind a local address and port
	if(bind(socket_id, (struct sockaddr*)&local_sockaddr, sockaddrlength) == SOCKET_ERROR)
	{
		int err = NETerrno;
		if(err == NETEADDRINUSE)
			std::clog << "Error (bind) port #" << GetPort() << " currently in use\n";
		else
			std::clog << "Error (bind): " << strerror(err) << "\n";
		Disconnect();
		return err;
	}
#endif//D2KSERVER

	//set blocking mode
	if(NETioctlsocket(socket_id, FIONBIO, (unsigned long*)&UDP::non_blocking) == SOCKET_ERROR)
	{
		int err = NETerrno;
		std::clog << "Error (NETioctlsocket): " << strerror(err) << "\n";
		Disconnect();

		return err;
	}
	connected = true;

	return 0;
}

int Disconnect()
{
	if(EMULATOR)				//skip if emulating
		return 0;
	if(IsConnected())
	{
		connected = false;	//udp system disconnects even if NETclosesocket returns error
		if(NETclosesocket(socket_id) == SOCKET_ERROR)
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
		std::clog << "Error (UDP::Send): Not connected\n";
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
		int sockaddrlength = sizeof(remote_sockaddr);
		if(sendto(socket_id, (char*)buffer, length, 0, (struct sockaddr*)&remote_sockaddr, sockaddrlength) == SOCKET_ERROR)
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
		std::clog << "Error (UDP::Recv): Not connected\n";
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
		socklen_t sockaddrlength = sizeof(remote_sockaddr);
		if(recvfrom(socket_id, (char*)buffer, length, 0, (struct sockaddr*)&remote_sockaddr, &sockaddrlength) == SOCKET_ERROR)
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
	return local_sockaddr.sin_addr.s_addr;
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
	std::string IP = inet_ntoa(local_sockaddr.sin_addr);
#endif//ARM9

	return IP;
}

unsigned long GetRemoteIP()
{
	return remote_sockaddr.sin_addr.s_addr;
}
std::string GetRemoteIPString()
{
	std::string IP = inet_ntoa(remote_sockaddr.sin_addr);
	return IP;
}

uint16_t GetPort()
{
	return port;
}
std::string GetPortString()
{
	return ltos(GetPort());
}

void SetRemoteIP(const std::string& text)
{
	remote_sockaddr.sin_addr.s_addr = inet_addr(text.c_str());
}
void SetRemoteIP(unsigned long ip)
{
	remote_sockaddr.sin_addr.s_addr = ip;
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
		UDP::port = DEFAULT_PORT;			//use default port 9501
	else
		UDP::port = port;
}

#ifdef D2KCLIENT
void SendCommand(uint8_t command)
{
	if(command >= UDP::SETTINGS_PACKET_MAX_BUTTONS)										//valid range is 0 - 11
		return;

	packet = DS2KeyPacket{ };			//clear the packet
	packet.type = UDP::PACKET::COMMAND;
	packet.profile = GetProfile();
	packet.keys = command;

	Send(&packet, sizeof(DS2KeyPacket));		//send packet
}

void Update(uint32_t keys, uint32_t keysTurbo, uint32_t gripKeys, uint32_t gripKeysTurbo, touchPosition* pos)
{
	if(EMULATOR)
		return;					//skip if emulating
	packet = DS2KeyPacket{ };			//clear the packet
	packet.type = UDP::PACKET::NORMAL;
	packet.profile = GetProfile();
	packet.keys = keys;
	packet.keys_turbo = keysTurbo;
	if(pos != nullptr)				//touch status active
	{
		packet.touch_x = pos->px;		//update x
		packet.touch_y = pos->py;		//update y
	}
	else						//touch status inactive
	{
		packet.keys &= ~KEY_TOUCH;		//clear touch status
	}
	packet.gh_keys = gripKeys;
	packet.gh_keys_turbo = gripKeysTurbo;

	Send(&packet, sizeof(DS2KeyPacket));		//send packet
}

void ServerLookup()
{
	if(EMULATOR)						//skip if emulating
		return;					
	unsigned long saved_remote_ip = GetRemoteIP();		//save the remote IP
	unsigned long LocalIP = GetLocalIP();			//get the local IP
	SetRemoteIP(((LocalIP) & 0xFF) |			//setup the broadcast IP
		   (((LocalIP >> 8) & 0xFF) << 8) |
		   (((LocalIP >> 16) & 0xFF) << 16) |
		   (0xFF << 24));

	packet = DS2KeyPacket{ };				//clear the packet
	packet.type = UDP::PACKET::LOOKUP;			//set as a lookup packet

	Send(&packet, sizeof(DS2KeyPacket));			//send the packet out

	if(Recv((char*)&packet, sizeof(DS2KeyPacket)) != 0)	//didn't receive anything
	{
		SetRemoteIP(saved_remote_ip);			//reset the remote IP
	}
	else if(packet.type == UDP::PACKET::LOOKUP)		//received a lookup packet
	{
		if(GetLocalIP() == GetRemoteIP())		//if it's from the local IP
			SetRemoteIP(saved_remote_ip);		//reset the remote IP
	}

}

uint8_t GetProfile()
{
	return profile;
}
std::string GetProfileString()
{
	return ltos(GetProfile());
}

void SetProfile(const std::string& profile)
{
	SetProfile(stol(profile));
}
void SetProfile(char* profile)
{
	SetProfile(atoi(profile));
}
void SetProfile(unsigned int profile)
{
	UDP::profile = profile;
}
DS2KeySettingsPacket GetCommandSettings()
{
	DS2KeySettingsPacket settings = DS2KeySettingsPacket{ };
	packet = DS2KeyPacket{ };						//clear the packet
	packet.type = UDP::PACKET::COMMAND_SETTINGS;
	packet.profile = GetProfile();
	Send(&packet, sizeof(DS2KeyPacket));					//send the packet out

	//try 3 times
	const int SEND_SETTINGS_MAX = 3;
	for(int i = 0; i < SEND_SETTINGS_MAX; i++)
	{
		if(Recv((char*)&settings, sizeof(DS2KeySettingsPacket)) == 0)	//received something
		{
			if(settings.type == UDP::PACKET::COMMAND_SETTINGS)	//received a lookup packet
			{
				return settings;
			}
			std::clog << "Error (GetCommandSettings): Received invalid packet\n";
		}
		swiWaitForVBlank();//wait a second before trying again
	}
	//if we didn't receive anything, or something invalid we return NULL_VALUE
	settings = DS2KeySettingsPacket{ };
	std::clog << "Error (GetCommandSettings): Failed to receive packet\n";
	return settings;
}
#else//D2KSERVER
void SendCommandSettings(DS2KeySettingsPacket settings)
{
	Send(&settings, sizeof(DS2KeySettingsPacket));
}
#endif//D2KCLIENT

}}//namespace D2K::UDP
