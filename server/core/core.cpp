/*
	DS2Key Core functions
*/

#include <iostream>//std::cout, std::clog
#include <algorithm>	//std::max,std::min
#include <sstream>//ostringstream
#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#ifdef WIN32GUI
#include "gui/gui.h"
#endif//WIN32GUI
#elif defined __linux__
#include <cstring>
#include <chrono>
#include <thread>
#define Sleep(a) std::this_thread::sleep_for(std::chrono::milliseconds(a))
#endif//_WIN32

#include "common/udp.h"
#include "key.h"
#include "input.h"
#include "config.h"
#include "client.h"
#include "core.h"

namespace D2K {
	//used only once in code so far
	inline bool touchBetween(int x, int y, int x1, int y1, int x2, int y2)
	{
		return ((x >= std::min((x1), (x2)) &&
				 x <= std::max((x1), (x2)) &&
				 y >= std::min((y1), (y2)) &&
				 y <= std::max((y1), (y2))) &&
				(x1 + y1 + x2 + y2 > 0));
	}
	void ExecuteCommand(std::string Command) {
		if(Command != "") {
			if(Command.substr(0, D2K_COMMAND_LENGTH) == D2K_COMMAND) {
				Command = Command.substr(D2K_COMMAND_LENGTH);
			}
			#ifdef _WIN32
			WinExec(Command.c_str(), SW_SHOWNORMAL);
			#elif defined __linux__
			std::ostringstream stringStream;
			stringStream << Command << " &";
			system(stringStream.str().c_str());
			#endif//_WIN32
		}
	}
	void ProcessPacket(D2K::Client *Client) {
		//static values
		static uint16_t lastX = 0, lastY = 0;
		static bool lastZ = false;

		//turbo status, true == press, false == release
		static bool turbo = false;

		ProfileData *Profile = Client->GetProfileDataPointer();

		//buttons
		for(int i = kUp; i <= kLid; i++) {
			uint16_t DSButton = bit2button(i);
			uint16_t Joystick = Profile->GetValue(kJoy);
			uint16_t PCButton = Profile->GetVirtualKey(i);

			if(PCButton) {
				if(Client->Down(DSButton))						//pressed
					Input::Press(PCButton, Joystick);
				else if(Client->Up(DSButton))					//released, even in turbo mode
					Input::Release(PCButton, Joystick);
				else if(turbo && Client->Turbo(DSButton))		//turbo set, button enabled for turbo and pressed
					Input::Press(PCButton, Joystick);
				else if(Client->Turbo(DSButton))				//turbo UNSET, button enabled for turbo and pressed
					Input::Release(PCButton, Joystick);			//we release because turbo is UNSET
			}
			else {
				if(Client->Down(DSButton)) {					//pressed
					std::string Command = Profile->GetCommand(i);
					ExecuteCommand(Command);
				}
			}
		}
		//gh buttons
		for(int i = kBlue; i <= kGreen; i++) {
			uint16_t DSButton = bit2button(i);
			uint16_t Joystick = Profile->GetValue(kJoy);
			uint16_t PCButton = Profile->GetVirtualKey(i);

			if(PCButton) {
				if(Client->GHDown(DSButton))						//pressed
					Input::Press(PCButton, Joystick);
				else if(Client->GHUp(DSButton))					//released, even in turbo mode
					Input::Release(PCButton, Joystick);
				else if(turbo && Client->GHTurbo(DSButton))		//turbo set, button enabled for turbo and pressed
					Input::Press(PCButton, Joystick);
				else if(Client->GHTurbo(DSButton))				//turbo UNSET, button enabled for turbo and pressed
					Input::Release(PCButton, Joystick);//we release because turbo is UNSET
			}
			else {
				std::string Command = Profile->GetCommand(i);
				ExecuteCommand(Command);
			}
		}

		turbo = !turbo;//toggle turbo status

		//touch screen
		uint8_t x = Client->GetX();
		uint8_t y = Client->GetY();
		bool z = Client->Held(DS2KEY_TOUCH);
		std::string moveType = Profile->Mouse;

		if(z) {																	//if touched
			if(lastZ == false) {												//if newly pressed
				//Input::Press(KEY_LBUTTON);
				lastX = x;
				lastY = y;
				lastZ = true;
			}
			static const int i = 25;											//ignore
			static const int s = 3;												//sensitivity
			static const int border = 5;
			if(!((x - lastX < -i) || (x - lastX > i) || (y - lastY < -i)
				   || (y - lastY > i))) {										//check that we've moved
				if(moveType == "Relative") {									//relative movement
					Input::Move((x - lastX) * s, (y - lastY) * s);
				}
				else if(moveType == "Absolute") {								//absolute movement
					//the next 8 lines apply our border to X and Y values
					int tempX = x;
					int tempY = y;
					if(tempX < border) tempX = border;
					if(tempX > 255 - border) tempX = 255 - border;
					if(tempY < border) tempY = border;
					if(tempY > 191 - border) tempY = 191 - border;
					tempX -= border;
					tempY -= border;
					Input::MoveAbsolute(tempX * (65535  / (255 - border - border)), tempY * (65535  / (191 - border - border)));
				}
				lastX = x;
				lastY = y;
			}
		}
		else {
			if(lastZ == true) {													//if newly released
				lastX = lastY = 0;
				lastZ = false;
			}
		}
	}

