#pragma once

#include <cstdint> // uint16_t
#include <string>  // std::string

#include "common/ds2keyPacket.h"

#ifdef __linux__
#include <netdb.h>
#elif defined(_NDS)
#include <nds.h>
#include <netinet/in.h>  // sockaddr_in
#include "dummy_types.h" // 3ds types: circlePosition, etc
#elif defined(_3DS)
#include <3ds.h>
#include <netinet/in.h>  // sockaddr_in
#endif

#ifndef _WIN32
typedef int SOCKET;
#endif

namespace D2K {namespace UDP {

enum PACKET : uint8_t
{
	// Packet Type Definitions
	NORMAL = ('/' + 1),
	COMMAND,
	COMMAND_SETTINGS,
	NORMAL_SETTING,
	ALIVE,
	LOOKUP = 0xFF,
};

// Sets up Winsock
void Init();

// Disconnects UDP if connected and Cleans up Winsock
void DeInit();

// return: (true) if connected, else (false)
bool IsConnected();

// connect udp system
// return: (0) if connected, else (errno)
int Connect();
// connect udp system
//  param: _port Port to connect to, if port 0, SetConfigPort defaults to 9501
// return (0) if connected, else (errno)
int Connect(uint16_t _port);
// connect udp system
//  param: _non_blocking false == blocking, true == non-blocking
//  param: _port Port to connect to, if port 0, SetConfigPort assigns to default port 9501
// return: (0) if connected, else (errno)
int Connect(bool _non_blocking, uint16_t _port);

// disconnect udp system
// return: (0) without error, else (errno)
int Disconnect();

// sends contents of (buffer) up to (length) in size
//  param: buffer Pointer to data
//  param: length Length of data to send
// return: (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, else (errno)
int Send(const void* buffer, unsigned int length);

// receives contents into (buffer) up to (length) in size
//  param: buffer Pointer to data
//         buffer must be allocated before calling
//         buffer is only modified up to (length) if recvfrom is successful, otherwise it should be left un-modified
//  param: length Length of data to receive
// return: (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, (-4) NULL _remote_sockaddr, else (errno)
int Recv(void* buffer, unsigned int length);
// Receives contents into (buffer) up to (length) in size
//  param: buffer Pointer to data
//         buffer must be allocated before calling
//         buffer is only modified up to (length) if recvfrom is successful, otherwise it should be left un modified
//  param: length Length of data to receive
//  param: _remote_sockaddr Container for recvfrom to fill in with remote address
// return: (0) without error, (-1) not connected, (-2) invalid length, (-3) invalid pointer, (-4) NULL _remote_sockaddr, else (errno)
int Recv(void* buffer, unsigned int length, struct sockaddr* _remote_sockaddr);

unsigned long GetLocalIP();
std::string GetLocalIPString();

unsigned long GetRemoteIP();
std::string GetRemoteIPString();

void SetRemoteIP(const std::string& text);
void SetRemoteIP(unsigned long ip);

uint16_t GetPort();
std::string GetPortString();

// set port to connect to
// note: we use SetConfigPort to avoid clash with SetPort defined in windows
//  param: _port UDP port between 1-65535. If port is 0, we default to 9501
void SetConfigPort(const std::string& _port);
// set port to connect to
// note: we use SetConfigPort to avoid clash with SetPort defined in windows
//  param: _port UDP port between 1-65535. If port is 0, we default to 9501
void SetConfigPort(const char* _port);
// set port to connect to
// note: we use SetConfigPort to avoid clash with SetPort defined in windows
//  param: _port UDP port between 1-65535. If port is 0, we default to 9501
void SetConfigPort(uint16_t _port);

#if defined(D2KCLIENT)
// TODO:: this will change
void SendNormalSetting(DS2KeyNormalSettingsPacket setting);

// sends a command packet
//  param: command Value between 0 - 11, 12 - 255 are ignored
void SendCommand(uint8_t command);

// updates current gamepad status
void Update(uint32_t keys, uint32_t keysTurbo, const touchPosition* touch_position,
            const circlePosition* circle_position, const circlePosition* cstick_position,
            const accelVector* accel_status, const angularRate* gyro_status,
            const uint8_t* slider_volume, const float* slider_3d, uint16_t keyboard);

// sends a blank COMMAND_SETTINGS packet afterwards the server replys with a real one
void RequestSettingsCommand();

// searches for running servers on current port
// todo: this should actually just return any found ip and not change our currently connected IP
void ServerLookup();

// listens for anything from the server and deals with it
void ListenForServer();

uint8_t GetProfile();
std::string GetProfileString();

void SetProfile(const std::string& _profile);
void SetProfile(const char* _profile);
void SetProfile(uint8_t _profile);

DS2KeyCommandSettingsPacket GetCommandSettings();

#elif defined(D2KSERVER)
// sends a packet containing all the command button strings, and positions
// refer to DS2KeyCommandSettingsPacket in ds2keyPacket.h for more details
void SendCommandSettings(DS2KeyCommandSettingsPacket settings);
#endif

}} // namespace D2K::UDP
