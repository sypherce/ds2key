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
#include <nds.h>
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

// shared (linux, nds, 3ds) defines
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
#include "commandWindow.h"
#endif

#include "udp.h"
#include "misc.h"

namespace D2K {namespace UDP {

bool g_non_blocking = true;
bool connected = false;
uint16_t g_port = DEFAULT_PORT;
SOCKET socket_id = INVALID_SOCKET;
#ifdef D2KCLIENT
DS2KeyPacket packet = DS2KeyPacket{ };
uint8_t profile = 0;
std::string remote_ip = "";
#endif
#if defined(D2KSERVER)
struct sockaddr_in local_sockaddr = { };
#endif
struct sockaddr_in g_remote_sockaddr = { };

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
	return Connect(g_non_blocking, g_port);
}

int Connect(uint16_t port)
{
	return Connect(g_non_blocking, port);
}

int Connect(bool non_blocking, uint16_t port)
{
	if(EMULATOR)          // Skip if emulating
		return 0;

	if(IsConnected())     // If already connected
		Disconnect();     // Disconnect first

	SetConfigPort(port);  // Set port

	UDP::g_non_blocking = non_blocking;
	
	g_remote_sockaddr.sin_family = AF_INET;
	g_remote_sockaddr.sin_port = htons(GetPort());
#if defined(D2KSERVER)
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
		std::clog << "Error #" << err << " (socket): " << strerror(err) << "\n";
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
			std::clog << "Error #" << err << " (bind): " << strerror(err) << "\n";
		Disconnect();

		return err;
	}
#endif

#ifdef _3DS
	int flags = fcntl(socket_id, F_GETFL, 0);
	if(non_blocking && fcntl(socket_id, F_SETFL, flags | O_NONBLOCK))
	{
		int err = NETerrno;
		std::clog << "Error #" << err << " (fcntl): " << strerror(err) << "\n";
#else
	// set blocking mode
	if(NETioctlsocket(socket_id, FIONBIO, (unsigned long*)&UDP::g_non_blocking) == SOCKET_ERROR)
	{
		int err = NETerrno;
		std::clog << "Error #" << err << " (NETioctlsocket): " << strerror(err) << "\n";
#endif
		Disconnect();

		return err;
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
			std::clog << "Error #" << err << " (NETclosesocket): " << strerror(err) << "\n";

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
		int sockaddrlength = sizeof(struct sockaddr_in);
		if(sendto(socket_id, (const char*)buffer, length, 0, (struct sockaddr*)&g_remote_sockaddr, sockaddrlength) == SOCKET_ERROR)
		{
			int err = NETerrno;
			std::clog << "Error #" << err << " (sendto): " << strerror(err) << "\n";
			return err;
		}
	}

	return 0;
}

int Recv(void* buffer, unsigned int length)
{
	return Recv(buffer, length, (struct sockaddr*)&g_remote_sockaddr);
}

int Recv(void* buffer, unsigned int length, struct sockaddr* remote_sockaddr)
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
	else if(remote_sockaddr == nullptr)
	{
		std::clog << "Error (UDP::Recv) g_remote_sockaddr is NULL\n";
		return -4;
	}
	else // Successful
	{
		socklen_t sockaddrlength = sizeof(struct sockaddr_in);
		if(recvfrom(socket_id, (char*)buffer, length, 0, remote_sockaddr, &sockaddrlength) == SOCKET_ERROR)
		{
			int err = NETerrno;
			if(err != NETEWOULDBLOCK)
				std::clog << "Error #" << err << " (Error (recvfrom): " << strerror(err) << "\n";
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

#if defined(D2KCLIENT)
unsigned long GetRemoteIP()
{
	return g_remote_sockaddr.sin_addr.s_addr;
}
std::string GetRemoteIPString()
{
	std::string IP = inet_ntoa(g_remote_sockaddr.sin_addr);
	return IP;
}
#endif

void SetRemoteIP(const std::string& text)
{
	g_remote_sockaddr.sin_addr.s_addr = inet_addr(text.c_str());
}
void SetRemoteIP(unsigned long ip)
{
	g_remote_sockaddr.sin_addr.s_addr = ip;
}

uint16_t GetPort()
{
	return g_port;
}
std::string GetPortString()
{
	return ltos(GetPort());
}

void SetConfigPort(const std::string& port)
{
	SetConfigPort(D2K::stol(port));
}
void SetConfigPort(char* port)
{
	SetConfigPort(atoi(port));
}
void SetConfigPort(uint16_t port)
{
	if(port == 0)                 // If port 0
		UDP::g_port = DEFAULT_PORT; // Use default port 9501
	else
		UDP::g_port = port;
}

#if defined(D2KCLIENT)

void SendNormalSetting(DS2KeyNormalSettingsPacket setting)
{
	Send(&setting, sizeof(DS2KeyNormalSettingsPacket));
}
void SendCommand(uint8_t command)
{
	if(command >= UDP::SETTINGS_PACKET_MAX_BUTTONS) // Valid range is 0 - 11
		return;

	packet = DS2KeyPacket{ };                       // Clear the packet
	packet.type = UDP::PACKET::COMMAND;
	packet.ip_address = UDP::GetLocalIP();
	packet.profile = GetProfile();
	packet.keys = command;

	Send(&packet, sizeof(DS2KeyPacket));            // Send packet
}

void Update(uint32_t keys, uint32_t keysTurbo, touchPosition* touch_position,
            circlePosition* circle_position, accelVector* g_accel_status,
            angularRate* gyro_status, uint16_t keyboard)
{
	if(EMULATOR)                                 // Skip if emulating
		return;

	ListenForServer();                           // Listen for the server

	packet = DS2KeyPacket{ };                    // Clear the packet
	packet.type = UDP::PACKET::NORMAL;
	packet.ip_address = UDP::GetLocalIP();
	packet.profile = GetProfile();//this was before ip_address. I'm not sure why.
	packet.keys = keys;
	packet.keys_turbo = keysTurbo;
	if(touch_position != nullptr)                // Touch status is active
	{
		packet.touch_x = touch_position->px; // Update x
		packet.touch_y = touch_position->py; // Update y
	}
	else                                         // Touch status is inactive
	{
		packet.keys &= ~KEY_TOUCH;           // Clear touch status
	}
	if(circle_position != nullptr)             // Circle status is active
	{
		packet.circle_x = circle_position->dx; // Update x
		packet.circle_y = circle_position->dy; // Update y
	}
	if(g_accel_status != nullptr)              // Circle status is active
	{
		packet.accel_x = g_accel_status->x;    // Update x
		packet.accel_y = g_accel_status->y;    // Update y
		packet.accel_z = g_accel_status->z;    // Update z
	}
	if(gyro_status != nullptr)                 // Circle status is active
	{
		packet.gyro_x = gyro_status->x;        // Update x
		packet.gyro_y = gyro_status->y;        // Update y
		packet.gyro_z = gyro_status->z;        // Update z
	}
	packet.keyboard = keyboard;

	Send(&packet, sizeof(DS2KeyPacket));         // Send packet
}

void SendLookupPacket()
{
	packet = DS2KeyPacket{ };            // Clear the packet
	packet.type = UDP::PACKET::LOOKUP;   // Set as a lookup packet
	packet.ip_address = UDP::GetLocalIP();

	Send(&packet, sizeof(DS2KeyPacket)); // Send the packet out
}


void RequestSettingsCommand()
{
	packet = UDP::DS2KeyPacket{ };                  // Clear the packet
	packet.type = UDP::PACKET::COMMAND_SETTINGS;    // Set as command settings packet
	packet.profile = UDP::GetProfile();             // Set profile
	packet.ip_address = UDP::GetLocalIP();

	UDP::Send(&packet, sizeof(UDP::DS2KeyPacket));  // Send the packet out
}

void ServerLookup()
{
	if(EMULATOR)                                   // Skip if emulating
		return;
	unsigned long saved_remote_ip = GetRemoteIP(); // Save the remote IP
	unsigned long LocalIP = GetLocalIP();          // Get the local IP
	SetRemoteIP(((LocalIP) & 0xFF) |               // Setup the broadcast IP
		   (((LocalIP >> 8) & 0xFF) << 8) |
		   (((LocalIP >> 16) & 0xFF) << 16) |
		   (0xFF << 24));

	SendLookupPacket();                            // Send the lookup packet
	
	//wait for 1 second
	for(int i = 0; i < 60; i++)
	{
		WaitForVBlank();
	}

	if(Recv((char*)&packet, sizeof(DS2KeyPacket)) != 0) // Didn't receive anything
	{
		SetRemoteIP(saved_remote_ip);               // Reset the remote IP
	}
	else if(packet.type == UDP::PACKET::LOOKUP)         // Received a lookup packet
	{
		if(GetLocalIP() == GetRemoteIP())           // If it's from the local IP
			SetRemoteIP(saved_remote_ip);       // Reset the remote IP
	}
}

void ListenForServer()
{
	if(EMULATOR)                        // Skip if emulating
		return;

	DS2KeyCommandSettingsPacket command_settings_packet = DS2KeyCommandSettingsPacket{ }; // Large packet

	if(UDP::IsConnected()               // Received something
	&& UDP::Recv(&command_settings_packet, sizeof(DS2KeyCommandSettingsPacket)) == 0)
	{
		switch(command_settings_packet.type)
		{
		case UDP::PACKET::ALIVE:  // Received a status query
			SendLookupPacket(); // Send the lookup packet
			break;
		case UDP::PACKET::COMMAND_SETTINGS: //Received Command Settings
			GUI::Command::ProcessCommandSettingsPacket(command_settings_packet);
			break;
		default:
			break;
		}
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
#elif defined(D2KSERVER)
void SendCommandSettings(DS2KeyCommandSettingsPacket settings)
{
	Send(&settings, sizeof(DS2KeyCommandSettingsPacket));
}
#endif

}}//namespace D2K::UDP