	bool Running = false;

	int Setup(int argc, char *argv[]) {
		bool block = true;//non-blocking == true
		Config::Load();
		UDP::Init();
		Input::Init();
		Running = true;

		for(int arg = 1; arg < argc; arg++) {					//command arguments
			if(strcmp(argv[arg], "--block") == 0) {				//setup blocking mode
				block = false;
			}
			else if(strcmp(argv[arg], "--console") == 0) {		//setup console mode
				#ifdef WINXP
				BOOL f = AllocConsole();
				freopen("CONIN$", "r", stdin);
				freopen("CONOUT$", "w", stdout);
				freopen("CONOUT$", "w", stderr);
				#endif
			}
			else if(strncmp(argv[arg], "--port=", 7) == 0) {	//assign a specific port
				Config::SetPort(atoi(&argv[arg][7]));
				std::cout << "\nPort: " << Config::GetPort() << "\n";
			}
		}

		return UDP::Connect(block, Config::GetPort());			//startup networking
	}

	void Loop() {
		if(Running && UDP::IsConnected()) {
			UDP::DS2KeyPacket Packet;
			if(UDP::Recv(&Packet, sizeof(UDP::DS2KeyPacket)) == 0) {								//if we receive something without error
				if(Packet.Type == UDP_PACKET_LOOKUP) {												//looking for servers
					UDP::Send(&Packet, sizeof(UDP::DS2KeyPacket));									//bounce back
				}
				else if(Packet.Type == UDP_PACKET_COMMAND_SETTINGS) {								//looking for command settings
					UDP::DS2KeySettingsPacket settings = (UDP::DS2KeySettingsPacket){0};
					if(ClientArray[Packet.Profile] != (D2K::Client*)NULL) {							//if profile is active
						ProfileData *profile = ClientArray[Packet.Profile]->GetProfileDataPointer();//make a pointer to the profile
						settings.Type = UDP_PACKET_COMMAND_SETTINGS;
						for(int i = 0; i <= 11; i++) {
							settings.X1[i] = profile->TouchX[i];
							settings.X2[i] = profile->TouchW[i];
							settings.Y1[i] = profile->TouchY[i];
							settings.Y2[i] = profile->TouchH[i];
							strncpy(settings.text[i], profile->TouchString[i].c_str(), 10);
							settings.text[i][10] = 0;
						}
						UDP::SendCommandSettings(settings);											//send settings packet
					}
				}
				else if(Packet.Type >= UDP_PACKET_NORMAL && Packet.Type <= UDP_PACKET_COMMAND) {	//make sure this is a packet we accept
					if(ClientArray[Packet.Profile] == (D2K::Client*)NULL) {							//if profile not active,
						ClientArray[Packet.Profile] = new D2K::Client();							//create it
						Config::LoadProfile(
							ClientArray[Packet.Profile]->GetProfileDataPointer(), Packet.Profile);	//then load it
					}
					D2K::Client *pClient = ClientArray[Packet.Profile];								//then make a pointer to it

					if(Packet.Type == UDP_PACKET_NORMAL) {											//normal update
						pClient->SetPacket(Packet);													//insert packet data
						pClient->Scan();															//update
						ProcessPacket(pClient);														//process
					}
					else if(Packet.Type == UDP_PACKET_COMMAND) {									//command update
						int TouchButton = kTouch00 + Packet.Keys;
						ProfileData *Data = pClient->GetProfileDataPointer();						//pointer to profile data
						uint16_t Joystick = Data->GetValue(kJoy);
						uint16_t PCButton = Data->GetVirtualKey(TouchButton);

						if(PCButton) {
							Input::Press(PCButton, Joystick);
		Sleep(100);
							Input::Release(PCButton, Joystick);
						}
						else {
							std::string Command = Data->GetCommand(TouchButton);
							ExecuteCommand(Command);
						}
					}
				}
			}
		}

		Sleep(1);																		//sleep to avoid 99% cpu when not using -O2
		#ifdef WIN32GUI
		D2K::GUI::GetMessages();														//Take care of GUI stuff
		#endif//WIN32GUI
	}
	void Destroy() {
		Running = false;
		for(int i = 0; i < 255; i++)
			if(ClientArray[i] != NULL) {
				delete(ClientArray[i]);
				ClientArray[i] = NULL;
			}
		UDP::DeInit();
		Input::DeInit();
		Config::Save();
	}
}

