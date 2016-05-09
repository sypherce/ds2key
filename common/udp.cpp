// UDP Communication

#if !defined(D2KCLIENT) && !defined(D2KSERVER)
#error "Must define D2KCLIENT or D2KSERVER"
#endif

#include <cstdlib>  // atoi
#include <iostream> // std::cout, std::clog
#include <sstream>  // std::stringstream
#include <cstring>  // strerror

// system specific includes
#if defined(_WIN32)
#include <ws2tcpip.h>     // socklength_t
#elif defined(__linux__)
#include <unistd.h>       // close
#include <arpa/inet.h>    // inet_ntoa
#include <sys/ioctl.h>    // ioctl
#elif defined(_NDS)
#include <nds/ndstypes.h> // dswifi9.h
#include <dswifi9.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#elif defined(_3DS)
#include <3ds.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#endif

// shared (linux, nds) defines
#if defined(__linux__) || defined(_NDS) || defined(_3DS)
#define NETerrno errno
#define NETEADDRINUSE EADDRINUSE
#define NETEWOULDBLOCK EWOULDBLOCK
#define NETioctlsocket ioctl
#define INVALID_SOCKET -1
#endif

// system specific defines
#if defined(_WIN32)
#define NETerrno WSAGetLastError()
#define NETEADDRINUSE WSAEADDRINUSE
#define NETEWOULDBLOCK WSAEWOULDBLOCK
#define NETclosesocket closesocket
#define NETioctlsocket ioctlsocket
#elif defined(__linux__)
#define NETclosesocket close
#define SOCKET_ERROR -1
#elif defined(_NDS)
typedef int socklen_t;
#define NETclosesocket closesocket
#elif defined(_3DS)
#define SOCKET_ERROR -1
#define NETclosesocket closesocket
#endif

#if defined(_NDS) || defined(_3DS)
#include "core.h"
#endif

#include "udp.h"
#include "misc.h"

