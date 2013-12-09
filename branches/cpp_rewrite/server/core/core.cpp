/*
	DS2Key Core functions
*/

#include <iostream>//std::cout, std::clog
#include <algorithm>	//std::max,std::min
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
#include <sstream>//ostringstream
#define Sleep(a) std::this_thread::sleep_for(std::chrono::milliseconds(a))
#endif//_WIN32

#include "common/udp.h"
#include "key.h"
#include "input.h"
#include "config.h"
#include "client.h"
#include "core.h"

//used only once in code so far
#define touchBetween(x1, y1, x2, y2) ((x >= std::min((x1), (x2)) && x <= std::max((x1), (x2)) && y >= std::min((y1), (y2)) && y <= std::max((y1), (y2))) && (x1 + y1 + x2 + y2 > 0))

namespace D2K {
	namespace Core {
		void ProcessPacket(C::Client *Client) {
			//static values
			static uint16_t lastX = 0, lastY = 0;
			static bool lastZ = false;

			//settings pointer
			uint16_t *Settings = Client->GetSettings();

			//turbo status, true == press, false == release
			static bool turbo = false;

			//buttons
			for(int i = kUp; i <= kLid; i++) {
				uint16_t Button = bit2button(i);
				if(Client->Down(Button))						//pressed
					Input->Press(Settings[i], Settings[kJoy]);
				else if(Client->Up(Button))						//released, even in turbo mode
					Input->Release(Settings[i], Settings[kJoy]);
				else if(turbo && Client->Turbo(Button))			//turbo set, button enabled for turbo and pressed
					Input->Press(Settings[i], Settings[kJoy]);
				else if(Client->Turbo(Button))					//turbo UNSET, button enabled for turbo and pressed
					Input->Release(Settings[i], Settings[kJoy]);//we release because turbo is UNSET
			}
			//gh buttons
			for(int i = kBlue; i <= kGreen; i++) {
				uint16_t Button = bit2button(i);
				if(Client->GHDown(Button))						//pressed
					Input->Press(Settings[i], Settings[kJoy]);
				else if(Client->GHUp(Button))					//released, even in turbo mode
					Input->Release(Settings[i], Settings[kJoy]);
				else if(turbo && Client->GHTurbo(Button))		//turbo set, button enabled for turbo and pressed
					Input->Press(Settings[i], Settings[kJoy]);
				else if(Client->GHTurbo(Button))				//turbo UNSET, button enabled for turbo and pressed
					Input->Release(Settings[i], Settings[kJoy]);//we release because turbo is UNSET
			}

			turbo = !turbo;//toggle turbo status

			//touch screen
			uint8_t x = Client->GetX();
			uint8_t y = Client->GetY();
			bool z = Client->Held(DS2KEY_TOUCH);
			uint16_t moveType = Settings[kMouse];

			if(z) {																	//if touched
				if(lastZ == false) {												//if newly pressed
					//Input->Press(KEY_LBUTTON);
					lastX = x;
					lastY = y;
					lastZ = true;
				}
				static const int i = 25;											//ignore
				static const int s = 3;												//sensitivity
				static const int border = 5;
				if(moveType == mButtons) {											//touch buttons
					bool touchedButton = false;
					for(int c = 0; c <= 11; c++) {									//check all 12 buttons
						int _x = Settings[kTouch00X + c];
						int _y = Settings[kTouch00Y + c];
						int w = _x + Settings[kTouch00W + c];
						int h = _y + Settings[kTouch00H + c];
						if(touchBetween(_x, _y, w, h)) {							//if button pressed
							Input->Press(Settings[kTouch00 + c], Settings[kJoy]);
							Input->Release(Settings[kTouch00 + c], Settings[kJoy]);
							touchedButton = true;									//don't return till we've checked every button
						}
					}
					if(touchedButton)
						return;
				}
				else if(!((x - lastX < -i) || (x - lastX > i) || (y - lastY < -i)
					   || (y - lastY > i))) {										//check that we've moved
					if(moveType == mRelative) {										//relative movement
						Input->Move((x - lastX) * s, (y - lastY) * s);
					}
					else if(moveType == mAbsolute) {								//absolute movement
						//the next 8 lines apply our border to X and Y values
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
				if(lastZ == true) {													//if newly released
					lastX = lastY = 0;
					lastZ = false;
				}
			}
		}

		bool Running = false;

		int Setup(int argc, char *argv[]) {
			bool block = false;
			Config = new C::Config();
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
					std::cout << "\nPort: " << Config->GetPort() << "\n";
				}
			}

			return UDP->Connect(block, Config->GetPort());			//startup networking
		}

		void Loop() {
			if(Running && UDP->IsConnected()) {
				DS2KeyPacket Packet;
				if(UDP->Recv(&Packet, sizeof(DS2KeyPacket)) == 0) {						//if we receive something without error
					C::Client *pClient = Client[Packet.Profile];//pointer to client
					if(Packet.Type == '/' + 1) {										//new protocal
						if(pClient == (C::Client*)NULL) {								//if profile not active,
							pClient = new C::Client();									//create it
							Config->LoadProfile(pClient->GetSettings(), Packet.Profile);//then load it
						}

						pClient->SetPacket(Packet);										//insert packet data
						pClient->Scan();												//update
						ProcessPacket(pClient);											//process
					}
					else if(Packet.Type == '/' + 2) {									//sending command
						std::cout << "Command #" << Packet.Profile << "\\" << Config->GetCommand(Packet.Profile) << "\\\n";
						if(Config->GetCommand(Packet.Profile)) {
							#ifdef _WIN32
							ShellExecute(NULL, "open", Config->GetCommand(Packet.Profile), NULL, NULL, SW_SHOWNORMAL);
							#elif defined __linux__
							std::ostringstream command;
							command << Config->GetCommand(Packet.Profile) << " &";
							system(command.str().c_str());
							#endif//_WIN32
						}
					}
					else if(Packet.Type == 0xFF) {											//looking for servers
						UDP->Send(&Packet, sizeof(DS2KeyPacket));							//bounce back
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

