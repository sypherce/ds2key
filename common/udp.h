#pragma once

#include <cstdint>//uint16_t
#include <string>//std::string

#include "common/ds2keyPacket.h"

#ifdef __linux__
#include <netdb.h>
#elif defined(_NDS)
#include <nds.h>
#include <netinet/in.h>//sockaddr_in
#elif defined(_3DS)
#include <3ds.h>
#include <netinet/in.h>//sockaddr_in
#endif

#ifndef _WIN32
typedef int SOCKET;
#endif

namespace D2K {namespace UDP {

enum PACKET : uint8_t
{
	//Packet Type Definitions
	NORMAL = ('/' + 1),
	COMMAND,
	COMMAND_SETTINGS,
	LOOKUP = 0xFF,
};

//Sets up Winsock
void Init();

//Disconnects UDP if connected and Cleans up Winsock
void DeInit();

bool IsConnected();

int Connect();
int Connect(uint16_t port);
//connect udp system
//@param block false == blocking, true == non-blocking
//@param port Port to connect to. if port 0, SetConfigPort assigns to default port 9501
//@return (0) if connected, else (errno)
int Connect(bool non_blocking, uint16_t port);

//disconnect udp system
//@return (0) without error, else (errno)
int Disconnect();

//Sends contents of (buffer) up to (length) in size
//@param buffer Pointer to data
//@param length Length of data to send
//@return (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, else (errno)
int Send(const void* buffer, unsigned int length);

//Receives contents into (buffer) up to (length) in size
//@param buffer Pointer to data.
//(buffer) must be allocated before calling.
//(buffer) is only modified up to (length) if recvfrom is successful, otherwise it should be left un modified.
//@param length Length of data to receive
//@return (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, else (errno)
int Recv(void* buffer, unsigned int length);

unsigned long GetLocalIP();
unsigned long GetRemoteIP();
uint16_t GetPort();

std::string GetLocalIPString();
std::string GetRemoteIPString();
std::string GetPortString();

void SetRemoteIP(const std::string& text);
void SetRemoteIP(unsigned long ip);

void SetConfigPort(const std::string& port);
void SetConfigPort(char* port);
void SetConfigPort(unsigned int port);

#ifdef D2KCLIENT
//sends a command packet
//@param command Value between 0 - 11, 12 - 255 are ignored
void SendCommand(uint8_t command);

//updates current button and touch screen status
void Update(uint32_t keys, uint32_t keysTurbo, touchPosition* touch_position);

//searches for running servers on current port
//todo: this should actually just return any found ip and not change our currently connected IP
void ServerLookup();

uint8_t GetProfile();
std::string GetProfileString();

void SetProfile(const std::string& profile);
void SetProfile(char* profile);
void SetProfile(unsigned int profile);

DS2KeySettingsPacket GetCommandSettings();

#elif defined(D2KSERVER)
void SendCommandSettings(DS2KeySettingsPacket settings);
#endif

}}//namespace D2K::UDP