namespace D2K {namespace UDP {

bool non_blocking = true;
bool connected = false;
uint16_t port = DEFAULT_PORT;
SOCKET socket_id = INVALID_SOCKET;
#ifdef D2KCLIENT
DS2KeyPacket packet = DS2KeyPacket{ };
uint8_t profile = 0;
std::string remote_ip = "";
#endif
#if defined(D2KSERVER) || defined(_WIN32)
struct sockaddr_in local_sockaddr = { };
#endif
struct sockaddr_in remote_sockaddr = { };

void Init()
{
#ifdef _WIN32
	WSADATA wsa_data;
	// Startup with Winsock version 2.2
	WSAStartup(0x0202, &wsa_data);
#endif
}
void DeInit()
{
	Disconnect();
#ifdef _WIN32
	WSACleanup();
#endif
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
	if(EMULATOR)          // Skip if emulating
		return 0;

	if(IsConnected())     // If already connected
		Disconnect();     // Disconnect first

	SetConfigPort(port);  // Set port

	UDP::non_blocking = non_blocking;
#if defined(D2KCLIENT)
	remote_sockaddr.sin_family = AF_INET;
	remote_sockaddr.sin_port = htons(GetPort());
#elif defined(D2KSERVER) || defined(_WIN32)
	int sockaddrlength = sizeof(struct sockaddr_in);
	local_sockaddr = sockaddr_in{ };
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;

	local_sockaddr.sin_port = htons(GetPort());
#endif
	socket_id = socket(PF_INET, SOCK_DGRAM, 0);  // create a socket
	if(socket_id == INVALID_SOCKET)
	{
		int err = NETerrno;
		std::clog << "Error (socket): " << strerror(err) << "\n";
		Disconnect();

		return err;
	}

#ifdef D2KSERVER
	// bind a local address and port
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
#endif

#ifdef _3DS
	int flags = fcntl(socket_id, F_GETFL, 0);
	if(non_blocking && fcntl(socket_id, F_SETFL, flags | O_NONBLOCK))
	{
		int err = NETerrno;
		std::clog << "Error (fcntl): " << strerror(err) << "\n";
#else
	// set blocking mode
	if(NETioctlsocket(socket_id, FIONBIO, (unsigned long*)&UDP::non_blocking) == SOCKET_ERROR)
	{
		int err = NETerrno;
		std::clog << "Error (NETioctlsocket): " << strerror(err) << "\n";
#endif
#if defined(_NDS) | defined (_3DS)//TODO check if this needs done for everything
		Disconnect();

		return err;
#endif
	}
	connected = true;

	return 0;
}

int Disconnect()
{
	if(EMULATOR) // Skip if emulating
		return 0;
	if(IsConnected())
	{
		// udp system disconnects even if NETclosesocket returns error
		connected = false;	
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
	else // Successful
	{
		int sockaddrlength = sizeof(remote_sockaddr);
		if(sendto(socket_id, (const char*)buffer, length, 0, (struct sockaddr*)&remote_sockaddr, sockaddrlength) == SOCKET_ERROR)
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
	else // Successful
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
#if defined(_NDS)
	return Wifi_GetIP();
#elif defined(_3DS)
	return gethostid();
#elif defined(_WIN32) || defined(__linux__)
	return local_sockaddr.sin_addr.s_addr;
#endif
	return 0;
}
std::string GetLocalIPString()
{
#if defined(_NDS) || defined(_3DS)
	struct in_addr sin_addr;
	sin_addr.s_addr = GetLocalIP();
	std::string IP = inet_ntoa(sin_addr);
#elif defined(_WIN32) || defined(__linux__)
	std::string IP = inet_ntoa(local_sockaddr.sin_addr);
#endif

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
	if(port == 0)                 // If port 0
		UDP::port = DEFAULT_PORT; // Use default port 9501
	else
		UDP::port = port;
}

#if defined(D2KCLIENT)
void SendCommand(uint8_t command)
{
	if(command >= UDP::SETTINGS_PACKET_MAX_BUTTONS) // Valid range is 0 - 11
		return;

	packet = DS2KeyPacket{ };                       // Clear the packet
	packet.type = UDP::PACKET::COMMAND;
	packet.profile = GetProfile();
	packet.keys = command;

	Send(&packet, sizeof(DS2KeyPacket));            // Send packet
}

void Update(uint32_t keys, uint32_t keysTurbo, uint32_t gripKeys, uint32_t gripKeysTurbo, touchPosition* pos)
{
	if(EMULATOR)                         // Skip if emulating
		return;					
	packet = DS2KeyPacket{ };            // Clear the packet
	packet.type = UDP::PACKET::NORMAL;
	packet.profile = GetProfile();
	packet.keys = keys;
	packet.keys_turbo = keysTurbo;
	if(pos != nullptr)                   // Touch status is active
	{
		packet.touch_x = pos->px;        // Update x
		packet.touch_y = pos->py;        // Update y
	}
	else                                 // Touch status is inactive
	{
		packet.keys &= ~KEY_TOUCH;       // Clear touch status
	}
	packet.gh_keys = gripKeys;
	packet.gh_keys_turbo = gripKeysTurbo;

	Send(&packet, sizeof(DS2KeyPacket)); // Send packet
}

void ServerLookup()
{
	if(EMULATOR)                                        // Skip if emulating
		return;					
	unsigned long saved_remote_ip = GetRemoteIP();      // Save the remote IP
	unsigned long LocalIP = GetLocalIP();               // Get the local IP
	SetRemoteIP(((LocalIP) & 0xFF) |                    // Setup the broadcast IP
		   (((LocalIP >> 8) & 0xFF) << 8) |
		   (((LocalIP >> 16) & 0xFF) << 16) |
		   (0xFF << 24));

	packet = DS2KeyPacket{ };                           // Clear the packet
	packet.type = UDP::PACKET::LOOKUP;                  // Set as a lookup packet

	Send(&packet, sizeof(DS2KeyPacket));                // Send the packet out
	
	//wait for 1 second
	for(int i = 0; i < 60; i++)
	{
		WaitForVBlank();
	}

	if(Recv((char*)&packet, sizeof(DS2KeyPacket)) != 0) // Didn't receive anything
	{
		SetRemoteIP(saved_remote_ip);                   // Reset the remote IP
	}
	else if(packet.type == UDP::PACKET::LOOKUP)         // Received a lookup packet
	{
		if(GetLocalIP() == GetRemoteIP())               // If it's from the local IP
			SetRemoteIP(saved_remote_ip);               // Reset the remote IP
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
	SetProfile(D2K::stol(profile));
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
	packet = DS2KeyPacket{ };// Clear the packet
	packet.type = UDP::PACKET::COMMAND_SETTINGS;
	packet.profile = GetProfile();
	Send(&packet, sizeof(DS2KeyPacket));  // Send the packet out

	// Try 3 times
	const int SEND_SETTINGS_MAX = 3;
	for(int i = 0; i < SEND_SETTINGS_MAX; i++)
	{
		if(Recv((char*)&settings, sizeof(DS2KeySettingsPacket)) == 0)  // Received something
		{
			if(settings.type == UDP::PACKET::COMMAND_SETTINGS)  // Received a lookup packet
			{
				return settings;
			}
			std::clog << "Error (GetCommandSettings): Received invalid packet\n";
		}
		
		// Wait a second before trying again
		WaitForVBlank();
	}
	// If we didn't receive anything, or something invalid we return NULL_VALUE
	settings = DS2KeySettingsPacket{ };
	std::clog << "Error (GetCommandSettings): Failed to receive packet\n";
	return settings;
}
#elif defined(D2KSERVER)
void SendCommandSettings(DS2KeySettingsPacket settings)
{
	Send(&settings, sizeof(DS2KeySettingsPacket));
}
#endif

}}//namespace D2K::UDP
