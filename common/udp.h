#pragma once

#include <cstdint>//uint16_t
#include <string>//std::string

#include "common/ds2keyPacket.h"

#ifdef __linux__
#include <netdb.h>
#elif defined ARM9
#include <nds.h>
#include <netinet/in.h>//sockaddr_in
#endif//__linux__

#ifndef _WIN32
typedef int SOCKET;
#endif//_WIN32

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
//@param port Port to connect to. if port = 0, SetConfigPort assigns to default port 9501
//\return (0) if connected, else (errno)
int Connect(bool non_blocking, uint16_t port);

//disconnect udp system
//@return (0) without error, else (errno)
int Disconnect();

//Sends contents of (buf) up to (len) in size
//@param buf Pointer to data
//@param len Length of data to send
//@return (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, else (errno)
int Send(const void* buffer, unsigned int length);

//Receives contents into (buf) up to (len) in size
//@param buf Pointer to data.
//(buf) must be allocated before calling.
//(buf) is only modified up to (len) if recvfrom is successful, otherwise it should be left un modified.
//@param len Length of data to receive
//\return (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, else (errno)
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
void Update(uint32_t keys, uint32_t keysTurbo, uint32_t gripKeys, uint32_t gripKeysTurbo, touchPosition* pos);

//searches for running servers on current port
//todo: this should actually just return any found ip and not change our currently connected IP
void ServerLookup();

uint8_t GetProfile();
std::string GetProfileString();

void SetProfile(const std::string& profile);
void SetProfile(char* profile);
void SetProfile(unsigned int profile);

DS2KeySettingsPacket GetCommandSettings();

#else//D2KSERVER
void SendCommandSettings(DS2KeySettingsPacket settings);
#endif//D2KCLIENT

}}//namespace D2K::UDP
