#ifndef __UDP_H__
#define __UDP_H__

//Packet Type Definitions
#define UDP_PACKET_NORMAL			('/' + 1)
#define UDP_PACKET_COMMAND			('/' + 2)
#define UDP_PACKET_COMMAND_SETTINGS	('/' + 3)
#define UDP_PACKET_LOOKUP			0xFF

#include "common/ds2keyPacket.h"

#include <cstdint>//uint16_t
#include <string>//std::string

#ifdef _WIN32
#elif defined __linux__
#include <netdb.h>
#elif defined ARM9
#include <nds.h>
#include <netinet/in.h>//sockaddr_in
#elif defined PSP_DEF
#include "../pspclient/pspTouch.h"
#endif//_WIN32

#if defined(__linux__) || defined(ARM9) || defined(PSP_DEF)
typedef int SOCKET;
#endif//defined(__linux__) || defined(ARM9)

namespace D2K {
	namespace UDP {
		//Sets up Winsock
		void Init();
		//Disconnects UDP if connected and Cleans up Winsock
		void DeInit();
		bool IsConnected();
		int Connect();
		int Connect(uint16_t port);
		//connect udp system
		//@param block false == blocking, true == non-blocking
		//@param port Port to connect to. if port = 0, SetPort assigns to default port 9501
		//\return (0) if connected, else (errno)
		int Connect(bool block, uint16_t port);

		//disconnect udp system
		//@return (0) without error, else (errno)
		int Disconnect();

		//Sends contents of (buf) up to (len) in size
		//@param buf Pointer to data
		//@param len Length of data to send
		//@return (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, else (errno)
		int Send(const void *buf, unsigned int len);

		//Receives contents into (buf) up to (len) in size
		//@param buf Pointer to data.
		//(buf) must be allocated before calling.
		//(buf) is only modified up to (len) if recvfrom is successful, otherwise it should be left un modified.
		//@param len Length of data to receive
		//\return (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, else (errno)
		int Recv(void *buf, unsigned int len);

		unsigned long GetLocalIP();
		std::string GetLocalIPString();
		unsigned long GetRemoteIP();
		std::string GetRemoteIPString();
		uint16_t GetPort();
		std::string GetPortString();

		void SetRemoteIP(const std::string& text);
		void SetRemoteIP(unsigned long ip);

		void SetPort(const std::string& port);
		void SetPort(char *port);
		void SetPort(unsigned int port);
#ifdef D2KCLIENT
		//sends a command packet
		//@param command Value between 0 - 11, 12 - 255 are ignored
		void SendCommand(uint8_t command);

		//updates current button and touch screen status
		void Update(uint32_t keys, uint32_t keysTurbo, uint32_t gripKeys, uint32_t gripKeysTurbo, touchPosition *pos);

		//searches for running servers on current port
		//todo: this should actually just return any found ip and not change our currently connected IP
		void ServerLookup();

		uint8_t GetProfile();
		std::string GetProfileString();

		void SetProfile(const std::string& profile);
		void SetProfile(char *profile);
		void SetProfile(unsigned int profile);
		DS2KeySettingsPacket GetCommandSettings();
#else//D2KSERVER
		void SendCommandSettings(DS2KeySettingsPacket settings);
#endif//D2KCLIENT
	}
}
#endif//__UDP_H__
