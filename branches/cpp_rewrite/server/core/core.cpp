/*
	DS2Key Core functions
*/

#include <stdio.h>
#include <algorithm>//std::max,std::min
#ifdef WIN32
#include <windows.h>
#ifdef WIN32GUI
#include "gui/gui.h"
#endif//WIN32GUI
#else//LINUX
#include <string.h>
#include <chrono>
#include <thread>
#define Sleep(a) std::this_thread::sleep_for(std::chrono::milliseconds(a))
#endif//WIN32
#include "udp.h"
#include "key.h"
#include "input.h"
#include "config.h"
#include "client.h"
#include "core.h"

#define pressAndRelease(a,b) \
		if(Client->Down()&a) \
			input->Press(Settings[b], Settings[pJoy]); \
		else if(Client->Up()&a) \
			input->Release(Settings[b], Settings[pJoy]);

#define touchBetween(x1, y1, x2, y2) ((x >= std::min((x1), (x2)) && x <= std::max((x1), (x2)) && y >= std::min((y1), (y2)) && y <= std::max((y1), (y2))) && (x1+y1+x2+y2>0))

namespace D2K {
	namespace Core {
		void _processPackets(Client *Client, Input *input) {
			//static values
			static uint16_t lastX = 0, lastY = 0, lastZ = 0;

			//short pointer
			uint16_t *Settings = Client->GetSettings();

			//buttons
			pressAndRelease(DS2KEY_A, pA);
			pressAndRelease(DS2KEY_B, pB);
			pressAndRelease(DS2KEY_X, pX);
			pressAndRelease(DS2KEY_Y, pY);
			pressAndRelease(DS2KEY_L, pL);
			pressAndRelease(DS2KEY_R, pR);
			pressAndRelease(DS2KEY_START, pStart);
			pressAndRelease(DS2KEY_SELECT, pSelect);
			pressAndRelease(DS2KEY_LID, pLid);
			pressAndRelease(DS2KEY_UP, pUp);
			pressAndRelease(DS2KEY_DOWN, pDown);
			pressAndRelease(DS2KEY_LEFT, pLeft);
			pressAndRelease(DS2KEY_RIGHT, pRight);

			//touch screen
			unsigned char x = Client->GetX();
			unsigned char y = Client->GetY();
			bool z = Client->Held()&DS2KEY_TOUCH;
			int moveType = Settings[pMouse];//absolute == true

			if(z) {//if touchedll
				if(lastZ == 0) {//if newly pressed
					//input->Press(KEY_LBUTTON);
					lastX = x;
					lastY = y;
					lastZ = 1;
				}
				int i = 25;//ignore
				int s = 3;//sensitivity
				int border = 5;
				if(moveType == mButtons) {//touch buttons
					bool retVal = false;
					for(int c = 0; c <= 11; c++) {//check all 12 buttons
						int _x = Settings[pTouch00X + c];
						int _y = Settings[pTouch00Y + c];
						int w = _x + Settings[pTouch00W + c];
						int h = _y + Settings[pTouch00H + c];
						if(touchBetween(_x, _y, w, h)) {//if button pressed
							input->Press(Settings[pTouch00 + c], Settings[pJoy]);
							input->Release(Settings[pTouch00 + c], Settings[pJoy]);
							retVal = true;
						}
					}
					if(retVal)
						return;
				}
				else if(!((x - lastX < -i) || (x - lastX > i) || (y - lastY < -i) || (y - lastY > i))) {
					if(moveType == mRelative) {//relative movement
						input->Move((x - lastX) * s, (y - lastY) * s);
					}
					else if(moveType == mAbsolute) {//absolute movement
						int tempX = x;
						int tempY = y;
						if(tempX < border) tempX = border;
						if(tempX > 255 - border) tempX = 255 - border;
						if(tempY < border) tempY = border;
						if(tempY > 191 - border) tempY = 191 - border;
						tempX -= border;
						tempY -= border;
						input->MoveAbsolute(tempX * (65535  / (255 - border - border)), tempY * (65535  / (191 - border - border)));
					}
					lastX = x;
					lastY = y;
				}
				//absolute movement
			}
			else {
				if(lastZ == 1) {//if newly released
					lastX = lastY = lastZ = 0;
				}
			}
		}

