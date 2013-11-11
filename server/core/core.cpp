/*
	DS2Key Core functions
*/

#include <stdio.h>		//printf
#include <algorithm>	//std::max,std::min
#ifdef _WIN32
#include <windows.h>
#ifdef WIN32GUI
#include "gui/gui.h"
#endif//WIN32GUI
#elif defined __linux__
#include <string.h>
#include <chrono>
#include <thread>
#include <sstream>//ostringstream
#define Sleep(a) std::this_thread::sleep_for(std::chrono::milliseconds(a))
#endif//_WIN32

#include "udp.h"
#include "key.h"
#include "input.h"
#include "config.h"
#include "client.h"
#include "core.h"

//used only once in code so far
#define touchBetween(x1, y1, x2, y2) ((x >= std::min((x1), (x2)) && x <= std::max((x1), (x2)) && y >= std::min((y1), (y2)) && y <= std::max((y1), (y2))) && (x1+y1+x2+y2>0))

namespace D2K {
	namespace Core {
		void ProcessPacket(C::Client *Client) {
			//static values
			static uint16_t lastX = 0, lastY = 0;
			static bool lastZ = false;

			//settings pointer
			uint16_t *Settings = Client->GetSettings();

			//turbo status
			static bool turbo = false;//true == press, false == release

			//buttons
			for(int i = kUp; i <= kLid; i++) {
				uint16_t Button = bit2button(i);
				if(Client->Down(Button))
					Input->Press(Settings[i], Settings[kJoy]);
				else if(Client->Up(Button))
					Input->Release(Settings[i], Settings[kJoy]);
				else if(turbo && Client->Turbo(Button))
					Input->Press(Settings[i], Settings[kJoy]);
				else if(Client->Turbo(Button))
					Input->Release(Settings[i], Settings[kJoy]);
			}

			turbo = !turbo;//toggle turbo status

			//touch screen
			uint8_t x = Client->GetX();
			uint8_t y = Client->GetY();
			bool z = Client->Held(DS2KEY_TOUCH);
			uint16_t moveType = Settings[kMouse];

			if(z) {//if touched
				if(lastZ == false) {								//if newly pressed
					//Input->Press(KEY_LBUTTON);
					lastX = x;
					lastY = y;
					lastZ = true;
				}
				int i = 25;//ignore
				int s = 3;//sensitivity
				int border = 5;
				if(moveType == mButtons) {							//touch buttons
					bool retVal = false;
					for(int c = 0; c <= 11; c++) {					//check all 12 buttons
						int _x = Settings[kTouch00X + c];
						int _y = Settings[kTouch00Y + c];
						int w = _x + Settings[kTouch00W + c];
						int h = _y + Settings[kTouch00H + c];
						if(touchBetween(_x, _y, w, h)) {			//if button pressed
							Input->Press(Settings[kTouch00 + c], Settings[kJoy]);
							Input->Release(Settings[kTouch00 + c], Settings[kJoy]);
							retVal = true;							//don't return till we've checked every button
						}
					}
					if(retVal)
						return;
				}
				else if(!((x - lastX < -i) || (x - lastX > i) || (y - lastY < -i) || (y - lastY > i))) {	//check that we've moved
					if(moveType == mRelative) {													//relative movement
						Input->Move((x - lastX) * s, (y - lastY) * s);
					}
					else if(moveType == mAbsolute) {												//absolute movement
						int tempX = x;
						int tempY = y;
						if(tempX < border) tempX = border;
						if(tempX > 255 - border) tempX = 255 - border;
						if(tempY < border) tempY = border;
						if(tempY > 191 - border) tempY = 191 - border;
						tempX -= border;
						tempY -= border;
						Input->MoveAbsolute(tempX * (65535  / (255 - border - border)), tempY * (65535  / (191 - border - border)));
					}
					lastX = x;
					lastY = y;
				}
			}
			else {
				if(lastZ == true) {																			//if newly released
					lastX = lastY = 0;
					lastZ = false;
				}
			}
		}

		bool Running = false;
		uint8_t Debug = dNone;

		int Setup(int argc, char *argv[]) {
			bool block = false;
			Config = new C::Config();
				Debug = Config->GetDebugLevel();
			UDP = new C::UDP();
			Input = new C::Input();
			Running = true;

			for(int arg = 1; arg < argc; arg++) {					//command arguments
				if(strcmp(argv[arg], "--block") == 0) {				//setup blocking mode
					block = true;
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
					Config->SetPort(atoi(&argv[arg][7]));
					printf("\nPort: %d\n", Config->GetPort());
				}
			}
			UDP->Connect(block, Config->GetPort());					//startup networking

			return 0;
		}

		void Loop() {
			if(Running) {
				ds2keyPacket Packet;
				if(UDP->IsConnected())
					if(UDP->RecvFrom(&Packet, sizeof(ds2keyPacket)) != -1) {					//if we receive something without error
						if(Packet.type == '/' + 1) {											//new protocal
							if(Client[Packet.profile] == (C::Client*)NULL) {								//if profile not active,
								Client[Packet.profile] = new C::Client();									//create it
								Config->ReadProfile(Client[Packet.profile]->GetSettings(), Packet.profile);	//then load it
							}

							Client[Packet.profile]->SetPacket(Packet);										//insert packet data
							Client[Packet.profile]->Scan();													//update
							ProcessPacket(Client[Packet.profile]);											//process
						}
						else if(Packet.type == '/' + 2) {										//sending command
							printf("Command #%i \"%s\"\n", Packet.profile, Config->GetCommand(Packet.profile));
							if(Config->GetCommand(Packet.profile)) {
								#ifdef _WIN32
								ShellExecute(NULL, "open", Config->GetCommand(Packet.profile), NULL, NULL, SW_SHOWNORMAL);
								#elif defined __linux__
								std::ostringstream command;
								command << Config->GetCommand(Packet.profile) << " &";
								system(command.str().c_str());
								#endif//_WIN32
							}
						}
						else if(Packet.type == 0xFF) {											//looking for servers
							UDP->Send(&Packet, sizeof(ds2keyPacket));							//bounce back
						}
					}

				Sleep(1);																		//sleep to avoid 99% cpu when not using -O2
				#ifdef WIN32GUI
				D2K::GUI::GetMessages();														//Take care of GUI stuff
				#endif//WIN32GUI
			}
		}
		void Destroy() {
			Running = false;
			for(int i = 0; i < 255; i++)
				if(Client[i] != NULL) {
					delete(Client[i]);
					Client[i] = NULL;
				}
			delete(UDP);
			delete(Input);
			delete(Config);
		}
	}
}

