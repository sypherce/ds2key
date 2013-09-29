/*
	ds2key networking
*/
#include <nds.h>
#include <dswifi9.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <string>
#include <sstream>
#include <errno.h>

#include "ds2key.h"

using std::string;

//private
	string ip;
	uint16 port;
	uint8 profile;
	int sock;
	struct sockaddr_in sockaddr;

	#pragma pack(1)
	typedef struct ds2keyPacket {
		uint8_t type;
		uint8_t profile;
		uint16_t keys;
		uint8_t ghKeys;
		uint8_t touchX;
		uint8_t touchY;
	} ds2keyPacket;
	#pragma pack()
	ds2keyPacket packet = {0};

	template <class T>
	inline string to_string (const T& t) {
		std::stringstream ss;
		ss << t;

		return ss.str();
	}

	template <typename T>
	long to_number(const std::basic_string<T> &str) {
		std::basic_stringstream<T> stream(str);
		long res;
		return !(stream >>res)?0:res;
	}
//end
namespace D2K {
	//functions
	unsigned long DS2Key::GetIP() {
		inet_aton(ip.c_str(), &sockaddr.sin_addr);

		return sockaddr.sin_addr.s_addr;
	}

	string DS2Key::GetIPString() {
		return ip;
	}

	uint16 DS2Key::GetPort() {
		return port;
	}

	string DS2Key::GetPortString() {
		return to_string(port);
	}

	uint8 DS2Key::GetProfile() {
		return profile;
	}

	string DS2Key::GetProfileString() {
		return to_string((uint16)profile);
	}

	void DS2Key::SetIP(const string& text) {
		ip.assign(text);
	}

	void DS2Key::SetPort(const string& _port) {
		port = to_number(_port);
	}

	void DS2Key::SetPort(char *_port) {
		port = atoi(_port);
	}

	void DS2Key::SetPort(unsigned int _port) {
		port = _port;
	}

	void DS2Key::SetProfile(const string& _profile) {
		profile = to_number(_profile);
	}

	void DS2Key::SetProfile(char *_profile) {
		profile = atoi(_profile);
	}

	void DS2Key::SetProfile(unsigned int _profile) {
		profile = _profile;
	}

	void DS2Key::Init() {
		if(EMULATOR) return;
		if((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
			printf("Error (socket): #%d\n", errno);
			return;
		}

		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = htons(GetPort());
		sockaddr.sin_addr.s_addr = INADDR_ANY;

		if(bind(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1) {
			printf("\nError (bind): #%d\nCan't bind port number %d\n", errno, GetPort());
			DeInit();

			return;
		}

		int i = 1;
		if(ioctl(sock, FIONBIO, &i) == -1) {
			printf("Error (ioctl): #%d\n", errno);
			DeInit();
		}
	}

	void DS2Key::DeInit() {
		if(EMULATOR) return;
		/*shutdown always fails with errno 22**
		**and the code seems to work without **
		**it so I'm commenting it out for	 **
		**now								 **
		if(shutdown(sock, SHUT_RDWR) == -1) {**
			printf("Error (shutdown): #%d\n", errno);
		}									 */

		if(closesocket(sock) == -1) {
			printf("Error (closesocket): #%d\n", errno);
		}

		sock = 0;
		sockaddr.sin_family = 0;
		sockaddr.sin_port = 0;
		sockaddr.sin_addr.s_addr = 0;
	}

	void DS2Key::Update(uint32 keys, uint32 gripKeys, touchPosition *pos) {
		if(EMULATOR) return;
		packet.type = '/' + 1;
		packet.profile = GetProfile();
		packet.keys = keys;
		if(pos == (touchPosition*)NULL)
			packet.keys &= ~KEY_TOUCH;
		else {//if(d2kMode == iMouse)
			packet.touchX = pos->px;
			packet.touchY = pos->py;
		}
		packet.ghKeys = gripKeys;

		sockaddr.sin_addr.s_addr = GetIP();

		if(sendto(sock, (char*)&packet, sizeof(ds2keyPacket), 0, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
			printf("Error (sendto): #%d\n", errno);
	}

	void DS2Key::ServerLookup()
	{
		if(EMULATOR) return;
		char host[128];
		struct hostent *hostEnt;
		struct in_addr addr;
		gethostname(host, 128);
		hostEnt = gethostbyname(host);
		if(hostEnt == NULL)
		{
			printf("Error (gethostbyname): #%d\n", errno);
			return;
		}
		struct in_addr originalAddr = addr;
		int i = 0;
		//while(hostEnt->h_addr_list[i] != 0)
		{
			addr.s_addr = *(u_long *) hostEnt->h_addr_list[i++];
			u32 ip = Wifi_GetIP();
			sprintf(host, "%i.%i.%i.%i", (ip ) & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, 0xFF);
			inet_aton(host, &addr);
			sockaddr.sin_addr = addr;

			printf("\tIP Address %d: %s\n", addr.s_addr, inet_ntoa(addr));
		}
		memset(&packet, 0, sizeof(ds2keyPacket));
		packet.type = 255;


		if(sendto(sock, (char*)&packet, sizeof(ds2keyPacket), 0, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
			printf("Error (sendto): #%d\n", errno);
		sockaddr.sin_addr = originalAddr;
		int sockaddrLen = sizeof(struct sockaddr_in);

		memset(&packet, 0, sizeof(ds2keyPacket));
		if(recvfrom(sock, (char*)&packet, sizeof(ds2keyPacket), 0, (struct sockaddr *)&sockaddr, &sockaddrLen) == -1) {
			printf("Error (recvfrom): #%d\n", errno);
			return;
		}
		if(packet.type == 0xFF) {
			char host[128];
			u32 ip = sockaddr.sin_addr.s_addr;
			sprintf(host, "%i.%i.%i.%i", (ip ) & 0xFF, (ip >> 8) & 0xFF, (ip >> 16) & 0xFF, (ip >> 24) & 0xFF);
			inet_aton(host, &addr);
			SetIP(host);
		}
	}
}