		Config *config = (Config*)NULL;
		UDP *udp = (UDP*)NULL;
		Input *input = (Input*)NULL;
		Client *ClientArray[256] = {(Client*)NULL};
		bool running = false;
		#ifdef WIN32GUI
		bool console = false;
		#else//LINUX || !WIN32GUI
		bool console = true;
		#endif//WIN32GUI
		uint16_t port = 0;
		bool block = false;
		int debug = Core::lNone;
		int Setup(int argc, char *argv[])
		{
			config = new Config();
			udp = new UDP();
			input = new Input();
			running = true;
			debug = config->GetDebugLevel();
			//Client *ClientArray[256] = {(Client*)NULL};

			uint16_t port = config->GetPort();
			{//command arguments
				for(int arg = 1; arg < argc; arg++)
				{
					if(strcmp(argv[arg], "--block") == 0)
					{
						block = true;
					}
					if(strcmp(argv[arg], "--console") == 0)
					{
						console = true;
						#ifdef WINXP
						BOOL f = AllocConsole();
						freopen("CONIN$", "r", stdin);
						freopen("CONOUT$", "w", stdout);
						freopen("CONOUT$", "w", stderr);
						#endif
					}
					if(strncmp(argv[arg], "--port=", 7) == 0)
					{
						port = atoi(&argv[arg][7]);
						if(port == 0)
							port = DEFAULT_SERVER_PORT;
						printf("\nPort: %d\n", port);
					}
				}
				udp->Connect(block, port);
			}
			return 0;
		}

		void loop()//while(running)
		{
			if(running)
			{
				ds2keyPacket Packet;
				if(port != config->GetPort())
				{
					port = config->GetPort();
					udp->Connect(block, port);
				}
				if(udp->IsConnected())
					if(udp->RecvFrom(&Packet, sizeof(ds2keyPacket)) != -1)//if we receive something without error
					{
						if(Packet.type == '/' + 1)//new protocal
						{
							int Profile = Packet.profile;
							if(ClientArray[Profile] == (Core::Client*)NULL)		//if profile not active, load it, then send data
							{
								uint8_t d2kSettingsPacket[12*5+1];//12 buttons, button active & xyhw
								ClientArray[Profile] = new Core::Client();
								config->ReadProfile((uint16_t*)ClientArray[Profile]->GetSettings(), Profile);
								d2kSettingsPacket[0] = '/' + 2;//settings packet
								for(int i = 0; i < 12*5; i++)//-1?
								{
									d2kSettingsPacket[i + 1] = ClientArray[Profile]->GetSettings()[i + pTouch];
								}
								udp->Send(&d2kSettingsPacket, sizeof(uint8_t) * 12 * 5 + 1);
							//printf("new profile\n");

							}
							ClientArray[Profile]->SetPacket(Packet);		//set packet
							ClientArray[Profile]->Scan();						//update
							_processPackets(ClientArray[Profile], input);		//process
						}
						else if(Packet.type == 255)//looking for servers
						{
							//printf("bounce back\n");
							udp->Send(&Packet, sizeof(ds2keyPacket));//bounce back
						}
					}

				Sleep(1);//99% cpu without when using -O2
		#ifdef WIN32
		#ifdef WIN32GUI
				if(!console)
					D2K::GUI::GetMessages();//Application::processEvents();
		#endif//GUI
		#endif//WIN32
			}
		}
		void Destroy()
		{

			for(int i = 0; i < 255; i++)
				if(ClientArray[i] != 0)
				{
					delete(ClientArray[i]);
					ClientArray[i] = 0;
				}
			delete(udp);

			delete(input);
			delete(config);
		}
	}
}

